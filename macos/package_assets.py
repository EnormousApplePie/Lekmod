"""Prepare repository assets for the native Mac gameplay library."""
from pathlib import Path
import re
import shutil
import sys


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


def prepare_lekmod(source, destination, eui=None):
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
    if eui is None:
        for src, dst in mappings:
            target = destination / dst.replace('\\', '/')
            if not target.resolve().is_relative_to(ui.resolve()):
                raise RuntimeError(f'Invalid UI destination: {dst}')
            shutil.copy2(source_path(destination, src), target)
    else:
        # Reuse the Windows compatibility rules with native paths and strict copies.
        root = str(Path(__file__).resolve().parent.parent)
        if root not in sys.path:
            sys.path.insert(0, root)
        from LekmodInstaller.ui_manager import UIManager

        class MacUIManager(UIManager):
            def _find_eui_folder(self, _civ5_path):
                return str(eui)

            def _copy_ui_file(self, lekmod_path, tmp_rel, ui_dest, dest_name=None):
                src = source_path(Path(lekmod_path), 'Lua/tmp/' + tmp_rel + '.ignore')
                shutil.copy2(src, Path(ui_dest) / (dest_name or Path(tmp_rel).name))
                return True

        MacUIManager().configure_ui_files(str(destination), 'Enhanced UI', lambda _: None)
        # ui_check.bat also overlays this helper; the Windows Python helper omits it.
        if (eui / 'Core/CityStateStatusHelper.lua').is_file():
            shutil.copy2(source_path(destination, 'Lua/tmp/eui/Core/CityStateStatusHelper.lua.ignore'),
                         ui / 'CityStateStatusHelper.lua')
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
