#!/usr/bin/env python3
"""Install this checkout's native Lekmod and/or Lekmap into Civilization V."""
import argparse
from contextlib import nullcontext
from datetime import datetime, timezone
import json
from pathlib import Path
import shutil
import subprocess
import sys
import eui

from audit import check_imports
from game_install import (ASSETS, CORE, MANIFEST, STOCK_CORE_SHA256, app_path,
                          detect_apps, ensure_closed, installation_lock,
                          installed_state, replace_app, sha256, sign_app, sign_core, sign_nested,
                          validate_app, validate_core)
from package_assets import prepare_lekmap, prepare_lekmod
from crossplay import configure_staged as configure_crossplay
from integrity import source_digest, tree_digest

HERE = Path(__file__).resolve().parent
ROOT = HERE.parent


def preflight(app, component):
    validate_app(app)
    if component in ('lekmod', 'both', 'eui'):
        validate_core(app)
        eui.guard(app, installed_state(app))
        if not (ROOT / 'LEKMOD/ui_check.bat').is_file():
            raise RuntimeError('The checkout is missing LEKMOD assets.')
    if component in ('lekmap', 'both') and not any((ROOT / 'Lekmap').glob('Lekmap*.lua')):
        raise RuntimeError('The checkout is missing Lekmap scripts.')


def describe(app, component):
    lines = [f'Game: {app}']
    if component in ('lekmod', 'both'):
        lines += [f'Lekmod: {app / ASSETS / "DLC/LEKMOD"}',
                  f'Native library: {app / CORE}']
        existing = [p.name for p in (app / ASSETS / 'DLC').iterdir()
                    if p.name.upper().startswith('LEKMOD')]
        if existing:
            lines.append('Replace existing Lekmod packages: ' + ', '.join(sorted(existing)))
    if component in ('lekmap', 'both'):
        lines.append(f'Lekmap: {app / ASSETS / "Maps/Lekmap"}')
    lines += ['Uses the versions in this checkout.',
              f'Previous app retained under: {app.parent / ".lekmod-backups"}',
              'Steam updates or Verify Files may undo the installation.']
    return '\n'.join(lines)


