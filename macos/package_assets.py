"""Prepare repository assets for the native Mac gameplay library."""
from pathlib import Path
import re
import shutil


def source_path(root, relative):
    """Resolve Windows UI mappings on case-sensitive Mac filesystems too."""
    current = root
    for part in relative.replace('\\', '/').split('/'):
        if part in ('', '.', '..'):
            raise RuntimeError(f'Invalid UI source path: {relative}')
        matches = [p for p in current.iterdir() if p.name.casefold() == part.casefold()]
        if not matches and part.casefold() == 'iconsupport.lua':
            matches = [p for p in current.iterdir()
                       if p.name.casefold() == 'iconsupport.lua.ignore']
        if len(matches) != 1:
            raise RuntimeError(f'Missing or ambiguous UI source: {relative}')
        current = matches[0]
    return current


def prepare_lekmod(source, destination):
    shutil.copytree(source, destination,
                    ignore=lambda _path, names: [n for n in names
                        if n.startswith('.') or Path(n).suffix.lower() in ('.dll', '.pdb', '.bat')])
    script = (source / 'ui_check.bat').read_text()
    mappings = re.findall(r'copy /y "%patchfolder%\\(Lua\\tmp\\ui\\[^"\n]+)" '
                          r'"%patchfolder%\\(Lua\\UI\\[^"\n]+)"', script, re.I)
    if not mappings:
        raise RuntimeError('No standard UI mappings found in ui_check.bat.')
    ui = destination / 'Lua/UI'
    keep = {'CityStatePersonalityHelper.lua', 'LegalScreen.lua', 'LegalScreen.xml'}
    for path in ui.iterdir():
        if path.is_file() and path.name not in keep:
            path.unlink()
    for src, dst in mappings:
        target = destination / dst.replace('\\', '/')
        if not target.resolve().is_relative_to(ui.resolve()):
            raise RuntimeError(f'Invalid UI destination: {dst}')
        shutil.copy2(source_path(destination, src), target)
    for subdir in ('UI', 'Utilities'):
        (destination / 'Lua' / subdir / 'LekmodUiConfigured.lua').write_text(
            'LekmodUiConfigured = true\n')
    frontend = ui / 'FrontEnd.lua'
    frontend.write_text(frontend.read_text().replace(
        'local LEKMOD_UI_CHECK_DONE = false', 'local LEKMOD_UI_CHECK_DONE = true'))
    # Aspyr's HttpRequest crashes natively; Lua pcall cannot catch it.
    version = destination / 'Lua/Utilities/Lekmod_version.lua'
    original = 'return Network ~= nil and type(Network.HttpRequest) == "function"'
    contents = version.read_text()
    if contents.count(original) != 1:
        raise RuntimeError('Mac HTTP compatibility patch no longer matches the version helper.')
    version.write_text(contents.replace(
        original, 'return false -- Native Mac HTTP requests are unsupported'))


def prepare_lekmap(source, destination):
    if not any(source.glob('Lekmap*.lua')):
        raise RuntimeError(f'No Lekmap scripts found in {source}')
    shutil.copytree(source, destination, ignore=shutil.ignore_patterns('.*'))
