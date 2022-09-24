#include "raylib.h"

#define WINDOW_TITLE "ERMAHGAWD ITS A GAME"
const int screenWidth = 800;
const int screenHeight = 450;

int main(void)
{
    InitWindow(screenWidth, screenHeight, WINDOW_TITLE);
    SetTargetFPS(60);
    Vector2 pos = {screenWidth / 2, screenHeight / 2};

    while (!WindowShouldClose())
    {
        //Update
        //------------------------------------------------------------------------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) pos.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) pos.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) pos.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) pos.y += 2.0f;

        //------------------------------------------------------------------------------------------------------------------

        //Draw
        //------------------------------------------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawCircleV(pos, 10, SKYBLUE);

        EndDrawing();
        //------------------------------------------------------------------------------------------------------------------
    }

    CloseWindow();

    return 0;
}
