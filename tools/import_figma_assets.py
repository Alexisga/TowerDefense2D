"""Guarda los bytes PNG exportados por Figma; no recorta ni modifica las imágenes."""
from pathlib import Path
import base64
from PIL import Image

root=Path(__file__).resolve().parents[1]/'assets/figma'
for p in root.glob('*.b64'):
    data=base64.b64decode(p.read_text(encoding='ascii').strip(),validate=True)
    assert data[:8]==b'\x89PNG\r\n\x1a\n'
    target=p.with_suffix('.png')
    target.write_bytes(data)
    with Image.open(target) as im:
        im.verify()
    assert p.resolve().is_relative_to(root.resolve())
    p.unlink()
    print(target.name,len(data))
