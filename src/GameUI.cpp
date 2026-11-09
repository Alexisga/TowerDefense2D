#include "GameUI.h"
#include "AssetManager.h"
#include <string>
namespace
{
const Color gold{205, 162, 85, 255}, light{244, 225, 184, 255}, muted{187, 175, 145, 255},
    panel{38, 32, 24, 255}, green{56, 91, 48, 255};
}
void GameUI::Panel(Rectangle r, Color fill, bool hi) const
{
    DrawRectangleRounded(r, 0.03f, 6, fill);
    DrawRectangleRoundedLinesEx(r, 0.03f, 6, hi ? 2.0f : 1.0f, hi ? light : gold);
    for (int dx : {0, 1})
        for (int dy : {0, 1})
        {
            float x = r.x + (dx ? r.width - 5 : 5), y = r.y + (dy ? r.height - 5 : 5);
            DrawLineEx({x, y}, {x + (dx ? -9 : 9), y}, 2, gold);
            DrawLineEx({x, y}, {x, y + (dy ? -9 : 9)}, 2, gold);
        }
}
void GameUI::Button(const AssetManager &a, Rectangle r, const std::string &text, Color fill) const
{
    bool hover = CheckCollisionPointRec(GetMousePosition(), r);
    Panel(r,
          hover ? Color{static_cast<unsigned char>(fill.r + 12),
                        static_cast<unsigned char>(fill.g + 12),
                        static_cast<unsigned char>(fill.b + 8), 255}
                : fill,
          hover);
    a.Text(text, {r.x + (r.width - a.TextWidth(text, 22, true)) / 2, r.y + (r.height - 24) / 2}, 22,
           light, true);
}
void GameUI::Draw(const AssetManager &a, const UiSnapshot &u) const
{
    DrawRectangleLinesEx({7, 7, 1266, 706}, 1, gold);
    if (u.state == GameState::MENU)
    {
        DrawRectangle(8, 8, 1264, 704, Color{49, 80, 48, 255});
        for (int i = 0; i < 90; ++i)
        {
            float x = static_cast<float>(15 + (i * 173) % 1250),
                  y = static_cast<float>(30 + (i * 91) % 680);
            DrawEllipse(static_cast<int>(x), static_cast<int>(y), 36, 19, Color{69, 97, 51, 90});
        }
        const Vector2 road[] = {{8, 238}, {250, 238}, {250, 448}, {1040, 448}, {1040, 315}};
        for (int i = 1; i < 5; ++i)
        {
            DrawLineEx(road[i - 1], road[i], 80, Color{165, 140, 82, 255});
            DrawCircleV(road[i], 40, Color{165, 140, 82, 255});
        }
        for (int i = 0; i < 22; ++i)
        {
            float x = static_cast<float>(i * 62);
            a.Sprite("tree", {x, 105 + static_cast<float>((i * 19) % 65)}, 1.25f);
            a.Sprite("tree", {x, 735 + static_cast<float>((i * 13) % 40)}, 1.5f);
        }
        for (int i = 0; i < 6; ++i)
        {
            a.Sprite("tree", {52, 250 + static_cast<float>(i * 70)}, 1.2f);
            a.Sprite("tree", {1230, 210 + static_cast<float>(i * 80)}, 1.2f);
        }
        a.Sprite("castle", {1080, 355}, 1.8f);
        DrawRectangle(0, 0, 1280, 720, Color{9, 20, 13, 120});
        Panel({362, 154, 556, 418}, panel, true);
        DrawCircle(640, 186, 45, gold);
        DrawCircle(640, 186, 39, Color{66, 57, 36, 255});
        a.Text("TD", {609, 165}, 39, light, true);
        a.Text("TOWER DEFENSE", {405, 258}, 45, light, true);
        a.Text("Defiende la fortaleza", {491, 328}, 27, muted);
        a.Text("Dos torres. Una fortaleza.", {521, 386}, 19, gold);
        Button(a, ActionButton(), "JUGAR", green);
        a.Text("C++ / Raylib  |  Proyecto APF2", {491, 610}, 16, muted);
        return;
    }
    Panel({24, 21, 1232, 72}, Color{49, 39, 28, 255});
    const std::string selected =
        u.hasSelection ? (u.selection == TowerType::Basic ? "Básica" : "Largo alcance") : "Ninguna";
    const char *labels[] = {"BASE", "MONEDAS", "OLEADA", "TORRE"};
    std::string values[] = {
        std::to_string(u.life) + " / " + std::to_string(u.maxLife), std::to_string(u.coins),
        std::to_string(u.wave) + " / " + std::to_string(u.totalWaves), selected};
    int xs[] = {80, 328, 576, 824};
    for (int i = 0; i < 4; ++i)
    {
        a.Text(labels[i], {static_cast<float>(xs[i]), 35}, 12, muted);
        a.Text(values[i], {static_cast<float>(xs[i]), 51}, 24,
               i == 0 && u.life < 6 ? Color{221, 97, 76, 255} : light);
    }
    DrawPoly({56, 56}, 6, 9, 30, gold);
    DrawCircle(305, 56, 9, gold);
    a.Text("$", {300, 46}, 17, panel);
    a.Text("~", {542, 42}, 30, gold);
    a.Text("T", {792, 43}, 24, gold, true);
    Button(a, PauseButton(), "II  PAUSA", panel);
    Panel({930, 112, 326, 536}, panel);
    a.Text("DEFENSA DEL REINO", {945, 130}, 12, gold);
    a.Text("CONSTRUIR TORRE", {945, 150}, 22, light, true);
    for (auto type : {TowerType::Basic, TowerType::LongRange})
    {
        bool basic = type == TowerType::Basic;
        auto r = basic ? BasicButton() : LongButton();
        auto stats = Stats(type);
        bool active = u.hasSelection && u.selection == type;
        Panel(r, active ? Color{51, 48, 30, 255} : Color{33, 29, 23, 255}, active);
        a.Sprite(basic ? "basic" : "long", {984, r.y + 72}, 0.75f);
        a.Text(basic ? "TORRE BÁSICA" : "TORRE DE LARGO", {1038, r.y + 15}, 16, light, true);
        if (!basic)
            a.Text("ALCANCE", {1038, r.y + 36}, 16, light, true);
        a.Text(std::to_string(stats.cost) + " monedas", {1038, r.y + (basic ? 44 : 59)}, 16, gold);
        a.Text("DAÑO       ALCANCE       INTERVALO", {958, r.y + 96}, 11, muted);
        a.Text(std::to_string(stats.damage), {958, r.y + 114}, 16, light);
        a.Text(std::to_string(static_cast<int>(stats.range)) + " px", {1046, r.y + 114}, 16, light);
        a.Text(basic ? "0,8 s" : "1,4 s", {1138, r.y + 114}, 16, light);
        a.Text(active ? "Seleccionada · Elige una casilla" : "Clic para seleccionar",
               {958, r.y + 145}, 12, muted);
    }
    a.Text("Dos torres. Una fortaleza.", {945, 569}, 16, gold);
    a.Text("Selecciona un tipo y colócalo en", {945, 594}, 14, muted);
    a.Text("una casilla libre fuera del camino.", {945, 613}, 14, muted);
    Panel({24, 667, 1232, 35}, Color{35, 30, 23, 255});
    const Color status = u.error ? Color{238, 109, 82, 255} : light;
    a.Text(u.error ? "!" : "*", {36, 673}, 18, status);
    a.Text(u.message, {62, 674}, 15, status);
    a.Text("1 / 2: torre   Esc: cancelar   P: pausa", {944, 652}, 11, muted);
    if (u.state == GameState::ACTIVA)
    {
        std::string s =
            u.nextWave > 0
                ? "Próxima oleada: " + std::to_string(static_cast<int>(u.nextWave) + 1) + " s"
                : "Defiende la fortaleza";
        a.Text(s, {42, 122}, 13, light);
        return;
    }
    DrawRectangle(0, 0, 1280, 720, Color{6, 12, 8, 155});
    Panel({416, 185, 448, 381}, panel, true);
    bool paused = u.state == GameState::PAUSADA, won = u.state == GameState::VICTORIA;
    Color accent = paused ? gold : (won ? Color{217, 181, 86, 255} : Color{211, 86, 66, 255});
    DrawCircleLines(640, 245, 34, accent);
    a.Text(paused ? "II" : (won ? "V" : "X"), {paused ? 626.0f : 625.0f, 223}, 40, accent, true);
    std::string heading = paused ? "PAUSA" : (won ? "¡VICTORIA!" : "DERROTA");
    a.Text(heading, {640 - a.TextWidth(heading, 42, true) / 2, 299}, 42, accent, true);
    DrawLine(467, 359, 813, 359, gold);
    std::string desc = paused ? "La partida está detenida"
                              : (won ? "¡Has defendido la fortaleza!" : "La fortaleza ha caído");
    a.Text(desc, {640 - a.TextWidth(desc, 23) / 2, 387}, 23, light);
    std::string detail =
        paused ? "Reanuda para continuar"
               : (won ? "Todas las oleadas completadas" : "La vida de la base llegó a cero");
    a.Text(detail, {640 - a.TextWidth(detail, 17) / 2, 425}, 17, muted);
    Button(a, ActionButton(), paused ? "REANUDAR" : "REINICIAR PARTIDA",
           won || paused ? green : Color{112, 49, 36, 255});
}
