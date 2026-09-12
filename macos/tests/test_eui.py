"""EUI package validation, transactional switching, and launch checks."""
from contextlib import ExitStack
import hashlib
import io
from pathlib import Path
import shutil
import sys
import tempfile
import unittest
from unittest.mock import patch
import zipfile

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import eui
import game_install as game
import install as installer
import launcher
from package_assets import prepare_lekmod
import test_installer as fixtures


class EUITests(unittest.TestCase):
    make_app = fixtures.InstallerTests.make_app
    install = fixtures.InstallerTests.install

    def setUp(self):
        fixtures.InstallerTests.setUp(self)
        package = self.root / 'package/UI_bc1'
        package.mkdir(parents=True)
        (package / 'readme.txt').write_text('EUI 1.28g')
        data = io.BytesIO()
        with zipfile.ZipFile(data, 'w') as archive:
            archive.write(package / 'readme.txt', 'UI_bc1/readme.txt')
            archive.writestr('EUI_text_en_us.xml', 'English text')
        self.data = data.getvalue()
        self.archive = self.root / 'cached.zip'
        self.archive.write_bytes(self.data)
        stack = ExitStack()
        self.addCleanup(stack.close)
        stack.enter_context(patch.multiple(eui, ARCHIVE=self.archive,
            TEXT=self.root / 'Text/EUI_text_en_us.xml', TEXT_SHA256=hashlib.sha256(b'English text').hexdigest(),
            ARCHIVE_SHA256=hashlib.sha256(self.data).hexdigest(), TREE_SHA256=eui.tree_digest(package)))

    def switch(self, enabled):
        # Tiny installer fixture has only two UI sources; real overlay rules are
        # exercised separately below against the actual repository assets.
        def package(source, destination, eui=None):
            prepare_lekmod(source, destination)
            (destination / 'Lua/UI/mode.lua').write_text('eui' if eui else 'standard')
        with patch.object(installer, 'prepare_lekmod', side_effect=package):
            return installer.install(self.app, component='eui', eui_enabled=enabled, log=lambda _: None)

    def test_switch_preserves_engine_maps_and_restores_standard_ui(self):
        self.install()
        core = game.sha256(self.app / game.CORE)
        backup = self.switch(True)
        self.assertFalse((backup / eui.PACKAGE).exists())
        state = game.installed_state(self.app)
        eui.check(self.app, state, True)
        self.assertEqual(game.sha256(self.app / game.CORE), core)
        self.assertEqual(state['validation']['ui'], 'eui')
        self.assertTrue((self.app / game.ASSETS / 'Maps/Lekmap').is_dir())
        backup = self.switch(False)
        self.assertTrue((backup / eui.PACKAGE).is_dir())
        state = game.installed_state(self.app)
        eui.check(self.app, state, False)
        self.assertFalse((self.app / eui.PACKAGE).exists())
        self.assertFalse(eui.TEXT.exists())
        self.assertEqual((self.app / game.ASSETS / 'DLC/LEKMOD/Lua/UI/mode.lua').read_text(), 'standard')
        self.assertEqual(game.sha256(self.app / game.CORE), core)
        self.assertTrue(eui.cached())

    def test_changed_package_and_missing_files_require_repair(self):
        self.install()
        self.switch(True)
        (self.app / eui.PACKAGE / 'readme.txt').write_text('changed')
        state = game.installed_state(self.app)
        eui.guard(self.app, state)
        with self.assertRaises(RuntimeError):
            eui.check(self.app, state, True)
        self.switch(True)
        shutil.rmtree(self.app / eui.PACKAGE)
        with self.assertRaises(RuntimeError):
            eui.check(self.app, game.installed_state(self.app), True)
        self.switch(True)
        eui.check(self.app, game.installed_state(self.app), True)

    def test_failed_signing_and_wrong_archive_do_not_change_game(self):
        self.install()
        original = (self.app / game.MANIFEST).read_bytes()
        with patch.object(installer, 'sign_app', side_effect=RuntimeError('signing failed')):
            with self.assertRaisesRegex(RuntimeError, 'signing failed'):
                self.switch(True)
        self.assertFalse((self.app / eui.PACKAGE).exists())
        self.assertFalse(eui.TEXT.exists())
        self.archive.write_bytes(b'newer or corrupt archive')
        self.assertFalse(eui.cached())
        with self.assertRaisesRegex(RuntimeError, 'supported EUI'):
            self.switch(True)
        self.assertEqual((self.app / game.MANIFEST).read_bytes(), original)

    def test_unmanaged_package_is_preserved(self):
        path = self.app / eui.PACKAGE / 'readme.txt'
        path.parent.mkdir(parents=True)
        path.write_text('EUI 1.30')
        with self.assertRaisesRegex(RuntimeError, 'unsupported EUI'):
            self.switch(True)
        self.assertEqual(path.read_text(), 'EUI 1.30')

    def test_failed_uninstall_restores_text_and_package(self):
        self.install()
        self.switch(True)
        with patch.object(installer, 'sign_app', side_effect=RuntimeError('signing failed')):
            with self.assertRaisesRegex(RuntimeError, 'signing failed'):
                self.switch(False)
        eui.check(self.app, game.installed_state(self.app), True)

    def test_removing_lekmod_then_eui_leaves_stock_game_and_maps(self):
        self.install()
        self.switch(True)
        fixtures.remover.uninstall(self.app, 'lekmod', log=lambda _: None)
        self.switch(False)
        eui.check(self.app, game.installed_state(self.app), False)
        self.assertEqual((self.app / game.CORE).read_text(), 'stock')
        self.assertFalse((self.app / game.ASSETS / 'DLC/LEKMOD').exists())
        self.assertTrue((self.app / game.ASSETS / 'Maps/Lekmap').is_dir())

    def test_custom_text_is_preserved_and_missing_text_restored(self):
        self.install()
        self.switch(True)
        eui.TEXT.unlink()
        with self.assertRaisesRegex(RuntimeError, 'text resources'):
            eui.check(self.app, game.installed_state(self.app), True)
        self.switch(True)
        self.assertEqual(eui.TEXT.read_text(), 'English text')
        eui.TEXT.write_text('custom translation')
        with self.assertRaisesRegex(RuntimeError, 'different EUI text'):
            self.switch(True)
        self.switch(False)
        self.assertEqual(eui.TEXT.read_text(), 'custom translation')

    def test_preferences_survive_steam_reset_and_uninstall(self):
        directory = self.root / 'preferences'
        launcher.preferences(self.app, directory=directory, eui=True)
        self.assertTrue(launcher.preferences(self.app, directory=directory)['installations'][str(self.app)]['eui'])
        launcher.preferences(self.app, directory=directory, eui=False)
        self.assertFalse(launcher.preferences(self.app, directory=directory)['installations'][str(self.app)]['eui'])

    def test_archive_traversal_rejected(self):
        data = io.BytesIO()
        with zipfile.ZipFile(data, 'w') as archive:
            archive.writestr('UI_bc1/../../escape', 'bad')
        with patch.object(eui, 'ARCHIVE_SHA256', hashlib.sha256(data.getvalue()).hexdigest()):
            with self.assertRaisesRegex(RuntimeError, 'archive path'):
                eui.prepare(self.app, {}, True, data.getvalue())
        self.assertFalse((self.app / game.ASSETS / 'escape').exists())


