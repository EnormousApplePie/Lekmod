"""Steam discovery and transactional replacement of a macOS Civ V app."""
from contextlib import contextmanager
from datetime import datetime, timezone
import fcntl
import hashlib
import json
from pathlib import Path
import re
import shutil
import subprocess
import tempfile

CORE = Path('Contents/MacOS/libCvGameCoreDLL_Expansion2_DLL.dylib')
ASSETS = Path('Contents/Assets/Assets')
MANIFEST = Path('Contents/Resources/lekmod-install.json')
STOCK_CORE_SHA256 = '0da6a5ffc283c3f147b20a7ec426e4ed85a6838ab891faf61b50af4e25c4a09c'


def read_vdf(path):
    """Read Steam's quoted KeyValues files, including escaped library paths."""
    tokens = re.finditer(r'"((?:\\.|[^"\\])*)"|([{}])|//[^\n]*',
                        path.read_text(encoding='utf-8-sig'))
    root, stack, key = {}, [], None
    current = root
    for token in tokens:
        value, brace = token.groups()
        if value is None and brace is None:
            continue
        if brace == '{':
            if key is None:
                raise ValueError('Unexpected opening brace')
            child = {}
            current[key] = child
            stack.append(current)
            current, key = child, None
        elif brace == '}':
            if not stack or key is not None:
                raise ValueError('Unexpected closing brace')
            current = stack.pop()
        else:
            value = re.sub(r'\\([\\"])', r'\1', value)
            if key is None:
                key = value
            else:
                current[key], key = value, None
    if stack or key is not None:
        raise ValueError('Incomplete Steam metadata')
    return root


def app_path(path):
    path = Path(path).expanduser().resolve()
    if path.suffix.lower() != '.app':
        path /= 'Civilization V.app'
    return path


def validate_app(app):
    for relative in (CORE, Path('Contents/MacOS/Civilization V')):
        if not (app / relative).is_file():
            raise RuntimeError(f'Not a supported Mac Civilization V installation: {app}')
    for relative in (ASSETS / 'DLC', ASSETS / 'Maps'):
        if not (app / relative).is_dir():
            raise RuntimeError(f'Missing game directory: {app / relative}')
    # Never write through bundle links to files outside the staged copy.
    for relative in (CORE, ASSETS, ASSETS / 'DLC', ASSETS / 'Maps', MANIFEST):
        target = app / relative
        if not target.resolve().is_relative_to(app.resolve()):
            raise RuntimeError(f'Game path points outside its app bundle: {target}')


def detect_apps(steam_root=None):
    steam_root = Path(steam_root or Path.home() / 'Library/Application Support/Steam')
    libraries = [steam_root]
    for metadata in (steam_root / 'steamapps/libraryfolders.vdf',
                     steam_root / 'config/libraryfolders.vdf'):
        if not metadata.is_file():
            continue
        try:
            entries = next((value for key, value in read_vdf(metadata).items()
                            if key.lower() == 'libraryfolders'), {})
            for key, value in entries.items():
                if key.isdigit():
                    folder = value.get('path') if isinstance(value, dict) else value
                    if folder:
                        libraries.append(Path(folder).expanduser())
        except (OSError, ValueError, AttributeError):
            continue  # A stale library file must not hide the default installation.
    found = []
    for library in libraries:
        steamapps = library / 'steamapps'
        install_dir = "Sid Meier's Civilization V"
        manifest = steamapps / 'appmanifest_8930.acf'
        if manifest.is_file():
            try:
                install_dir = read_vdf(manifest)['AppState']['installdir']
                if not isinstance(install_dir, str) or Path(install_dir).name != install_dir:
                    continue
            except (OSError, ValueError, KeyError, TypeError):
                continue
        app = app_path(steamapps / 'common' / install_dir)
        try:
            validate_app(app)
        except RuntimeError:
            continue
        if app not in found:
            found.append(app)
    return found


def sha256(path):
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(chunk)
    return digest.hexdigest()


