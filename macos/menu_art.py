"""Read the installed Brave New World menu texture; no game artwork is shipped."""
import hashlib
from pathlib import Path
import struct
import subprocess
import tempfile
import zlib


def read_texture(archive, name):
    """Read one bounded entry from the Aspyr FPK version-6 texture directory."""
    size = archive.stat().st_size
    with archive.open('rb') as stream:
        header = stream.read(14)
        if len(header) != 14 or header[:10] != b'\x06\0\0\0FPK_\0\0':
            raise ValueError('Unsupported menu texture archive')
        count = struct.unpack_from('<I', header, 10)[0]
        if count > 100000:
            raise ValueError('Invalid texture directory size')
        for _ in range(count):
            length_data = stream.read(4)
            if len(length_data) != 4:
                break
            length = struct.unpack('<I', length_data)[0]
            if not 0 < length < 4096:
                break
            entry = stream.read(length)
            flag = stream.read(1)
            if not flag or flag[0] not in (0, 1, 2, 3):
                break
            metadata = flag + stream.read(15 + flag[0])
            if len(metadata) != 16 + flag[0]:
                break
            if entry.decode('ascii', errors='replace').lower() != name.lower():
                continue
            length, offset = struct.unpack_from('<II', metadata, 8 + flag[0])
            if not 128 <= length <= 32 * 1024 * 1024 or offset + length > size:
                break
            stream.seek(offset)
            texture = stream.read(length)
            if texture.startswith(b'DDS '):
                return texture
            break
    raise ValueError('The installed menu texture could not be read')


def background(app, cache):
    archive = app / 'Contents/Assets/Resource/DX9/Expansion2UITextures.fpk'
    stamp = archive.stat()
    key = hashlib.sha256(f'{archive}:{stamp.st_size}:{stamp.st_mtime_ns}'.encode()).hexdigest()[:20]
    cache.mkdir(parents=True, exist_ok=True)
    output = cache / f'menu-{key}.png'
    if output.is_file():
        return str(output)
    with tempfile.TemporaryDirectory(prefix='menu-', dir=cache) as temporary:
        dds = Path(temporary) / 'menu.dds'
        png = Path(temporary) / 'menu.png'
        dds.write_bytes(read_texture(archive, 'civilzationvatlas.dds'))
        subprocess.run(['/usr/bin/sips', '-s', 'format', 'png', str(dds), '--out', str(png)],
                       check=True, capture_output=True, timeout=30)
        png.replace(output)
    return str(output)


def rgba_texture(data, index=None):
    """Decode Civ's BGRA texture, optionally indexed in 2×2 pixel blocks."""
    height, width = struct.unpack_from('<II', data, 12)
    if not data.startswith(b'DDS ') or not 0 < width * height <= 16000000:
        raise ValueError('Invalid logo texture')
    if struct.unpack_from('<I', data, 88)[0] != 32 or len(data) != 128 + width * height * 4:
        raise ValueError('Unsupported logo pixel format')
    pixels = bytearray(data[128:])
    if index is not None:
        ih, iw = struct.unpack_from('<II', index, 12)
        if width % 2 or height % 2 or not 0 < iw * ih <= 4000000 or len(index) != 128 + iw * ih * 2:
            raise ValueError('Invalid logo index')
        output = bytearray(iw * ih * 16)
        for k, (block,) in enumerate(struct.iter_unpack('<H', index[128:])):
            if block >= width * height // 4:
                raise ValueError('Logo index outside texture')
            for dy in range(2):
                source = ((block // (width // 2) * 2 + dy) * width + block % (width // 2) * 2) * 4
                target = ((k // iw * 2 + dy) * iw * 2 + k % iw * 2) * 4
                output[target:target + 8] = pixels[source:source + 8]
        pixels, width, height = output, iw * 2, ih * 2
    pixels[0::4], pixels[2::4] = pixels[2::4], pixels[0::4]
    def chunk(kind, value):
        return struct.pack('>I', len(value)) + kind + value + struct.pack('>I', zlib.crc32(kind + value))
    raw = b''.join(b'\0' + pixels[y * width * 4:(y + 1) * width * 4] for y in range(height))
    return (b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', width, height, 8, 6, 0, 0, 0))
            + chunk(b'IDAT', zlib.compress(raw)) + chunk(b'IEND', b''))


def logos(app, root, cache):
    archive = app / 'Contents/Assets/Resource/DX9/Expansion2UITextures.fpk'
    cache.mkdir(parents=True, exist_ok=True)
    result = {'civ': str(app / 'Contents/Resources/App.icns')}
    for name, data in (
        ('bnw', rgba_texture(read_texture(archive, 'civilzationv_logo.dds'),
                             read_texture(archive, 'civilzationv_logo-index.dds'))),
        ('lekmod', rgba_texture((root / 'LEKMOD/Art/Lekmod (v 1)/UI/lekmodlogo.dds').read_bytes())),
    ):
        output = cache / f'{name}-{hashlib.sha256(data).hexdigest()[:20]}.png'
        if not output.exists():
            output.write_bytes(data)
        result[name] = str(output)
    return result
