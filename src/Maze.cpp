// Maze.cpp - game logic: maze memory, maze generation and player movement.

#include "Maze.h"

#include <cstdlib>

// Chance in percent that a wall outside the path becomes an open cell.
const int OPEN_CELL_CHANCE = 60;

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

void destroyMaze(char** maze, int size)
{
    // Nothing to release if the maze was never created.
    if (maze == nullptr)
    {
        return;
    }

    // First every row, then the array of row pointers.
    for (int row = 0; row < size; row++)
    {
        delete[] maze[row];
    }

    delete[] maze;
}

bool isValidSize(int size)
{
    return size >= MIN_MAZE_SIZE && size <= MAX_MAZE_SIZE;
}

// Opens a random path of right and down steps from (0,0) to the bottom-right corner.
// Because of this path the exit can always be reached.
void carvePath(char** maze, int size)
{
    int row = 0;
    int col = 0;
    maze[row][col] = OPEN_CELL;

    while (row < size - 1 || col < size - 1)
    {
        bool goRight;

        if (row == size - 1)
        {
            // Already in the last row: only right is possible.
            goRight = true;
        }
        else if (col == size - 1)
        {
            // Already in the last column: only down is possible.
            goRight = false;
        }
        else
        {
            // Otherwise choose right or down at random.
            goRight = rand() % 2 == 0;
        }

        if (goRight)
        {
            col++;
        }
        else
        {
            row++;
        }

        maze[row][col] = OPEN_CELL;
    }
}

// Turns some of the walls outside the path into open cells at random.
void openRandomCells(char** maze, int size)
{
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            if (maze[row][col] == WALL_CELL && rand() % 100 < OPEN_CELL_CHANCE)
            {
                maze[row][col] = OPEN_CELL;
            }
        }
    }
}

void generateMaze(char** maze, int size)
{
    // Start again from a maze full of walls.
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            maze[row][col] = WALL_CELL;
        }
    }

    carvePath(maze, size);
    openRandomCells(maze, size);

    // The exit is always in the bottom-right corner.
    maze[size - 1][size - 1] = EXIT_CELL;
}

bool isInside(int size, int row, int col)
{
    return row >= 0 && row < size && col >= 0 && col < size;
}

bool canMoveTo(char** maze, int size, int row, int col)
{
    // isInside is checked first, so maze[row][col] is never read outside the array.
    return isInside(size, row, col) && maze[row][col] != WALL_CELL;
}

void getTargetPosition(int row, int col, int direction, int& targetRow, int& targetCol)
{
    // Start from the current cell, then change one coordinate.
    targetRow = row;
    targetCol = col;

    if (direction == DIRECTION_UP)
    {
        targetRow = row - 1;
    }
    else if (direction == DIRECTION_DOWN)
    {
        targetRow = row + 1;
    }
    else if (direction == DIRECTION_LEFT)
    {
        targetCol = col - 1;
    }
    else if (direction == DIRECTION_RIGHT)
    {
        targetCol = col + 1;
    }
}

bool movePlayer(char** maze, int size, int& playerRow, int& playerCol, int direction)
{
    // Only the four directions move the player. DIRECTION_NONE is not a move.
    if (direction < DIRECTION_UP || direction > DIRECTION_RIGHT)
    {
        return false;
    }

    int targetRow;
    int targetCol;
    getTargetPosition(playerRow, playerCol, direction, targetRow, targetCol);

    // A wall or the edge of the maze blocks the move.
    if (!canMoveTo(maze, size, targetRow, targetCol))
    {
        return false;
    }

    playerRow = targetRow;
    playerCol = targetCol;
    return true;
}

bool isAtExit(int size, int row, int col)
{
    return row == size - 1 && col == size - 1;
}