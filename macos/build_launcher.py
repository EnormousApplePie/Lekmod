#!/usr/bin/env python3
"""Build and open the native launcher; the gameplay checkout stays beside it."""
import argparse
import hashlib
from pathlib import Path
import platform
import plistlib
import subprocess
import sys
import tempfile

HERE = Path(__file__).resolve().parent
APP = HERE / 'build/Lekmod Launcher.app'


def build():
    source = HERE / 'Launcher.swift'
    key = hashlib.sha256(source.read_bytes() + Path(__file__).read_bytes()
                         + str(HERE).encode() + sys.executable.encode()).hexdigest()
    stamp = APP / 'Contents/Resources/build-key'
    binary = APP / 'Contents/MacOS/Lekmod Launcher'
    if stamp.is_file() and stamp.read_text() == key and binary.is_file():
        return APP
    APP.parent.mkdir(parents=True, exist_ok=True)
    with tempfile.TemporaryDirectory(prefix='launcher-', dir=APP.parent) as temporary:
        output = Path(temporary) / 'Lekmod Launcher'
        print('Building Lekmod Launcher…', flush=True)
        subprocess.run(['xcrun', 'swiftc', '-O', '-target',
                        f'{platform.machine()}-apple-macosx13.0',
                        '-module-cache-path', str(HERE / 'build/swift-module-cache'),
                        str(source), '-o', str(output)], check=True)
        binary.parent.mkdir(parents=True, exist_ok=True)
        stamp.parent.mkdir(parents=True, exist_ok=True)
        iconset = Path(temporary) / 'Launcher.iconset'
        iconset.mkdir()
        subprocess.run([str(output), '--write-icons', str(iconset)], check=True)
        subprocess.run(['iconutil', '-c', 'icns', str(iconset), '-o',
                        str(APP / 'Contents/Resources/Launcher.icns')], check=True)
        output.replace(binary)
        info = dict(CFBundleExecutable='Lekmod Launcher', CFBundleIdentifier='org.lekmod.launcher',
                    CFBundleName='Lekmod Launcher', CFBundleDisplayName='Lekmod Launcher',
                    CFBundlePackageType='APPL', CFBundleVersion='1', CFBundleShortVersionString='1.0',
                    LSMinimumSystemVersion='13.0', NSHighResolutionCapable=True,
                    CFBundleIconFile='Launcher',
                    LekmodRepository=str(HERE.parent), LekmodPython=sys.executable)
        (APP / 'Contents/Info.plist').write_bytes(plistlib.dumps(info))
        stamp.write_text(key)
        subprocess.run(['codesign', '--force', '--sign', '-', str(APP)], check=True)
    return APP


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--open', action='store_true')
    args = parser.parse_args()
    try:
        app = build()
        print(app)
        if args.open:
            subprocess.run(['/usr/bin/open', str(app)], check=True)
        return 0
    except (OSError, subprocess.SubprocessError) as error:
        print(f'Could not open the launcher: {error}', file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main())
