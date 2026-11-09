# Decisiones y trazabilidad

## Fuentes y reglas

Fuente técnica: APF2 PDF de 62 páginas y su DOCX editable. Los documentos de semanas 11–13 se usan como contexto académico, sin extender el alcance con las tecnologías de otros grupos. No se localizó la variante de PDF con `(1)`. No se usó código de ZIPs anteriores.

El PDF exige nueve clases y RF-01 a RF-15. Su objetivo de 60 FPS prevalece sobre la mención de 30 FPS de Semana 11. El trabajo preserva el calendario académico como planificación; implementar anticipadamente mecánicas de los sprints 5 y 6 no equivale a cerrar formalmente esos sprints. Se registra el trabajo efectivo del 2026-11-09.

## Requisitos funcionales

| RF | Implementación | Evidencia automatizada |
|---|---|---|
| 01 | Enemy::Update, GameMap::Waypoints | Movimiento, esquina, distancia sobrante y fin exacto |
| 02 | WaveManager::Update | Conteos de las ocho oleadas |
| 03 | Game::SelectTower, GameUI | Construcción de ambos tipos; recorrido gráfico |
| 04 | GameMap::SlotAt, Game::TryBuild | Ruta, exterior, panel, ocupación, saldo y separación de casillas |
| 05 | Game::TryBuild | Cobros únicos de 50 y 90 y rechazos sin cobro |
| 06 | Tower::FindTarget | Dentro, fuera y borde de alcance |
| 07 | Tower::FindTarget y Update | Más cercano, exclusión de eliminados, intervalo |
| 08 | Projectile::Update, Enemy::TakeDamage | Daño exacto, impacto único, vida mínima cero |
| 09 | Game::Update, Projectile::Update | Retiro de eliminados y proyectiles sin objetivo |
| 10 | Game::Update | Recompensa única bajo ataques concurrentes; ningún pago por escape |
| 11 | Enemy::Update, Game::Update y Finish | Escapes, daño único, derrota, vida mínima cero |
| 12 | WaveManager::Finished, Game::Finish | Victoria con ocho oleadas, incluso campaña con parámetros reales |
| 13 | WaveManager::CountFor, LifeFor, SpeedFor | Progresión de los tres parámetros |
| 14 | Game::TogglePause y Update | Congelación exacta de posiciones, proyectiles, vida, saldo, oleada y temporizadores |
| 15 | Game::Restart y Start | Restauración completa desde ambos resultados y nuevo partida_id |

Pruebas ejecutables en tests/tests.cpp. Resultados en docs/evidence/functional-tests.txt. Todas son ejecuciones de herramientas; no se atribuyen a estudiantes como sesiones manuales.

## Decisiones de implementación

- Trece casillas explícitas, congruentes con los espacios marcados de los mockups. Los clics se ajustan al centro de la casilla y ninguna construcción invade el camino.
- Un ID estable identifica al objetivo de cada proyectil. No se almacenan punteros a elementos de un vector que puede reorganizarse.
- La torre adquiere el objetivo activo más cercano al disparar; un proyectil ya lanzado lo persigue hasta impactar o perder el objetivo. El alcance se comprueba al adquirirlo, no se cancela un disparo ya emitido al salir el enemigo del alcance.
- Una entidad sale como Killed o Escaped, nunca ambos. El coordinador liquida una sola recompensa o daño y la retira en el mismo cuadro. Se admite que varias torres elijan el mismo objetivo.
- Solo Update de ACTIVA hace avanzar la simulación. Los mensajes y tiempos de combate quedan congelados en pausa; la entrada para reanudar y el dibujo permanecen activos.
- Los parámetros iniciales se basan en los mockups como calibración provisional, no como valores aprobados. La campaña automatizada real demuestra al menos una estrategia ganadora; falta evaluación de dificultad con jugadores.
- Sprites independientes de primitivas 2D en AssetManager, convertidos en texturas GPU al cargar. No se utiliza ninguna captura completa como interfaz.
- CSV en UTF-8 con fechas UTC ISO 8601; IDs de reloj de alta resolución y contador local. La aplicación es de una sola instancia por archivo de registro. No se presupone bloqueo entre procesos.
- Runtime C++ Debug /MDd: se excluye MSVCRT de release introducida por la biblioteca estática C de NuGet. Release usa /MD. Se conserva v145 y raylib 6.2.0.

## Seguridad y límites

Aplicación local sin cuentas, contraseñas, servicios externos de juego ni datos personales del jugador. No se añaden cuentas de servicio, certificados ni cifrado sin necesidad. GitHub usa la autorización externa del usuario; ninguna credencial se escribe en código o CSV. El resultado del escaneo local de patrones es evidencia acotada, no una certificación ni un análisis exhaustivo de secretos.

La retención y rotación automática del CSV no están implementadas. Respaldo recomendado con la aplicación cerrada; el historial Git conserva el código, no los registros privados locales. Protecciones de rama, aprobación de terceros y CI no están configuradas.
