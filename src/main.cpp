// main.cpp - entry point of the ByteMaze game. Owner: Tatiana Kovacheva.
// Opens the raylib window and runs the frame loop until the window is closed.

#include "GameUI.h"
#include "Maze.h"
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
    // A different random maze every time the program runs.
    srand((unsigned int)time(nullptr));

    // Show only warnings and errors from raylib in the console.
    SetTraceLogLevel(LOG_WARNING);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ByteMaze");
    SetTargetFPS(60);

    int currentScreen = SCREEN_MENU;
    bool exitRequested = false;

    // Data of the size screen.
    int typedSize = 0;
    bool showSizeError = false;

    // Data of the game. maze stays nullptr until a game starts.
    char** maze = nullptr;
    int mazeSize = 0;

    // One pass of this loop is one frame: 60 frames per second.
    while (!exitRequested && !WindowShouldClose())
    {
        // 1. Read the keys and change the data of the active screen.
        switch (currentScreen)
        {
        case SCREEN_MENU:
        {
            int choice = readMenuChoice();
            if (choice == MENU_NEW_GAME)
            {
                typedSize = 0;
                showSizeError = false;
                currentScreen = SCREEN_SIZE;
            }
            else if (choice == MENU_HELP)
            {
                currentScreen = SCREEN_HELP;
            }
            else if (choice == MENU_EXIT)
            {
                exitRequested = true;
            }
            break;
        }
        case SCREEN_HELP:
            if (readBackKey())
            {
                currentScreen = SCREEN_MENU;
            }
            break;
        case SCREEN_SIZE:
            if (readBackKey())
            {
                currentScreen = SCREEN_MENU;
            }
            else if (readSizeInput(typedSize))
            {
                // Enter was pressed: start a game only with a valid size.
                if (isValidSize(typedSize))
                {
                    destroyMaze(maze, mazeSize);
                    mazeSize = typedSize;
                    maze = createMaze(mazeSize);
                    generateMaze(maze, mazeSize);
                    currentScreen = SCREEN_GAME;
                }
                else
                {
                    showSizeError = true;
                }
            }
            break;
        default:
            break;
        }

        // 2. Draw the screen that is active in this frame.
        BeginDrawing();

        switch (currentScreen)
        {
        case SCREEN_MENU:
            drawMenuScreen();
            break;
        case SCREEN_HELP:
            drawHelpScreen();
            break;
        case SCREEN_SIZE:
            drawSizeScreen(typedSize, showSizeError);
            break;
        case SCREEN_GAME:
            drawMaze(maze, mazeSize);
            drawLegend();
            break;
        case SCREEN_WIN:
            drawWinScreen(0);
            break;
        case SCREEN_TESTS:
        default:
            // The test results need data from a later commit.
            drawMenuScreen();
            break;
        }

        EndDrawing();
    }

    // Release the maze if a game was started.
    destroyMaze(maze, mazeSize);

    CloseWindow();
    return 0;
}