#pragma once
#include "Enemy.h"
class Projectile {
public:
    Projectile(Vector2 start,int targetId,int damage,float speed=400);
    void Update(float dt,std::vector<Enemy>& enemies);
    void Draw() const;
    bool Active() const { return active_; }
    Vector2 Position() const { return position_; }
    int TargetId() const { return targetId_; }
private:
    Vector2 position_,previous_;
    int targetId_,damage_;
    float speed_;
    bool active_=true;
};
