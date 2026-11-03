// Maze.cpp - game logic.
// The functions are empty for now so the project builds.
// Parameter names are in /* */ comments until the function uses them,
// because Visual Studio warns about parameters that are never used.

#include "Maze.h"

char** createMaze(int /*size*/)
{
    return nullptr;
}

void destroyMaze(char** /*maze*/, int /*size*/)
{
}

bool isValidSize(int /*size*/)
{
    return false;
}

void generateMaze(char** /*maze*/, int /*size*/)
{
}

bool isInside(int /*size*/, int /*row*/, int /*col*/)
{
    return false;
}

bool canMoveTo(char** /*maze*/, int /*size*/, int /*row*/, int /*col*/)
{
    return false;
}

void getTargetPosition(int row, int col, int /*direction*/, int& targetRow, int& targetCol)
{
    targetRow = row;
    targetCol = col;
}

bool movePlayer(char** /*maze*/, int /*size*/, int& /*playerRow*/, int& /*playerCol*/, int /*direction*/)
{
    return false;
}

bool isAtExit(int /*size*/, int /*row*/, int /*col*/)
{
    return false;
}
