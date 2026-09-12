"""Remove selected components transactionally, retaining the previous game app."""
import json
import shutil

import crossplay
from game_install import (ASSETS, CORE, MANIFEST, STOCK_CORE_SHA256, ensure_closed,
                          installation_lock, installed_state, replace_app, sha256,
                          sign_app, validate_app, validate_core)


def stock_library(app):
    backups = app.parent / '.lekmod-backups'
    if backups.is_symlink():
        raise RuntimeError('Backup directory must not be a symbolic link.')
    for candidate in sorted(backups.glob(f'*/{app.name}/{CORE}')):
        if candidate.resolve().is_relative_to(backups.resolve()) and candidate.is_file():
            if sha256(candidate) == STOCK_CORE_SHA256:
                return candidate
    raise RuntimeError('The original game library was not found in the backups. '
                       'Restore Civilization V using Steam’s Verify Files before removing Lekmod.')


def remove(path):
    if path.is_symlink() or path.is_file():
        path.unlink()
    elif path.exists():
        shutil.rmtree(path)


def uninstall(app, component, log=print):
    if component not in ('lekmod', 'lekmap'):
        raise ValueError('Choose Lekmod or Lekmap to uninstall.')
    with installation_lock(app):
        ensure_closed()
        validate_app(app)
        validate_core(app)
        before_core = sha256(app / CORE)
        before_host = sha256(app / crossplay.HOST)
        stock = stock_library(app) if component == 'lekmod' and before_core != STOCK_CORE_SHA256 else None
        def populate(staged):
            validate_app(staged)
            state = installed_state(staged)
            validation = state.setdefault('validation', {})
            if component == 'lekmod':
                if stock:
                    shutil.copy2(stock, staged / CORE)
                if sha256(staged / CORE) != STOCK_CORE_SHA256:
                    raise RuntimeError('The original library could not be verified; the game was not changed.')
                for package in (staged / ASSETS / 'DLC').iterdir():
                    if package.name.upper().startswith('LEKMOD'):
                        remove(package)
                remove(staged / crossplay.FLAG)
                state.pop('crossplay', None)
                state.update(core_sha256=STOCK_CORE_SHA256, stock_core_sha256=STOCK_CORE_SHA256)
                for key in ('source_sha256', 'lekmod_sha256'):
                    validation.pop(key, None)
            else:
                remove(staged / ASSETS / 'Maps/Lekmap')
                for key in ('lekmap_sha256', 'lekmap_source_sha256'):
                    validation.pop(key, None)
            state.update(installer='lekmod-macos')
            state[component] = False
            (staged / MANIFEST).write_text(json.dumps(state, indent=2) + '\n')
            sign_app(staged)
            if sha256(app / CORE) != before_core or sha256(app / crossplay.HOST) != before_host:
                raise RuntimeError('Steam changed the game during removal. Please retry.')
            log(f'{component.capitalize()} removed; other components and saved games are preserved.')
        return replace_app(app, populate, log)
