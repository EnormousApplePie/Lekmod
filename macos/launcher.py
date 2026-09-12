#!/usr/bin/env python3
"""JSON-lines service for the native Lekmod launcher."""
import argparse
from contextlib import redirect_stdout
import io
import json
import os
from datetime import datetime
from pathlib import Path
import plistlib
import re
import subprocess
import sys

import crossplay
from audit import check_imports
from game_install import (ASSETS, CORE, STOCK_CORE_SHA256, app_path, detect_apps,
                          ensure_closed, game_running, installed_state, installation_lock, read_vdf,
                          sha256, validate_app, validate_core)
from integrity import source_digest, tree_digest
import install as installer
from menu_art import background, logos
from uninstall import uninstall
from launcher_preferences import preferences

ROOT = Path(__file__).resolve().parent.parent
CACHE = Path.home() / 'Library/Caches/Lekmod Launcher'
REQUIRED_DLC = ('Expansion', 'Expansion2', 'DLC_Deluxe',
                'DLC_01', 'DLC_02', 'DLC_03', 'DLC_04', 'DLC_05', 'DLC_06', 'DLC_07')


def connection_status(text, started):
    """Use only connection events from the currently running Steam session."""
    for line in reversed(text.splitlines()):
        match = re.match(r'\[([^]]+)\] \[([^],]+)', line)
        if not match:
            continue
        try:
            observed = datetime.strptime(match[1], '%Y-%m-%d %H:%M:%S')
        except ValueError:
            continue
        if observed < started:
            break
        state = match[2]
        if 'LogOff()' in line or 'ConnectionDisconnected(' in line:
            return dict(state='warning', label='Logged out')
        if state == 'Logged On':
            return dict(state='ok', label='Logged in')
        if state in ('Connected', 'Connecting', 'Logging On'):
            return dict(state='warning', label='Connecting')
        if state in ('Logged Off', 'Logging Off'):
            return dict(state='warning', label='Logged out')
    return dict(state='warning', label='Sign-in unknown')


def steam_session():
    try:
        processes = subprocess.run(['pgrep', '-u', str(os.getuid()), '-x', 'steam_osx'],
                                   capture_output=True, text=True, timeout=10)
        if processes.returncode == 1:
            return dict(state='warning', label='Not running')
        if processes.returncode:
            return dict(state='warning', label='Status unavailable')
        pid = processes.stdout.split()[0]
        environment = dict(os.environ, LC_ALL='C')
        start = subprocess.run(['ps', '-p', pid, '-o', 'lstart='], capture_output=True,
                               text=True, check=True, timeout=10, env=environment)
        started = datetime.strptime(start.stdout.strip(), '%a %b %d %H:%M:%S %Y')
        path = Path.home() / 'Library/Application Support/Steam/logs/connection_log.txt'
        with path.open('rb') as stream:
            stream.seek(max(0, path.stat().st_size - 256 * 1024))
            text = stream.read().decode(errors='replace')
        return connection_status(text, started)
    except (OSError, ValueError, IndexError, subprocess.SubprocessError):
        return dict(state='warning', label='Sign-in unknown')


def emit(event, **data):
    print(json.dumps(dict(event=event, **data)), flush=True)


def steam_ready(app):
    """The Steam URL must refer to this installed game, not a detached app copy."""
    steamapps = app.parent.parent.parent
    if steamapps.name != 'steamapps' or app.parent.parent.name != 'common':
        raise RuntimeError('Choose Civilization V inside a Steam library; detached app copies cannot launch through Steam.')
    path = steamapps / 'appmanifest_8930.acf'
    try:
        state = read_vdf(path)['AppState']
        if state.get('installdir') != app.parent.name:
            raise ValueError('Steam installation path differs')
        if int(state.get('StateFlags', '0')) != 4:
            raise RuntimeError('Finish downloading or verifying Civilization V in Steam, then check again.')
    except (OSError, KeyError, ValueError) as error:
        raise RuntimeError('Steam does not list this copy as installed. Finish installing it in Steam first.') from error


def verify_signature(app):
    result = subprocess.run(['/usr/bin/codesign', '--verify', '--strict', str(app)],
                            capture_output=True, text=True, timeout=60)
    if result.returncode:
        raise RuntimeError('The game bundle changed or its signature needs repair.')


