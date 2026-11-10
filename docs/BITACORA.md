# Bitácora de desarrollo

## 2026-11-09 · Inspección y respaldo

- Proyecto físico existente: solución SLNX de Visual Studio 2026, v145, C++20, NuGet raylib 6.2.0. Solo main.cpp y Game.h; no existía Git.
- Copia inicial de cuatro archivos en docs/backup, excluida de Git. El primer commit conserva el código original.
- Fuentes: PDF APF2 de 62 páginas y DOCX editable (351 párrafos, 38 tablas); semanas 11, 12 y 13 leídas como contexto académico. No se encontró APF2_Tower_Defense(1).pdf.
- El PDF formal exige 60 FPS con 30 entidades; Semana 11 menciona 30 FPS. Se conserva el criterio formal de 60 FPS, pendiente de medir en laboratorio.
- Los valores de los mockups no son parámetros aprobados. Se adoptan como punto de partida y se documentará la calibración.
- Desarrollo y pruebas ejecutados por Codex mediante herramientas bajo solicitud de Kevin. No se atribuyen aportes ni aprobaciones a Naomy o Diego.
- Se trabajará en develop y se integrará en main tras las comprobaciones. No se presume protección de ramas ni revisión humana.

## Incremento integrado de mecánicas e interfaz

- Nueve clases en include/ y src/, GameTypes.h como definiciones auxiliares. main conserva exclusivamente inicialización, entrada, actualización, dibujo y cierre.
- Game integra estados, economía y eventos; Enemy consume la distancia sobrante al pasar un waypoint. Tower resuelve el objetivo más cercano y Projectile lo sigue mediante un ID estable.
- GameMap usa trece casillas explícitas fuera del camino; se rechazan ruta, exteriores, ocupación y saldo insuficiente antes de cobrar.
- AssetManager crea texturas de sprites independientes temporales; GameUI dibuja controles reales, HUD, menú y resultados. Exportaciones de Figma pendientes.
- La primera compilación del incremento llegó al enlazador, pero no acreditó un ejecutable válido: el aislamiento bloqueó MSPDB140 y el ajuste inicial de runtime falló con símbolos debug. La corrección comprobada usa /MDd en Debug y excluye MSVCRT de release aportada por Raylib. Debug y Release x64 reconstruidos y enlazados correctamente, sin advertencias en la compilación final.
- Repositorio privado creado y verificado: https://github.com/Alexisga/TowerDefense2D (propietario Alexisga, permisos de administrador verificados por API). La autenticación no se incluye en los archivos.

## Pruebas e incidencias corregidas

- Batería automatizada de 48 comprobaciones: 48 aprobadas, cero fallidas. La primera comparación de fixture CSV falló por CRLF frente a LF; se corrigió el fixture a modo binario y se repitieron todas las pruebas.
- Campaña completa con configuración predeterminada: victoria, ocho oleadas, 104 eliminaciones, vida 20 y 232,987 s simulados. La estrategia automática construye torres al obtener monedas; no es una sesión manual de Kevin.
- Ejecución gráfica OpenGL real y revisión de menú, combate, pausa, victoria, derrota y previsualizaciones. Recursos temporales claramente separados de las referencias originales.
- Prueba de rendimiento: 1.800 cuadros medidos, mínimo 83 entidades, 59,7844 FPS medios, p99 17,0838 ms y máximo 80,5197 ms. Intel i5-13400F y RTX 3060. Pendiente validación en laboratorio; no se afirma estabilidad de 60 FPS en todos los cuadros ni latencia visual de 100 ms.
- Recorrido nativo de Windows por herramientas de Codex: Jugar, tarjeta, construcción y pausa. Se detectó escritura de CSV relativa a un directorio no escribible al lanzar desde otra aplicación. Se corrigió main para ubicar el directorio en la raíz del proyecto o junto al EXE distribuido; la segunda ejecución escribió eventos correctamente.
- Pausa y reanudación verificadas en la ejecución real: ambas filas con tiempo activo 68,341 s. Capturas 09–11 conservan el recorrido de interfaz; no se atribuyen como pruebas manuales del estudiante.
- CSV leído mediante parser de Python: 159 filas de muestra, diez columnas y los ocho eventos. Búsqueda local de cuatro patrones de secretos: cero hallazgos en el alcance indicado por security-and-csv.json. No equivale a Gitleaks ni a un análisis exhaustivo.

## Documentación y entrega

