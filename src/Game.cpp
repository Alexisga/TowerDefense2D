#include "Game.h"
#include <algorithm>
#include <cmath>
Game::Game(GameConfig c,std::filesystem::path log):config_(c),waves_(c),logger_(std::move(log)),coins_(c.initialCoins),life_(c.baseLife){}
void Game::Log(const std::string& event,const std::string& detail,const std::string& result){
    logger_.Write({matchId_,activeTime_,event,waves_.Current(),life_,coins_,kills_,result,detail});
}
void Game::Start(){
    if(state_!=GameState::MENU && state_!=GameState::VICTORIA && state_!=GameState::DERROTA)return;
    enemies_.clear();towers_.clear();shots_.clear();waves_.Reset();coins_=config_.initialCoins;life_=config_.baseLife;
    kills_=0;activeTime_=0;selected_=false;error_=false;messageTime_=0;state_=GameState::ACTIVA;
    message_="Selecciona una torre y una casilla libre.";matchId_=CsvLogger::NewMatchId();Log("INICIO_PARTIDA");
}
bool Game::SelectTower(TowerType t){if(state_!=GameState::ACTIVA)return false;selection_=t;selected_=true;error_=false;return true;}
bool Game::Occupied(int slot) const{return std::any_of(towers_.begin(),towers_.end(),[slot](const Tower& t){return t.Slot()==slot;});}
bool Game::TryBuild(Vector2 p){
    if(state_!=GameState::ACTIVA || !selected_)return false;
    int slot=map_.SlotAt(p);error_=true;messageTime_=2;
    if(slot<0){message_="No puedes construir en esta zona.";return false;}
    if(Occupied(slot)){message_="Esta casilla ya está ocupada.";return false;}
    auto stats=Stats(selection_);
    if(coins_<stats.cost){message_="Monedas insuficientes para construir esta torre.";return false;}
    // Validaciones completas antes de modificar el saldo o las entidades.
    towers_.emplace_back(selection_,map_.BuildSlots()[slot],slot);coins_-=stats.cost;error_=false;
    message_="Torre construida: -"+std::to_string(stats.cost)+" monedas.";
    Log("TORRE_CONSTRUIDA",std::string(selection_==TowerType::Basic?"Basic":"LongRange")+"; casilla="+std::to_string(slot));return true;
}
void Game::TogglePause(){
    if(state_==GameState::ACTIVA){state_=GameState::PAUSADA;Log("PAUSA");}
    else if(state_==GameState::PAUSADA){state_=GameState::ACTIVA;Log("REANUDACION");}
}
void Game::Restart(){if(state_==GameState::VICTORIA||state_==GameState::DERROTA)Start();}
void Game::HandleInput(){
    const Vector2 mouse=GetMousePosition();bool click=IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    if(state_==GameState::MENU){if((click&&CheckCollisionPointRec(mouse,GameUI::ActionButton()))||IsKeyPressed(KEY_ENTER))Start();return;}
    if(state_==GameState::VICTORIA||state_==GameState::DERROTA){if((click&&CheckCollisionPointRec(mouse,GameUI::ActionButton()))||IsKeyPressed(KEY_ENTER))Restart();return;}
    if(IsKeyPressed(KEY_P)||(click&&CheckCollisionPointRec(mouse,GameUI::PauseButton()))){TogglePause();return;}
    if(state_==GameState::PAUSADA){if((click&&CheckCollisionPointRec(mouse,GameUI::ActionButton()))||IsKeyPressed(KEY_ENTER))TogglePause();return;}
    if(IsKeyPressed(KEY_ESCAPE)||IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){selected_=false;messageTime_=0;}
    if(IsKeyPressed(KEY_ONE))SelectTower(TowerType::Basic);if(IsKeyPressed(KEY_TWO))SelectTower(TowerType::LongRange);
    if(click){
        if(CheckCollisionPointRec(mouse,GameUI::BasicButton()))SelectTower(TowerType::Basic);
        else if(CheckCollisionPointRec(mouse,GameUI::LongButton()))SelectTower(TowerType::LongRange);
        else if(CheckCollisionPointRec(mouse,{24,112,888,536}))TryBuild(mouse);
    }
}
void Game::Update(){Update(GetFrameTime());}
void Game::Update(float dt){
    if(state_!=GameState::ACTIVA || !std::isfinite(dt) || dt<=0)return;
    activeTime_+=dt;messageTime_=std::max(0.0f,messageTime_-dt);
    if(waves_.Update(dt,enemies_.empty(),enemies_,map_.Waypoints().front()))Log("INICIO_OLEADA");
    for(auto& e:enemies_)e.Update(dt,map_.Waypoints());
    for(auto& t:towers_)t.Update(dt,enemies_,shots_);
    for(auto& s:shots_)s.Update(dt,enemies_);
    for(const auto& e:enemies_){
        if(e.State()==EnemyState::Killed){++kills_;coins_+=e.Reward();Log("ENEMIGO_ELIMINADO","enemigo="+std::to_string(e.Id()));}
        else if(e.State()==EnemyState::Escaped){life_=std::max(0,life_-e.BaseDamage());Log("BASE_DAÑADA","enemigo="+std::to_string(e.Id()));}
    }
    std::erase_if(enemies_,[](const Enemy& e){return !e.Active();});std::erase_if(shots_,[](const Projectile& s){return !s.Active();});
    if(life_==0)Finish(GameState::DERROTA);else if(waves_.Finished(enemies_.empty()))Finish(GameState::VICTORIA);
}
void Game::Finish(GameState s){state_=s;selected_=false;error_=s==GameState::DERROTA;message_=error_?"La fortaleza ha caído.":"¡Todas las oleadas han sido superadas!";Log("PARTIDA_FINALIZADA","",error_?"DERROTA":"VICTORIA");}
void Game::Draw(){Draw(GetMousePosition());}
void Game::Draw(Vector2 pointer){
    ClearBackground(Color{23,32,23,255});map_.Draw(assets_);
    for(const auto& t:towers_)t.Draw(assets_);
    for(const auto& e:enemies_)e.Draw(assets_,activeTime_);
    for(const auto& s:shots_)s.Draw();
    std::string message=message_;bool error=error_;
    if(state_==GameState::ACTIVA && messageTime_<=0){
        error=false;message="Selecciona una torre y una casilla libre.";
        if(selected_){
            auto p=pointer;int slot=map_.SlotAt(p);
            if(CheckCollisionPointRec(p,{24,112,888,536})){
                bool valid=slot>=0&&!Occupied(slot)&&coins_>=Stats(selection_).cost;
                Vector2 center=slot>=0?map_.BuildSlots()[slot]:p;
                Color c=valid?Color{134,205,122,255}:Color{230,89,68,255};
                BeginScissorMode(25,113,886,534);
                DrawCircleV(center,Stats(selection_).range,Fade(c,0.1f));DrawCircleLinesV(center,Stats(selection_).range,Fade(c,0.6f));
                DrawRectangleLinesEx({center.x-22,center.y-22,44,44},2,c);
                if(valid)assets_.Sprite(selection_==TowerType::Basic?"basic":"long",center,1,Fade(WHITE,0.55f));
                else {DrawLineEx({center.x-7,center.y-7},{center.x+7,center.y+7},3,c);DrawLineEx({center.x-7,center.y+7},{center.x+7,center.y-7},3,c);}
                EndScissorMode();error=!valid;message=valid?"Casilla válida. Clic para construir: "+std::to_string(Stats(selection_).cost)+" monedas.":(slot<0?"No puedes construir en esta zona.":(Occupied(slot)?"Esta casilla ya está ocupada.":"Monedas insuficientes."));
            }else message="Torre seleccionada. Elige una casilla marcada.";
        }
    }
    if(!logger_.Error().empty()){message="Registro CSV: "+logger_.Error();error=true;}
    if(state_==GameState::PAUSADA){message="La partida está en pausa.";error=false;}
    float next=enemies_.empty()&&waves_.Remaining()==0?waves_.Timer():0;
    ui_.Draw(assets_,{state_,life_,config_.baseLife,coins_,waves_.Current(),config_.totalWaves,kills_,selected_,selection_,message,error,next,activeTime_});
}
