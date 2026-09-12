#!/usr/bin/env python3
"""Enable the experimental Windows registration identity in a native Mac install.

This enables an adaptation in the native pre-game registration boundary. It does not claim packet or
simulation compatibility; the first Windows session is an integration test.
"""
import argparse
from datetime import datetime, timezone
import json
from pathlib import Path
import plistlib
import re
import sys

from game_install import (ASSETS, CORE, MANIFEST, app_path, detect_apps, ensure_closed,
                          installation_lock, installed_state, replace_app, sha256,
                          sign_app, validate_app, validate_core)

HOST = Path('Contents/MacOS/Civilization V')
PLIST = Path('Contents/Info.plist')
APP_STRINGS = Path('Contents/AspyrAssets/global/String/App.json')
FLAG = Path('Contents/Resources/lekmod-crossplay.txt')
PROTOTYPE = 'pregame-registration-v2'
CORE_MARKER = b'LEKMOD_MACOS_REGISTRATION_V2'
HOST_SHA256 = 'd56d6bfbc0ef517fcb7cbaff46c42d1bdfab809c084684045761bd9d85807ee9'
MAC_BUILD = '180925'
WINDOWS_BUILD = '403694'
DIAGNOSTIC_MARKER = '-- LEKMOD_MACOS_CROSSPLAY_DIAGNOSTICS'
EVENTS = {
    'OnHostConnect': 'connected-to-host',
    'OnNetRegistered': 'registered; waiting-for-gamestate',
    'OnConnectionCompete': 'connection-complete',
    'OnVersionMismatch': 'engine-version-mismatch',
}