- APF2 editable actualizado solo en los apartados 8–10, con nueva paginación del índice. Se conservaron los 16 recursos originales byte a byte, 295 nodos anteriores y 37 posteriores al bloque sustituido. Las secciones 11 y 12 siguen como fichas originales para el cierre posterior.
- Documento final de 68 páginas. Conversión por Microsoft Word mediante API COM y rasterizado con Poppler: las primeras 54 páginas son idénticas al original salvo la página 3, donde se corrigieron únicamente los números del índice. Páginas renderizadas inspeccionadas; se corrigieron párrafos aislados y una cabecera de tabla sin su primera fila en la paginación nueva.
- El renderizador habitual render_docx.py no encontró LibreOffice en Windows. Se usó Word instalado para la conversión de documentos; no se instaló software ni se sustituyó el original por un documento reconstruido.
- Código formateado con clang-format de Visual Studio para facilitar lectura. Debug, Release y las 48 comprobaciones se repitieron después del formato: correctos, sin advertencias.
- Carpeta local output/PC3 preparada con ejecutable Release. Se abrió desde esa carpeta y se comprobó la creación de su propio data/eventos.csv. No requiere descargar sprites ni acceder a Internet para jugar.
- Word original de OneDrive actualizado con la copia final revisada; coincidencia SHA-256 comprobada. Su versión inicial íntegra se conserva en docs/backup/APF2_Tower_Defense_original_2026-11-09.docx, excluida de Git.

## Revisión estética posterior — 9 de noviembre de 2026

- Solo se modificó el proyecto original; el Word y los archivos externos permanecieron sin cambios durante esta revisión. No se hicieron commits ni publicaciones en GitHub.
- Se inspeccionó el Figma original por integración y se exportaron 23 PNG RGBA individuales: sprites, iconos, cursor, adornos y capas del mapa. Se integraron las fuentes Cinzel e Inter con sus licencias OFL. Los dibujos procedurales anteriores se conservan como reserva.
- Se corrigieron centrado mediante ancho medido, tamaño y métrica de fuentes, jerarquía, tarjetas, columnas, proporciones, bordes y avisos. Menú/resultados usan emblemas originales aproximados por código; los faltantes se declaran en el informe.
- Debug y Release x64 compilados sin advertencias ni errores; 48 comprobaciones funcionales y 7 gráficas aprobadas. Campaña predeterminada sin cambios de resultado. Auditoría de ocho regiones centradas, 23 PNG y conservación del código lógico: correcta.
- Rendimiento nuevo: 59,7141 FPS medios, 83 entidades mínimas, p99 17,2678 ms y máximo 97,5616 ms. La evidencia inicial se conserva y la nueva queda en `docs/evidence/visual/`.
- Recorrido nativo del EXE Release: JUGAR, selección, construcción, rechazo, PAUSA y REANUDAR. CSV generado; primer par de pausa/reanudación con tiempo activo idéntico de 26,137 s. El paquete existente `output/PC3` incluye los 33 archivos de recursos verificados por hash.
- Informe final A–G en [REVISION_VISUAL.md](REVISION_VISUAL.md); ocho vistas comparadas con las referencias en [comparacion.html](evidence/visual/comparacion.html). No se afirma fidelidad artística exacta ni medición de latencia física.

## Corrección del inicio desde la solución — 9 de noviembre de 2026

- La captura del usuario mostraba `ASSET: 0/23` al iniciar desde Visual Studio. La compilación de la solución sitúa el EXE un nivel más arriba que la compilación directa del proyecto; main asumía tres niveles fijos y no encontraba los recursos.
- Se corrigió la localización de la raíz sin una profundidad fija, manteniendo los recursos y datos propios del paquete distribuido. No se modificaron reglas de juego.
- Se compilaron Debug y Release x64 desde `TowerDefense2D.slnx`, sin advertencias ni errores. El EXE Debug de esa ubicación se abrió realmente y cargó los 23 PNG y las fuentes; menú y partida observados con el aspecto nuevo. Evidencias: `startup-solution-debug.txt`, `14-solucion-debug-corregida.jpg` y registros `build-solution-*.log`.
- Se actualizaron los ejecutables de compilación directa y `output/PC3`; las 48 pruebas funcionales se repitieron correctamente. Sin commits ni publicación en GitHub.

## Publicación de las mejoras — 10 de noviembre de 2026

- El usuario autorizó registrar las mejoras, publicarlas en `feature/mejoras-visuales` y usar dos PR sucesivos hacia `develop` y `main`, integrándolos si las comprobaciones son satisfactorias.
- Antes de preparar los commits se guardó un respaldo local de los 92 archivos pendientes en `docs/backup/publicacion-2026-11-10/`, excluido del repositorio. La rama parte de `develop` en `813285d`.
- La publicación comprende los 23 PNG individuales de Figma, fuentes y licencias, interfaz centrada, dibujo del escenario, corrección de inicio, pruebas, evidencias y documentación técnica. La auditoría visual utiliza el commit base fijo para que la comparación siga siendo reproducible después de integrar los cambios.
- Por indicación expresa del usuario, el Word académico se retira de la versión actual del repositorio y permanece intacto en su computadora mientras lo corrige. El informe de corrección documental también se conserva solo localmente. Las entradas documentales anteriores son históricas y no acreditan una versión APF3 aprobada.
- Los registros de reconstrucción Debug y Release x64, pruebas y carga de recursos para esta publicación están en `docs/evidence/publicacion/`. No se incluyen ejecutables, paquetes NuGet, carpetas de compilación, respaldos ni credenciales en los commits.
