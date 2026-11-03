// GameUI.cpp - raylib drawing and keyboard input. Owner: Plamen Rusev.
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
}

bool readBackKey()
{
    return false;
}

bool readSizeInput(int& /*typedSize*/)
{
    return false;
}

void drawSizeScreen(int /*typedSize*/, bool /*showError*/)
{
}

int readDirection()
{
    return COMMAND_QUIT;
}

void drawMaze(char** /*maze*/, int /*size*/)
{
}

void drawPlayer(int /*size*/, int /*playerRow*/, int /*playerCol*/)
{
}

void drawLegend()
{
}

void drawPlayerInfo(int /*playerRow*/, int /*playerCol*/, int /*moveCount*/)
{
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