def installed_state(app):
    path = app / MANIFEST
    if not path.exists():
        return {}
    try:
        state = json.loads(path.read_text())
        if not isinstance(state, dict) or state.get('installer') != 'lekmod-macos':
            raise ValueError('Unknown manifest format')
        return state
    except (ValueError, OSError) as error:
        raise RuntimeError(f'Invalid installation record: {path}: {error}') from error


def validate_core(app):
    digest = sha256(app / CORE)
    if digest == STOCK_CORE_SHA256:
        return
    state = installed_state(app)
    if (state.get('stock_core_sha256') == STOCK_CORE_SHA256
            and state.get('core_sha256') == digest):
        return
    raise RuntimeError('The installed gameplay library is not the validated Aspyr build '
                       '180925 or a library installed by this installer. Restore the stock '
                       'game through Steam before installing Lekmod.')


def ensure_closed():
    result = subprocess.run(['pgrep', '-x', 'Civilization V'], capture_output=True, text=True)
    if result.returncode == 0:
        raise RuntimeError('Close Civilization V before installing.')
    if result.returncode != 1:
        raise RuntimeError('Cannot check whether Civilization V is running: ' + result.stderr)


def clone_app(source, destination):
    result = subprocess.run(['cp', '-cR', str(source), str(destination)],
                            capture_output=True, text=True)
    if result.returncode:
        # Cross-volume and non-APFS installs cannot use copy-on-write cloning.
        if destination.exists():
            shutil.rmtree(destination)
        shutil.copytree(source, destination, symlinks=True)


def sign_core(app):
    subprocess.run(['codesign', '--force', '--sign', '-', str(app / CORE)], check=True)


def sign_nested(app):
    # Some Steam-shipped binaries have stale signatures. Repair them before
    # sealing the outer bundle; never follow a link outside the staged app.
    for binary in (app / 'Contents/MacOS').iterdir():
        if binary.suffix != '.dylib' and binary.name not in ('AppBundleExe', 'Civilization V'):
            continue
        if not binary.resolve().is_relative_to(app.resolve()):
            raise RuntimeError(f'Native library points outside the app: {binary}')
        result = subprocess.run(['codesign', '--verify', '--strict', str(binary)],
                                capture_output=True, text=True)
        if result.returncode:
            subprocess.run(['codesign', '--force', '--sign', '-', str(binary)], check=True)


def sign_app(app):
    subprocess.run(['codesign', '--force', '--sign', '-', str(app)], check=True)
    subprocess.run(['codesign', '--verify', '--strict', str(app)], check=True)


@contextmanager
def installation_lock(app):
    # Lock the parent directory itself, leaving no lock files in Steam's library.
    with open_directory(app.parent) as descriptor:
        try:
            fcntl.flock(descriptor, fcntl.LOCK_EX | fcntl.LOCK_NB)
        except BlockingIOError as error:
            raise RuntimeError('Another installer is using this game directory.') from error
        try:
            yield
        finally:
            fcntl.flock(descriptor, fcntl.LOCK_UN)


@contextmanager
def open_directory(path):
    import os
    descriptor = os.open(path, os.O_RDONLY)
    try:
        yield descriptor
    finally:
        os.close(descriptor)


def replace_app(app, populate, log=print):
    """Prepare and sign off to the side; retain the entire previous app as backup."""
    with tempfile.TemporaryDirectory(prefix='.lekmod-stage-', dir=app.parent) as temporary:
        staged = Path(temporary) / app.name
        log('Preparing installation…')
        clone_app(app, staged)
        populate(staged)
        ensure_closed()
        backup_root = app.parent / '.lekmod-backups'
        backup_root.mkdir(exist_ok=True)
        if backup_root.is_symlink():
            raise RuntimeError('Backup directory must not be a symbolic link.')
        timestamp = datetime.now(timezone.utc).strftime('%Y%m%dT%H%M%SZ-')
        backup_dir = Path(tempfile.mkdtemp(prefix=timestamp, dir=backup_root))
        backup = backup_dir / app.name
        app.rename(backup)
        try:
            staged.rename(app)
        except BaseException:
            try:
                backup.rename(app)
            except OSError as error:
                raise RuntimeError(f'Could not restore the app automatically. '
                                   f'Your previous app is saved at {backup}') from error
            raise
        log(f'Backup: {backup}')
        return backup