def configure_staged(app, state, enabled):
    """Only call on a staged bundle. Preserve all unrelated plist/manifest keys."""
    if sha256(app / HOST) != HOST_SHA256:
        raise RuntimeError('Cross-play prototype requires the inspected Aspyr 180925 executable.')
    if enabled and CORE_MARKER not in (app / CORE).read_bytes():
        raise RuntimeError('Rebuild and install native Lekmod before enabling cross-play.')
    plist = app / PLIST
    if not plist.resolve().is_relative_to(app.resolve()):
        raise RuntimeError('Info.plist points outside the app bundle.')
    info = plistlib.loads(plist.read_bytes())
    if info.get('FiraxisBuildString') != WINDOWS_BUILD:
        raise RuntimeError('Unexpected Firaxis build; refusing to guess a Windows network identity.')
    old = state.get('crossplay', {})
    legacy = old.get('enabled') and old.get('prototype') == 'registration-identity-v1'
    expected = WINDOWS_BUILD if legacy else MAC_BUILD
    if info.get('CFBundleVersion') != expected:
        raise RuntimeError('Bundle version differs from its installation record.')
    # The bundle version is not used by the network engine. Undo the earlier
    # metadata experiment when upgrading an existing prototype installation.
    info['CFBundleVersion'] = MAC_BUILD
    strings = app / APP_STRINGS
    if not strings.resolve().is_relative_to(app.resolve()):
        raise RuntimeError('App.json points outside the app bundle.')
    app_text = strings.read_text()
    app_info = json.loads(app_text)
    app_build_legacy = old.get('enabled') and old.get('prototype') == 'app-build-identity-v1'
    expected_build = WINDOWS_BUILD if app_build_legacy else MAC_BUILD
    if (app_info.get('App.Build') != expected_build
            or app_info.get('App.WinFileVersion') != '1.0.3.279'
            or app_info.get('App.Version') != '1.4.2'):
        raise RuntimeError('Engine build metadata differs from the inspected installation.')
    app_text, count = re.subn(r'("App\.Build"\s*:\s*")[^"]*(")',
                             lambda m: m[1] + MAC_BUILD + m[2],
                             app_text)
    if count != 1:
        raise RuntimeError('Missing or ambiguous App.Build setting.')
    flag = app / FLAG
    if not flag.resolve().is_relative_to(app.resolve()):
        raise RuntimeError('Cross-play setting points outside the app bundle.')
    if flag.exists() and flag.read_text() != WINDOWS_BUILD + ' FINAL_RELEASE\n':
        raise RuntimeError('Unrecognized cross-play setting; refusing to overwrite it.')

    joining = app / ASSETS / 'DLC/LEKMOD/Lua/UI/JoiningRoom.lua'
    if not joining.resolve().is_relative_to(app.resolve()):
        raise RuntimeError('JoiningRoom.lua points outside the app bundle.')
    text = joining.read_text()
    # Strip only our own single-line diagnostics, making toggles idempotent.
    text = '\n'.join(line for line in text.split('\n') if DIAGNOSTIC_MARKER not in line)
    if enabled:
        for function, event in EVENTS.items():
            pattern = rf'(function {function}\([^\n]*\)\s*\n)'
            detail = (' .. " player=" .. tostring(iPlayerID) .. " host=" .. tostring(bIsHost)'
                      if function == 'OnVersionMismatch' else '')
            diagnostic = f'    print("[Lekmod crossplay] {event}"{detail}); {DIAGNOSTIC_MARKER}\n'
            text, count = re.subn(pattern, lambda m: m[1] + diagnostic, text)
            if count != 1:
                raise RuntimeError(f'Missing or ambiguous multiplayer event: {function}')
        text += f'\nprint("[Lekmod crossplay] Windows registration prototype enabled; target {WINDOWS_BUILD} FINAL_RELEASE"); {DIAGNOSTIC_MARKER}\n'
    # Validate every input before changing the staged files.
    plist.write_bytes(plistlib.dumps(info, fmt=plistlib.FMT_BINARY, sort_keys=False))
    strings.write_text(app_text)
    joining.write_text(text)
    if enabled:
        flag.parent.mkdir(parents=True, exist_ok=True)
        flag.write_text(WINDOWS_BUILD + ' FINAL_RELEASE\n')
    elif flag.exists():
        flag.unlink()
    state['crossplay'] = {
        'enabled': enabled, 'prototype': PROTOTYPE,
        'native_bundle_version': MAC_BUILD,
        'native_app_build': MAC_BUILD,
        'target_registration_version': WINDOWS_BUILD + ' FINAL_RELEASE',
        'host_sha256': HOST_SHA256,
        'configured_at': datetime.now(timezone.utc).isoformat(),
        'multiplayer_validated': False,
    }


def configure(app, enabled):
    with installation_lock(app):
        ensure_closed()
        validate_app(app)
        validate_core(app)
        state = installed_state(app)
        if not state.get('lekmod'):
            raise RuntimeError('Install native Lekmod with macos/install.py first.')
        before = {relative: sha256(app / relative)
                  for relative in (HOST, CORE, PLIST, APP_STRINGS, MANIFEST)}

        def populate(staged):
            configure_staged(staged, state, enabled)
            (staged / MANIFEST).write_text(json.dumps(state, indent=2) + '\n')
            sign_app(staged)
            if any(sha256(app / relative) != digest for relative, digest in before.items()):
                raise RuntimeError('The installation changed while staging cross-play.')

        return replace_app(app, populate)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--app', type=Path)
    mode = parser.add_mutually_exclusive_group(required=True)
    mode.add_argument('--enable', action='store_true')
    mode.add_argument('--disable', action='store_true')
    args = parser.parse_args()
    apps = [app_path(args.app)] if args.app else detect_apps()
    if len(apps) != 1:
        parser.error('Use --app to select exactly one installed game.')
    try:
        backup = configure(apps[0], args.enable)
        print(f'Native cross-play prototype {"enabled" if args.enable else "disabled"}. Backup: {backup}')
        return 0
    except (OSError, ValueError, RuntimeError) as error:
        print(f'Cross-play configuration failed: {error}', file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main())
