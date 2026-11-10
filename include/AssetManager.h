#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>
class AssetManager
{
  public:
    AssetManager() = default;
    ~AssetManager();
    AssetManager(const AssetManager &) = delete;
    AssetManager &operator=(const AssetManager &) = delete;
    void Load();
    void Unload();
    void Sprite(const std::string &name, Vector2 foot, float scale = 1, Color tint = WHITE) const;
    void Text(const std::string &text, Vector2 position, float size, Color color,
              bool title = false, bool strong = false) const;
    float TextWidth(const std::string &text, float size, bool title = false,
                    bool strong = false) const;
    void CenteredText(const std::string &text, Rectangle box, float size, Color color,
                      bool title = false, bool strong = false) const;
    void Texture(const std::string &name, Rectangle destination, Color tint = WHITE) const;
    bool HasTexture(const std::string &name) const;
    int ImportedTextureCount() const
    {
        return importedCount_;
    }

  private:
    std::unordered_map<std::string, Texture2D> textures_;
    Font body_{}, title_{}, strong_{};
    bool ownsBody_ = false, ownsTitle_ = false, ownsStrong_ = false, loaded_ = false;
    int importedCount_ = 0;
    float bodyScale_ = 1, titleScale_ = 1, strongScale_ = 1;
};