def inspect(app, desired, log=lambda _: None):
    checks = []
    def add(key, title, state, detail):
        checks.append(dict(id=key, title=title, state=state, detail=detail))
    def checked(key, title, operation, detail, failure='blocked'):
        log('Checking ' + title.lower() + '…')
        try:
            operation()
            add(key, title, 'ok', detail)
            return True
        except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
            add(key, title, failure, str(error))
            return False

    version_file = ROOT / 'LEKMOD/Lua/Utilities/Lekmod_version.lua'
    match = re.search(r'LOCAL_VERSION\s*=\s*"([^"]+)"', version_file.read_text())
    result = dict(app=str(app), version=match[1] if match else 'Local checkout',
                  crossplay=desired, checks=checks, running=False, steam_session=steam_session())
    if not checked('game', 'Civilization V', lambda: validate_app(app), 'Game files found'):
        result.update(ready=False, repairable=False)
        return result
    try:
        ensure_closed()
    except RuntimeError as error:
        result['running'] = True
        result['running_message'] = str(error).replace('before installing.', 'before checking or repairing.')
    checked('steam', 'Steam installation', lambda: steam_ready(app), 'Installed and ready in Steam')
    def host():
        if sha256(app / crossplay.HOST) != crossplay.HOST_SHA256:
            raise RuntimeError('This Civ V executable is not the supported Aspyr 180925 build. A launcher compatibility update is required.')
        info = plistlib.loads((app / crossplay.PLIST).read_bytes())
        strings = json.loads((app / crossplay.APP_STRINGS).read_text())
        if (info.get('CFBundleVersion') != '180925' or info.get('FiraxisBuildString') != '403694'
                or strings.get('App.Build') != '180925' or strings.get('App.WinFileVersion') != '1.0.3.279'):
            raise RuntimeError('Civ V build metadata differs from the supported installation. Restore the game through Steam first.')
    checked('host', 'Mac game compatibility', host, 'Supported Aspyr build 180925')
    def dlc():
        directory = app / ASSETS / 'DLC'
        missing = [name for name in REQUIRED_DLC if not (directory / name).is_dir()
                   or not any((directory / name).iterdir())]
        if missing:
            raise RuntimeError('Install the required DLC in Steam: ' + ', '.join(missing))
        if any(p.name.lower() in ('ui_bc1', 'ui_bc1_xits') for p in directory.iterdir()):
            raise RuntimeError('Remove EUI (UI_bc1/UI_bc1_xits) before using the standard Mac UI.')
    checked('dlc', 'Expansions & DLC', dlc, 'Required DLC folders present · standard UI')
    try:
        state = installed_state(app)
    except RuntimeError as error:
        add('record', 'Installation record', 'blocked', str(error))
        state = {}
    known = checked('core', 'Native Lekmod library', lambda: validate_core(app), 'Installed library recognized')
    digest = sha256(app / CORE)
    if known and (digest == STOCK_CORE_SHA256 or not state.get('lekmod')):
        checks[-1].update(state='repair', detail='Steam restored the original library, or Lekmod has not been installed yet.')
    elif known:
        def abi():
            with redirect_stdout(io.StringIO()):
                check_imports(app / CORE, app)
        checked('abi', 'Game interfaces', abi, 'Native library matches the game interfaces', 'repair')
    result['lekmod_installed'] = digest != STOCK_CORE_SHA256 and bool(state.get('lekmod'))
    result['lekmap_installed'] = (app / ASSETS / 'Maps/Lekmap').is_dir()
    validation = state.get('validation', {})
    for identifier, title, key, directory, optional in (
            ('lekmod_assets', 'Lekmod Assets', 'lekmod_sha256', app / ASSETS / 'DLC/LEKMOD', False),
            ('lekmap_assets', 'Lekmap Assets', 'lekmap_sha256', app / ASSETS / 'Maps/Lekmap', True)):
        if optional and state.get('lekmap') is False and not directory.exists():
            add(identifier, title, 'ok', 'Lekmap is not installed; its assets are optional.')
            continue
        def content(directory=directory, key=key, title=title):
            if validation.get('format') != 1 or not validation.get(key):
                raise RuntimeError(f'Install or repair {title} to enable file validation.')
            if tree_digest(directory) != validation[key]:
                raise RuntimeError(f'{title} are missing or changed. Repair will restore them from this checkout.')
        checked(identifier, title, content, f'{title} verified against the installed fingerprints', 'repair')
    if validation.get('format') != 1:
        add('source', 'Lekmod version', 'repair', 'Refresh your existing installation to enable version checks.')
    else:
        def source():
            if (source_digest(ROOT) != validation.get('source_sha256')
                    or (state.get('lekmap') is not False and tree_digest(ROOT / 'Lekmap') != validation.get('lekmap_source_sha256'))):
                raise RuntimeError('This checkout has changed. Update the installed game before playing.')
        checked('source', 'Lekmod version', source, 'Installed game matches this checkout', 'repair')
    def crossplay_setting():
        flag = app / crossplay.FLAG
        enabled = state.get('crossplay', {}).get('enabled', False)
        if enabled != desired:
            raise RuntimeError('Your cross-play preference will be applied before launch.')
        if desired and (not flag.is_file() or flag.read_text() != '403694 FINAL_RELEASE\n'
                        or crossplay.CORE_MARKER not in (app / CORE).read_bytes()):
            raise RuntimeError('Windows cross-play needs to be restored.')
        if not desired and flag.exists():
            raise RuntimeError('The cross-play setting needs to be removed before launch.')
    checked('crossplay', 'Windows cross-play', crossplay_setting,
            'Experimental mode enabled' if desired else 'Disabled · original Mac registration', 'repair')
    checked('signature', 'App integrity', lambda: verify_signature(app), 'App signature verified', 'repair')
    blocked = any(check['state'] == 'blocked' for check in checks)
    repairs = any(check['state'] == 'repair' for check in checks)
    result.update(ready=not blocked and not repairs and not result['running'],
                  repairable=not blocked and not result['running'])
    return result


