
#include "Game.h"
#include <algorithm>

int main()
{
    // Inicio reproducible también al abrir el EXE desde el Explorador.
    auto directory = std::filesystem::path(GetApplicationDirectory());
    const auto projectRoot = directory / ".." / ".." / "..";
    if (std::filesystem::exists(projectRoot / "TowerDefense2D.slnx"))
    {
        directory = std::filesystem::weakly_canonical(projectRoot);
    }
    std::filesystem::current_path(directory);
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
            if (IsKeyPressed(KEY_F12))
            {
                std::filesystem::create_directories("data");
                TakeScreenshot("data/captura.png");
            }
        }
    }

    // Cerrar la ventana
    CloseWindow();

    return 0;
}
