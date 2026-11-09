#pragma once
#include "Projectile.h"
class AssetManager;
class Tower
{
  public:
    Tower(TowerType type, Vector2 position, int slot);
    void Update(float dt, const std::vector<Enemy> &enemies, std::vector<Projectile> &shots);
    int FindTarget(const std::vector<Enemy> &enemies) const;
    void Draw(const AssetManager &assets) const;
    int Slot() const
    {
        return slot_;
    }
    TowerType Type() const
    {
        return type_;
    }
    Vector2 Position() const
    {
        return position_;
    }
    float Cooldown() const
    {
        return cooldown_;
    }

  private:
    TowerType type_;
    Vector2 position_;
    int slot_;
    float cooldown_ = 0;
};
