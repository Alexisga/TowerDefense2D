#pragma once
#include "GameTypes.h"
class AssetManager;
class GameUI {
public:
    void Draw(const AssetManager& assets,const UiSnapshot& ui) const;
    static Rectangle BasicButton(){return {945,198,287,171};}
    static Rectangle LongButton(){return {945,382,287,171};}
    static Rectangle PauseButton(){return {1060,35,142,42};}
    static Rectangle ActionButton(){return {470,465,340,64};}
private:
    void Panel(Rectangle rect,Color fill,bool highlighted=false) const;
    void Button(const AssetManager& assets,Rectangle rect,const std::string& label,Color fill) const;
};
