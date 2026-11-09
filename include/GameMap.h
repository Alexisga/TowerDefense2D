#pragma once
#include "GameTypes.h"
#include <vector>
class AssetManager;
class GameMap {
public:
    GameMap();
    const std::vector<Vector2>& Waypoints() const { return waypoints_; }
    const std::vector<Vector2>& BuildSlots() const { return slots_; }
    int SlotAt(Vector2 position) const;
    bool IsPermitted(Vector2 position) const;
    void Draw(const AssetManager& assets) const;
private:
    std::vector<Vector2> waypoints_, slots_;
};
