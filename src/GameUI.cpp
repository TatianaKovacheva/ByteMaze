// GameUI.cpp - raylib drawing and keyboard input. Owner: Plamen Rusev.
// The functions are empty for now so the project builds.
// Parameter names are in /* */ comments until the function uses them,
// because Visual Studio warns about parameters that are never used.

#include "GameUI.h"

void drawMenuScreen()
{
}

int readMenuChoice()
{
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