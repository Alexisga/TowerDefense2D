
#include "Game.h"
#include <algorithm>

int main()
{
    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1280, 720, "Tower Defense 2D");

    SetExitKey(KEY_NULL); // Esc cancela la construcción; la X cierra la ventana.
    SetTargetFPS(60);

    {
        Game game;
        game.LoadAssets();
        while (!WindowShouldClose())
        {
            game.HandleInput();
            // Limitar saltos tras mover o detener la ventana.
            game.Update(std::min(GetFrameTime(), 0.1f));
            BeginDrawing();
            game.Draw();
            EndDrawing();
        }
    }

    // Cerrar la ventana
    CloseWindow();

    return 0;
}
