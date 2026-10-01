#!/usr/bin/env python3
"""Create an isolated, ad-hoc-signed Mac app with Lekmod assets."""
import argparse
from pathlib import Path
import shutil
from audit import DEFAULT_APP, check_imports
from game_install import (ASSETS, CORE, STOCK_CORE_SHA256, clone_app,
                          ensure_closed, sha256, sign_app, sign_core, sign_nested, validate_app)
from package_assets import prepare_lekmod

HERE = Path(__file__).resolve().parent
DEST = HERE / 'build/Civilization V Lekmod.app'


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--app', type=Path, default=DEFAULT_APP)
    args = parser.parse_args()
    app = args.app.expanduser().resolve()
    ensure_closed()
    validate_app(app)
    if sha256(app / CORE) != STOCK_CORE_SHA256:
        raise RuntimeError('Installed Mac core differs from the build used for ABI validation.')
    library = HERE / 'build' / CORE.name
    check_imports(library, app)
    if not DEST.exists():
        clone_app(app, DEST)
    shutil.copy2(library, DEST / CORE)
    package = DEST / ASSETS / 'DLC/LEKMOD'
    if package.exists():
        shutil.rmtree(package)
    prepare_lekmod(HERE.parent / 'LEKMOD', package)
    sign_core(DEST)
    sign_nested(DEST)
    sign_app(DEST)
    print(f'Prepared {DEST}')


if __name__ == '__main__':
    main()
