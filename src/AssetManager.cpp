#include "AssetManager.h"
#include <functional>
namespace
{
const Color ink{39, 43, 31, 255}, wood{152, 108, 57, 255}, gold{215, 175, 91, 255},
    stone{154, 169, 150, 255}, roof{58, 89, 101, 255};
void TowerArt(bool tall)
{
    DrawEllipse(64, 116, 29, 8, Color{18, 30, 22, 80});
    if (tall)
    {
        DrawRectangle(48, 51, 32, 58, ink);
        DrawRectangle(51, 53, 26, 52, stone);
        DrawRectangle(43, 106, 42, 8, stone);
        DrawTriangle({40, 55}, {88, 55}, {64, 22}, ink);
        DrawTriangle({43, 52}, {85, 52}, {64, 25}, roof);
        DrawLine(64, 25, 64, 9, gold);
        DrawTriangle({64, 9}, {64, 21}, {82, 13}, gold);
        DrawRectangle(60, 67, 8, 15, ink);
        DrawRectangle(61, 69, 5, 10, gold);
        DrawRectangle(58, 93, 12, 17, ink);
        DrawRectangle(60, 95, 8, 14, wood);
        DrawLine(52, 85, 76, 85, Color{120, 137, 122, 255});
    }
    else
    {
        DrawRectangle(38, 108, 52, 7, stone);
        DrawRectangle(44, 59, 8, 49, wood);
        DrawRectangle(76, 59, 8, 49, wood);
        DrawLineEx({47, 65}, {80, 108}, 5, stone);
        DrawLineEx({80, 65}, {47, 108}, 5, stone);
        DrawRectangle(35, 47, 58, 20, ink);
        DrawRectangle(38, 49, 52, 14, wood);
        for (int x = 37; x < 91; x += 18)
        {
            DrawRectangle(x, 37, 12, 15, ink);
            DrawRectangle(x + 2, 39, 8, 11, gold);
        }
        DrawRectangle(54, 48, 19, 12, Color{60, 54, 34, 255});
        DrawTriangle({60, 37}, {75, 37}, {68, 31}, Color{78, 119, 68, 255});
    }
}
} // namespace
AssetManager::~AssetManager()
{
    Unload();
}
void AssetManager::Load()
{
    if (loaded_)
        return;
    body_ = GetFontDefault();
    title_ = body_;
    int glyphs[224];
    for (int i = 0; i < 224; ++i)
        glyphs[i] = 32 + i;
    if (FileExists("C:/Windows/Fonts/segoeui.ttf"))
    {
        body_ = LoadFontEx("C:/Windows/Fonts/segoeui.ttf", 40, glyphs, 224);
        ownsBody_ = body_.texture.id != GetFontDefault().texture.id;
    }
    if (FileExists("C:/Windows/Fonts/georgia.ttf"))
    {
        title_ = LoadFontEx("C:/Windows/Fonts/georgia.ttf", 64, glyphs, 224);
        ownsTitle_ = title_.texture.id != GetFontDefault().texture.id;
    }
    SetTextureFilter(body_.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(title_.texture, TEXTURE_FILTER_BILINEAR);
    auto make = [this](const std::string &name, const std::function<void()> &draw) {
        RenderTexture2D rt = LoadRenderTexture(128, 128);
        BeginTextureMode(rt);
        ClearBackground(BLANK);
        draw();
        EndTextureMode();
        Image im = LoadImageFromTexture(rt.texture);
        ImageFlipVertical(&im);
        textures_[name] = LoadTextureFromImage(im);
        SetTextureFilter(textures_[name], TEXTURE_FILTER_BILINEAR);
        UnloadImage(im);
        UnloadRenderTexture(rt);
    };
    make("basic", [] { TowerArt(false); });
    make("long", [] { TowerArt(true); });
    make("tree", [] {
        DrawEllipse(64, 119, 38, 8, Color{15, 32, 22, 70});
        DrawRectangle(58, 67, 13, 49, wood);
        DrawTriangle({12, 96}, {116, 96}, {64, 20}, Color{28, 66, 46, 255});
        DrawTriangle({19, 86}, {109, 86}, {64, 13}, Color{44, 89, 55, 255});
        DrawTriangle({29, 61}, {99, 61}, {64, 3}, Color{54, 101, 59, 255});
        DrawTriangle({40, 49}, {75, 49}, {64, 13}, Color{86, 122, 66, 255});
    });
    make("enemy", [] {
        DrawEllipse(64, 117, 20, 6, Color{17, 27, 16, 85});
        DrawRectangle(50, 99, 10, 17, ink);
        DrawRectangle(69, 99, 10, 17, ink);
        DrawRectangle(48, 83, 33, 22, wood);
        DrawRectangle(52, 86, 26, 15, Color{96, 83, 43, 255});
        DrawTriangle({49, 64}, {32, 53}, {43, 80}, ink);
        DrawTriangle({80, 64}, {97, 53}, {86, 80}, ink);
        DrawTriangle({48, 65}, {36, 58}, {44, 76}, Color{112, 150, 74, 255});
        DrawTriangle({80, 65}, {93, 58}, {85, 76}, Color{112, 150, 74, 255});
        DrawEllipse(64, 73, 22, 20, ink);
        DrawEllipse(64, 72, 19, 16, Color{112, 150, 74, 255});
        DrawLineEx({50, 67}, {59, 70}, 3, ink);
        DrawLineEx({69, 70}, {78, 67}, 3, ink);
        DrawRectangle(53, 71, 5, 4, gold);
        DrawRectangle(71, 71, 5, 4, gold);
        DrawLine(58, 82, 72, 82, ink);
    });
    make("castle", [] {
        DrawEllipse(64, 116, 61, 9, Color{17, 30, 23, 85});
        DrawRectangle(30, 57, 69, 55, stone);
        DrawRectangle(31, 98, 67, 14, Color{126, 144, 128, 255});
        for (int x : {11, 91})
        {
            DrawRectangle(x, 48, 26, 66, ink);
            DrawRectangle(x + 3, 50, 20, 61, stone);
            DrawTriangle({static_cast<float>(x - 5), 53}, {static_cast<float>(x + 32), 53},
                         {static_cast<float>(x + 13), 22}, ink);
            DrawTriangle({static_cast<float>(x - 2), 50}, {static_cast<float>(x + 29), 50},
                         {static_cast<float>(x + 13), 25}, roof);
            DrawRectangle(x + 10, 65, 7, 17, ink);
            DrawRectangle(x + 11, 66, 5, 11, gold);
        }
        DrawRectangle(44, 36, 40, 48, stone);
        DrawTriangle({37, 39}, {91, 39}, {64, 4}, roof);
        DrawLine(64, 4, 64, -6, gold);
        DrawRectangle(58, 49, 12, 19, gold);
        DrawRectangle(53, 87, 24, 27, ink);
        DrawRectangle(56, 90, 18, 24, wood);
        DrawLine(64, 91, 64, 113, ink);
        DrawTriangle({44, 125}, {84, 125}, {64, 103}, Color{186, 170, 128, 255});
    });
    loaded_ = true;
}
void AssetManager::Unload()
{
    if (!loaded_)
        return;
    for (auto &[name, t] : textures_)
        UnloadTexture(t);
    textures_.clear();
    if (ownsBody_)
        UnloadFont(body_);
    if (ownsTitle_)
        UnloadFont(title_);
    loaded_ = false;
}
void AssetManager::Sprite(const std::string &name, Vector2 p, float s, Color tint) const
{
    auto it = textures_.find(name);
    if (it == textures_.end())
        return;
    // Cada sprite tiene su propio lienzo transparente y ancla en el suelo.
    float size = 80 * s;
    if (name == "enemy")
        size = 62 * s;
    if (name == "tree")
        size = 100 * s;
    if (name == "castle")
        size = 145 * s;
    DrawTexturePro(it->second, {0, 0, 128, 128}, {p.x - size / 2, p.y - size * 0.92f, size, size},
                   {0, 0}, 0, tint);
}
void AssetManager::Text(const std::string &s, Vector2 p, float size, Color c, bool title) const
{
    DrawTextEx(title ? title_ : body_, s.c_str(), p, size, 0, c);
}
float AssetManager::TextWidth(const std::string &s, float size, bool title) const
{
    return MeasureTextEx(title ? title_ : body_, s.c_str(), size, 0).x;
}