def run_action(app, desired, action, log=lambda _: None):
    report = inspect(app, desired, log)
    if action == 'status':
        return report
    if report['running']:
        raise RuntimeError(report['running_message'])
    if not report['repairable']:
        raise RuntimeError('Resolve the checks marked “Needs attention”, then check again.')
    if action.startswith(('install-', 'uninstall-')):
        operation, component = action.split('-', 1)
        if component not in ('lekmod', 'lekmap'):
            raise ValueError('Unknown launcher component')
        if operation == 'install':
            installer.install(app, component=component, crossplay_enabled=desired, log=log)
        else:
            uninstall(app, component, log=log)
        return inspect(app, desired, log)
    if action == 'repair' or not report['ready']:
        log('Installing and repairing Lekmod. The previous game app will be kept as a backup…')
        installer.install(app, component='lekmod' if installed_state(app).get('lekmap') is False else 'both', crossplay_enabled=desired, log=log)
        report = inspect(app, desired, log)
    if not report['ready']:
        raise RuntimeError('Validation did not pass after repair. The game was not launched.')
    if action == 'launch':
        # Recheck under the same lock used by the installer, just before Steam handoff.
        with installation_lock(app):
            report = inspect(app, desired, log)
            if not report['ready']:
                raise RuntimeError('The installation changed before launch. Check it again.')
            ensure_closed()
            subprocess.run(['/usr/bin/open', 'steam://rungameid/8930'], check=True, timeout=30)
        report['launched'] = True
    return report


class Progress(io.TextIOBase):
    def write(self, text):
        if text.strip():
            # Installer helpers write to stdout; reserve it for structured messages.
            sys.__stdout__.write(json.dumps(dict(event='progress', message=text.strip())) + '\n')
            sys.__stdout__.flush()
        return len(text)
    def flush(self):
        sys.__stdout__.flush()


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('action', choices=('process-status', 'status', 'repair', 'launch', 'install-lekmod', 'uninstall-lekmod', 'install-lekmap', 'uninstall-lekmap'))
    parser.add_argument('--app', type=Path)
    parser.add_argument('--crossplay', choices=('on', 'off'))
    args = parser.parse_args()
    try:
        if args.action == 'process-status':
            emit('process-status', running=game_running())
            return 0
        saved = preferences()
        apps = detect_apps()
        selected = args.app or saved.get('selected_app') or (apps[0] if len(apps) == 1 else None)
        if selected is None:
            emit('result', apps=list(map(str, apps)), app='', checks=[], ready=False,
                 repairable=False, running=False, crossplay=False, version='',
                 message='Choose your Steam Civilization V installation to begin.')
            return 0
        app = app_path(selected)
        desired = None if args.crossplay is None else args.crossplay == 'on'
        saved = preferences(app, desired)
        desired = bool(saved['installations'][str(app)]['crossplay'])
        emit('selection', app=str(app), crossplay=desired)
        try:
            emit('artwork', background=background(app, CACHE))
            emit('logos', **logos(app, ROOT, CACHE))
        except (OSError, ValueError, subprocess.SubprocessError) as error:
            emit('progress', message='Menu artwork unavailable: ' + str(error))
        with redirect_stdout(Progress()):
            report = run_action(app, desired, args.action, log=print)
        report['apps'] = list(map(str, apps))
        emit('result', **report)
        return 0
    except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
        emit('error', message=str(error))
        return 1


if __name__ == '__main__':
    sys.exit(main())
