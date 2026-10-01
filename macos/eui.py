"""Pinned EUI assets, kept outside Steam for offline installation and repair."""
import hashlib
import io
from contextlib import contextmanager
from pathlib import Path, PurePosixPath
import shutil
import tempfile
import zipfile

from game_install import ASSETS, sha256
from integrity import tree_digest
from launcher_preferences import SUPPORT

VERSION = '1.28g'
# Original 1.28g release: CivFanatics resource 24303, version 22637.
ARCHIVE_SHA256 = '772a3dae6c512725f6bb5d51c70a4ed8bcf70ca0fc454a1c5019448f3ff666e5'
TREE_SHA256 = '62b412f7a5a7aabc6dfc0dd0ed301c65a4bb27dcb88f5c25a8cd7cc49034915e'
TEXT_SHA256 = '7d6af624ea695be3afde63ef2d493c6b7ac97820babc137e72cd7aa06651e40c'
PACKAGE = ASSETS / 'DLC/UI_bc1'
ARCHIVE = SUPPORT / 'eui_v1_28g.zip'
TEXT = Path.home() / "Library/Application Support/Sid Meier's Civilization 5/Text/EUI_text_en_us.xml"


def read_archive(path=None):
    path = Path(path) if path is not None else ARCHIVE
    if not path.is_file() or path.is_symlink() or path.stat().st_size > 8 * 1024 * 1024:
        raise RuntimeError('Choose the original EUI 1.28g ZIP using Install EUI.')
    data = path.read_bytes()
    if hashlib.sha256(data).hexdigest() != ARCHIVE_SHA256:
        raise RuntimeError('This is not the supported EUI 1.28g archive. Use the original ZIP from CivFanatics.')
    return data


def cached():
    try:
        return ARCHIVE.is_file() and not ARCHIVE.is_symlink() and sha256(ARCHIVE) == ARCHIVE_SHA256
    except OSError:
        return False


def import_archive(path):
    data = read_archive(path)
    ARCHIVE.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.NamedTemporaryFile(dir=ARCHIVE.parent, delete=False) as stream:
        stream.write(data)
    try:
        Path(stream.name).replace(ARCHIVE)
    finally:
        Path(stream.name).unlink(missing_ok=True)


@contextmanager
def text_files(enabled, data):
    """Roll back the user Text file if the app transaction fails too."""
    if TEXT.is_symlink() or TEXT.parent.is_symlink():
        raise RuntimeError('The EUI text path must not be a symbolic link.')
    original = TEXT.read_bytes() if TEXT.exists() else None
    if enabled:
        with zipfile.ZipFile(io.BytesIO(data)) as archive:
            replacement = archive.read(TEXT.name)
        if original is not None and original != replacement:
            raise RuntimeError(f'A different EUI text file already exists. Move it aside first: {TEXT}')
    else:
        # Preserve translations or edits not installed by this launcher.
        replacement = None if original is not None and hashlib.sha256(original).hexdigest() == TEXT_SHA256 else original
    if original == replacement:
        yield
        return

    def write(content):
        if content is None:
            TEXT.unlink(missing_ok=True)
        else:
            TEXT.parent.mkdir(parents=True, exist_ok=True)
            with tempfile.NamedTemporaryFile(dir=TEXT.parent, delete=False) as stream:
                stream.write(content)
            try:
                Path(stream.name).replace(TEXT)
            finally:
                Path(stream.name).unlink(missing_ok=True)
    write(replacement)
    try:
        yield
    except BaseException:
        write(original)
        raise


def existing(app):
    return [p for p in (app / ASSETS / 'DLC').iterdir()
            if p.name.casefold() in ('ui_bc1', 'ui_bc1_xits')]


def guard(app, state):
    """Never replace an unrelated EUI installation silently."""
    folders = existing(app)
    if not folders:
        return
    record = state.get('eui') or {}
    if not isinstance(record, dict):
        raise RuntimeError('Invalid EUI installation record.')
    if (folders != [app / PACKAGE] or folders[0].is_symlink()
            or not folders[0].is_dir()):
        raise RuntimeError('Move the existing UI_bc1/UI_bc1_xits installation out of DLC before installing EUI 1.28g.')
    digest = tree_digest(folders[0])
    if record.get('archive_sha256') != ARCHIVE_SHA256 and digest != TREE_SHA256:
        raise RuntimeError('An unmanaged or unsupported EUI version is installed. Move it out of DLC, then use Install EUI.')


def check(app, state, desired):
    record = state.get('eui') or {}
    if not desired:
        if existing(app) or record or (state.get('lekmod') and state.get('validation', {}).get('ui') == 'eui'):
            raise RuntimeError('Repair will restore the standard UI.')
        return
    if (not isinstance(record, dict) or record.get('version') != VERSION
            or record.get('archive_sha256') != ARCHIVE_SHA256
            or tree_digest(app / PACKAGE) != record.get('sha256')):
        raise RuntimeError('EUI files are missing or changed. Repair will restore EUI 1.28g.')
    if state.get('lekmod') and state.get('validation', {}).get('ui') != 'eui':
        raise RuntimeError('Lekmod needs its EUI compatibility files. Repair before playing.')
    if not TEXT.is_file() or TEXT.is_symlink() or sha256(TEXT) != TEXT_SHA256:
        raise RuntimeError('EUI text resources are missing or changed. Repair before playing.')


def prepare(staged, state, enabled, data=None):
    destination = staged / PACKAGE
    if destination.exists():
        shutil.rmtree(destination)
    if not enabled:
        state.pop('eui', None)
        return None
    # Only a checksum-verified, fixed release is accepted, even during repairs.
    if data is None or hashlib.sha256(data).hexdigest() != ARCHIVE_SHA256:
        raise RuntimeError('The EUI archive has not been verified.')
    with zipfile.ZipFile(io.BytesIO(data)) as archive:
        for member in archive.infolist():
            path = PurePosixPath(member.filename)
            if path.is_absolute() or '..' in path.parts or '\\' in member.filename:
                raise RuntimeError('Invalid EUI archive path.')
            if member.is_dir() or not path.parts or path.parts[0] != 'UI_bc1':
                continue
            if member.external_attr >> 16 & 0o170000 == 0o120000:
                raise RuntimeError('EUI archive contains a symbolic link.')
            target = destination.joinpath(*path.parts[1:])
            target.parent.mkdir(parents=True, exist_ok=True)
            target.write_bytes(archive.read(member))
    digest = tree_digest(destination)
    if digest != TREE_SHA256:
        raise RuntimeError('EUI package contents differ from the supported release.')
    state['eui'] = dict(version=VERSION, archive_sha256=ARCHIVE_SHA256, sha256=digest)
    return destination
