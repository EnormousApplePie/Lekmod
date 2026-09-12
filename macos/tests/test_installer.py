"""Filesystem regression tests; compiler, ABI checks and signing are mocked."""
from contextlib import ExitStack
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import game_install as game
import install as installer


def write(path, text):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(text)


class InstallerTests(unittest.TestCase):
    def setUp(self):
        temporary = tempfile.TemporaryDirectory()
        self.addCleanup(temporary.cleanup)
        self.root = Path(temporary.name).resolve()
        self.app = self.make_app(self.root / 'Civilization V.app')
        self.repo = self.root / 'repo'
        files = {
            'LEKMOD/Lua/tmp/ui/FrontEnd.lua.IGNORE': 'local LEKMOD_UI_CHECK_DONE = false',
            'LEKMOD/Lua/tmp/ui/IconSupport.lua.ignore': 'icons',
            'LEKMOD/Lua/UI/old-eui.lua': 'old',
            'LEKMOD/Lua/UI/LegalScreen.lua': 'legal',
            'LEKMOD/Lua/Utilities/Lekmod_version.lua':
                'return Network ~= nil and type(Network.HttpRequest) == "function"',
            'LEKMOD/Windows.DLL': 'windows',
            'Lekmap/LekmapPangaea.lua': 'map',
            'Lekmap/HBHelper.lua': 'helper',
            'macos/build/' + game.CORE.name: 'native',
        }
        for name, text in files.items():
            write(self.repo / name, text)
        write(self.repo / 'LEKMOD/ui_check.bat', '\n'.join(
            f'copy /y "%patchfolder%\\Lua\\tmp\\ui\\{source}" "%patchfolder%\\Lua\\UI\\{dest}"'
            for source, dest in [('FrontEnd.lua.ignore', 'FrontEnd.lua'),
                                 ('IconSupport.lua', 'IconSupport.lua')]))
        stack = ExitStack()
        self.addCleanup(stack.close)
        digest = game.sha256(self.app / game.CORE)
        stack.enter_context(patch.multiple(installer, ROOT=self.repo, HERE=self.repo / 'macos',
                                          STOCK_CORE_SHA256=digest))
        stack.enter_context(patch.object(game, 'STOCK_CORE_SHA256', digest))
        for module, name in [(game, 'ensure_closed')] + [
                (installer, n) for n in ('ensure_closed', 'check_imports', 'sign_nested', 'sign_app')]:
            stack.enter_context(patch.object(module, name))
        stack.enter_context(patch.object(installer, 'sign_core', side_effect=lambda app:
            write(app / game.CORE, (app / game.CORE).read_text() + ' signed')))

    def make_app(self, app):
        write(app / game.CORE, 'stock')
        write(app / 'Contents/MacOS/Civilization V', 'host')
        for name in ('DLC', 'Maps'):
            (app / game.ASSETS / name).mkdir(parents=True)
        return app

    def install(self, component='both', skip_build=True):
        return installer.install(self.app, component, skip_build=skip_build, log=lambda _: None)

    def test_steam_detection(self):
        steam, external = self.root / 'Steam', self.root / 'External Library'
        default = self.make_app(steam / "steamapps/common/Sid Meier's Civilization V/Civilization V.app")
        other = self.make_app(external / 'steamapps/common/Custom Civ/Civilization V.app')
        write(external / 'steamapps/appmanifest_8930.acf',
              '"AppState" { "empty" "" "installdir" "Custom Civ" }')
        metadata = steam / 'steamapps/libraryfolders.vdf'
        for content, expected in [
            (f'// Libraries\n"libraryfolders" {{ "0" {{ "path" "{steam}" }} '
             f'"1" {{ "path" "{external}" }} "2" "{external}" }}', [default, other]),
            ('"libraryfolders" {', [default]),
        ]:
            with self.subTest(metadata=content):
                write(metadata, content)
                self.assertEqual(game.detect_apps(steam), expected)

    def test_install_and_update_preserve_backup_maps_and_configure_ui(self):
        dlc, maps = self.app / game.ASSETS / 'DLC', self.app / game.ASSETS / 'Maps'
        write(dlc / 'LEKMOD_old/old.txt', 'previous')
        write(maps / 'unrelated.lua', 'keep')
        backup = self.install()
        self.assertEqual((backup / game.CORE).read_text(), 'stock')
        self.assertTrue((backup / game.ASSETS / 'DLC/LEKMOD_old/old.txt').exists())
        self.assertFalse((dlc / 'LEKMOD_old').exists())
        self.assertEqual((self.app / game.CORE).read_text(), 'native signed')
        for name, expected in {'FrontEnd.lua': 'local LEKMOD_UI_CHECK_DONE = true',
                               'IconSupport.lua': 'icons', 'LegalScreen.lua': 'legal'}.items():
            self.assertEqual((dlc / 'LEKMOD/Lua/UI' / name).read_text(), expected)
        self.assertFalse((dlc / 'LEKMOD/Lua/UI/old-eui.lua').exists())
        self.assertFalse((dlc / 'LEKMOD/Windows.DLL').exists())
        self.assertIn('return false', (dlc / 'LEKMOD/Lua/Utilities/Lekmod_version.lua').read_text())
        self.install()
        self.assertEqual((maps / 'Lekmap/HBHelper.lua').read_text(), 'helper')
        self.assertEqual((maps / 'unrelated.lua').read_text(), 'keep')

    def test_failure_preserves_original(self):
        rename = Path.rename
        def fail_commit(path, target):
            if path.parent.name.startswith('.lekmod-stage-'):
                raise OSError('commit failed')
            return rename(path, target)
        for failure in (patch.object(installer, 'sign_app', side_effect=OSError('signing failed')),
                        patch.object(Path, 'rename', fail_commit)):
            with self.subTest(failure=failure), failure, self.assertRaises(OSError):
                self.install()
            self.assertEqual((self.app / game.CORE).read_text(), 'stock')
            self.assertFalse((self.app / game.MANIFEST).exists())
            self.assertFalse(list(self.root.glob('.lekmod-stage-*')))

    def test_maps_only_skips_build_and_lekmod_assets(self):
        with patch.object(installer.subprocess, 'run', wraps=subprocess.run) as run:
            self.install('lekmap', skip_build=False)
        self.assertFalse(any(str(self.repo / 'macos/build.py') in call.args[0]
                             for call in run.call_args_list))
        self.assertFalse((self.app / game.ASSETS / 'DLC/LEKMOD').exists())
        self.assertEqual((self.app / game.ASSETS / 'Maps/Lekmap/LekmapPangaea.lua').read_text(), 'map')

    def test_maps_update_cannot_authorize_unknown_core(self):
        self.install()
        write(self.app / game.CORE, 'unknown replacement')
        self.install('lekmap')
        with self.assertRaisesRegex(RuntimeError, 'validated Aspyr'):
            self.install('lekmod')

    def test_external_asset_symlink_rejected(self):
        maps = self.app / game.ASSETS / 'Maps'
        maps.rmdir()
        maps.symlink_to(self.root, target_is_directory=True)
        with self.assertRaisesRegex(RuntimeError, 'outside'):
            self.install('lekmap')
