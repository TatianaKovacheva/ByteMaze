// GameUI.cpp - raylib drawing and keyboard input.
// Every function either draws one part of a screen or checks the keyboard.
// No function waits for a key: raylib calls them again in every frame.
// Functions that are still empty get their code in later commits.
// Until then their parameter names stay in /* */ comments.

#include "GameUI.h"
#include "Maze.h"
#include "raylib.h"

// Text sizes in pixels.
const int TITLE_SIZE = 60;
const int TEXT_SIZE = 30;
const int SMALL_TEXT_SIZE = 20;

// The maze is drawn in a square in the top-left part of the window.
const int MAZE_LEFT = 20;
const int MAZE_TOP = 20;
const int MAZE_AREA_SIZE = 650;

// The information panel on the right of the maze.
const int PANEL_LEFT = 690;

void drawMenuScreen()
{
    ClearBackground(RAYWHITE);

    DrawText("ByteMaze", 310, 120, TITLE_SIZE, DARKBLUE);
    DrawText("1 - New game", 330, 260, TEXT_SIZE, DARKGRAY);
    DrawText("2 - Help", 330, 310, TEXT_SIZE, DARKGRAY);
    DrawText("3 - Self-tests", 330, 360, TEXT_SIZE, DARKGRAY);
    DrawText("0 - Exit", 330, 410, TEXT_SIZE, DARKGRAY);
    DrawText("Press a number key.", 330, 500, SMALL_TEXT_SIZE, GRAY);
}

int readMenuChoice()
{
    if (IsKeyPressed(KEY_ONE))
    {
        return MENU_NEW_GAME;
    }
    if (IsKeyPressed(KEY_TWO))
    {
        return MENU_HELP;
    }
    if (IsKeyPressed(KEY_THREE))
    {
        return MENU_SELF_TEST;
    }
    if (IsKeyPressed(KEY_ZERO))
    {
        return MENU_EXIT;
    }

    // No menu key was pressed in this frame.
    return MENU_NONE;
}

void drawHelpScreen()
{
    ClearBackground(RAYWHITE);

    DrawText("How to play", 100, 60, TEXT_SIZE + 10, DARKBLUE);
    DrawText("Start in the top-left corner and find the exit E", 100, 140, SMALL_TEXT_SIZE, DARKGRAY);
    DrawText("in the bottom-right corner.", 100, 170, SMALL_TEXT_SIZE, DARKGRAY);
    DrawText("You cannot walk through walls.", 100, 200, SMALL_TEXT_SIZE, DARKGRAY);

    DrawText("Map symbols", 100, 260, TEXT_SIZE, DARKBLUE);
    DrawText("#  wall", 100, 310, SMALL_TEXT_SIZE, DARKGRAY);
    DrawText(".  open cell", 100, 340, SMALL_TEXT_SIZE, DARKGRAY);
    DrawText("E  exit", 100, 370, SMALL_TEXT_SIZE, DARKGRAY);
    DrawText("P  player", 100, 400, SMALL_TEXT_SIZE, DARKGRAY);

    DrawText("Controls", 100, 460, TEXT_SIZE, DARKBLUE);
    DrawText("Arrow keys or W, A, S, D - move", 100, 510, SMALL_TEXT_SIZE, DARKGRAY);
    DrawText("Q - back to the menu", 100, 540, SMALL_TEXT_SIZE, DARKGRAY);
}

bool readBackKey()
{
    return IsKeyPressed(KEY_Q);
}

bool readSizeInput(int& typedSize)
{
    // GetCharPressed returns the typed characters one by one, then 0.
    int key = GetCharPressed();
    while (key > 0)
    {
        // Only digits are accepted, and at most 3 of them.
        if (key >= '0' && key <= '9' && typedSize < 100)
        {
            typedSize = typedSize * 10 + (key - '0');
        }
        key = GetCharPressed();
    }

    // Backspace removes the last digit.
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        typedSize = typedSize / 10;
    }

    return IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER);
}

