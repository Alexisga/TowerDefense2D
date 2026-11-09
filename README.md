# TowerDefense2D

Tower Defense 2D académico en C++20, Visual Studio 2026 y Raylib 6.2.0 (NuGet). Versión jugable para PC3: ocho oleadas automáticas, dos torres, economía, proyectiles, pausa, victoria, derrota y reinicio. Interfaz base 1280 × 720.

Repositorio oficial **privado**: https://github.com/Alexisga/TowerDefense2D. Los cambios realizados por Codex se identifican como tales en Git; no representan pruebas manuales ni contribuciones de Naomy o Diego.

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

Los sprites temporales se generan al iniciar; no se necesitan archivos de texturas externos. Georgia y Segoe UI se usan si existen en Windows; hay fuente de reserva de Raylib. No se redistribuyen las fuentes de Microsoft.

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

La batería lógica usa los mismos módulos que el juego y devuelve un código distinto de cero si falla. Las capturas usan un contexto OpenGL real; victoria y derrota se alcanzan por simulación con configuraciones de prueba identificadas en `tests/tests.cpp`. El benchmark muestra 70 enemigos de vida alta y 13 torres durante 1.920 cuadros (120 de calentamiento, 1.800 medidos), a resolución 1280 × 720 y MSAA x4. Guarda tiempos crudos y resumen en `docs/evidence/`. No mide latencia física de entrada ni equivale a una prueba del laboratorio.

Resultados y límites: [informe de validación](docs/VALIDACION.md). Registro permanente: [bitácora](docs/BITACORA.md). Decisiones y mapa de requisitos: [trazabilidad](docs/TRAZABILIDAD.md).

## Estructura

`TowerDefense2D/main.cpp` inicializa y cierra Raylib. `include/` declara nueve clases; `src/` las implementa. `GameTypes.h` contiene enumerados y estructuras auxiliares. `assets/` explica los sprites temporales. `data/` recibe eventos locales. `tests/` verifica reglas e integración. `tools/` contiene construcción y verificación. `docs/` conserva decisiones y evidencias.

CSV acumulativo UTF-8 en `data/eventos.csv`, relativo al directorio de trabajo; contiene exactamente diez columnas y ocho eventos. Fechas UTC con `Z`, identificador por partida y tiempo activo sin pausa. Se comprueba el encabezado antes de continuar, se escapan campos y se muestran errores de escritura. No guarda partidas recuperables. No ejecutar dos instancias escribiendo al mismo CSV; no se implementa concurrencia entre procesos ni rotación automática. Respaldar el CSV con el juego cerrado si se desea conservarlo.

## Estado y pendientes

RF-01 a RF-15 implementados y comprobados automáticamente. RNF-01 tiene medición local; faltan el equipo de laboratorio y la aceptación del docente. RNF-07 (100 ms entrada–respuesta visual) queda pendiente de medición específica. La evaluación de usabilidad y la sustentación individual son actividades humanas pendientes.

Los recursos visuales son temporales: falta exportar sprites originales de Figma. Se conserva la distribución de los mockups, sin afirmar fidelidad exacta. No se implementan multijugador, A*, venta, mejoras, reparación ni guardado/carga.

Se trabaja en `develop` y se integra en `main` después de validar. No hay protección de ramas, revisión humana ni CI configuradas; no se afirma lo contrario. Los commits son incrementos reales del 9 de noviembre de 2026. Las pruebas automatizadas son de Codex bajo solicitud de Kevin.
