
#include "raylib.h"

int main()
{
    // Crear la ventana del videojuego
    InitWindow(1280, 720, "Tower Defense 2D");

    // Establecer un objetivo de 60 FPS
    SetTargetFPS(60);

    // Bucle principal del videojuego
    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(DARKGREEN);
        DrawText("TOWER DEFENSE 2D", 420, 280, 40, GOLD);
        DrawText("Raylib funcionando correctamente", 400, 350, 24, RAYWHITE);

        EndDrawing();
    }

    // Cerrar la ventana
    CloseWindow();

    return 0;
}
