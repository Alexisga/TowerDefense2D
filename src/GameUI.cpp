#include "GameUI.h"
#include "AssetManager.h"
#include <cmath>
#include <string>

namespace
{
const Color gold{197, 161, 100, 255}, goldLight{240, 206, 139, 255}, light{242, 223, 183, 255},
    muted{196, 182, 154, 255}, wood{51, 42, 32, 255}, deep{36, 31, 25, 255}, green{54, 87, 43, 255},
    red{209, 82, 66, 255};

// Emblemas de resultados aproximados; no se presentan como exportaciones de Figma.
void Shield(Vector2 c, Color accent, bool cracked)
{
    const Vector2 outer[] = {{c.x - 42, c.y - 34}, {c.x - 34, c.y + 22}, {c.x, c.y + 49},
                             {c.x + 34, c.y + 22}, {c.x + 42, c.y - 34}, {c.x, c.y - 49}};
    DrawTriangleFan(outer, 6, accent);
    const Vector2 inner[] = {{c.x - 34, c.y - 28}, {c.x - 27, c.y + 18}, {c.x, c.y + 38},
                             {c.x + 27, c.y + 18}, {c.x + 34, c.y - 28}, {c.x, c.y - 40}};
    DrawTriangleFan(inner, 6, cracked ? Color{82, 86, 76, 255} : wood);
    if (cracked)
    {
        DrawLineEx({c.x + 7, c.y - 36}, {c.x - 6, c.y - 10}, 5, deep);
        DrawLineEx({c.x - 6, c.y - 10}, {c.x + 8, c.y + 4}, 5, deep);
        DrawLineEx({c.x + 8, c.y + 4}, {c.x - 7, c.y + 33}, 5, deep);
    }
    else
    {
        DrawTriangle({c.x - 21, c.y + 10}, {c.x - 7, c.y - 2}, {c.x - 25, c.y - 13}, accent);
        DrawTriangle({c.x - 10, c.y + 8}, {c.x + 10, c.y + 8}, {c.x, c.y - 22}, accent);
        DrawTriangle({c.x + 7, c.y - 2}, {c.x + 21, c.y + 10}, {c.x + 25, c.y - 13}, accent);
        DrawRectangle(static_cast<int>(c.x) - 21, static_cast<int>(c.y) + 8, 42, 8, accent);
    }
}
void Trophy(Vector2 c)
{
    DrawEllipse(static_cast<int>(c.x) - 27, static_cast<int>(c.y) - 6, 15, 21, gold);
    DrawEllipse(static_cast<int>(c.x) + 27, static_cast<int>(c.y) - 6, 15, 21, gold);
    DrawEllipse(static_cast<int>(c.x) - 27, static_cast<int>(c.y) - 9, 10, 13, deep);
    DrawEllipse(static_cast<int>(c.x) + 27, static_cast<int>(c.y) - 9, 10, 13, deep);
    DrawRectangleRounded({c.x - 27, c.y - 31, 54, 55}, 0.7f, 12, gold);
    DrawRectangleRounded({c.x - 21, c.y - 27, 42, 43}, 0.7f, 12, goldLight);
    DrawRectangle(static_cast<int>(c.x) - 6, static_cast<int>(c.y) + 18, 12, 24, gold);
    DrawRectangleRounded({c.x - 26, c.y + 39, 52, 9}, 0.2f, 4, gold);
    DrawLineEx({c.x - 28, c.y - 31}, {c.x + 28, c.y - 31}, 4, light);
}
void Divider(float y)
{
    DrawLineEx({478, y}, {619, y}, 1, gold);
    DrawLineEx({661, y}, {802, y}, 1, gold);
    DrawPoly({640, y}, 4, 6, 0, goldLight);
}
} // namespace

