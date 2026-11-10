# Revisión estética de TowerDefense2D

Fecha de las mediciones visuales: 9 de noviembre de 2026. Trabajo realizado por Codex sobre el proyecto existente. No se creó otra solución, repositorio ni worktree. La revisión se preparó sobre el commit `813285dc8922b76d1f2fe49ad6289fa2d5ef5213`. Su publicación, autorizada el 10 de noviembre, sigue el flujo `feature/mejoras-visuales` → `develop` → `main`, con validación local en `docs/evidence/publicacion/`. El documento académico en revisión se conserva solo en la computadora del usuario.

La evidencia anterior permanece en `docs/evidence/`. Las nuevas capturas, registros y comparaciones están en [evidence/visual/](evidence/visual/). [Abrir comparación lado a lado](evidence/visual/comparacion.html).

## A. Defectos encontrados y corregidos

- El título del menú dependía de una posición fija y se veía desplazado. Ahora se centra con `MeasureTextEx` a través de `AssetManager::TextWidth`; se centra también la altura visible de los glifos. El botón JUGAR centra el conjunto icono/texto. Los títulos y botones de pausa/resultados usan el mismo criterio.
- Se sustituyeron tipografías de reserva por Cinzel Bold e Inter Regular/SemiBold, compatibles con el Figma original. Se corrigió la diferencia entre el tamaño em de Figma y la métrica ascendente/descendente de Raylib. Aumentaron el título, HUD, textos de tarjetas y mensajes; se conservan jerarquías legibles sin recortar nombres ni columnas.
- Se alinearon las tarjetas, sus nombres de dos líneas, precios, columnas de daño/alcance/intervalo y ayudas. Selección, hover y errores tienen bordes y colores diferenciados. Los botones conservaron sus rectángulos de entrada originales.
- Se reemplazaron los sprites temporales visibles por torres, enemigo, castillo, árboles, pinos y rocas del Figma. Se respetó su proporción y un anclaje de pie consistente. El bosque utiliza las capas originales de suelo y hierbas, sin capturas completas del mockup.
- Se incorporaron iconos y remates de paneles originales. El rechazo sobre el camino muestra el icono de error y aviso; el saldo insuficiente conserva su mensaje propio. Las flechas se orientan por su trayectoria real. El camino tiene bordes suaves y piedras dibujadas sobre los waypoints existentes.
- Se restauró el borde exterior del menú tras dibujar su fondo. El texto de espera usa redondeo hacia arriba y ya no muestra 6 segundos cuando faltan exactamente 5; el temporizador real no cambió.

Auditoría sobre PNG reales a 1280 × 720: centro del título del menú **x=638,5**, frente a x=640 de la ventana; grupo JUGAR x=639. Pausa/resultados y sus botones quedan entre x=640 y x=640,5. Son límites de tinta rasterizada, distintos del avance tipográfico completo. Las ocho regiones comprobadas están dentro de 3 px. [Datos y hashes](evidence/visual/visual-audit.json), [script reproducible](../tools/audit_visual.py).

## B. Recursos recuperados realmente de Figma

