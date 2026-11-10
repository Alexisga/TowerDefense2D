
#include "Game.h"
#include <algorithm>

int main()
{
    // El paquete usa assets junto al EXE. Visual Studio puede generar el EXE
    // en x64/Debug o en TowerDefense2D/x64/Debug: no asumir una profundidad fija.
    auto directory = std::filesystem::path(GetApplicationDirectory());
    for (auto candidate = directory; !candidate.empty();)
    {
        const bool besideExecutable = candidate == directory;
        const bool projectRoot = std::filesystem::exists(candidate / "TowerDefense2D.slnx");
        if ((besideExecutable || projectRoot) &&
            std::filesystem::is_directory(candidate / "assets"))
        {
            directory = std::filesystem::weakly_canonical(candidate);
            break;
        }
        const auto parent = candidate.parent_path();
        if (parent == candidate)
            break;
        candidate = parent;
    }
    std::filesystem::current_path(directory);
    TraceLog(LOG_INFO, "PATH: Recursos y datos: %s", directory.string().c_str());
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
