#include "Tower.h"
#include "AssetManager.h"
#include <algorithm>
Tower::Tower(TowerType type, Vector2 p, int slot) : type_(type), position_(p), slot_(slot) {}
int Tower::FindTarget(const std::vector<Enemy> &enemies) const
{
    float best = Stats(type_).range * Stats(type_).range;
    int target = -1;
    for (const auto &e : enemies)
        if (e.Active())
        {
            float d = DistanceSquared(position_, e.Position());
            if (d <= best && (target < 0 || d < best))
            {
                best = d;
                target = e.Id();
            }
        }
    return target;
}
void Tower::Update(float dt, const std::vector<Enemy> &enemies, std::vector<Projectile> &shots)
{
    cooldown_ = std::max(0.0f, cooldown_ - std::max(0.0f, dt));
    if (cooldown_ > 0)
        return;
    int target = FindTarget(enemies);
    if (target >= 0)
    {
        shots.emplace_back(position_, target, Stats(type_).damage);
        cooldown_ = Stats(type_).interval;
    }
}
void Tower::Draw(const AssetManager &a) const
{
    a.Sprite(type_ == TowerType::Basic ? "basic" : "long", position_, 1);
}