void drawSizeScreen(int typedSize, bool showError)
{
    ClearBackground(RAYWHITE);

    DrawText("Choose the maze size", 160, 120, TEXT_SIZE + 10, DARKBLUE);
    DrawText(TextFormat("Type a number from %d to %d and press Enter.", MIN_MAZE_SIZE, MAX_MAZE_SIZE),
        160, 200, SMALL_TEXT_SIZE, DARKGRAY);

    // Show the typed number, or a line when nothing is typed yet.
    if (typedSize > 0)
    {
        DrawText(TextFormat("%d", typedSize), 400, 280, TITLE_SIZE, BLACK);
    }
    else
    {
        DrawText("_", 400, 280, TITLE_SIZE, BLACK);
    }

    if (showError)
    {
        DrawText(TextFormat("The size must be from %d to %d.", MIN_MAZE_SIZE, MAX_MAZE_SIZE),
            160, 380, SMALL_TEXT_SIZE, RED);
    }

    DrawText("Backspace - delete     Q - back to the menu", 160, 600, SMALL_TEXT_SIZE, GRAY);
}

int readDirection()
{
    return COMMAND_QUIT;
}

// Width and height of one cell in pixels, so that the whole maze fits in MAZE_AREA_SIZE.
int getCellSize(int size)
{
    return MAZE_AREA_SIZE / size;
}

void drawMaze(char** maze, int size)
{
    ClearBackground(RAYWHITE);
    int cellSize = getCellSize(size);

    // From the top row to the bottom row, every cell is drawn as its symbol.
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            char symbol[2] = { maze[row][col], '\0' };

            Color color = DARKGRAY;
            if (maze[row][col] == OPEN_CELL)
            {
                color = GRAY;
            }
            else if (maze[row][col] == EXIT_CELL)
            {
                color = DARKGREEN;
            }

            DrawText(symbol, MAZE_LEFT + col * cellSize, MAZE_TOP + row * cellSize, cellSize, color);
        }
    }
}

void drawPlayer(int size, int playerRow, int playerCol)
{
    int cellSize = getCellSize(size);
    int x = MAZE_LEFT + playerCol * cellSize;
    int y = MAZE_TOP + playerRow * cellSize;

    // Cover the symbol of the cell, then draw P on top.
    DrawRectangle(x, y, cellSize, cellSize, RAYWHITE);
    DrawText("P", x, y, cellSize, RED);
}

void drawLegend()
{
    DrawText("Map", PANEL_LEFT, 20, TEXT_SIZE, DARKBLUE);
    DrawText("#  wall", PANEL_LEFT, 60, SMALL_TEXT_SIZE, DARKGRAY);
    DrawText(".  open cell", PANEL_LEFT, 85, SMALL_TEXT_SIZE, GRAY);
    DrawText("E  exit", PANEL_LEFT, 110, SMALL_TEXT_SIZE, DARKGREEN);
    DrawText("P  you", PANEL_LEFT, 135, SMALL_TEXT_SIZE, RED);

    DrawText("Keys", PANEL_LEFT, 185, TEXT_SIZE, DARKBLUE);
    DrawText("Arrows or", PANEL_LEFT, 225, SMALL_TEXT_SIZE, DARKGRAY);
    DrawText("W, A, S, D", PANEL_LEFT, 250, SMALL_TEXT_SIZE, DARKGRAY);
    DrawText("Q - menu", PANEL_LEFT, 275, SMALL_TEXT_SIZE, DARKGRAY);
}

void drawPlayerInfo(int playerRow, int playerCol, int moveCount)
{
    DrawText("Position", PANEL_LEFT, 330, TEXT_SIZE, DARKBLUE);
    DrawText(TextFormat("Row: %d", playerRow), PANEL_LEFT, 370, SMALL_TEXT_SIZE, DARKGRAY);
    DrawText(TextFormat("Column: %d", playerCol), PANEL_LEFT, 395, SMALL_TEXT_SIZE, DARKGRAY);
    DrawText(TextFormat("Moves: %d", moveCount), PANEL_LEFT, 420, SMALL_TEXT_SIZE, DARKGRAY);
}

void drawCellInfo(char** /*maze*/, int /*size*/, int /*row*/, int /*col*/)
{
}

void drawWinScreen(int /*moveCount*/)
{
}

void drawTestScreen(const bool /*results*/[])
{
}