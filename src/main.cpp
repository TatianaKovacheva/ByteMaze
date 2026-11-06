#include "GameUI.h"
#include "raylib.h"

#include <cstdlib>
#include <ctime>

int main()
{
    // A different random maze every time the program runs.
    srand((unsigned int)time(nullptr));

    // Show only warnings and errors from raylib in the console.
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ByteMaze");
    SetTargetFPS(60);

    // One pass of this loop is one frame: 60 frames per second.
    while (!WindowShouldClose())
    {
        BeginDrawing();
        drawMenuScreen();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}