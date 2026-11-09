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
- Compilación Debug x64 real mediante MSBuild 18: correcta. Se corrigió el conflicto de runtime de la biblioteca NuGet usando /MD también en Debug. Pendiente recompilar tras quitar una advertencia de conversión.
- Repositorio privado creado y verificado: https://github.com/Alexisga/TowerDefense2D (propietario Alexisga, permisos de administrador verificados por API). La autenticación no se incluye en los archivos.
