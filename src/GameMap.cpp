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
    const Rectangle area{24, 110, 888, 540};
    DrawRectangleRec(area, Color{57, 91, 54, 255});
    BeginScissorMode(25, 111, 886, 538);
    a.Texture("ground", area);
    a.Texture("foliage", area);
    // Suavizar solo el dibujo de la carretera; los waypoints lógicos se conservan.
    std::vector<Vector2> road{waypoints_.front()};
    for (size_t i = 1; i + 1 < waypoints_.size(); ++i)
    {
        const auto before = waypoints_[i - 1], corner = waypoints_[i], after = waypoints_[i + 1];
        const float d1 = std::sqrt(DistanceSquared(before, corner)),
                    d2 = std::sqrt(DistanceSquared(corner, after));
        const float radius = std::min({32.0f, d1 / 2, d2 / 2});
        const Vector2 enter{corner.x + (before.x - corner.x) * radius / d1,
                            corner.y + (before.y - corner.y) * radius / d1};
        const Vector2 leave{corner.x + (after.x - corner.x) * radius / d2,
                            corner.y + (after.y - corner.y) * radius / d2};
        road.push_back(enter);
        for (int step = 1; step <= 8; ++step)
        {
            const float t = step / 8.0f, u = 1 - t;
            road.push_back({u * u * enter.x + 2 * u * t * corner.x + t * t * leave.x,
                            u * u * enter.y + 2 * u * t * corner.y + t * t * leave.y});
        }
    }
    road.push_back(waypoints_.back());
    const float widths[] = {82, 76, 68, 60, 51};
    const Color colors[] = {{65, 85, 43, 255},
                            {113, 129, 67, 255},
                            {168, 143, 87, 255},
                            {199, 168, 106, 255},
                            {215, 185, 123, 255}};
    for (int layer = 0; layer < 5; ++layer)
        for (size_t i = 1; i < road.size(); ++i)
        {
            DrawLineEx(road[i - 1], road[i], widths[layer], colors[layer]);
            DrawCircleV(road[i], widths[layer] / 2, colors[layer]);
        }
    // Piedras pequeñas junto a la carretera, sin invadir las casillas permitidas.
    for (size_t i = 1; i < road.size(); ++i)
    {
        const auto from = road[i - 1], to = road[i];
        const float length = std::sqrt(DistanceSquared(from, to));
        if (length < 24)
            continue;
        for (float d = 16; d < length; d += 43)
        {
            const float x = from.x + (to.x - from.x) * d / length,
                        y = from.y + (to.y - from.y) * d / length;
            for (int side : {-1, 1})
            {
                const float px = x - (to.y - from.y) / length * side * 32,
                            py = y + (to.x - from.x) / length * side * 32;
                DrawEllipse(static_cast<int>(px), static_cast<int>(py), 6, 3,
                            Color{166, 157, 112, 255});
                DrawEllipseLines(static_cast<int>(px), static_cast<int>(py), 6, 3,
                                 Color{216, 195, 139, 180});
            }
        }
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
    // Composición ambiental procedente de los componentes editables de Figma.
    const float trees[][3] = {{-21, -37, 1.12f}, {151, -44, 1.05f}, {327, -58, 0.95f},
                              {505, -68, 1},     {683, -44, 0.98f}, {838, -24, 1.05f},
                              {-37, 214, 0.95f}, {-25, 415, 1.18f}, {142, 470, 0.95f},
                              {376, 472, 1.06f}, {558, 473, 1},     {777, 457, 1.1f},
                              {845, 360, 1},     {92, 357, 0.74f}};
    for (const auto &t : trees)
        a.Sprite("tree", {24 + t[0] + 52 * t[2], 110 + t[1] + 122 * t[2]}, t[2]);
    const float pines[][3] = {{71, -26, 0.94f}, {250, -35, 1.1f},  {402, -45, 0.98f},
                              {590, -59, 1},    {779, -40, 1.04f}, {-14, 302, 1.07f},
                              {74, 438, 1.05f}, {251, 464, 0.9f},  {475, 459, 1.04f},
                              {680, 456, 1.1f}, {842, 239, 0.92f}, {329, 41, 0.67f}};
    for (const auto &t : pines)
        a.Sprite("pine", {24 + t[0] + 42 * t[2], 110 + t[1] + 126 * t[2]}, t[2]);
    const float rocks[][3] = {{137, 68, 0.66f},  {46, 320, 0.75f}, {388, 409, 0.8f},
                              {693, 56, 0.7f},   {844, 317, 0.7f}, {216, 421, 0.52f},
                              {579, 180, 0.52f}, {369, 127, 0.5f}};
    for (const auto &r : rocks)
    {
        a.Sprite("rock", {24 + r[0] + 33 * r[2], 110 + r[1] + 44 * r[2]}, r[2]);
    }
    a.Sprite("castle", {820, 372}, 1);
    a.Text("ENTRADA", {48, 208}, 13, Color{243, 224, 178, 255}, false, true);
    DrawTriangle({797, 233}, {843, 249}, {850, 233}, Color{47, 66, 39, 255});
    DrawLine(797, 233, 850, 233, Color{197, 161, 100, 255});
    a.CenteredText("BASE", {793, 228, 60, 19}, 12, Color{243, 224, 178, 255}, false, true);
    a.Texture("icon-arrow", {31, 238, 24, 24});
    EndScissorMode();
    DrawRectangleRoundedLinesEx(area, 0.025f, 6, 2, Color{172, 138, 78, 255});
}
