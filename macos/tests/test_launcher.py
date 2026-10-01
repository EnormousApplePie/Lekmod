"""Launch gating, Steam repair recovery, and persisted user intent."""
from contextlib import ExitStack
from datetime import datetime
import json
from pathlib import Path
import plistlib
import struct
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import crossplay
import game_install as game
from integrity import tree_digest
import launcher
from menu_art import read_texture


def write(path, value):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(value if isinstance(value, bytes) else value.encode())


class LauncherTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name).resolve()
        self.app = self.root / 'steamapps/common/Civ/Civilization V.app'
        self.metadata = self.root / 'steamapps/appmanifest_8930.acf'
        write(self.metadata, '"AppState" { "installdir" "Civ" "StateFlags" "4" }')
        write(self.app / crossplay.HOST, 'host')
        write(self.app / game.CORE, 'stock')
        stock_digest = game.sha256(self.app / game.CORE)
        for name in launcher.REQUIRED_DLC:
            write(self.app / game.ASSETS / 'DLC' / name / 'content', 'dlc')
        write(self.app / crossplay.PLIST, plistlib.dumps(
            {'CFBundleVersion': '180925', 'FiraxisBuildString': '403694'}))
        write(self.app / crossplay.APP_STRINGS, json.dumps(
            {'App.Build': '180925', 'App.WinFileVersion': '1.0.3.279'}))
        self.repo = self.root / 'checkout'
        write(self.repo / 'LEKMOD/Lua/Utilities/Lekmod_version.lua', 'LekmodVersion.LOCAL_VERSION = "v35.3000"')
        write(self.repo / 'Lekmap/LekmapPangaea.lua', 'map')
        stack = ExitStack()
        self.addCleanup(stack.close)
        stack.enter_context(patch.object(launcher, 'ROOT', self.repo))
        stack.enter_context(patch.object(crossplay, 'HOST_SHA256', game.sha256(self.app / crossplay.HOST)))
        for module in (game, launcher):
            stack.enter_context(patch.object(module, 'STOCK_CORE_SHA256', stock_digest))
        for name in ('ensure_closed', 'check_imports', 'verify_signature'):
            stack.enter_context(patch.object(launcher, name))
        stack.enter_context(patch.object(launcher, 'steam_session', return_value={'state': 'ok', 'label': 'Logged in'}))
        self.source = stack.enter_context(patch.object(launcher, 'source_digest', return_value='source'))
        self.repair = stack.enter_context(patch.object(launcher.installer, 'install', side_effect=self.restore))
        self.open = stack.enter_context(patch.object(launcher.subprocess, 'run'))
        self.restore(self.app, crossplay_enabled=True)

    def restore(self, app, crossplay_enabled=True, **kwargs):
        write(app / game.CORE, crossplay.CORE_MARKER)
        write(app / game.ASSETS / 'DLC/LEKMOD/rules.xml', 'rules')
        write(app / game.ASSETS / 'Maps/Lekmap/LekmapPangaea.lua', 'map')
        if crossplay_enabled:
            write(app / crossplay.FLAG, '403694 FINAL_RELEASE\n')
        else:
            (app / crossplay.FLAG).unlink(missing_ok=True)
        state = dict(installer='lekmod-macos', lekmod=True, lekmap=True,
                     stock_core_sha256=game.STOCK_CORE_SHA256,
                     core_sha256=game.sha256(app / game.CORE),
                     crossplay={'enabled': crossplay_enabled},
                     validation=dict(format=1, source_sha256='source',
                                     lekmod_sha256=tree_digest(app / game.ASSETS / 'DLC/LEKMOD'),
                                     lekmap_sha256=tree_digest(app / game.ASSETS / 'Maps/Lekmap'),
                                     lekmap_source_sha256=tree_digest(self.repo / 'Lekmap')))
        write(app / game.MANIFEST, json.dumps(state))

    def test_intentionally_removed_maps_do_not_reinstall_on_launch(self):
        import shutil
        shutil.rmtree(self.app / game.ASSETS / 'Maps/Lekmap')
        state = game.installed_state(self.app)
        state['lekmap'] = False
        state['validation'].pop('lekmap_sha256')
        state['validation'].pop('lekmap_source_sha256')
        write(self.app / game.MANIFEST, json.dumps(state))
        self.assertTrue(launcher.inspect(self.app, True)['ready'])
        report = launcher.run_action(self.app, True, 'launch')
        self.assertTrue(report['launched'])
        self.repair.assert_not_called()
        self.assertFalse(report['lekmap_installed'])

    def test_healthy_launch_checks_then_opens_only_steam(self):
        report = launcher.run_action(self.app, True, 'launch')
        self.assertTrue(report['launched'])
        self.repair.assert_not_called()
        self.open.assert_called_once_with(['/usr/bin/open', 'steam://rungameid/8930'], check=True, timeout=30)

    def test_steam_reset_restores_persistent_crossplay_before_launch(self):
        directory = self.root / 'preferences'
        launcher.preferences(self.app, directory=directory)
        write(self.app / game.CORE, 'stock')
        (self.app / game.MANIFEST).unlink()
        (self.app / crossplay.FLAG).unlink()
        saved = launcher.preferences(self.app, directory=directory)
        desired = saved['installations'][str(self.app)]['crossplay']
        self.assertTrue(desired)
        report = launcher.run_action(self.app, desired, 'launch')
        self.assertTrue(report['ready'])
        self.repair.assert_called_once()
        self.assertTrue(self.repair.call_args.kwargs['crossplay_enabled'])
        self.open.assert_called_once()

    def test_unknown_host_or_core_never_repairs_or_launches(self):
        for target in (crossplay.HOST, game.CORE):
            with self.subTest(target=target):
                original = (self.app / target).read_bytes()
                write(self.app / target, 'unknown replacement')
                with self.assertRaisesRegex(RuntimeError, 'Needs attention'):
                    launcher.run_action(self.app, True, 'launch')
                self.repair.assert_not_called()
                self.open.assert_not_called()
                write(self.app / target, original)

    def test_changed_mod_file_is_detected_and_repaired(self):
        write(self.app / game.ASSETS / 'DLC/LEKMOD/rules.xml', 'changed')
        report = launcher.inspect(self.app, True)
        self.assertFalse(report['ready'])
        self.assertEqual(next(c['state'] for c in report['checks'] if c['id'] == 'lekmod_assets'), 'repair')
        self.assertEqual(next(c['state'] for c in report['checks'] if c['id'] == 'lekmap_assets'), 'ok')
        launcher.run_action(self.app, True, 'repair')
        self.assertTrue(launcher.inspect(self.app, True)['ready'])
        self.open.assert_not_called()

    def test_missing_map_and_changed_checkout_require_update(self):
        (self.app / game.ASSETS / 'Maps/Lekmap/LekmapPangaea.lua').unlink()
        self.source.return_value = 'new-source'
        report = launcher.inspect(self.app, True)
        self.assertEqual({c['id'] for c in report['checks'] if c['state'] == 'repair'}, {'lekmap_assets', 'source'})

    def test_switch_off_is_applied_by_repair(self):
        report = launcher.inspect(self.app, False)
        self.assertFalse(report['ready'])
        launcher.run_action(self.app, False, 'repair')
        self.assertFalse((self.app / crossplay.FLAG).exists())
        self.assertTrue(launcher.inspect(self.app, False)['ready'])
        self.open.assert_not_called()

    def test_command_line_crossplay_change_updates_launcher_preference(self):
        with patch.object(crossplay, 'ensure_closed'), \
                patch.object(crossplay, 'replace_app', return_value=self.root / 'backup'), \
                patch.object(crossplay, 'preferences') as remember:
            crossplay.configure(self.app, False)
        remember.assert_called_once_with(self.app, False)

    def test_steam_download_or_running_game_blocks_mutation(self):
        write(self.metadata, '"AppState" { "installdir" "Civ" "StateFlags" "6" }')
        with self.assertRaises(RuntimeError):
            launcher.run_action(self.app, True, 'launch')
        self.repair.assert_not_called()
        self.open.assert_not_called()
        with patch.object(launcher, 'ensure_closed', side_effect=RuntimeError('Close Civilization V before installing.')):
            with self.assertRaisesRegex(RuntimeError, 'Close Civilization V'):
                launcher.run_action(self.app, True, 'repair')
        self.repair.assert_not_called()

    def test_failed_repair_never_launches(self):
        write(self.app / game.CORE, 'stock')
        self.repair.side_effect = RuntimeError('signing failed')
        with self.assertRaisesRegex(RuntimeError, 'signing failed'):
            launcher.run_action(self.app, True, 'launch')
        self.open.assert_not_called()

    def test_change_before_steam_handoff_prevents_launch(self):
        report = launcher.inspect(self.app, True)
        changed = dict(report, ready=False)
        with patch.object(launcher, 'inspect', side_effect=[report, changed]):
            with self.assertRaisesRegex(RuntimeError, 'changed before launch'):
                launcher.run_action(self.app, True, 'launch')
        self.open.assert_not_called()

    def test_bad_manifest_and_external_content_links_do_not_pass(self):
        manifest = self.app / game.MANIFEST
        state = json.loads(manifest.read_text())
        state['validation'] = []
        write(manifest, json.dumps(state))
        self.assertFalse(launcher.inspect(self.app, True)['repairable'])
        self.restore(self.app)
        (self.app / game.ASSETS / 'DLC/LEKMOD/external').symlink_to(self.root, target_is_directory=True)
        self.assertFalse(launcher.inspect(self.app, True)['ready'])


