# Procedencia de recursos

## Figma del usuario

- Fuente: [Tower Defense original](https://www.figma.com/design/pSD8QGOknqb9diYtcecgda/).
- Recuperación: integración Figma, 9 de noviembre de 2026. Solo lectura y exportación; no se reconstruyeron ni modificaron pantallas en Figma.
- Páginas inspeccionadas: `0:1` Sistema de diseño y `6:777` Mockups. Pantallas editables: `6:778`, `21:7585`, `21:8846`.
- Los **23 PNG individuales** están identificados por nombre y nodo en `figma/manifest.json`; dimensiones reales, escala y SHA-256 están en `../docs/evidence/visual/visual-audit.json`.
- Uso: materiales del proyecto proporcionados por el usuario y exportados bajo su autorización para este videojuego. No se les atribuye una licencia CC0 ni se afirma que sean recursos públicos de terceros.
- `ground` y `foliage` son capas independientes de suelo y hierbas; no incluyen HUD, enemigos ni botones. La captura de referencia `docs/evidence/visual/figma-mk01.png` solo documenta la comparación y no se carga en el juego.

## Fuentes libres

Distribución bajo **SIL Open Font License 1.1**. Se incluyen los avisos íntegros junto a los TTF; no se venden por separado ni se cambia su licencia.

| Familia | Fuente oficial | Original incluido | Instancias utilizadas | Licencia incluida |
|---|---|---|---|---|
| Cinzel | [Google Fonts / Cinzel](https://github.com/google/fonts/tree/main/ofl/cinzel) | Cinzel-Variable.ttf | Cinzel-Bold.ttf, peso 700 | fonts/OFL-Cinzel.txt |
| Inter | [Google Fonts / Inter](https://github.com/google/fonts/tree/main/ofl/inter) | Inter-Variable.ttf | Inter-Regular.ttf, peso 400; Inter-SemiBold.ttf, peso 600; opsz 24 | fonts/OFL-Inter.txt |

Los originales se descargaron de `raw.githubusercontent.com/google/fonts/main/ofl/cinzel/Cinzel[wght].ttf` y `raw.githubusercontent.com/google/fonts/main/ofl/inter/Inter[opsz,wght].ttf`. Las instancias estáticas se obtuvieron con fontTools 4.68.0 mediante `tools/prepare_visual_fonts.py`; no se alteraron los contornos de las letras.

## Dibujo original por código

- GameUI: madera plana, bordes, divisores, corona/escudo del menú, trofeo y escudo roto, velo de pausa y resultados.
- GameMap: camino con curvas y piedras, casillas y rótulos sobre la geometría lógica conservada.
- Projectile: flechas orientadas por su trayectoria real.
- AssetManager: sprites procedurales anteriores conservados como reserva.

Son aportes originales del código de este proyecto, no descargas de bibliotecas ni material de pago. No se incorporaron otros packs gráficos externos. Los PNG de referencia suministrados por el usuario se conservan únicamente como comparación documental en `docs/evidence/visual/references/`.
