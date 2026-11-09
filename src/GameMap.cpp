#include "GameMap.h"
#include "AssetManager.h"
#include <algorithm>
#include <cmath>
GameMap::GameMap()
    : waypoints_{{24, 252},  {240, 252}, {240, 452}, {480, 452}, {480, 212},
                 {648, 212}, {648, 392}, {820, 392}, {820, 332}},
      slots_{{144, 356}, {336, 320}, {384, 344}, {576, 320}, {576, 500}, {624, 500}, {336, 548},
             {384, 548}, {744, 548}, {768, 476}, {96, 452},  {432, 160}, {720, 176}}
{
}
int GameMap::SlotAt(Vector2 p) const
{
    for (size_t i = 0; i < slots_.size(); ++i)
        if (std::abs(p.x - slots_[i].x) <= 22 && std::abs(p.y - slots_[i].y) <= 22)
            return static_cast<int>(i);
    return -1;
}
bool GameMap::IsPermitted(Vector2 p) const
{
    return SlotAt(p) >= 0;
}
void GameMap::Draw(const AssetManager &a) const
{
    const Rectangle area{24, 112, 888, 536};
    DrawRectangleRec(area, Color{57, 91, 54, 255});
    BeginScissorMode(25, 113, 886, 534);
    // Suelo y vegetación deterministas: no se altera el azar del juego.
    for (int i = 0; i < 190; ++i)
    {
        float x = static_cast<float>(30 + (i * 137) % 875),
              y = static_cast<float>(118 + (i * 83) % 524);
        DrawEllipse(static_cast<int>(x), static_cast<int>(y), 30, 13, Color{69, 103, 57, 65});
        DrawLineV({x, y}, {x + 2, y - 5}, Color{108, 130, 65, 110});
    }
    for (size_t i = 1; i < waypoints_.size(); ++i)
    {
        const auto p = waypoints_[i - 1], q = waypoints_[i];
        DrawLineEx(p, q, 80, Color{90, 108, 55, 255});
        DrawCircleV(p, 40, Color{90, 108, 55, 255});
        DrawCircleV(q, 40, Color{90, 108, 55, 255});
    }
    for (size_t i = 1; i < waypoints_.size(); ++i)
    {
        auto p = waypoints_[i - 1], q = waypoints_[i];
        DrawLineEx(p, q, 65, Color{194, 162, 99, 255});
        DrawCircleV(p, 32.5f, Color{194, 162, 99, 255});
        DrawCircleV(q, 32.5f, Color{194, 162, 99, 255});
        DrawLineEx(p, q, 49, Color{204, 174, 112, 255});
    }
    for (auto p : slots_)
    {
        const Color c{160, 189, 122, 170};
        for (int dx : {-1, 1})
            for (int dy : {-1, 1})
            {
                Vector2 k{p.x + dx * 22, p.y + dy * 22};
                DrawLineEx(k, {k.x - dx * 10, k.y}, 1, c);
                DrawLineEx(k, {k.x, k.y - dy * 10}, 1, c);
            }
    }
    for (int i = 0; i < 18; ++i)
    {
        const float x = static_cast<float>(36 + i * 54);
        a.Sprite("tree", {x, 137 + static_cast<float>((i * 11) % 30)}, 0.8f);
        a.Sprite("tree", {x, 645 + static_cast<float>((i * 17) % 20)}, 1.1f);
    }
    for (int i = 0; i < 5; ++i)
        a.Sprite("tree", {38, 360 + static_cast<float>(i * 58)}, 0.85f);
    a.Sprite("castle", {820, 340}, 1);
    a.Text("ENTRADA", {48, 209}, 12, Color{243, 224, 178, 255});
    a.Text("BASE", {798, 247}, 12, Color{243, 224, 178, 255});
    DrawTriangle({35, 244}, {35, 260}, {49, 252}, Color{63, 49, 30, 255});
    EndScissorMode();
    DrawRectangleRoundedLinesEx(area, 0.025f, 6, 2, Color{172, 138, 78, 255});
}
