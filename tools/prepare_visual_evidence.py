"""Prepara una comparación documental local; no genera ni modifica sprites del juego."""
import csv
import html
import argparse
from pathlib import Path
import shutil

ROOT = Path(__file__).resolve().parents[1]
DEST = ROOT / "docs/evidence/visual"
parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("--references", type=Path, help="Carpeta con los PNG originales suministrados")
SOURCE = parser.parse_args().references
screens = [
    ("Menú inicial", "Menú principal de defensa medieval.png", "01-menu.png", "menu"),
    ("MK-01 · Interfaz y HUD", "MK-01_corregido.png", "02-partida.png", "mk01"),
    ("MK-02 · Selección y colocación válida", "MK-02_colocacion_valida.png", "07-colocacion-valida.png", "mk02"),
    ("MK-03 · Colocación inválida", "MK-03_colocacion_invalida.png", "08-colocacion-invalida.png", "mk03"),
    ("MK-04 · Combate", "MK-01_corregido.png", "09-combate.png", "mk04"),
    ("MK-05 · Derrota", "Derrota en la Fortaleza del Reino.png", "05-derrota.png", "mk05"),
    ("MK-06 · Victoria", "¡Victoria! Fortaleza Defendida.png", "04-victoria.png", "mk06"),
    ("MK-07 · Pausa", "Pantalla de pausa en defensa forestal.png", "03-pausa.png", "mk07"),
]
(DEST / "references").mkdir(exist_ok=True)
sections = []
for title, original, capture, anchor in screens:
    reference = f"references/{anchor}.png"
    if SOURCE is not None:
        shutil.copyfile(SOURCE / original, DEST / reference)
    if not (DEST / reference).exists():
        raise FileNotFoundError("Falta la referencia; ejecutar con --references CARPETA")
    note = ("MK-04 usa la referencia MK-01, que contiene enemigos y proyectiles: no se proporcionó un PNG separado con ese nombre."
            if anchor == "mk04" else "")
    sections.append(f'''<section id="{anchor}"><h2>{html.escape(title)}</h2><p>{note}</p>
<div class="pair"><figure><figcaption>Referencia suministrada</figcaption><a href="{reference}"><img loading="lazy" src="{reference}" alt="{html.escape(title)} de referencia"></a></figure>
<figure><figcaption>Captura real del juego · 1280 × 720</figcaption><a href="{capture}"><img loading="lazy" src="{capture}" alt="{html.escape(title)} implementado"></a></figure></div></section>''')

page = '''<!doctype html><html lang="es"><meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1">
<title>TowerDefense2D · Revisión visual</title><style>
*{box-sizing:border-box}body{margin:0;background:#151d17;color:#f2dfb7;font:16px/1.6 system-ui,sans-serif}
main{max-width:1800px;margin:auto;padding:32px}h1{font-size:32px;margin:0}h2{font-size:23px;color:#f0ce8b}p{max-width:1000px;color:#c4b69a}
a{color:#f0ce8b}nav{display:flex;gap:15px;flex-wrap:wrap;margin:28px 0}section{padding:18px 0 36px;border-top:1px solid #80623d}
.pair{display:grid;grid-template-columns:1fr 1fr;gap:20px}figure{margin:0}figcaption{margin-bottom:8px}img{display:block;width:100%;height:auto;border:1px solid #80623d}
@media(max-width:850px){main{padding:18px}.pair{grid-template-columns:1fr}}@media print{body{background:white;color:black}p{color:#333}section{break-inside:avoid}}
</style><main><h1>TowerDefense2D · Comparación visual</h1>
<p>9 de noviembre de 2026. Exportaciones individuales originales de Figma integradas en el proyecto existente. Las capturas proceden de un contexto OpenGL real; victoria y derrota usan escenarios de prueba declarados. Los valores de vida, monedas, enemigos y oleada difieren porque son partidas funcionales, no composiciones estáticas.</p>
<p>Evaluación cualitativa en <a href="../../REVISION_VISUAL.md">REVISION_VISUAL.md</a>. Hacer clic en cualquier imagen abre su tamaño original.</p>
<nav>''' + "".join(f'<a href="#{s[3]}">{s[0]}</a>' for s in screens) + "</nav>" + "\n".join(sections) + '''
<section><h2>Antes y después del menú</h2><div class="pair"><figure><figcaption>Versión anterior conservada</figcaption><img src="../01-menu.png" alt="Menú anterior"></figure><figure><figcaption>Versión actual</figcaption><img src="01-menu.png" alt="Menú corregido"></figure></div></section>
<section><h2>Recorrido interactivo de Windows</h2><p>EXE Release del paquete existente: JUGAR, selección, construcción, rechazo sobre el camino, PAUSA y REANUDAR. Capturas de la ventana nativa; el puntero y el halo pertenecen a la herramienta de interacción.</p><div class="pair"><img src="12-partida-interactiva.jpg" alt="Partida interactiva"><img src="13-pausa-interactiva.jpg" alt="Pausa interactiva"></div></section></main></html>'''
(DEST / "comparacion.html").write_text(page, encoding="utf-8")

with (ROOT / "output/PC3/data/eventos.csv").open(encoding="utf-8-sig", newline="") as file:
    rows = list(csv.reader(file))
last_id = rows[-1][1]
session = [row for row in rows[1:] if row[1] == last_id]
assert all(len(row) == 10 for row in session)
with (DEST / "interactive-session.csv").open("w", encoding="utf-8", newline="") as file:
    csv.writer(file).writerows([rows[0]] + session)
print(f"Comparación preparada: 8 vistas y {len(session)} eventos de la sesión interactiva.")
