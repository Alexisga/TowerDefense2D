# TowerDefense2D

Tower Defense 2D académico en C++20, Visual Studio 2026 y Raylib 6.2.0 (NuGet). Versión jugable para PC3: ocho oleadas automáticas, dos torres, economía, proyectiles, pausa, victoria, derrota y reinicio. Interfaz base 1280 × 720.

Repositorio oficial: https://github.com/Alexisga/TowerDefense2D. Los cambios realizados por Codex se identifican como tales en Git; no representan pruebas manuales ni contribuciones de Naomy o Diego.

## Compilar y ejecutar

1. Instalar Visual Studio 2026 con **Desarrollo para el escritorio con C++**, MSVC v145 y Windows SDK 10/11.
2. Abrir `TowerDefense2D.slnx`. Restaurar el paquete NuGet **raylib 6.2.0** mediante el administrador NuGet de Visual Studio. Los paquetes deben quedar en `packages/raylib.6.2.0` en la raíz del repositorio.
3. Elegir **Debug x64** o **Release x64**, establecer TowerDefense2D como proyecto de inicio y compilar. Las pruebas tienen su propio proyecto x64; no se validó Win32.
4. Ejecutar con F5/Ctrl+F5. El directorio de trabajo del depurador es la raíz. Desde PowerShell abierto en la raíz también se puede usar:

```powershell
.\tools\build.ps1 -Configuration Release
& .\TowerDefense2D\x64\Release\TowerDefense2D.exe
```

No se requiere conexión de red para jugar. Windows debe disponer del runtime Microsoft Visual C++ 2015–2026 x64, normalmente presente al instalar Visual Studio. El ejecutable usa `/MD`; la edición Debug requiere las bibliotecas de depuración de Visual Studio. La distribución local utiliza Release.

Para preparar la carpeta de entrega después de compilar Release, ejecutar `.\tools\package.ps1`. El resultado queda en `output/PC3`; se puede abrir su EXE directamente. Esa copia crea sus registros junto al ejecutable y no necesita el resto del proyecto para jugar.

La versión visual utiliza 23 PNG individuales exportados del Figma original y fuentes Cinzel/Inter con licencia OFL. Conserva los sprites procedurales anteriores como reserva. Mantener la carpeta `assets` junto al EXE distribuido; el paquete la incluye. [Procedencia y licencias](assets/SOURCES.md).

El inicio localiza la raíz del proyecto ascendiendo desde el EXE, sin asumir cuántos niveles tiene la carpeta de compilación. Funciona tanto con `x64/Debug` al compilar la solución como con `TowerDefense2D/x64/Debug` al compilar el proyecto. El paquete utiliza sus propios recursos y datos junto al EXE. Si la consola muestra `ASSET: 0/23`, el aspecto simplificado corresponde a recursos de reserva por archivos no encontrados; la carga correcta indica `23/23`.

## Jugar

- **JUGAR / Enter** inicia. Seleccionar una tarjeta o **1 / 2** elige torre básica / largo alcance.
- Clic en una de las trece casillas marcadas construye. Se ven alcance, previsualización y motivo de rechazo. El camino y las zonas sin casilla están prohibidos.
- **Esc / clic derecho** cancela selección. **P / PAUSA** detiene o reanuda; el botón central también reanuda.
- Tras victoria o derrota, **REINICIAR PARTIDA / Enter** restaura la partida.
- **F12** guarda `data/captura.png`, reemplazando la captura anterior. La **X de la ventana** cierra el juego.

Condiciones iniciales: 20 de vida y 180 monedas. Torre básica: 50 monedas, 12 de daño, alcance 120 px e intervalo 0,8 s. Largo alcance: 90 monedas, 20 de daño, alcance 200 px e intervalo 1,4 s. La recompensa es 10 monedas y cada escape causa 1 de daño. La primera oleada tiene seis enemigos, vida 32 y velocidad 40 px/s; cada oleada añade dos enemigos, 12 de vida y 3 px/s. Separación de apariciones: 0,85 s; espera de oleada: 5 s desde que queda vacío el campo. Parámetros en `include/GameTypes.h`.

