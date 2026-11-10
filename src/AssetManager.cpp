#include "AssetManager.h"
#include <algorithm>
#include <functional>
#include <limits>
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
    strong_ = body_;
    int glyphs[111];
    for (int i = 0; i < 95; ++i)
        glyphs[i] = 32 + i;
    const int spanish[] = {161, 183, 191, 193, 201, 205, 209, 211,
                           218, 220, 225, 233, 237, 241, 243, 250};
    for (int i = 0; i < 16; ++i)
        glyphs[95 + i] = spanish[i];
    if (FileExists("C:/Windows/Fonts/segoeui.ttf"))
    {
        body_ = LoadFontEx("C:/Windows/Fonts/segoeui.ttf", 40, glyphs, 111);
        ownsBody_ = body_.texture.id != GetFontDefault().texture.id;
    }
    if (FileExists("C:/Windows/Fonts/georgia.ttf"))
    {
        title_ = LoadFontEx("C:/Windows/Fonts/georgia.ttf", 64, glyphs, 111);
        ownsTitle_ = title_.texture.id != GetFontDefault().texture.id;
    }
    // Fuentes del diseño original, distribuidas con su licencia OFL.
    auto fontFromAsset = [&](const char *path, Font &font, bool &owned, int pixels) {
        if (!FileExists(path))
            return false;
        Font replacement = LoadFontEx(path, pixels, glyphs, 111);
        if (replacement.texture.id == 0 || replacement.texture.id == GetFontDefault().texture.id)
            return false;
        if (owned)
            UnloadFont(font);
        font = replacement;
        owned = true;
        return true;
    };
    // Raylib mide ascendente+descendente; Figma usa el em de la fuente.
    // Factores hhea / unitsPerEm medidos en los TTF incluidos.
    bodyScale_ =
        fontFromAsset("assets/fonts/Inter-Regular.ttf", body_, ownsBody_, 40) ? 1.2099609375f : 1;
    titleScale_ =
        fontFromAsset("assets/fonts/Cinzel-Bold.ttf", title_, ownsTitle_, 64) ? 1.348f : 1;
    strongScale_ = fontFromAsset("assets/fonts/Inter-SemiBold.ttf", strong_, ownsStrong_, 40)
                       ? 1.2099609375f
                       : bodyScale_;
    if (!ownsStrong_)
        strong_ = body_;
    SetTextureFilter(body_.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(title_.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(strong_.texture, TEXTURE_FILTER_BILINEAR);
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
    // Conservar el dibujo anterior como reserva y sustituirlo solo tras cargar un PNG válido.
    const char *resources[] = {
        "basic",       "long",        "castle",       "enemy",      "tree",         "pine",
        "rock",        "icon-shield", "icon-coin",    "icon-wave",  "icon-tower",   "icon-pause",
        "icon-info",   "icon-valid",  "icon-invalid", "icon-arrow", "icon-warning", "cursor-build",
        "trim-button", "trim-hud",    "trim-sidebar", "ground",     "foliage"};
    importedCount_ = 0;
    for (const char *name : resources)
    {
        const std::string path = std::string("assets/figma/") + name + ".png";
        if (!FileExists(path.c_str()))
        {
            TraceLog(LOG_WARNING, "ASSET: Falta %s", path.c_str());
            continue;
        }
        Texture2D imported = LoadTexture(path.c_str());
        if (imported.id == 0)
            continue;
        auto previous = textures_.find(name);
        if (previous != textures_.end())
            UnloadTexture(previous->second);
        textures_[name] = imported;
        SetTextureFilter(imported, TEXTURE_FILTER_BILINEAR);
        ++importedCount_;
    }
    TraceLog(LOG_INFO, "ASSET: %d/23 texturas originales de Figma cargadas", importedCount_);
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
    if (ownsStrong_)
        UnloadFont(strong_);
    loaded_ = false;
}
void AssetManager::Sprite(const std::string &name, Vector2 p, float s, Color tint) const
{
    auto it = textures_.find(name);
    if (it == textures_.end())
        return;
    // Proporción original y pie estable: el dibujo nunca desplaza la posición lógica.
    float width = 76.8f * s, footOffset = 10 * s;
    if (name == "enemy")
    {
        width = 34 * s;
        footOffset = 13 * s;
    }
    if (name == "tree")
    {
        width = 104 * s;
        footOffset = 0;
    }
    if (name == "pine")
    {
        width = 84 * s;
        footOffset = 0;
    }
    if (name == "rock")
    {
        width = 66 * s;
        footOffset = 0;
    }
    if (name == "castle")
    {
        width = 151.2f * s;
        footOffset = 18 * s;
    }
    const float height = width * it->second.height / it->second.width;
    Texture(name, {p.x - width / 2, p.y + footOffset - height, width, height}, tint);
}
void AssetManager::Text(const std::string &s, Vector2 p, float size, Color c, bool title,
                        bool strong) const
{
    const float scale = title ? titleScale_ : (strong ? strongScale_ : bodyScale_);
    DrawTextEx(title ? title_ : (strong ? strong_ : body_), s.c_str(), p, size * scale, 0, c);
}
float AssetManager::TextWidth(const std::string &s, float size, bool title, bool strong) const
{
    const float scale = title ? titleScale_ : (strong ? strongScale_ : bodyScale_);
    return MeasureTextEx(title ? title_ : (strong ? strong_ : body_), s.c_str(), size * scale, 0).x;
}
void AssetManager::CenteredText(const std::string &s, Rectangle box, float size, Color color,
                                bool title, bool strong) const
{
    const Font font = title ? title_ : (strong ? strong_ : body_);
    const float renderScale =
        (title ? titleScale_ : (strong ? strongScale_ : bodyScale_)) * size / font.baseSize;
    float top = std::numeric_limits<float>::max(), bottom = -top;
    for (const char *p = s.c_str(); *p;)
    {
        int count = 0;
        const int codepoint = GetCodepointNext(p, &count);
        p += count;
        if (codepoint == 32)
            continue;
        const int index = GetGlyphIndex(font, codepoint);
        top = std::min(top, static_cast<float>(font.glyphs[index].offsetY));
        bottom = std::max(bottom, font.glyphs[index].offsetY + font.recs[index].height);
    }
    const float y =
        top <= bottom ? box.y + (box.height - (bottom - top) * renderScale) / 2 - top * renderScale
                      : box.y;
    Text(s, {box.x + (box.width - TextWidth(s, size, title, strong)) / 2, y}, size, color, title,
         strong);
}
void AssetManager::Texture(const std::string &name, Rectangle r, Color tint) const
{
    auto it = textures_.find(name);
    if (it == textures_.end())
        return;
    const Texture2D &t = it->second;
    DrawTexturePro(t, {0, 0, static_cast<float>(t.width), static_cast<float>(t.height)}, r, {0, 0},
                   0, tint);
}
bool AssetManager::HasTexture(const std::string &name) const
{
    return textures_.find(name) != textures_.end();
}
