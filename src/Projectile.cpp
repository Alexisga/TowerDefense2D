#include "Projectile.h"
#include <algorithm>
#include <cmath>
Projectile::Projectile(Vector2 p, int target, int damage, float speed)
    : position_(p), previous_(p), targetId_(target), damage_(damage), speed_(speed)
{
}
void Projectile::Update(float dt, std::vector<Enemy> &enemies)
{
    if (!active_)
        return;
    auto e = std::find_if(enemies.begin(), enemies.end(),
                          [this](const Enemy &x) { return x.Id() == targetId_ && x.Active(); });
    // Identificadores estables: el borrado del vector no deja punteros colgantes.
    if (e == enemies.end())
    {
        active_ = false;
        return;
    }
    previous_ = position_;
    const auto p = e->Position();
    float d = std::sqrt(DistanceSquared(position_, p)), step = speed_ * std::max(0.0f, dt);
    if (d <= step + 7)
    {
        position_ = p;
        e->TakeDamage(damage_);
        active_ = false;
        return;
    }
    position_.x += (p.x - position_.x) * step / d;
    position_.y += (p.y - position_.y) * step / d;
}
void Projectile::Draw() const
{
    if (!active_)
        return;
    const float length = std::sqrt(DistanceSquared(previous_, position_));
    if (length < 0.001f)
        return;
    const Vector2 direction{(position_.x - previous_.x) / length,
                            (position_.y - previous_.y) / length};
    const Vector2 tail{position_.x - direction.x * 17, position_.y - direction.y * 17};
    const Vector2 headBase{position_.x - direction.x * 6, position_.y - direction.y * 6};
    const Vector2 side{-direction.y * 3, direction.x * 3};
    DrawLineEx(tail, headBase, 2, Color{224, 199, 142, 255});
    DrawTriangle(position_, {headBase.x + side.x, headBase.y + side.y},
                 {headBase.x - side.x, headBase.y - side.y}, Color{255, 239, 192, 255});
    DrawLineEx(tail, {tail.x + direction.x * 5 + side.x, tail.y + direction.y * 5 + side.y}, 2,
               Color{242, 223, 183, 255});
    DrawLineEx(tail, {tail.x + direction.x * 5 - side.x, tail.y + direction.y * 5 - side.y}, 2,
               Color{242, 223, 183, 255});
}
