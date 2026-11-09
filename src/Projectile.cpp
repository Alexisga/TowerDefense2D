#include "Projectile.h"
#include <algorithm>
#include <cmath>
Projectile::Projectile(Vector2 p,int target,int damage,float speed) : position_(p),previous_(p),targetId_(target),damage_(damage),speed_(speed) {}
void Projectile::Update(float dt,std::vector<Enemy>& enemies) {
    if(!active_) return;
    auto e=std::find_if(enemies.begin(),enemies.end(),[this](const Enemy& x){return x.Id()==targetId_ && x.Active();});
    // Identificadores estables: el borrado del vector no deja punteros colgantes.
    if(e==enemies.end()) {active_=false;return;}
    previous_=position_;
    const auto p=e->Position();
    float d=std::sqrt(DistanceSquared(position_,p)), step=speed_*std::max(0.0f,dt);
    if(d<=step+7) {position_=p;e->TakeDamage(damage_);active_=false;return;}
    position_.x+=(p.x-position_.x)*step/d; position_.y+=(p.y-position_.y)*step/d;
}
void Projectile::Draw() const {
    if(!active_) return;
    DrawLineEx(previous_,position_,2,Color{248,218,135,255});
    DrawCircleV(position_,3,Color{255,238,180,255});
}