void GameUI::Panel(Rectangle r, Color fill, bool highlighted) const
{
    DrawRectangleRounded({r.x + 4, r.y + 6, r.width, r.height}, 0.025f, 8, Color{8, 12, 8, 95});
    DrawRectangleRounded(r, 0.025f, 8, fill);
    DrawRectangleGradientV(static_cast<int>(r.x + 3), static_cast<int>(r.y + 3),
                           static_cast<int>(r.width - 6), static_cast<int>(r.height - 6),
                           Color{98, 74, 41, 30}, Color{12, 13, 9, 45});
    DrawRectangleRoundedLinesEx(r, 0.025f, 8, highlighted ? 2.0f : 1.0f,
                                highlighted ? goldLight : Color{128, 98, 61, 255});
    DrawRectangleLinesEx({r.x + 6, r.y + 6, r.width - 12, r.height - 12}, 1,
                         Color{127, 99, 58, 75});
    for (int dx : {0, 1})
        for (int dy : {0, 1})
        {
            const float x = r.x + (dx ? r.width - 4 : 4), y = r.y + (dy ? r.height - 4 : 4);
            DrawLineEx({x, y}, {x + (dx ? -12 : 12), y}, 2, gold);
            DrawLineEx({x, y}, {x, y + (dy ? -12 : 12)}, 2, gold);
        }
}

void GameUI::Button(const AssetManager &a, Rectangle r, const std::string &label, Color fill) const
{
    const bool hover = CheckCollisionPointRec(GetMousePosition(), r);
    DrawRectangleRounded({r.x + 3, r.y + 4, r.width, r.height}, 0.08f, 8, Color{8, 12, 8, 95});
    const Color background = hover ? Color{static_cast<unsigned char>(fill.r + 12),
                                           static_cast<unsigned char>(fill.g + 12),
                                           static_cast<unsigned char>(fill.b + 8), 255}
                                   : fill;
    DrawRectangleRounded(r, 0.08f, 8, background);
    DrawRectangleRoundedLinesEx(r, 0.08f, 8, 1, gold);
    a.Texture("trim-button", r);
    const bool small = r.height < 50;
    const float size = small ? 15.0f : (label == "JUGAR" ? 32.0f : 23.0f);
    if (small)
    {
        const float textWidth = a.TextWidth(label, size, false, true);
        const float groupWidth = 22 + 10 + textWidth;
        const float x = r.x + (r.width - groupWidth) / 2;
        a.Texture("icon-pause", {x, r.y + (r.height - 22) / 2, 22, 22});
        a.CenteredText(label, {x + 32, r.y, textWidth, r.height}, size, light, false, true);
    }
    else if (label == "JUGAR")
    {
        const float groupWidth = 24 + 18 + a.TextWidth(label, size, true);
        const float x = r.x + (r.width - groupWidth) / 2;
        DrawTriangle({x, r.y + r.height / 2 - 13}, {x, r.y + r.height / 2 + 13},
                     {x + 22, r.y + r.height / 2}, goldLight);
        a.CenteredText(label, {x + 42, r.y, groupWidth - 42, r.height}, size, light, true);
    }
    else
        a.CenteredText(label, r, size, light, true);
}

