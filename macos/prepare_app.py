#!/usr/bin/env python3
"""Create an isolated, ad-hoc-signed Mac app with Lekmod assets."""
from pathlib import Path
import hashlib
import re
import shutil
import subprocess
from audit import DEFAULT_APP, check_imports

HERE = Path(__file__).resolve().parent
DEST = HERE / 'build/Civilization V Lekmod.app'
STOCK_CORE_SHA256 = '0da6a5ffc283c3f147b20a7ec426e4ed85a6838ab891faf61b50af4e25c4a09c'

def main():
    running = subprocess.run(['pgrep', '-x', 'Civilization V'], capture_output=True, text=True)
    if running.returncode == 0:
        raise RuntimeError('Close Civilization V before updating the test app.')
    if running.returncode != 1:
        raise RuntimeError('Cannot verify that the game is closed: ' + running.stderr)
    library = HERE / 'build/libCvGameCoreDLL_Expansion2_DLL.dylib'
    stock_core = DEFAULT_APP / 'Contents/MacOS/libCvGameCoreDLL_Expansion2_DLL.dylib'
    if hashlib.sha256(stock_core.read_bytes()).hexdigest() != STOCK_CORE_SHA256:
        raise RuntimeError('Installed Mac core differs from the build used for ABI validation.')
    check_imports(library)
    if not DEST.exists():
        subprocess.run(['cp', '-cR', str(DEFAULT_APP), str(DEST)], check=True)
    shutil.copy2(library, DEST / 'Contents/MacOS' / stock_core.name)
    dlc = DEST / 'Contents/Assets/Assets/DLC'
    if not dlc.is_dir():
        raise RuntimeError('Expected Mac DLC directory is missing')
    package = dlc / 'LEKMOD'
    if package.exists():
        shutil.rmtree(package)
    shutil.copytree(HERE.parent / 'LEKMOD', package,
                    ignore=shutil.ignore_patterns('*.dll','*.pdb','*.bat'))
    # Select exactly the stock UI source/destination mappings from ui_check.bat.
    script = (HERE.parent / 'LEKMOD/ui_check.bat').read_text()
    mappings = re.findall(r'copy /y "%patchfolder%\\(Lua\\tmp\\ui\\[^"\n]+)" "%patchfolder%\\(Lua\\UI\\[^"\n]+)"',script,re.I)
    ui = package / 'Lua/UI'
    keep = {'CityStatePersonalityHelper.lua','LegalScreen.lua','LegalScreen.xml'}
    for path in ui.iterdir():
        if path.is_file() and path.name not in keep:
            path.unlink()
    for src, dst in mappings:
        source = package / src.replace('\\','/')
        if not source.exists() and source.name == 'IconSupport.lua':
            source = source.with_name('IconSupport.lua.ignore')
        shutil.copy2(source, package / dst.replace('\\','/'))
    for subdir in ['UI','Utilities']:
        (package / 'Lua' / subdir / 'LekmodUiConfigured.lua').write_text('LekmodUiConfigured = true\n')
    frontend=ui/'FrontEnd.lua'
    frontend.write_text(frontend.read_text().replace('local LEKMOD_UI_CHECK_DONE = false','local LEKMOD_UI_CHECK_DONE = true'))
    # Aspyr exposes Network.HttpRequest but crashes in HINTERNETConnect_Mac.
    # pcall cannot catch a native fault; use the menu's unavailable-check state.
    version = package / 'Lua/Utilities/Lekmod_version.lua'
    original = 'return Network ~= nil and type(Network.HttpRequest) == "function"'
    contents = version.read_text()
    if contents.count(original) != 1:
        raise RuntimeError('Mac HTTP compatibility patch no longer matches the version helper')
    version.write_text(contents.replace(original, 'return false -- Native Mac HTTP requests are unsupported'))
    subprocess.run(['codesign','--force','--sign','-',str(DEST/'Contents/MacOS'/stock_core.name)],check=True)
    subprocess.run(['codesign','--force','--sign','-',str(DEST)],check=True)
    print(f'Prepared {DEST} with {len(mappings)} stock UI mappings')

if __name__ == '__main__':
    main()
