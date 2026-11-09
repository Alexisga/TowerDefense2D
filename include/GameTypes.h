#pragma once
#include "raylib.h"
#include <string>

enum class GameState { MENU, ACTIVA, PAUSADA, VICTORIA, DERROTA };
enum class TowerType { Basic, LongRange };
enum class EnemyState { Active, Killed, Escaped };
struct TowerStats { int cost; float range; int damage; float interval; };
inline TowerStats Stats(TowerType type) {
    return type == TowerType::Basic ? TowerStats{50,120,12,0.8f} : TowerStats{90,200,20,1.4f};
}
inline float DistanceSquared(Vector2 a, Vector2 b) {
    const float x=a.x-b.x, y=a.y-b.y; return x*x+y*y;
}
struct GameConfig {
    int initialCoins=180, baseLife=20, totalWaves=8;
    int firstWaveCount=6, countGrowth=2, enemyLife=32, lifeGrowth=12;
    float enemySpeed=40, speedGrowth=3, spawnInterval=0.85f, waveDelay=5;
};
struct LogEvent {
    std::string matchId;
    float activeTime=0;
    std::string event;
    int wave=0, baseLife=20, coins=180, kills=0;
    std::string result, detail;
};
struct UiSnapshot {
    GameState state=GameState::MENU;
    int life=20, maxLife=20, coins=180, wave=0, totalWaves=8, kills=0;
    bool hasSelection=false;
    TowerType selection=TowerType::Basic;
    std::string message;
    bool error=false;
    float nextWave=0, activeTime=0;
};