void GameUI::Draw(const AssetManager &a, const UiSnapshot &u) const
{
    DrawRectangleLinesEx({7, 7, 1266, 706}, 1, gold);
    DrawRectangleLinesEx({12, 12, 1256, 696}, 1, Color{85, 71, 43, 255});
    if (u.state == GameState::MENU)
    {
        a.Texture("ground", {8, 8, 1264, 704});
        const Vector2 road[] = {{8, 238}, {250, 238}, {250, 448}, {1040, 448}, {1040, 315}};
        for (int i = 1; i < 5; ++i)
        {
            DrawLineEx(road[i - 1], road[i], 88, Color{103, 117, 60, 255});
            DrawCircleV(road[i], 44, Color{103, 117, 60, 255});
        }
        for (int i = 1; i < 5; ++i)
        {
            DrawLineEx(road[i - 1], road[i], 70, Color{205, 174, 111, 255});
            DrawCircleV(road[i], 35, Color{205, 174, 111, 255});
        }
        for (int i = 0; i < 17; ++i)
        {
            const float x = static_cast<float>(i * 82);
            a.Sprite(i % 3 == 1 ? "pine" : "tree", {x, 104 + static_cast<float>((i * 19) % 55)},
                     1.2f);
            a.Sprite(i % 2 ? "pine" : "tree", {x + 24, 758 + static_cast<float>((i * 13) % 35)},
                     1.3f);
        }
        for (int i = 0; i < 5; ++i)
        {
            a.Sprite(i % 2 ? "pine" : "tree", {42, 250 + static_cast<float>(i * 94)}, 1.1f);
            a.Sprite(i % 2 ? "tree" : "pine", {1232, 232 + static_cast<float>(i * 98)}, 1.1f);
        }
        a.Sprite("rock", {252, 167}, 1.1f);
        a.Sprite("rock", {1120, 585}, 1.0f);
        a.Sprite("castle", {1072, 372}, 1.65f);
        DrawRectangle(8, 8, 1264, 704, Color{10, 21, 13, 48});
        DrawRectangleLinesEx({7, 7, 1266, 706}, 1, gold);
        DrawRectangleLinesEx({12, 12, 1256, 696}, 1, Color{85, 71, 43, 255});
        Panel({360, 176, 560, 402}, wood, true);
        Shield({640, 196}, goldLight, false);
        a.CenteredText("TOWER DEFENSE", {378, 277, 524, 65}, 48, light, true);
        Divider(357);
        a.CenteredText("Defiende la fortaleza", {380, 378, 520, 34}, 25, light);
        a.CenteredText("Dos torres. Una fortaleza.", {380, 420, 520, 25}, 17, goldLight);
        Button(a, ActionButton(), "JUGAR", green);
        a.CenteredText("C++ / Raylib  |  Proyecto APF2", {380, 610, 520, 26}, 14, light);
        return;
    }

    const Rectangle hud{24, 21, 1232, 72}, sidebar{928, 110, 328, 540};
    Panel(hud, wood);
    a.Texture("trim-hud", hud);
    const std::string selected =
        u.hasSelection ? (u.selection == TowerType::Basic ? "Básica" : "Largo alcance") : "Ninguna";
    const char *labels[] = {"BASE:", "MONEDAS:", "OLEADA:", "TORRE:"};
    const char *icons[] = {"icon-shield", "icon-coin", "icon-wave", "icon-tower"};
    const std::string values[] = {
        std::to_string(u.life) + " / " + std::to_string(u.maxLife), std::to_string(u.coins),
        std::to_string(u.wave) + " / " + std::to_string(u.totalWaves), selected};
    const float xs[] = {80, 328, 576, 824};
    for (int i = 0; i < 4; ++i)
    {
        a.Texture(icons[i], {xs[i] - 34, 45, 22, 22});
        a.Text(labels[i], {xs[i], 34}, 13, muted, false, true);
        a.Text(values[i], {xs[i], 51}, 25,
               i == 0 && u.life < 6 ? red : (i == 1 ? goldLight : light), false, true);
    }
    Button(a, PauseButton(), "PAUSA", wood);
    Panel(sidebar, wood);
    a.Texture("trim-sidebar", sidebar);
    a.Text("DEFENSA DEL REINO", {945, 129}, 13, gold);
    a.Text("CONSTRUIR TORRE", {945, 151}, 21, light, true);

    for (TowerType type : {TowerType::Basic, TowerType::LongRange})
    {
        const bool basic = type == TowerType::Basic;
        const Rectangle r = basic ? BasicButton() : LongButton();
        const auto stats = Stats(type);
        const bool chosen = u.hasSelection && u.selection == type;
        const bool hover = CheckCollisionPointRec(GetMousePosition(), r);
        DrawRectangleRounded({r.x + 2, r.y + 4, r.width, r.height}, 0.07f, 8, Color{9, 13, 8, 65});
        DrawRectangleRounded(r, 0.07f, 8, deep);
        DrawRectangleRoundedLinesEx(r, 0.07f, 8, chosen ? 2.0f : 1.0f,
                                    chosen ? goldLight : (hover ? gold : Color{128, 98, 61, 255}));
        a.Sprite(basic ? "basic" : "long", {984, r.y + 69}, 0.75f);
        a.Text(basic ? "Torre básica" : "Torre de largo", {1038, r.y + 15}, 17.5f, light, true);
        if (!basic)
            a.Text("alcance", {1038, r.y + 39}, 17.5f, light, true);
        const float costY = r.y + (basic ? 43 : 62);
        a.Texture("icon-coin", {1038, costY + 1, 16, 16});
        a.Text(std::to_string(stats.cost) + " monedas", {1060, costY}, 15, goldLight, false, true);
        const char *columns[] = {"DAÑO", "ALCANCE", "INTERVALO"};
        const float columnX[] = {958, 1046, 1138};
        const std::string numbers[] = {std::to_string(stats.damage),
                                       std::to_string(static_cast<int>(stats.range)) + " px",
                                       basic ? "0,8 s" : "1,4 s"};
        for (int i = 0; i < 3; ++i)
        {
            a.Text(columns[i], {columnX[i], r.y + 96}, 12, muted);
            a.Text(numbers[i], {columnX[i], r.y + 115}, 16, light, false, true);
        }
        a.Text(chosen ? "Seleccionada · Elige una casilla" : "Clic para seleccionar",
               {958, r.y + 146}, 13, chosen ? goldLight : muted);
    }
    a.Text("Dos torres. Una fortaleza.", {945, 569}, 15, goldLight, false, true);
    a.Text("Selecciona un tipo y colócalo en", {945, 595}, 14, muted);
    a.Text("una casilla libre fuera del camino.", {945, 616}, 14, muted);
    a.Text("1 / 2: torre   Esc: cancelar   P: pausa", {944, 652}, 11.5f, muted);

    Panel({24, 667, 1232, 35}, deep);
    if (u.error)
        DrawRectangleRoundedLinesEx({24, 667, 1232, 35}, 0.08f, 6, 1, red);
    a.Texture(u.error ? "icon-warning" : "icon-info", {35, 675, 18, 18});
    a.Text(u.message, {62, 675}, 14, u.error ? Color{247, 185, 153, 255} : light);
    if (u.state == GameState::ACTIVA)
    {
        if (u.nextWave > 0)
        {
            DrawRectangleRounded({35, 122, 202, 24}, 0.2f, 6, Color{25, 44, 28, 205});
            a.Text("Próxima oleada: " + std::to_string(static_cast<int>(std::ceil(u.nextWave))) +
                       " s",
                   {45, 126}, 13, light);
        }
        return;
    }

    DrawRectangle(0, 0, 1280, 720, Color{6, 12, 8, 145});
    Panel({416, 185, 448, 381}, wood, true);
    const bool paused = u.state == GameState::PAUSADA, won = u.state == GameState::VICTORIA;
    const Color accent = paused || won ? goldLight : red;
    if (paused)
    {
        DrawCircleLines(640, 244, 34, accent);
        a.Texture("icon-pause", {617, 221, 46, 46});
    }
    else if (won)
        Trophy({640, 238});
    else
        Shield({640, 241}, red, true);
    const std::string title = paused ? "PAUSA" : (won ? "¡VICTORIA!" : "DERROTA");
    a.CenteredText(title, {432, 295, 416, 58}, 43, accent, true);
    Divider(364);
    const std::string desc = paused
                                 ? "La partida está detenida"
                                 : (won ? "¡Has defendido la fortaleza!" : "La fortaleza ha caído");
    a.CenteredText(desc, {432, 387, 416, 30}, 22, light);
    const std::string detail =
        paused ? "Reanuda para continuar"
               : (won ? "Todas las oleadas completadas" : "La vida de la base llegó a cero");
    a.CenteredText(detail, {432, 425, 416, 24}, 16, muted);
    Button(a, ActionButton(), paused ? "REANUDAR" : "REINICIAR PARTIDA",
           won || paused ? green : Color{112, 49, 36, 255});
}
