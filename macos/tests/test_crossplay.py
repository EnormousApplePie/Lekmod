"""Checks for host pinning, reversible metadata changes, and real event handlers."""
from pathlib import Path
import json
import plistlib
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import crossplay


class CrossplayTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.app = Path(self.temp.name) / 'Civ.app'
        (self.app / 'Contents/MacOS').mkdir(parents=True)
        (self.app / crossplay.HOST).write_bytes(b'known host')
        (self.app / crossplay.CORE).write_bytes(crossplay.CORE_MARKER)
        self.plist = self.app / crossplay.PLIST
        self.original = {'CFBundleVersion': '180925', 'FiraxisBuildString': '403694',
                         'CFBundleShortVersionString': '1.4.2', 'unrelated': {'x': 1}}
        self.plist.write_bytes(plistlib.dumps(self.original))
        self.app_strings = self.app / crossplay.APP_STRINGS
        self.app_strings.parent.mkdir(parents=True)
        self.original_strings = {'App.Build': '180925', 'App.Version': '1.4.2',
                                 'App.WinFileVersion': '1.0.3.279', 'unrelated': 'keep'}
        self.app_strings.write_text(json.dumps(self.original_strings, indent=2))
        self.joining = self.app / crossplay.ASSETS / 'DLC/LEKMOD/Lua/UI/JoiningRoom.lua'
        self.joining.parent.mkdir(parents=True)
        self.joining.write_text('\n'.join(
            f'function {name}(iPlayerID, playerName, bIsHost)\n    original();\nend\n'
            for name in crossplay.EVENTS))
        self.state = {'lekmod': True, 'core_sha256': 'preserve'}
        pin = patch.object(crossplay, 'HOST_SHA256', crossplay.sha256(self.app / crossplay.HOST))
        pin.start()
        self.addCleanup(pin.stop)

    def test_enable_repeat_disable_preserves_metadata_and_behavior(self):
        for _ in range(2):
            crossplay.configure_staged(self.app, self.state, True)
            info = plistlib.loads(self.plist.read_bytes())
            self.assertEqual(info, self.original)
            self.assertEqual(json.loads(self.app_strings.read_text()),
                             self.original_strings)
            self.assertEqual((self.app / crossplay.FLAG).read_text(), '403694 FINAL_RELEASE\n')
            script = self.joining.read_text()
            self.assertEqual(script.count(crossplay.DIAGNOSTIC_MARKER), 5)
            self.assertEqual(script.count('original();'), 4)
            self.assertEqual(self.state['core_sha256'], 'preserve')
        crossplay.configure_staged(self.app, self.state, False)
        self.assertEqual(plistlib.loads(self.plist.read_bytes()), self.original)
        self.assertEqual(json.loads(self.app_strings.read_text()), self.original_strings)
        self.assertNotIn(crossplay.DIAGNOSTIC_MARKER, self.joining.read_text())
        self.assertFalse(self.state['crossplay']['enabled'])
        self.assertFalse((self.app / crossplay.FLAG).exists())

    def test_unknown_host_is_rejected_before_writes(self):
        before = self.plist.read_bytes()
        (self.app / crossplay.HOST).write_bytes(b'updated host')
        with self.assertRaisesRegex(RuntimeError, 'inspected Aspyr'):
            crossplay.configure_staged(self.app, self.state, True)
        self.assertEqual(before, self.plist.read_bytes())

    def test_unknown_firaxis_build_is_rejected(self):
        self.plist.write_bytes(plistlib.dumps(dict(self.original, FiraxisBuildString='unknown')))
        with self.assertRaisesRegex(RuntimeError, 'Unexpected Firaxis'):
            crossplay.configure_staged(self.app, self.state, True)

    def test_unrecorded_bundle_edit_is_rejected(self):
        self.plist.write_bytes(plistlib.dumps(dict(self.original, CFBundleVersion='403694')))
        with self.assertRaisesRegex(RuntimeError, 'installation record'):
            crossplay.configure_staged(self.app, self.state, True)

    def test_missing_event_does_not_silently_claim_diagnostics(self):
        before = self.app_strings.read_bytes()
        self.joining.write_text('wrong UI version')
        with self.assertRaisesRegex(RuntimeError, 'Missing or ambiguous'):
            crossplay.configure_staged(self.app, self.state, True)
        self.assertEqual(before, self.app_strings.read_bytes())

    def test_legacy_prototype_migrates_engine_and_restores_bundle_version(self):
        self.state['crossplay'] = {'enabled': True, 'prototype': 'registration-identity-v1'}
        self.plist.write_bytes(plistlib.dumps(dict(self.original, CFBundleVersion='403694')))
        crossplay.configure_staged(self.app, self.state, True)
        self.assertEqual(plistlib.loads(self.plist.read_bytes()), self.original)
        self.assertEqual(json.loads(self.app_strings.read_text())['App.Build'], '180925')
        self.assertEqual(self.state['crossplay']['prototype'], crossplay.PROTOTYPE)

    def test_unknown_engine_build_fails_before_writes(self):
        self.app_strings.write_text(json.dumps(dict(self.original_strings, **{'App.Build': 'unknown'})))
        before = self.plist.read_bytes()
        with self.assertRaisesRegex(RuntimeError, 'Engine build metadata'):
            crossplay.configure_staged(self.app, self.state, True)
        self.assertEqual(before, self.plist.read_bytes())

    def test_global_build_experiment_is_restored_when_upgrading(self):
        self.state['crossplay'] = {'enabled': True, 'prototype': 'app-build-identity-v1'}
        self.app_strings.write_text(json.dumps(dict(self.original_strings, **{'App.Build': '403694'})))
        crossplay.configure_staged(self.app, self.state, True)
        self.assertEqual(json.loads(self.app_strings.read_text()), self.original_strings)
        self.assertTrue((self.app / crossplay.FLAG).exists())

    def test_old_core_cannot_enable_an_unimplemented_feature(self):
        (self.app / crossplay.CORE).write_bytes(b'older core')
        with self.assertRaisesRegex(RuntimeError, 'Rebuild and install'):
            crossplay.configure_staged(self.app, self.state, True)
        self.assertFalse((self.app / crossplay.FLAG).exists())


if __name__ == '__main__':
    unittest.main()