class ProcessStatusTests(unittest.TestCase):
    def test_probe_reports_lifecycle_without_scanning_or_writing_preferences(self):
        import io
        from contextlib import redirect_stdout
        for running in (True, False):
            with self.subTest(running=running), \
                    patch.object(sys, 'argv', ['launcher.py', 'process-status']), \
                    patch.object(launcher, 'game_running', return_value=running), \
                    patch.object(launcher, 'preferences', side_effect=AssertionError('Must not write preferences')), \
                    patch.object(launcher, 'inspect', side_effect=AssertionError('Must not scan files')):
                output = io.StringIO()
                with redirect_stdout(output):
                    self.assertEqual(launcher.main(), 0)
                self.assertEqual(json.loads(output.getvalue()), {'event': 'process-status', 'running': running})


class MenuArchiveTests(unittest.TestCase):
    def test_named_texture_and_invalid_bounds(self):
        with tempfile.TemporaryDirectory() as directory:
            archive = Path(directory) / 'textures.fpk'
            name = b'civilzationvatlas.dds'
            header = b'\x06\0\0\0FPK_\0\0' + struct.pack('<I', 1)
            offset = len(header) + 4 + len(name) + 19
            texture = b'DDS ' + bytes(124)
            entry = struct.pack('<I', len(name)) + name + bytes([3]) + bytes(10)
            archive.write_bytes(header + entry + struct.pack('<II', len(texture), offset) + texture)
            self.assertEqual(read_texture(archive, name.decode()), texture)
            archive.write_bytes(header + entry + struct.pack('<II', len(texture), offset + 1) + texture)
            with self.assertRaises(ValueError):
                read_texture(archive, name.decode())


class SteamSessionTests(unittest.TestCase):
    def test_old_login_does_not_describe_a_new_steam_process(self):
        log = '[2026-09-12 06:02:47] [Logged On, 4, 7] processing complete'
        started = datetime(2026, 9, 12, 7)
        self.assertEqual(launcher.connection_status(log, started)['label'], 'Sign-in unknown')

    def test_logout_overrides_successful_login(self):
        log = ('[2026-09-12 06:02:47] [Logged On, 4, 7] processing complete\n'
               '[2026-09-12 06:05:00] [Logged On, 4, 7] LogOff()')
        self.assertEqual(launcher.connection_status(log, datetime(2026, 9, 12, 6))['label'], 'Logged out')

    def test_current_session_login_is_recognized(self):
        log = '[2026-09-12 06:02:47] [Logged On, 4, 7] processing complete'
        self.assertEqual(launcher.connection_status(log, datetime(2026, 9, 12, 6))['label'], 'Logged in')


if __name__ == '__main__':
    unittest.main()