## Pruebas reproducibles

Desde PowerShell en la raíz:

```powershell
.\tools\build.ps1 -Tests -Configuration Debug
& .\build\tests\Debug\TowerDefense2D.Tests.exe --screenshots
& .\build\tests\Debug\TowerDefense2D.Tests.exe --benchmark
```

La batería lógica usa los mismos módulos que el juego y devuelve un código distinto de cero si falla. Las capturas usan un contexto OpenGL real; victoria y derrota se alcanzan por simulación con configuraciones de prueba identificadas en `tests/tests.cpp`. El benchmark muestra 70 enemigos de vida alta y 13 torres durante 1.920 cuadros (120 de calentamiento, 1.800 medidos), a resolución 1280 × 720 y MSAA x4. Las nuevas capturas, tiempos y registros se guardan en `docs/evidence/visual/`; la evidencia anterior se conserva. No mide latencia física de entrada ni equivale a una prueba del laboratorio.

Revisión estética actual: [informe A–G](docs/REVISION_VISUAL.md), [comparación de pantallas](docs/evidence/visual/comparacion.html). Auditoría de centrado, PNG y conservación de lógica: `python tools/audit_visual.py` (requiere Pillow). No es necesario Python para jugar.

Resultados y límites: [informe de validación](docs/VALIDACION.md). Registro permanente: [bitácora](docs/BITACORA.md). Decisiones y mapa de requisitos: [trazabilidad](docs/TRAZABILIDAD.md).

La documentación académica está en revisión y se gestiona por separado de esta actualización. El Word se conserva únicamente en la computadora del responsable y se incorporará a `docs/` cuando entregue la versión aprobada. Los documentos presentes en commits anteriores son históricos y no constituyen un APF3 aprobado.

## Estructura

`TowerDefense2D/main.cpp` inicializa y cierra Raylib. `include/` declara nueve clases; `src/` las implementa. `GameTypes.h` contiene enumerados y estructuras auxiliares. `assets/` contiene PNG y fuentes con su procedencia. `data/` recibe eventos locales. `tests/` verifica reglas e integración. `tools/` contiene construcción y verificación. `docs/` conserva decisiones y evidencias.

CSV acumulativo UTF-8 en `data/eventos.csv`, relativo al directorio de trabajo; contiene exactamente diez columnas y ocho eventos. Fechas UTC con `Z`, identificador por partida y tiempo activo sin pausa. Se comprueba el encabezado antes de continuar, se escapan campos y se muestran errores de escritura. No guarda partidas recuperables. No ejecutar dos instancias escribiendo al mismo CSV; no se implementa concurrencia entre procesos ni rotación automática. Respaldar el CSV con el juego cerrado si se desea conservarlo.

## Estado y pendientes

RF-01 a RF-15 implementados y comprobados automáticamente. RNF-01 tiene medición local; faltan el equipo de laboratorio y la aceptación del docente. RNF-07 (100 ms entrada–respuesta visual) queda pendiente de medición específica. La evaluación de usabilidad y la sustentación individual son actividades humanas pendientes.

La revisión visual incorpora los sprites reales de Figma, tipografías y jerarquía de los mockups. Menú y resultados conservan aproximaciones por código de sus emblemas; las ilustraciones de referencia no disponen de todos sus elementos como capas editables. El informe visual detalla la fidelidad por pantalla y los pendientes. No se implementan multijugador, A*, venta, mejoras, reparación ni guardado/carga.

Las mejoras se preparan en `feature/mejoras-visuales`, se revisan mediante un PR hacia `develop` y después mediante otro PR hacia `main`. La validación local de publicación está en `docs/evidence/publicacion/`. No hay protección de ramas, revisión humana ni CI configuradas; la autorización de integración corresponde al usuario. Las pruebas automatizadas son de Codex bajo solicitud de Kevin.
