#pragma once
#include "GameTypes.h"
#include <vector>
class AssetManager;
class Enemy
{
  public:
    Enemy(int id, Vector2 start, int life, float speed, int reward = 10, int baseDamage = 1);
    void Update(float dt, const std::vector<Vector2> &route);
    void TakeDamage(int damage);
    void Draw(const AssetManager &assets, float activeTime) const;
    int Id() const
    {
        return id_;
    }
    Vector2 Position() const
    {
        return position_;
    }
    int Life() const
    {
        return life_;
    }
    int MaxLife() const
    {
        return maxLife_;
    }
    int Reward() const
    {
        return reward_;
    }
    int BaseDamage() const
    {
        return baseDamage_;
    }
    bool Active() const
    {
        return state_ == EnemyState::Active;
    }
    EnemyState State() const
    {
        return state_;
    }

  private:
    int id_, life_, maxLife_, reward_, baseDamage_;
    Vector2 position_;
    float speed_;
    size_t waypoint_ = 1;
    EnemyState state_ = EnemyState::Active;
};
