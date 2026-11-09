#pragma once
#include "Enemy.h"
class WaveManager {
public:
    explicit WaveManager(GameConfig config={});
    void Reset();
    bool Update(float dt,bool fieldEmpty,std::vector<Enemy>& enemies,Vector2 spawn);
    bool Finished(bool fieldEmpty) const;
    int Current() const { return current_; }
    int CountFor(int wave) const;
    int LifeFor(int wave) const;
    float SpeedFor(int wave) const;
    int Remaining() const { return remaining_; }
    float Timer() const { return timer_; }
private:
    GameConfig config_;
    int current_=0,remaining_=0,nextId_=1;
    float timer_=0;
    bool spawning_=false;
};
