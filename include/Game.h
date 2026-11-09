#pragma once
#include "AssetManager.h"
#include "CsvLogger.h"
#include "GameMap.h"
#include "GameUI.h"
#include "Tower.h"
#include "WaveManager.h"
#include <filesystem>
class Game {
public:
    explicit Game(GameConfig config={},std::filesystem::path log="data/eventos.csv");
    void LoadAssets(){assets_.Load();}
    void HandleInput();
    void Update();
    void Update(float dt);
    void Draw();
    void Start();
    bool SelectTower(TowerType type);
    bool TryBuild(Vector2 position);
    void TogglePause();
    void Restart();
    GameState State() const{return state_;}
    int Coins() const{return coins_;}
    int Life() const{return life_;}
    int Kills() const{return kills_;}
    float ActiveTime() const{return activeTime_;}
    const std::vector<Enemy>& Enemies() const{return enemies_;}
    const std::vector<Tower>& Towers() const{return towers_;}
    const std::vector<Projectile>& Projectiles() const{return shots_;}
    const WaveManager& Waves() const{return waves_;}
    const GameMap& Map() const{return map_;}
    const std::string& MatchId() const{return matchId_;}
    const std::string& LoggerError() const{return logger_.Error();}
private:
    void Log(const std::string& event,const std::string& detail="",const std::string& result="");
    bool Occupied(int slot) const;
    void Finish(GameState result);
    GameConfig config_;
    GameState state_=GameState::MENU;
    GameMap map_;
    WaveManager waves_;
    AssetManager assets_;
    GameUI ui_;
    CsvLogger logger_;
    std::vector<Enemy> enemies_;
    std::vector<Tower> towers_;
    std::vector<Projectile> shots_;
    int coins_,life_,kills_=0;
    float activeTime_=0,messageTime_=0;
    bool selected_=false,error_=false;
    TowerType selection_=TowerType::Basic;
    std::string matchId_,message_="Selecciona una torre y una casilla libre.";
};
