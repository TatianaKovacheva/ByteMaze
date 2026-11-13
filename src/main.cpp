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
const int SCREEN_DIFFICULTY = 6;

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
    int difficulty = DIFFICULTY_MEDIUM;
    int keyCount = 0;

    // Time and score. GetTime returns the seconds since InitWindow.
    double startTime = 0.0;
    int seconds = 0;
    int score = 0;

    // The best score for every size and difficulty while the program runs.
    // {} sets every element to 0.
    int bestScores[MAX_MAZE_SIZE + 1][DIFFICULTY_HARD + 1] = {};

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
                // Enter was pressed: a valid size opens the difficulty screen.
                if (isValidSize(typedSize))
                {
                    currentScreen = SCREEN_DIFFICULTY;
                }
                else
                {
                    showSizeError = true;
                }
            }
            break;
        case SCREEN_DIFFICULTY:
        {
            int choice = readDifficultyChoice();
            if (readBackKey())
            {
                currentScreen = SCREEN_MENU;
            }
            else if (choice != DIFFICULTY_NONE)
            {
                // Create a new maze with the chosen size and difficulty.
                difficulty = choice;
                destroyMaze(maze, mazeSize);
                mazeSize = typedSize;
                maze = createMaze(mazeSize);
                generateMaze(maze, mazeSize, difficulty);

                // Every game starts in the top-left corner with 0 moves and no keys.
                playerRow = 0;
                playerCol = 0;
                moveCount = 0;
                keyCount = 0;
                lastTargetRow = 0;
                lastTargetCol = 0;
                startTime = GetTime();
                currentScreen = SCREEN_GAME;
            }
            break;
        }
        case SCREEN_GAME:
        {
            int direction = readDirection();
            seconds = (int)(GetTime() - startTime);
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

                if (movePlayerWithKeys(maze, mazeSize, playerRow, playerCol, direction, keyCount))
                {
                    moveCount++;
                }

                if (isAtExit(mazeSize, playerRow, playerCol))
                {
                    score = calculateScore(moveCount, seconds, difficulty);
                    if (score > bestScores[mazeSize][difficulty])
                    {
                        bestScores[mazeSize][difficulty] = score;
                    }

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
        case SCREEN_DIFFICULTY:
            drawDifficultyScreen();
            break;
        case SCREEN_GAME:
            drawMaze(maze, mazeSize, playerRow, playerCol, getFogRadius(difficulty), difficulty);
            drawPlayer(mazeSize, playerRow, playerCol, difficulty);
            drawLegend();
            drawPlayerInfo(playerRow, playerCol, moveCount);
            drawCellInfo(maze, mazeSize, lastTargetRow, lastTargetCol);
            drawInventory(keyCount);
            drawTimer(seconds);
            break;
        case SCREEN_WIN:
            drawWinScreen(moveCount, seconds, score, bestScores[mazeSize][difficulty]);
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