Se accedió al [archivo original](https://www.figma.com/design/pSD8QGOknqb9diYtcecgda/) mediante su integración, sin depender de Chrome. Se inspeccionaron el sistema de diseño y las tres pantallas editables: principal `6:778`, válida `21:7585` e inválida `21:8846`. No se modificó el archivo de Figma.

| Grupo | PNG individuales exportados |
|---|---|
| Sprites, 7 | basic, long, castle, enemy, tree, pine, rock |
| Iconos, 10 | icon-shield, icon-coin, icon-wave, icon-tower, icon-pause, icon-info, icon-valid, icon-invalid, icon-arrow, icon-warning |
| Cursor, 1 | cursor-build |
| Remates, 3 | trim-button, trim-hud, trim-sidebar |
| Capas, 2 | ground, foliage |

Total: **23 PNG RGBA** guardados en `assets/figma/`; 21 a escala 2 y las dos capas a escala 1. [Manifiesto de nodos y dimensiones](../assets/figma/manifest.json). Todos cargaron sin errores en las pruebas gráficas. `icon-valid` y `cursor-build` se conservan disponibles; el juego mantiene su cursor normal y la previsualización de torre existente. No se presenta cada exportación como un elemento usado en todas las pantallas.

## C. Suficiencia y elementos faltantes

Los recursos editables son suficientes para la base común del HUD, construcción, combate y escenario. Permiten reutilizar el mapa funcional en pausa y resultados. No son suficientes para una reproducción exacta de todas las ilustraciones PNG: no se encontraron como elementos independientes la gran corona con laureles/cintas, el trofeo ilustrado, el escudo roto detallado, el río/puente del menú ni las llamas/humo del castillo derrotado. Tampoco se recuperó una hoja de animación de marcha del enemigo.

Las referencias de menú, pausa, victoria y derrota tienen un acabado pictórico más detallado que los componentes vectoriales editables. Se usó un lenguaje visual común derivado de estos últimos. Los valores de HUD y la cantidad/posición de enemigos son datos reales de cada escenario, por lo que no coinciden con los números ilustrativos del mockup.

## D. Recursos adicionales y procedencia

Se incorporaron **Cinzel e Inter**, obtenidas del repositorio oficial Google Fonts bajo **SIL OFL 1.1**; se incluyen los TTF originales, las tres instancias utilizadas y ambas licencias. [Fuentes, URL y condiciones](../assets/SOURCES.md). No se compraron ni incorporaron packs gráficos externos. Los emblemas grandes, paneles, camino y flechas faltantes se aproximaron con código original del proyecto. Los sprites procedurales anteriores permanecen como reserva de carga.

## E. Fidelidad respecto a cada referencia

Valoración cualitativa tras observar las capturas reales; no es una puntuación automática de similitud. La numeración siguiente corresponde a la última solicitud del usuario. No se proporcionó un PNG independiente llamado MK-04; se usa la escena de combate visible en MK-01 como referencia.

| Vista | Fidelidad alcanzada | Diferencias conservadas | Captura real |
|---|---|---|---|
| Menú | Media: bosque, castillo, madera/oro, jerarquía y botón central | Emblema simplificado; sin río, puente, cintas ni laureles ilustrados | [01](evidence/visual/01-menu.png) |
| MK-01, HUD | Alta en distribución, tipografía, paleta, iconos y sprites | Camino adaptado a waypoints actuales; mapa y valores de partida propios | [02](evidence/visual/02-partida.png) |
| MK-02, selección | Alta: tarjeta activa, torre translúcida, rango y mensaje | Se conservan las 13 casillas funcionales existentes; no se sustituyen por las posiciones ilustrativas de Figma | [07](evidence/visual/07-colocacion-valida.png), [10](evidence/visual/10-seleccion-largo.png) |
| MK-03, inválida | Alta: rojo, cruz/aviso y mensaje al pie | El juego conserva el círculo de alcance rojo y su geometría real de validación | [08](evidence/visual/08-colocacion-invalida.png), [11](evidence/visual/11-monedas-insuficientes.png) |
| MK-04, combate | Alta en estilo de unidades, torres y HUD | Proyectiles por código; enemigo sin animación de marcha; posiciones dependen de la simulación | [09](evidence/visual/09-combate.png) |
| MK-05, derrota | Media: overlay, jerarquía, escudo roto rojo y reinicio | Emblema plano y sin castillo en llamas | [05](evidence/visual/05-derrota.png) |
| MK-06, victoria | Media: trofeo, dorado, mensajes y reinicio verde | Trofeo plano; sin laureles ni cintas ilustrados | [04](evidence/visual/04-victoria.png) |
| MK-07, pausa | Media-alta: oscurecimiento, símbolo, título y reanudación | Panel y ornamentos simplificados frente al PNG pictórico | [03](evidence/visual/03-pausa.png) |

## F. Compilación, ejecución y regresiones

- **Debug x64 y Release x64: correctos, cero advertencias y errores.** [Registro Debug](evidence/visual/build-debug.log), [Release](evidence/visual/build-release.log), [proyecto de pruebas](evidence/visual/build-tests.log).
- **48 comprobaciones funcionales aprobadas, 0 fallidas.** La campaña predeterminada conserva victoria en ocho oleadas, 104 eliminaciones, 20 de vida, 13 torres y 232,987 s simulados. [Registro completo](evidence/visual/functional-tests.txt).
- **7 comprobaciones gráficas aprobadas**: 23 texturas, ancho de título, botón de reinicio, columnas, proyectiles reales y resultados de victoria/derrota por simulación. No se observaron avisos de carga ni errores de recursos. [Registro](evidence/visual/graphics-tests.txt).
- Se observaron menú, partida, ambas selecciones, previsualización válida, rechazo sobre camino, monedas insuficientes, combate, pausa y ambos resultados en capturas OpenGL reales. Victoria y derrota gráficas usan escenarios de prueba acelerados identificados en `tests/tests.cpp`; la campaña completa se comprueba aparte con parámetros predeterminados.
- Se abrió el **EXE Release del paquete existente** en Windows y se probaron con clics JUGAR, selección básica, construcción (180 → 130 monedas), rechazo sobre el camino, PAUSA y REANUDAR. Se observó combate con recompensa y daño a la base. [Partida nativa](evidence/visual/12-partida-interactiva.jpg), [pausa nativa](evidence/visual/13-pausa-interactiva.jpg), [CSV del recorrido](evidence/visual/interactive-session.csv). El primer par PAUSA/REANUDACION mantiene el mismo tiempo activo, 26,137 s.
- La auditoría compara el contenido anterior a `Draw` de Game, GameMap y Projectile con el commit base de la revisión; es idéntico salvo espacios. Enemy, Tower, WaveManager, CsvLogger y GameTypes permanecen iguales. En main solo se corrigió posteriormente la resolución del directorio de recursos; su bucle permanece igual. Se mantienen las nueve clases y los rectángulos de entrada; no cambian precios, daño, recompensas, waypoints, casillas, oleadas, pausa ni condiciones de finalización. El CSV sigue generándose y las pruebas originales siguen cubriéndolo.
- Rendimiento local: **59,7141 FPS medios**, mínimo **83 entidades** y **1.800 cuadros** medidos; p95 16,7883 ms, p99 17,2678 ms, máximo 97,5616 ms. CPU de actualización/envío de dibujo, máximo 7,9548 ms. La referencia anterior obtuvo 59,7844 FPS medios: diferencia de aproximadamente −0,12 %, sin una caída material del promedio en esta ejecución. No es una prueba de equivalencia estadística ni de latencia física. [Resumen](evidence/visual/benchmark.json), [tiempos crudos](evidence/visual/frame-times.csv).
- Se actualizó `output/PC3` incluyendo `assets` y licencias. [SHA-256 del EXE y estado local](evidence/visual/release-artifact.json). El manifiesto declara `local_modifications: true`: no corresponde a un nuevo commit publicado.

La revisión mantiene el alcance RF-01–RF-15/RNF-01–RNF-09. Las validaciones humanas, el equipo de laboratorio y la medición específica de entrada–respuesta de RNF-07 siguen pendientes como antes; estas pruebas no los acreditan.

## G. Pendientes visuales y motivos

No se observaron textos cortados o superpuestos en las vistas comprobadas a 1280 × 720. Queda una diferencia de acabado artístico en menú/resultados: laureles, cintas, relieve pictórico, río/puente y castillo dañado no estaban disponibles como capas independientes. Se priorizó la coherencia de los sprites originales recuperados y las aproximaciones declaradas. El trazado y las casillas difieren de la ilustración para conservar la jugabilidad autorizada; cambiarlos implicaría revisar el diseño funcional. El enemigo conserva un sprite estático porque no se obtuvo una secuencia de animación. No se afirma fidelidad exacta ni validación en resoluciones distintas de la referencia.

## Corrección posterior: ejecución desde Visual Studio

El usuario detectó un aspecto antiguo al iniciar desde la solución. Su captura mostraba `ASSET: 0/23`: el código nuevo estaba activo, pero usaba gráficos de reserva. MSBuild genera el EXE en `x64/Debug` al compilar el `.slnx`, mientras que la compilación directa del `.vcxproj` lo sitúa en `TowerDefense2D/x64/Debug`. La búsqueda inicial subía exactamente tres niveles y fallaba desde la primera ubicación.

Se sustituyó esa suposición por una búsqueda de la raíz que contiene la solución y `assets`, conservando la prioridad de los recursos junto al ejecutable distribuido. La comprobación real del EXE `x64/Debug/TowerDefense2D.exe` abrió menú y partida con los gráficos originales y registró `23/23` sin advertencias de carga. [Registro de inicio](evidence/visual/startup-solution-debug.txt), [captura nativa](evidence/visual/14-solucion-debug-corregida.jpg). La solución completa compiló en Debug y Release x64 con cero advertencias y errores: [Debug](evidence/visual/build-solution-debug.log), [Release](evidence/visual/build-solution-release.log). Se actualizaron también los ejecutables de compilación directa y el paquete existente. Las 48 pruebas funcionales volvieron a pasar: [registro](evidence/visual/startup-functional-tests.txt).
