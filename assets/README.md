# Recursos visuales

`figma/` contiene **23 PNG individuales** exportados del archivo original del usuario: siete sprites del escenario/personajes, diez iconos, un cursor, tres remates de interfaz y dos capas de suelo/vegetación. El detalle de nombres y nodos está en [manifest.json](figma/manifest.json).

Las texturas conservan RGBA, proporción y transparencia; son exportaciones a escala 2, salvo las dos capas del mapa a escala 1. No se utiliza una captura completa de pantalla como interfaz del juego. El HUD, los botones y los estados se dibujan con datos vivos; enemigos, torres y proyectiles siguen siendo entidades funcionales.

`fonts/` incluye Inter Regular, Inter SemiBold y Cinzel Bold bajo SIL OFL 1.1, además de los originales variables y sus licencias. Las instancias estáticas se generan con `tools/prepare_visual_fonts.py`. Consulte [SOURCES.md](SOURCES.md) para procedencia y condiciones.

AssetManager conserva los sprites procedurales anteriores como reserva y reemplaza cada textura solo si el PNG carga correctamente. También conserva fuentes de Windows y Raylib como alternativas de carga, sin redistribuir fuentes de Microsoft.

El camino se dibuja sobre los waypoints existentes; los paneles combinan primitivas y adornos exportados. Corona, escudos grandes, trofeo y flechas son aproximaciones originales por código. No representan exportaciones del Figma ni reproducciones exactas de las ilustraciones de referencia.
