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
