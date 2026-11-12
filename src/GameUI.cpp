// GameUI.cpp - raylib drawing and keyboard input.
// Every function either draws one part of a screen or checks the keyboard.
// No function waits for a key: raylib calls them again in every frame.

#include "GameUI.h"
#include "Maze.h"
#include "Tests.h"
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

// The font of the game. loadGameFont loads it once after the window is opened.
Font gameFont;

void loadGameFont()
{
    // Kenney Pixel, CC0 (public domain). If the file is missing, raylib uses its default font.
    gameFont = LoadFontEx("assets/fonts/kenney_pixel.ttf", 64, nullptr, 0);
}

void unloadGameFont()
{
    UnloadFont(gameFont);
}

// Draws text with the game font. It is used instead of DrawText in the whole game.
void drawGameText(const char* text, int x, int y, int size, Color color)
{
    DrawTextEx(gameFont, text, { (float)x, (float)y }, (float)size, 1.0f, color);
}

void drawMenuScreen()
{
    ClearBackground(RAYWHITE);

    drawGameText("ByteMaze", 310, 120, TITLE_SIZE, DARKBLUE);
    drawGameText("1 - New game", 330, 260, TEXT_SIZE, DARKGRAY);
    drawGameText("2 - Help", 330, 310, TEXT_SIZE, DARKGRAY);
    drawGameText("3 - Self-tests", 330, 360, TEXT_SIZE, DARKGRAY);
    drawGameText("0 - Exit", 330, 410, TEXT_SIZE, DARKGRAY);
    drawGameText("Press a number key.", 330, 500, SMALL_TEXT_SIZE, GRAY);
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

    drawGameText("How to play", 100, 60, TEXT_SIZE + 10, DARKBLUE);
    drawGameText("Start in the top-left corner and find the exit E", 100, 140, SMALL_TEXT_SIZE, DARKGRAY);
    drawGameText("in the bottom-right corner.", 100, 170, SMALL_TEXT_SIZE, DARKGRAY);
    drawGameText("You cannot walk through walls.", 100, 200, SMALL_TEXT_SIZE, DARKGRAY);

    drawGameText("Map symbols", 100, 260, TEXT_SIZE, DARKBLUE);
    drawGameText("#  wall", 100, 310, SMALL_TEXT_SIZE, DARKGRAY);
    drawGameText(".  open cell", 100, 340, SMALL_TEXT_SIZE, DARKGRAY);
    drawGameText("E  exit", 100, 370, SMALL_TEXT_SIZE, DARKGRAY);
    drawGameText("P  player", 100, 400, SMALL_TEXT_SIZE, DARKGRAY);

    drawGameText("Controls", 100, 460, TEXT_SIZE, DARKBLUE);
    drawGameText("Arrow keys or W, A, S, D - move", 100, 510, SMALL_TEXT_SIZE, DARKGRAY);
    drawGameText("Q - back to the menu", 100, 540, SMALL_TEXT_SIZE, DARKGRAY);
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

    drawGameText("Choose the maze size", 160, 120, TEXT_SIZE + 10, DARKBLUE);
    drawGameText(TextFormat("Type a number from %d to %d and press Enter.", MIN_MAZE_SIZE, MAX_MAZE_SIZE),
        160, 200, SMALL_TEXT_SIZE, DARKGRAY);

    // Show the typed number, or a line when nothing is typed yet.
    if (typedSize > 0)
    {
        drawGameText(TextFormat("%d", typedSize), 400, 280, TITLE_SIZE, BLACK);
    }
    else
    {
        drawGameText("_", 400, 280, TITLE_SIZE, BLACK);
    }

    if (showError)
    {
        drawGameText(TextFormat("The size must be from %d to %d.", MIN_MAZE_SIZE, MAX_MAZE_SIZE),
            160, 380, SMALL_TEXT_SIZE, RED);
    }

    drawGameText("Backspace - delete     Q - back to the menu", 160, 600, SMALL_TEXT_SIZE, GRAY);
}

