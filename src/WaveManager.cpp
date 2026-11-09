#include "WaveManager.h"
#include <algorithm>
WaveManager::WaveManager(GameConfig c):config_(c){Reset();}
void WaveManager::Reset(){current_=0;remaining_=0;nextId_=1;timer_=config_.waveDelay;spawning_=false;}
int WaveManager::CountFor(int w) const{return std::max(1,config_.firstWaveCount+(w-1)*config_.countGrowth);}
int WaveManager::LifeFor(int w) const{return std::max(1,config_.enemyLife+(w-1)*config_.lifeGrowth);}
float WaveManager::SpeedFor(int w) const{return std::max(1.0f,config_.enemySpeed+(w-1)*config_.speedGrowth);}
bool WaveManager::Update(float dt,bool empty,std::vector<Enemy>& enemies,Vector2 spawn) {
    if(Finished(empty)) return false;
    if(!spawning_ && !empty) return false;
    timer_-=std::max(0.0f,dt);
    bool started=false;
    if(!spawning_ && timer_<=0 && current_<config_.totalWaves) {
        ++current_;remaining_=CountFor(current_);spawning_=true;timer_=0;started=true;
    }
    while(spawning_ && timer_<=0) {
        enemies.emplace_back(nextId_++,spawn,LifeFor(current_),SpeedFor(current_));
        --remaining_;timer_+=std::max(0.01f,config_.spawnInterval);
        if(remaining_==0){spawning_=false;timer_=config_.waveDelay;}
    }
    return started;
}
bool WaveManager::Finished(bool empty) const{return current_>=config_.totalWaves && !spawning_ && remaining_==0 && empty;}
