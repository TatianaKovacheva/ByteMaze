// Maze.cpp - game logic: maze memory, maze generation and player movement.

#include "Maze.h"

#include <cstdlib>

int getOpenCellChance(int difficulty)
{
    switch (difficulty)
    {
    case DIFFICULTY_EASY:
        return 70;
    case DIFFICULTY_HARD:
        return 45;
    default:
        return 60;
    }
}

int getDoorCount(int difficulty)
{
    switch (difficulty)
    {
    case DIFFICULTY_EASY:
        return 3;
    case DIFFICULTY_HARD:
        return 1;
    default:
        return 2;
    }
}

int getFogRadius(int difficulty)
{
    switch (difficulty)
    {
    case DIFFICULTY_EASY:
        return NO_FOG;
    case DIFFICULTY_HARD:
        return 3;
    default:
        return 5;
    }
}

int getScoreMultiplier(int difficulty)
{
    switch (difficulty)
    {
    case DIFFICULTY_EASY:
        return 1;
    case DIFFICULTY_HARD:
        return 3;
    default:
        return 2;
    }
}

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
// openChance is the chance in percent for every wall.
void openRandomCells(char** maze, int size, int openChance)
{
    for (int row = 0; row < size; row++)
    {
        for (int col = 0; col < size; col++)
        {
            if (maze[row][col] == WALL_CELL && rand() % 100 < openChance)
            {
                maze[row][col] = OPEN_CELL;
            }
        }
    }
}

void generateMaze(char** maze, int size, int difficulty)
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
    openRandomCells(maze, size, getOpenCellChance(difficulty));

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