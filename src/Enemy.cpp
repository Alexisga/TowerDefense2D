#include "Enemy.h"
#include "AssetManager.h"
#include <algorithm>
#include <cmath>
Enemy::Enemy(int id, Vector2 start, int life, float speed, int reward, int damage)
    : id_(id), life_(std::max(1, life)), maxLife_(life_), reward_(std::max(0, reward)),
      baseDamage_(std::max(0, damage)), position_(start), speed_(std::max(0.0f, speed))
{
}
void Enemy::Update(float dt, const std::vector<Vector2> &route)
{
    if (!Active() || route.empty())
        return;
    float remaining = std::max(0.0f, dt) * speed_;
    // Consumir la distancia sobrante evita saltos o desvíos en las esquinas.
    while (waypoint_ < route.size())
    {
        Vector2 target = route[waypoint_];
        float d = std::sqrt(DistanceSquared(position_, target));
        if (d > remaining && d > 0)
        {
            position_.x += (target.x - position_.x) * remaining / d;
            position_.y += (target.y - position_.y) * remaining / d;
            return;
        }
        position_ = target;
        remaining -= d;
        ++waypoint_;
    }
    state_ = EnemyState::Escaped;
}
void Enemy::TakeDamage(int damage)
{
    if (!Active())
        return;
    life_ = std::max(0, life_ - std::max(0, damage));
    if (life_ == 0)
        state_ = EnemyState::Killed;
}
void Enemy::Draw(const AssetManager &a, float t) const
{
    if (!Active())
        return;
    a.Sprite("enemy", {position_.x, position_.y + std::sin(t * 9 + id_) * 1.5f}, 1);
    DrawRectangle(static_cast<int>(position_.x) - 17, static_cast<int>(position_.y) - 32, 34, 5,
                  Color{31, 36, 26, 255});
    DrawRectangle(static_cast<int>(position_.x) - 16, static_cast<int>(position_.y) - 31,
                  static_cast<int>(32.0f * life_ / maxLife_), 3, Color{120, 190, 101, 255});
}
