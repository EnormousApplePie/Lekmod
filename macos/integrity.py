"""Content fingerprints used by the launcher and transactional installer."""
import hashlib
from pathlib import Path

from game_install import sha256


def _fingerprint(files):
    digest = hashlib.sha256()
    for name, path in sorted(files):
        if path.is_symlink():
            raise RuntimeError(f'Unexpected symbolic link: {path}')
        digest.update(name.encode('utf-8') + b'\0')
        digest.update(sha256(path).encode('ascii') + b'\n')
    return digest.hexdigest()


def tree_digest(root):
    root = Path(root)
    if not root.is_dir() or root.is_symlink():
        raise RuntimeError(f'Missing or linked content directory: {root}')
    files = []
    for path in root.rglob('*'):
        if path.is_symlink():
            raise RuntimeError(f'Unexpected symbolic link: {path}')
        if path.is_file() and not any(part.startswith('.') for part in path.relative_to(root).parts):
            files.append((path.relative_to(root).as_posix(), path))
    if not files:
        raise RuntimeError(f'Empty content directory: {root}')
    return _fingerprint(files)


def source_digest(root):
    """Track build inputs and packaged assets, independent of Git commit state."""
    files = []
    for directory in ('LEKMOD', 'LEKMOD_DLL/CvGameCoreDLL_Expansion2', 'macos/include'):
        base = root / directory
        if not base.is_dir():
            raise RuntimeError(f'Missing checkout directory: {base}')
        for path in base.rglob('*'):
            relative = path.relative_to(root)
            if any(part.startswith('.') or part in ('BuildOutput', 'BuildTemp', 'Debug', 'Release')
                   for part in relative.parts):
                continue
            if path.suffix.lower() in ('.dll', '.pdb', '.obj', '.log', '.exe'):
                continue
            if path.is_symlink():
                raise RuntimeError(f'Unexpected source link: {path}')
            if path.is_file():
                files.append((relative.as_posix(), path))
    for name in ('build.py', 'package_assets.py', 'crossplay.py', 'integrity.py', 'eui.py'):
        path = root / 'macos' / name
        files.append(('macos/' + name, path))
    path = root / 'LekmodInstaller/ui_manager.py'
    files.append(('LekmodInstaller/ui_manager.py', path))
    return _fingerprint(files)
