// Maze.cpp - game logic: maze memory, maze generation and player movement.
// Functions that are still empty get their code in later commits.
// Until then their parameter names stay in /* */ comments.

#include "Maze.h"

char** createMaze(int size)
{
    // An array of row pointers: maze[row] will point to one row.
    char** maze = new char* [size];

    for (int row = 0; row < size; row++)
    {
        // Every row is an array of size cells.
        maze[row] = new char[size];

        for (int col = 0; col < size; col++)
        {
            maze[row][col] = WALL_CELL;
        }
    }

    return maze;
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