// main.cpp - entry point of the ByteMaze game.
// Opens the raylib window and runs the frame loop until the window is closed.

#include "GameUI.h"
#include "raylib.h"

#include <cstdlib>
#include <ctime>

// The screens of the game. currentScreen holds one of them.
const int SCREEN_MENU = 0;
const int SCREEN_HELP = 1;
const int SCREEN_SIZE = 2;
const int SCREEN_GAME = 3;
const int SCREEN_WIN = 4;
const int SCREEN_TESTS = 5;

int main()
{
    srand((unsigned int)time(nullptr));

    // Show only warnings and errors from raylib in the console.
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ByteMaze");
    SetTargetFPS(60);

    int currentScreen = SCREEN_MENU;

    // One pass of this loop is one frame: 60 frames per second.
    while (!WindowShouldClose())
    {
        BeginDrawing();

        // Draw the screen that is active in this frame.
        switch (currentScreen)
        {
        case SCREEN_MENU:
            drawMenuScreen();
            break;
        case SCREEN_HELP:
            drawHelpScreen();
            break;
        case SCREEN_SIZE:
            drawSizeScreen(0, false);
            break;
        case SCREEN_WIN:
            drawWinScreen(0);
            break;
        case SCREEN_GAME:
        case SCREEN_TESTS:
        default:
            // The game and the test results need data from later commits.
            drawMenuScreen();
            break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}