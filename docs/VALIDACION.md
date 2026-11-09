# Validación del avance PC3

Fecha: 2026-11-09. Ejecución automatizada por Codex, bajo solicitud de Kevin. No se registran pruebas manuales de los estudiantes ni aprobación del docente.

## Compilación y funcionalidad

- Visual Studio 2026 Community, MSBuild 18.10.1, MSVC v145, Windows x64, C++20 y NuGet raylib 6.2.0.
- Debug x64 y Release x64 compilados y enlazados realmente. Registros locales `docs/evidence/build-debug.log` y `build-release.log` (excluidos de Git por ser artefactos de compilación).
- 48 comprobaciones funcionales aprobadas, cero fallidas. Se ejecutan los mismos módulos del juego, sin mocks de las reglas. Véase `evidence/functional-tests.txt` y `tests/tests.cpp`.
- Campaña con parámetros predeterminados: ocho oleadas, victoria, 104 eliminaciones, 20 de vida, trece torres y 232,987 s de tiempo simulado. La simulación se ejecuta sin esperar ese tiempo real; una estrategia automática construye torres de largo alcance en casillas permitidas al disponer de monedas.
- Los escenarios de victoria y derrota destinados a capturas usan parámetros controlados para llegar al resultado; son distintos de la campaña predeterminada y no prueban por sí solos el equilibrio de dificultad.
- CSV: 159 filas de muestra de tres partidas automáticas, ocho eventos distintos y diez columnas exactas, leídos con el parser CSV de Python. Se probaron escape de coma/comillas/salto de línea, acumulación, encabezado incompatible y error de acceso.

## Ejecución gráfica

Contexto OpenGL 3.3 real de Raylib sobre NVIDIA GeForce RTX 3060, controlador 580.97. Capturas de menú, partida, pausa, victoria y derrota en `docs/evidence/01-menu.png` a `05-derrota.png`; la captura `06-rendimiento.png` corresponde a la carga de estrés. Se observaron visualmente las pantallas. Los gráficos son sprites temporales de primitivas, no exportaciones originales de Figma.

El recorrido adicional de interfaz se realizó con la herramienta de control de Windows: botón Jugar, selección de tarjeta, construcción, previsualización válida, rechazo sobre el camino, pausa y reanudación. Una primera ejecución desde un directorio no escribible detectó el error de CSV; se corrigió main para establecer el directorio de trabajo en la raíz del proyecto o junto al ejecutable distribuido. La ejecución posterior escribió data/eventos.csv con encabezado correcto; PAUSA y REANUDACION conservaron tiempo_activo_s=68,341. Capturas del recorrido: archivos 09 a 11 de docs/evidence. Las instantáneas automatizadas 07 y 08 renderizan la misma previsualización con una posición de puntero explícita, sin modificar las reglas ni generar entradas físicas.

## Rendimiento local medido

Equipo: Intel Core i5-13400F (10 núcleos, 16 hilos), NVIDIA GeForce RTX 3060. Resolución 1280 × 720, MSAA x4, objetivo de 60 FPS. Se midieron 1.800 cuadros tras 120 de calentamiento, con un mínimo de 83 entidades simultáneas. El escenario mantiene 70 enemigos de vida alta y trece torres, además de proyectiles.

| Medida | Resultado |
|---|---:|
| FPS medio | 59,7844 |
| Tiempo medio por cuadro | 16,7268 ms |
| Percentil 95 | 16,7657 ms |
| Percentil 99 | 17,0838 ms |
| Máximo | 80,5197 ms |
| Máximo medido de actualización y envío de dibujo en CPU | 6,9559 ms |

Tiempos crudos: `evidence/frame-times.csv`. Resumen: `evidence/benchmark.json`. El tiempo de CPU no incluye la espera de EndDrawing y no representa la finalización de la GPU. No se eliminó el cuadro máximo. No se afirma 60 FPS estables en todo momento ni cumplimiento del equipo de laboratorio.

## Estado de los requisitos no funcionales

| RNF | Estado verificable |
|---|---|
| 01 | Medición local realizada; validación del laboratorio pendiente |
| 02 | Nueve responsabilidades separadas en clases; revisión estructural automatizada y lectura de código |
| 03 | Atributos privados y consultas const; mutaciones mediante operaciones controladas |
| 04 | HUD, selección, casillas y mensajes presentes y observados; aceptación de usabilidad humana pendiente |
| 05 | Restricciones de vida, saldo, ocupación y objetivos cubiertas por las pruebas |
| 06 | Compilación y ejecución real en Windows x64 con Visual Studio y Raylib |
| 07 | Pendiente de medir latencia completa entre entrada y respuesta visual; el benchmark no acredita los 100 ms |
| 08 | Repositorio privado y commits auténticos; README reproducible; sin protección de ramas ni CI |
| 09 | Nombres por responsabilidad y comentarios breves; revisión y sustentación del equipo pendientes |

## Incidencias y pendientes

- El enlazador aislado quedó bloqueado con error MSPDB140; se repitió fuera del aislamiento. Un primer ajuste de runtime reveló símbolos debug sin resolver. Se corrigió Debug a /MDd y se excluyó MSVCRT de release; ambas configuraciones se reconstruyeron.
- Una aserción inicial del fixture CSV comparaba LF con CRLF de Windows. Se corrigió la escritura del fixture a binario y se repitió la batería completa.
- Faltan sprites originales de Figma, medición de respuesta visual, prueba en laboratorio, evaluación de dificultad con jugadores y revisión humana.
- Sin bloqueo de CSV entre instancias, rotación automática, guardado de partidas, CI ni protecciones de ramas. No se incorporan características fuera del APF2.