int readDirection()
{
    // Every direction has an arrow key and a letter key.
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        return DIRECTION_UP;
    }
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        return DIRECTION_DOWN;
    }
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
    {
        return DIRECTION_LEFT;
    }
    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
    {
        return DIRECTION_RIGHT;
    }
    if (IsKeyPressed(KEY_Q))
    {
        return COMMAND_QUIT;
    }

    // No movement key was pressed in this frame.
    return DIRECTION_NONE;
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

            drawGameText(symbol, MAZE_LEFT + col * cellSize, MAZE_TOP + row * cellSize, cellSize, color);
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
    drawGameText("P", x, y, cellSize, RED);
}

void drawLegend()
{
    drawGameText("Map", PANEL_LEFT, 20, TEXT_SIZE, DARKBLUE);
    drawGameText("#  wall", PANEL_LEFT, 60, SMALL_TEXT_SIZE, DARKGRAY);
    drawGameText(".  open cell", PANEL_LEFT, 85, SMALL_TEXT_SIZE, GRAY);
    drawGameText("E  exit", PANEL_LEFT, 110, SMALL_TEXT_SIZE, DARKGREEN);
    drawGameText("P  you", PANEL_LEFT, 135, SMALL_TEXT_SIZE, RED);

    drawGameText("Keys", PANEL_LEFT, 185, TEXT_SIZE, DARKBLUE);
    drawGameText("Arrows or", PANEL_LEFT, 225, SMALL_TEXT_SIZE, DARKGRAY);
    drawGameText("W, A, S, D", PANEL_LEFT, 250, SMALL_TEXT_SIZE, DARKGRAY);
    drawGameText("Q - menu", PANEL_LEFT, 275, SMALL_TEXT_SIZE, DARKGRAY);
}

void drawPlayerInfo(int playerRow, int playerCol, int moveCount)
{
    drawGameText("Position", PANEL_LEFT, 330, TEXT_SIZE, DARKBLUE);
    drawGameText(TextFormat("Row: %d", playerRow), PANEL_LEFT, 370, SMALL_TEXT_SIZE, DARKGRAY);
    drawGameText(TextFormat("Column: %d", playerCol), PANEL_LEFT, 395, SMALL_TEXT_SIZE, DARKGRAY);
    drawGameText(TextFormat("Moves: %d", moveCount), PANEL_LEFT, 420, SMALL_TEXT_SIZE, DARKGRAY);
}

void drawCellInfo(char** maze, int size, int row, int col)
{
    const char* description = "outside the maze";

    if (isInside(size, row, col))
    {
        if (maze[row][col] == WALL_CELL)
        {
            description = "a wall";
        }
        else if (maze[row][col] == OPEN_CELL)
        {
            description = "an open cell";
        }
        else if (maze[row][col] == EXIT_CELL)
        {
            description = "the exit";
        }
    }

    drawGameText(TextFormat("Cell (%d, %d) is", row, col), PANEL_LEFT, 470, SMALL_TEXT_SIZE, DARKGRAY);
    drawGameText(description, PANEL_LEFT, 495, SMALL_TEXT_SIZE, DARKGRAY);
}

void drawWinScreen(int moveCount)
{
    ClearBackground(RAYWHITE);

    drawGameText("You found the exit!", 160, 200, TEXT_SIZE + 10, DARKGREEN);
    drawGameText(TextFormat("Moves: %d", moveCount), 160, 280, TEXT_SIZE, DARKGRAY);
    drawGameText("Q - back to the menu", 160, 380, SMALL_TEXT_SIZE, GRAY);
}

void drawTestScreen(const bool results[])
{
    ClearBackground(RAYWHITE);
    drawGameText("Self-tests", 100, 60, TEXT_SIZE + 10, DARKBLUE);

    // One line for every test: PASS or FAIL and the name of the test.
    int passedCount = 0;
    for (int i = 0; i < TEST_COUNT; i++)
    {
        int y = 120 + i * 30;
        if (results[i])
        {
            drawGameText("PASS", 100, y, SMALL_TEXT_SIZE, DARKGREEN);
            passedCount++;
        }
        else
        {
            drawGameText("FAIL", 100, y, SMALL_TEXT_SIZE, RED);
        }
        drawGameText(getTestName(i), 180, y, SMALL_TEXT_SIZE, DARKGRAY);
    }

    drawGameText(TextFormat("%d of %d tests passed.", passedCount, TEST_COUNT), 100, 480, TEXT_SIZE, DARKBLUE);
    drawGameText("Q - back to the menu", 100, 620, SMALL_TEXT_SIZE, GRAY);
}