class OverlayTests(unittest.TestCase):
    def test_existing_windows_rules_select_native_eui_files(self):
        root = Path(__file__).resolve().parents[2]
        with tempfile.TemporaryDirectory() as temporary:
            directory = Path(temporary)
            pack = directory / 'UI_bc1'
            for name, text in (
                    ('Core/CityStateStatusHelper.lua', 'present'),
                    ('CityView/ProductionPopup.lua', 'present'),
                    ('CityView/CityView.lua', '-- coded by bc1 from 1.0.3.276 brave new world code'),
                    ('UnitPanel/UnitPanel.lua', '-- modified by bc1 from Civ V 1.0.3.276 code'),
                    ('CityBanners/CityBannerManager.lua', 'CityBannerProductionBox = function( city )')):
                path = pack / name
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_text(text)
            destination = directory / 'LEKMOD'
            prepare_lekmod(root / 'LEKMOD', destination, eui=pack)
            ui = destination / 'Lua/UI'
            self.assertFalse((ui / 'ProductionPopup.lua').exists())
            for source, target in (
                    ('Core/CityStateStatusHelper.lua.ignore', 'CityStateStatusHelper.lua'),
                    ('CityView/CityView.lua.ignore', 'CityView.lua'),
                    ('UnitPanel/UnitPanel.lua.ignore', 'UnitPanel.lua'),
                    ('CityBanners/CityBannerManager_1.lua.ignore', 'CityBannerManager.lua')):
                self.assertEqual((ui / target).read_bytes(), (root / 'LEKMOD/Lua/tmp/eui' / source).read_bytes())
            self.assertIn('local LEKMOD_UI_CHECK_DONE = true', (ui / 'FrontEnd.lua').read_text())
            self.assertIn('return false', (destination / 'Lua/Utilities/Lekmod_version.lua').read_text())
