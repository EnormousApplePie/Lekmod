#!/usr/bin/env python3
"""Check that each game-engine import is exported by the installed Mac host."""
from pathlib import Path
import subprocess

DEFAULT_APP = Path.home() / "Library/Application Support/Steam/steamapps/common/Sid Meier's Civilization V/Civilization V.app"

def check_imports(library, app=DEFAULT_APP):
    host = app / "Contents/MacOS/Civilization V"
    exports = {line.split()[-1] for line in subprocess.check_output(
        ["nm", "-gU", str(host)], text=True).splitlines() if line.strip()}
    imports = [line.split(" (dynamically")[0].split()[-1] for line in subprocess.check_output(
        ["nm", "-m", "-u", str(library)], text=True).splitlines() if "dynamically looked up" in line]
    missing = sorted(set(imports) - exports)
    if missing:
        demangled = subprocess.check_output(["c++filt"], input="\n".join(missing), text=True)
        raise RuntimeError("Imports absent from the Mac host:\n" + demangled)
    print(f"Verified {len(imports)} engine imports against {host}")
    check_pregame_abi(library)
    return imports



def check_pregame_abi(library):
    """Check Mac 180925 pre-game slots in the linked binary, not just class size."""
    import struct
    data = Path(library).read_bytes()
    if struct.unpack_from('<I', data)[0] != 0xfeedfacf:
        raise RuntimeError('Expected a thin 64-bit Mach-O gameplay library')
    symbols = {}
    for line in subprocess.check_output(['nm', '-n', str(library)], text=True).splitlines():
        parts = line.split()
        if len(parts) == 3 and parts[0] != 'U':
            try:
                symbols[parts[2]] = int(parts[0], 16)
            except ValueError:
                pass
    segments = []
    offset = 32
    for _ in range(struct.unpack_from('<I', data, 16)[0]):
        command, size = struct.unpack_from('<II', data, offset)
        if command == 0x19:  # LC_SEGMENT_64
            vmaddr, _, fileoff, filesize = struct.unpack_from('<QQQQ', data, offset + 24)
            segments.append((vmaddr, fileoff, filesize))
        offset += size
    address = symbols['__ZTV12CvDllPreGame'] + 16  # skip offset-to-top and RTTI
    table = next(fileoff + address - vmaddr for vmaddr, fileoff, filesize in segments
                 if vmaddr <= address < vmaddr + filesize)
    # Anchors reconstructed from the stock Aspyr 180925 vtable and host callers.
    expected = {
        28: '__ZN12CvDllPreGame6eraKeyEv',
        29: '__ZN12CvDllPreGame20findPlayerByNicknameEPKc',
        185: '__ZN12CvDllPreGame16setVersionStringERKNSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE',
        197: '__ZN12CvDllPreGame13versionStringEv',
        200: '__ZN12CvDllPreGame5writeER11FDataStream',
        217: '__ZN12CvDllPreGame22ReseatConnectedPlayersEv',
    }
    for slot, name in expected.items():
        actual = struct.unpack_from('<Q', data, table + slot * 8)[0]
        if actual != symbols.get(name):
            raise RuntimeError(f'Mac pre-game ABI mismatch at slot {slot}: expected {name}')
    print(f'Verified {len(expected)} Mac pre-game vtable anchors')


if __name__ == "__main__":
    check_imports(Path(__file__).parent / "build/libCvGameCoreDLL_Expansion2_DLL.dylib")
