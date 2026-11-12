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

// Colors of the ByteMaze style: a dark background with bright symbols.
const Color BACKGROUND_COLOR = { 18, 22, 30, 255 };
const Color TITLE_COLOR = { 120, 200, 255, 255 };
const Color TEXT_COLOR = { 220, 225, 235, 255 };
const Color HINT_COLOR = { 130, 140, 155, 255 };
const Color GOOD_COLOR = { 110, 220, 120, 255 };
const Color BAD_COLOR = { 240, 90, 90, 255 };
const Color WALL_COLOR = { 70, 140, 160, 255 };
const Color PATH_COLOR = { 90, 100, 115, 255 };
const Color PLAYER_COLOR = { 255, 210, 80, 255 };
const Color DOOR_COLOR = { 240, 140, 70, 255 };
const Color KEY_COLOR = { 250, 230, 120, 255 };

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
    ClearBackground(BACKGROUND_COLOR);

    drawGameText("ByteMaze", 310, 120, TITLE_SIZE, TITLE_COLOR);
    drawGameText("1 - New game", 330, 260, TEXT_SIZE, TEXT_COLOR);
    drawGameText("2 - Help", 330, 310, TEXT_SIZE, TEXT_COLOR);
    drawGameText("3 - Self-tests", 330, 360, TEXT_SIZE, TEXT_COLOR);
    drawGameText("0 - Exit", 330, 410, TEXT_SIZE, TEXT_COLOR);
    drawGameText("Press a number key.", 330, 500, SMALL_TEXT_SIZE, HINT_COLOR);
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
    ClearBackground(BACKGROUND_COLOR);

    drawGameText("How to play", 100, 60, TEXT_SIZE + 10, TITLE_COLOR);
    drawGameText("Start in the top-left corner and find the exit >", 100, 140, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText("in the bottom-right corner.", 100, 170, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText("You cannot walk through walls.", 100, 200, SMALL_TEXT_SIZE, TEXT_COLOR);

    drawGameText("Map symbols", 100, 260, TEXT_SIZE, TITLE_COLOR);
    drawGameText("#  wall", 100, 310, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText(".  path", 100, 340, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText(">  exit", 100, 370, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText("@  player", 100, 400, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText("D  door: you need a key to open it, it can be a shorter way", 100, 430, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText("K  key: step on it to put it into the inventory", 100, 460, SMALL_TEXT_SIZE, TEXT_COLOR);

    drawGameText("Controls", 100, 510, TEXT_SIZE, TITLE_COLOR);
    drawGameText("Arrow keys or W, A, S, D - move", 100, 555, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText("Q - back to the menu", 100, 585, SMALL_TEXT_SIZE, TEXT_COLOR);
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
    ClearBackground(BACKGROUND_COLOR);

    drawGameText("Choose the maze size", 160, 120, TEXT_SIZE + 10, TITLE_COLOR);
    drawGameText(TextFormat("Type a number from %d to %d and press Enter.", MIN_MAZE_SIZE, MAX_MAZE_SIZE),
        160, 200, SMALL_TEXT_SIZE, TEXT_COLOR);

    // Show the typed number, or a line when nothing is typed yet.
    if (typedSize > 0)
    {
        drawGameText(TextFormat("%d", typedSize), 400, 280, TITLE_SIZE, TEXT_COLOR);
    }
    else
    {
        drawGameText("_", 400, 280, TITLE_SIZE, TEXT_COLOR);
    }

    if (showError)
    {
        drawGameText(TextFormat("The size must be from %d to %d.", MIN_MAZE_SIZE, MAX_MAZE_SIZE),
            160, 380, SMALL_TEXT_SIZE, BAD_COLOR);
    }

    drawGameText("Backspace - delete     Q - back to the menu", 160, 600, SMALL_TEXT_SIZE, HINT_COLOR);
}

// Returns the name of a difficulty level, for example "Hard".
const char* getDifficultyName(int difficulty)
{
    switch (difficulty)
    {
    case DIFFICULTY_EASY:
        return "Easy";
    case DIFFICULTY_HARD:
        return "Hard";
    default:
        return "Medium";
    }
}

// Draws one line of the difficulty screen with the settings from Maze.cpp.
void drawDifficultyLine(int key, int difficulty, int y)
{
    int fogRadius = getFogRadius(difficulty);
    const char* settings;

    if (fogRadius == NO_FOG)
    {
        settings = TextFormat("%d%% open cells, no fog, doors: %d, score x%d",
            getOpenCellChance(difficulty), getDoorCount(difficulty), getScoreMultiplier(difficulty));
    }
    else
    {
        settings = TextFormat("%d%% open cells, fog %d cells, doors: %d, score x%d",
            getOpenCellChance(difficulty), fogRadius, getDoorCount(difficulty), getScoreMultiplier(difficulty));
    }

    drawGameText(TextFormat("%d - %s", key, getDifficultyName(difficulty)), 160, y, TEXT_SIZE, TEXT_COLOR);
    drawGameText(settings, 160, y + 38, SMALL_TEXT_SIZE, HINT_COLOR);
}

void drawDifficultyScreen()
{
    ClearBackground(BACKGROUND_COLOR);

    drawGameText("Choose the difficulty", 160, 100, TEXT_SIZE + 10, TITLE_COLOR);
    drawDifficultyLine(1, DIFFICULTY_EASY, 200);
    drawDifficultyLine(2, DIFFICULTY_MEDIUM, 300);
    drawDifficultyLine(3, DIFFICULTY_HARD, 400);
    drawGameText("Q - back to the menu", 160, 600, SMALL_TEXT_SIZE, HINT_COLOR);
}

int readDifficultyChoice()
{
    if (IsKeyPressed(KEY_ONE))
    {
        return DIFFICULTY_EASY;
    }
    if (IsKeyPressed(KEY_TWO))
    {
        return DIFFICULTY_MEDIUM;
    }
    if (IsKeyPressed(KEY_THREE))
    {
        return DIFFICULTY_HARD;
    }

    // No difficulty key was pressed in this frame.
    return DIFFICULTY_NONE;
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

// Returns the symbol that is drawn for a cell. The exit is stored as E but drawn as >.
char getCellSymbol(char cell)
{
    if (cell == EXIT_CELL)
    {
        return '>';
    }
    return cell;
}

// Returns the color of the symbol of a cell.
Color getCellColor(char cell)
{
    if (cell == OPEN_CELL)
    {
        return PATH_COLOR;
    }
    if (cell == EXIT_CELL)
    {
        return GOOD_COLOR;
    }
    if (cell == DOOR_CELL)
    {
        return DOOR_COLOR;
    }
    if (cell == KEY_CELL)
    {
        return KEY_COLOR;
    }
    return WALL_COLOR;
}

void drawMaze(char** maze, int size, int playerRow, int playerCol, int fogRadius)
{
    ClearBackground(BACKGROUND_COLOR);
    int cellSize = getCellSize(size);

    // From the top row to the bottom row, every cell is drawn as its symbol.
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            // A cell in the fog stays dark.
            if (isVisible(playerRow, playerCol, row, col, fogRadius))
            {
                char symbol[2] = { getCellSymbol(maze[row][col]), '\0' };
                drawGameText(symbol, MAZE_LEFT + col * cellSize, MAZE_TOP + row * cellSize, cellSize, getCellColor(maze[row][col]));
            }
        }
    }
}

void drawPlayer(int size, int playerRow, int playerCol)
{
    int cellSize = getCellSize(size);
    int x = MAZE_LEFT + playerCol * cellSize;
    int y = MAZE_TOP + playerRow * cellSize;

    // Cover the symbol of the cell, then draw @ on top.
    DrawRectangle(x, y, cellSize, cellSize, BACKGROUND_COLOR);
    drawGameText("@", x, y, cellSize, PLAYER_COLOR);
}

void drawLegend()
{
    drawGameText("Map", PANEL_LEFT, 20, TEXT_SIZE, TITLE_COLOR);
    drawGameText("#  wall", PANEL_LEFT, 60, SMALL_TEXT_SIZE, WALL_COLOR);
    drawGameText(".  path", PANEL_LEFT, 85, SMALL_TEXT_SIZE, PATH_COLOR);
    drawGameText(">  exit", PANEL_LEFT, 110, SMALL_TEXT_SIZE, GOOD_COLOR);
    drawGameText("@  you", PANEL_LEFT, 135, SMALL_TEXT_SIZE, PLAYER_COLOR);
    drawGameText("D  door", PANEL_LEFT, 160, SMALL_TEXT_SIZE, DOOR_COLOR);
    drawGameText("K  key", PANEL_LEFT, 185, SMALL_TEXT_SIZE, KEY_COLOR);

    drawGameText("Keys", PANEL_LEFT, 225, TEXT_SIZE, TITLE_COLOR);
    drawGameText("Arrows or", PANEL_LEFT, 260, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText("W, A, S, D", PANEL_LEFT, 282, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText("Q - menu", PANEL_LEFT, 304, SMALL_TEXT_SIZE, TEXT_COLOR);
}

void drawPlayerInfo(int playerRow, int playerCol, int moveCount)
{
    drawGameText("Position", PANEL_LEFT, 345, TEXT_SIZE, TITLE_COLOR);
    drawGameText(TextFormat("Row: %d", playerRow), PANEL_LEFT, 380, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText(TextFormat("Column: %d", playerCol), PANEL_LEFT, 402, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText(TextFormat("Moves: %d", moveCount), PANEL_LEFT, 424, SMALL_TEXT_SIZE, TEXT_COLOR);
}

void drawInventory(int keyCount)
{
    drawGameText("Inventory", PANEL_LEFT, 525, TEXT_SIZE, TITLE_COLOR);
    drawGameText(TextFormat("Keys: %d", keyCount), PANEL_LEFT, 560, SMALL_TEXT_SIZE, KEY_COLOR);
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
            description = "a path";
        }
        else if (maze[row][col] == EXIT_CELL)
        {
            description = "the exit";
        }
        else if (maze[row][col] == DOOR_CELL)
        {
            description = "a door, it needs a key";
        }
        else if (maze[row][col] == KEY_CELL)
        {
            description = "a key";
        }
    }

    drawGameText(TextFormat("Cell (%d, %d) is", row, col), PANEL_LEFT, 465, SMALL_TEXT_SIZE, TEXT_COLOR);
    drawGameText(description, PANEL_LEFT, 487, SMALL_TEXT_SIZE, TEXT_COLOR);
}

void drawTimer(int seconds)
{
    // 75 seconds are shown as 1:15. %02d always prints two digits.
    drawGameText(TextFormat("Time: %d:%02d", seconds / 60, seconds % 60), PANEL_LEFT, 600, SMALL_TEXT_SIZE, TEXT_COLOR);
}

void drawWinScreen(int moveCount, int seconds, int score, int bestScore)
{
    ClearBackground(BACKGROUND_COLOR);

    drawGameText("You found the exit!", 160, 160, TEXT_SIZE + 10, GOOD_COLOR);
    drawGameText(TextFormat("Moves: %d", moveCount), 160, 240, TEXT_SIZE, TEXT_COLOR);
    drawGameText(TextFormat("Time: %d:%02d", seconds / 60, seconds % 60), 160, 280, TEXT_SIZE, TEXT_COLOR);
    drawGameText(TextFormat("Score: %d", score), 160, 340, TEXT_SIZE + 10, PLAYER_COLOR);
    drawGameText(TextFormat("Best for this size and difficulty: %d", bestScore), 160, 395, SMALL_TEXT_SIZE, TEXT_COLOR);

    if (score >= bestScore)
    {
        drawGameText("New best result!", 160, 425, SMALL_TEXT_SIZE, GOOD_COLOR);
    }

    drawGameText("Q - back to the menu", 160, 600, SMALL_TEXT_SIZE, HINT_COLOR);
}

void drawTestScreen(const bool results[])
{
    ClearBackground(BACKGROUND_COLOR);
    drawGameText("Self-tests", 100, 60, TEXT_SIZE + 10, TITLE_COLOR);

    // One line for every test: PASS or FAIL and the name of the test.
    int passedCount = 0;
    for (int i = 0; i < TEST_COUNT; i++)
    {
        int y = 120 + i * 30;
        if (results[i])
        {
            drawGameText("PASS", 100, y, SMALL_TEXT_SIZE, GOOD_COLOR);
            passedCount++;
        }
        else
        {
            drawGameText("FAIL", 100, y, SMALL_TEXT_SIZE, BAD_COLOR);
        }
        drawGameText(getTestName(i), 180, y, SMALL_TEXT_SIZE, TEXT_COLOR);
    }

    drawGameText(TextFormat("%d of %d tests passed.", passedCount, TEST_COUNT), 100, 480, TEXT_SIZE, TITLE_COLOR);
    drawGameText("Q - back to the menu", 100, 620, SMALL_TEXT_SIZE, HINT_COLOR);
}