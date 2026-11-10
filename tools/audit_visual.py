"""Audita capturas reales, archivos exportados y conservación de lógica; no edita imágenes."""
import hashlib
import json
from pathlib import Path
import re
import subprocess
import argparse

from PIL import Image

ROOT = Path(__file__).resolve().parents[1]
EVIDENCE = ROOT / "docs/evidence/visual"
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--baseline", default="813285dc8922b76d1f2fe49ad6289fa2d5ef5213",
                    help="Commit anterior a las mejoras visuales")
BASELINE = parser.parse_args().baseline


def original(path):
    return subprocess.check_output(["git", "show", f"{BASELINE}:{path}"], cwd=ROOT).decode("utf-8")


def compact(text):
    return re.sub(r"\s+", "", text)


results = {"baseline_commit": BASELINE, "resolution": [1280, 720], "centering": [], "logic_preserved": [], "assets": []}
for filename, area, target in [
    ("01-menu.png", (378, 277, 902, 342), 640),
    ("03-pausa.png", (432, 295, 848, 353), 640),
    ("04-victoria.png", (432, 295, 848, 353), 640),
    ("05-derrota.png", (432, 295, 848, 353), 640),
    ("01-menu.png", (480, 470, 800, 524), 640),
    ("03-pausa.png", (480, 470, 800, 524), 640),
    ("04-victoria.png", (480, 470, 800, 524), 640),
    ("05-derrota.png", (480, 470, 800, 524), 640),
]:
    with Image.open(EVIDENCE / filename) as screenshot:
        assert screenshot.size == (1280, 720)
        crop = screenshot.convert("RGB").crop(area)
        pixels = []
        for y in range(crop.height):
            for x in range(crop.width):
                r, g, b = crop.getpixel((x, y))
                # Letras claras y rojas; excluye madera, verde y adornos oscuros.
                if (r > 218 and g > 185 and b > 120) or (r > 175 and 45 < g < 115 and b < 100):
                    pixels.append((x + area[0], y + area[1]))
        assert pixels, filename
        left, right = min(p[0] for p in pixels), max(p[0] for p in pixels)
        center = (left + right) / 2
        assert abs(center - target) <= 3, (filename, center)
        results["centering"].append({"file": filename, "region": area, "ink_left": left,
                                     "ink_right": right, "center_x": center,
                                     "error_pixels": center - target})

for path, boundary in [("src/Game.cpp", "void Game::Draw("),
                       ("src/GameMap.cpp", "void GameMap::Draw("),
                       ("src/Projectile.cpp", "void Projectile::Draw(")]:
    now = (ROOT / path).read_text(encoding="utf-8")
    assert compact(now.split(boundary)[0]) == compact(original(path).split(boundary)[0]), path
    results["logic_preserved"].append(path + ": contenido anterior a Draw idéntico, salvo espacios")
for path in ["src/Enemy.cpp", "src/Tower.cpp", "src/WaveManager.cpp", "src/CsvLogger.cpp",
             "include/GameTypes.h"]:
    assert compact((ROOT / path).read_text(encoding="utf-8")) == compact(original(path)), path
    results["logic_preserved"].append(path + ": idéntico, salvo espacios")

# main solo cambió la resolución del directorio de recursos antes de abrir Raylib.
main_path = "TowerDefense2D/main.cpp"
assert compact((ROOT / main_path).read_text(encoding="utf-8").split("SetConfigFlags", 1)[1]) == compact(original(main_path).split("SetConfigFlags", 1)[1])
results["logic_preserved"].append(main_path + ": bucle del juego idéntico; búsqueda de recursos corregida")

manifest_path = ROOT / "assets/figma/manifest.json"
manifest = json.loads(manifest_path.read_text(encoding="utf-8"))
assert len(manifest["exports"]) == 23
for asset in manifest["exports"]:
    file = ROOT / "assets/figma" / (asset["name"] + ".png")
    with Image.open(file) as im:
        assert im.mode == "RGBA", file
        scale = im.width / asset["width"]
        assert scale in (1, 2) and im.height == asset["height"] * scale, file
        info = {"file": file.relative_to(ROOT).as_posix(), "node": asset["id"],
                "pixels": list(im.size), "scale": scale,
                "sha256": hashlib.sha256(file.read_bytes()).hexdigest()}
        results["assets"].append(info)

(EVIDENCE / "visual-audit.json").write_text(json.dumps(results, indent=2, ensure_ascii=False) + "\n",
                                          encoding="utf-8")
print("PASS: 8 regiones centradas (tolerancia 3 px), 23 PNG RGBA y lógica conservada.")