def install(app, component='both', jobs=4, skip_build=False, log=print,
            crossplay_enabled=None, eui_enabled=None):
    with installation_lock(app):
        ensure_closed()
        preflight(app, component)
        library = HERE / 'build' / CORE.name
        mod = component in ('lekmod', 'both')
        maps = component in ('lekmap', 'both')
        state = installed_state(app)
        ui_only = component == 'eui'
        assets = mod or (ui_only and state.get('lekmod'))
        enabled = bool(state.get('eui') or eui.existing(app)) if eui_enabled is None else eui_enabled
        archive = eui.read_archive() if enabled and (mod or ui_only) else None
        before_source = source_digest(ROOT) if assets else None
        before_maps = tree_digest(ROOT / 'Lekmap') if maps else None
        if mod:
            if not skip_build:
                log('Building native Lekmod (first build may take several minutes)…')
                subprocess.run([sys.executable, str(HERE / 'build.py'), '--release',
                                '--jobs', str(jobs), '--app', str(app)], check=True)
            if not library.is_file():
                raise RuntimeError('Native library missing. Run without --skip-build to build it.')
            check_imports(library, app)
        ensure_closed()
        # Revalidate after the build in case Steam changed the installation.
        preflight(app, component)
        before_core = sha256(app / CORE)
        before_host = sha256(app / 'Contents/MacOS/Civilization V')

        def populate(staged):
            validate_app(staged)
            state = installed_state(staged)
            if (not mod and before_core != STOCK_CORE_SHA256
                    and state.get('core_sha256') != before_core):
                # A maps-only update must not bless an independently replaced core.
                state.pop('stock_core_sha256', None)
                state.pop('lekmod', None)
            eui_folder = None
            if mod or ui_only:
                eui_folder = eui.prepare(staged, state, enabled, archive)
            if assets:
                # Only modify the staged bundle; the previous packages remain in the backup.
                for package in (staged / ASSETS / 'DLC').iterdir():
                    if package.name.upper().startswith('LEKMOD'):
                        if package.is_symlink() or package.is_file():
                            package.unlink()
                        else:
                            shutil.rmtree(package)
                prepare_lekmod(ROOT / 'LEKMOD', staged / ASSETS / 'DLC/LEKMOD', eui=eui_folder)
            if mod:
                shutil.copy2(library, staged / CORE)
                sign_core(staged)
                state.update(stock_core_sha256=STOCK_CORE_SHA256,
                             core_sha256=sha256(staged / CORE), lekmod=True)
            if maps:
                destination = staged / ASSETS / 'Maps/Lekmap'
                if destination.is_symlink() or destination.is_file():
                    destination.unlink()
                elif destination.exists():
                    shutil.rmtree(destination)
                prepare_lekmap(ROOT / 'Lekmap', destination)
                state['lekmap'] = True
            if mod and (crossplay_enabled is not None or state.get('crossplay', {}).get('enabled')):
                crossplay_mode = (crossplay_enabled if crossplay_enabled is not None
                                 else state['crossplay']['enabled'])
                configure_crossplay(staged, state, crossplay_mode, repair=crossplay_enabled is not None)
            log('Recording installation fingerprints…')
            validation = state.setdefault('validation', {})
            if assets:
                if source_digest(ROOT) != before_source:
                    raise RuntimeError('The checkout changed during installation. Please retry.')
                validation.update(format=1, host_sha256=before_host,
                                  ui='eui' if enabled else 'standard',
                                  lekmod_sha256=tree_digest(staged / ASSETS / 'DLC/LEKMOD'))
                if mod:
                    # A UI-only switch must not mark an older native library up to date.
                    validation['source_sha256'] = before_source
            if maps:
                if tree_digest(ROOT / 'Lekmap') != before_maps:
                    raise RuntimeError('Lekmap changed during installation. Please retry.')
                validation.update(lekmap_sha256=tree_digest(staged / ASSETS / 'Maps/Lekmap'),
                                  lekmap_source_sha256=before_maps)
            sign_nested(staged)
            if before_core == STOCK_CORE_SHA256:
                state['stock_core_sha256'] = STOCK_CORE_SHA256
            state['core_sha256'] = sha256(staged / CORE)
            state.update(installer='lekmod-macos',
                         installed_at=datetime.now(timezone.utc).isoformat())
            manifest = staged / MANIFEST
            manifest.parent.mkdir(parents=True, exist_ok=True)
            manifest.write_text(json.dumps(state, indent=2) + '\n')
            sign_app(staged)
            if (sha256(app / CORE) != before_core
                    or sha256(app / 'Contents/MacOS/Civilization V') != before_host):
                raise RuntimeError('Steam changed the game during installation. Please retry.')

        update_text = (mod or ui_only) and (enabled or ui_only or state.get('eui'))
        with eui.text_files(enabled, archive) if update_text else nullcontext():
            return replace_app(app, populate, log)


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--app', type=Path, help='Civilization V.app or its containing folder')
    parser.add_argument('--component', choices=('lekmod', 'lekmap', 'both'), default='both')
    parser.add_argument('--jobs', type=int, default=4, help='Parallel compiler jobs (default: 4)')
    parser.add_argument('--skip-build', action='store_true', help='Use the existing native build')
    parser.add_argument('--list', action='store_true', help='List detected games without changes')
    parser.add_argument('--dry-run', action='store_true', help='Validate and show paths without changes')
    parser.add_argument('--gui', action='store_true', help='Open the native Lekmod launcher')
    args = parser.parse_args(argv)
    try:
        if sys.platform != 'darwin':
            raise RuntimeError('This installer requires macOS.')
        if args.jobs < 1:
            raise RuntimeError('--jobs must be at least 1.')
        if args.list:
            for app in detect_apps():
                print(app)
            return 0
        if args.gui and args.dry_run:
            raise RuntimeError('Use --dry-run without --gui.')
        if args.gui:
            from build_launcher import build
            if args.app:
                from launcher import preferences
                preferences(app_path(args.app))
            subprocess.run(['/usr/bin/open', str(build())], check=True)
            return 0
        else:
            apps = [app_path(args.app)] if args.app else detect_apps()
            if len(apps) != 1:
                raise RuntimeError('No unique Civilization V installation found. '
                                   'Use --list, then --app "/path/to/Civilization V.app".')
            app, component = apps[0], args.component
        preflight(app, component)
        print(describe(app, component), flush=True)
        if args.dry_run:
            return 0
        backup = install(app, component, args.jobs, args.skip_build)
        message = (f'Installation complete. Launch Civilization V through Steam.\n\n'
                   f'Previous app backup:\n{backup}')
        print(message)
        return 0
    except (OSError, RuntimeError, subprocess.CalledProcessError) as error:
        print(f'Installation failed: {error}', file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main())
