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
              bool title = false) const;
    float TextWidth(const std::string &text, float size, bool title = false) const;

  private:
    std::unordered_map<std::string, Texture2D> textures_;
    Font body_{}, title_{};
    bool ownsBody_ = false, ownsTitle_ = false, loaded_ = false;
};
