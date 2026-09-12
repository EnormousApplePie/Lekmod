"""Launcher preferences live outside the Steam game installation."""
import json
from pathlib import Path
import tempfile

from game_install import installed_state

SUPPORT = Path.home() / 'Library/Application Support/Lekmod Launcher'


def preferences(app=None, desired=None, directory=SUPPORT):
    """Remember intent outside the game bundle, including after Steam restores it."""
    path = directory / 'settings.json'
    state = json.loads(path.read_text()) if path.is_file() else {}
    if not isinstance(state, dict) or not isinstance(state.get('installations', {}), dict):
        raise RuntimeError('Launcher settings are invalid. Open the settings folder to inspect settings.json.')
    if app is not None:
        records = state.setdefault('installations', {})
        record = records.setdefault(str(app), {})
        if not isinstance(record, dict):
            raise RuntimeError('Invalid saved installation settings.')
        if 'crossplay' not in record:
            try:
                record['crossplay'] = bool(installed_state(app).get('crossplay', {}).get('enabled'))
            except RuntimeError:
                record['crossplay'] = False
        if desired is not None:
            record['crossplay'] = desired
        state['selected_app'] = str(app)
        directory.mkdir(parents=True, exist_ok=True)
        with tempfile.NamedTemporaryFile(mode='w', dir=directory, delete=False) as temporary:
            json.dump(state, temporary, indent=2)
            temporary.write('\n')
        try:
            Path(temporary.name).replace(path)
        finally:
            Path(temporary.name).unlink(missing_ok=True)
    return state
