// main.cpp - entry point of the ByteMaze game.
// Opens the raylib window, switches between the screens and runs the game.

#include "GameUI.h"
#include "Maze.h"
#include "Tests.h"
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
    loadGameFont();
    SetTargetFPS(60);

    int currentScreen = SCREEN_MENU;
    bool exitRequested = false;

    // Data of the size screen.
    int typedSize = 0;
    bool showSizeError = false;

    // Data of the game. maze stays nullptr until a game starts.
    char** maze = nullptr;
    int mazeSize = 0;
    int playerRow = 0;
    int playerCol = 0;
    int moveCount = 0;

    // The cell the player tried to enter last. drawCellInfo describes it.
    int lastTargetRow = 0;
    int lastTargetCol = 0;

    // Results of the self-tests. {} sets every element to false.
    bool testResults[TEST_COUNT] = {};

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
            else if (choice == MENU_SELF_TEST)
            {
                // Run every test once, then show the results.
                for (int i = 0; i < TEST_COUNT; i++)
                {
                    testResults[i] = runTest(i);
                }
                currentScreen = SCREEN_TESTS;
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

                    // Every game starts in the top-left corner with 0 moves.
                    playerRow = 0;
                    playerCol = 0;
                    moveCount = 0;
                    lastTargetRow = 0;
                    lastTargetCol = 0;
                    currentScreen = SCREEN_GAME;
                }
                else
                {
                    showSizeError = true;
                }
            }
            break;
        case SCREEN_GAME:
        {
            int direction = readDirection();
            if (direction == COMMAND_QUIT)
            {
                // The game is left, so its memory is released.
                destroyMaze(maze, mazeSize);
                maze = nullptr;
                currentScreen = SCREEN_MENU;
            }
            else if (direction != DIRECTION_NONE)
            {
                // Remember which cell the player tried to enter, for drawCellInfo.
                getTargetPosition(playerRow, playerCol, direction, lastTargetRow, lastTargetCol);

                if (movePlayer(maze, mazeSize, playerRow, playerCol, direction))
                {
                    moveCount++;
                }

                if (isAtExit(mazeSize, playerRow, playerCol))
                {
                    destroyMaze(maze, mazeSize);
                    maze = nullptr;
                    currentScreen = SCREEN_WIN;
                }
            }
            break;
        }
        case SCREEN_WIN:
        case SCREEN_TESTS:
            if (readBackKey())
            {
                currentScreen = SCREEN_MENU;
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
            drawMaze(maze, mazeSize, playerRow, playerCol, NO_FOG);
            drawPlayer(mazeSize, playerRow, playerCol);
            drawLegend();
            drawPlayerInfo(playerRow, playerCol, moveCount);
            drawCellInfo(maze, mazeSize, lastTargetRow, lastTargetCol);
            drawInventory(0);
            break;
        case SCREEN_WIN:
            drawWinScreen(moveCount);
            break;
        case SCREEN_TESTS:
            drawTestScreen(testResults);
            break;
        default:
            break;
        }

        EndDrawing();
    }

    // Release the maze if a game was started.
    destroyMaze(maze, mazeSize);

    unloadGameFont();
    CloseWindow();
    return 0;
}