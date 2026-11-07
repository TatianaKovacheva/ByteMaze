// Tests.cpp - self-tests for the game logic.
// Every test returns true if it passed. GameUI shows the results on the test screen.
// Tests that are still empty return false until they get their code.

#include "Tests.h"
#include "Maze.h"

// The Visual Studio debug heap can count the memory blocks that are in use.
#ifdef _MSC_VER
#include <crtdbg.h>
#endif

const char* getTestName(int index)
{
    switch (index)
    {
    case 0:
        return "Valid and invalid maze sizes";
    case 1:
        return "Maze memory is created and released";
    case 2:
        return "A wall blocks the player";
    case 3:
        return "The player can enter an open cell";
    case 4:
        return "The edge of the maze blocks the player";
    case 5:
        return "The exit is detected";
    case 6:
        return "Every generated maze can be solved";
    case 7:
        return "Start and exit of a generated maze";
    default:
        return "Unknown test";
    }
}

bool runTest(int index)
{
    switch (index)
    {
    case 0:
        return testValidSizes();
    case 1:
        return testCreateAndDestroy();
    case 2:
        return testWallBlocksMove();
    case 3:
        return testValidMove();
    case 4:
        return testMoveOutsideMaze();
    case 5:
        return testExitReached();
    case 6:
        return testPathExists();
    case 7:
        return testGeneratedCells();
    default:
        return false;
    }
}

bool testValidSizes()
{
    // The two limits are allowed.
    bool limitsAccepted = isValidSize(MIN_MAZE_SIZE) && isValidSize(MAX_MAZE_SIZE);

    // One below, one above, zero and a negative number are not allowed.
    bool othersRejected = !isValidSize(MIN_MAZE_SIZE - 1) && !isValidSize(MAX_MAZE_SIZE + 1)
        && !isValidSize(0) && !isValidSize(-5);

    return limitsAccepted && othersRejected;
}

bool testCreateAndDestroy()
{
#if defined(_MSC_VER) && defined(_DEBUG)
    // Remember which memory is in use before the test.
    _CrtMemState before;
    _CrtMemCheckpoint(&before);
#endif

    char** maze = createMaze(MIN_MAZE_SIZE);
    if (maze == nullptr)
    {
        return false;
    }

    // A new maze must be full of walls.
    bool allWalls = true;
    for (int row = 0; row < MIN_MAZE_SIZE; row++)
    {
        for (int col = 0; col < MIN_MAZE_SIZE; col++)
        {
            if (maze[row][col] != WALL_CELL)
            {
                allWalls = false;
            }
        }
    }

    destroyMaze(maze, MIN_MAZE_SIZE);

#if defined(_MSC_VER) && defined(_DEBUG)
    // After destroyMaze exactly the same memory must be in use as before.
    _CrtMemState after;
    _CrtMemState difference;
    _CrtMemCheckpoint(&after);
    if (_CrtMemDifference(&difference, &before, &after))
    {
        return false;
    }
#endif

    return allWalls;
}

// A small hand-made maze for the movement tests:
// row 0 is open from (0,0) to (0,2), (0,3) is a wall, the exit is in the corner,
// and every other cell is a wall.
char** createTestMaze()
{
    char** maze = createMaze(MIN_MAZE_SIZE);
    maze[0][0] = OPEN_CELL;
    maze[0][1] = OPEN_CELL;
    maze[0][2] = OPEN_CELL;
    maze[MIN_MAZE_SIZE - 1][MIN_MAZE_SIZE - 1] = EXIT_CELL;
    return maze;
}

bool testWallBlocksMove()
{
    char** maze = createTestMaze();
    int row = 0;
    int col = 2;

    // (0,3) on the right and (1,2) below are walls.
    bool movedRight = movePlayer(maze, MIN_MAZE_SIZE, row, col, DIRECTION_RIGHT);
    bool movedDown = movePlayer(maze, MIN_MAZE_SIZE, row, col, DIRECTION_DOWN);
    bool stayed = row == 0 && col == 2;

    destroyMaze(maze, MIN_MAZE_SIZE);
    return !movedRight && !movedDown && stayed;
}

bool testValidMove()
{
    char** maze = createTestMaze();
    int row = 0;
    int col = 0;

    // (0,1) on the right is open.
    bool moved = movePlayer(maze, MIN_MAZE_SIZE, row, col, DIRECTION_RIGHT);
    bool positionChanged = row == 0 && col == 1;

    destroyMaze(maze, MIN_MAZE_SIZE);
    return moved && positionChanged;
}

bool testMoveOutsideMaze()
{
    char** maze = createTestMaze();
    int row = 0;
    int col = 0;

    // Up and left from (0,0) are outside the maze. DIRECTION_NONE is not a move.
    bool movedUp = movePlayer(maze, MIN_MAZE_SIZE, row, col, DIRECTION_UP);
    bool movedLeft = movePlayer(maze, MIN_MAZE_SIZE, row, col, DIRECTION_LEFT);
    bool movedNowhere = movePlayer(maze, MIN_MAZE_SIZE, row, col, DIRECTION_NONE);
    bool stayed = row == 0 && col == 0;

    destroyMaze(maze, MIN_MAZE_SIZE);
    return !movedUp && !movedLeft && !movedNowhere && stayed;
}

bool testExitReached()
{
    int last = MIN_MAZE_SIZE - 1;

    // Only the bottom-right corner is the exit, not the other three corners.
    return isAtExit(MIN_MAZE_SIZE, last, last)
        && !isAtExit(MIN_MAZE_SIZE, 0, 0)
        && !isAtExit(MIN_MAZE_SIZE, last, 0)
        && !isAtExit(MIN_MAZE_SIZE, 0, last);
}

// Recursive flood fill: visits every open cell that can be reached from (row, col).
// Returns true as soon as the exit is reached.
bool canReachExit(char** maze, bool** visited, int size, int row, int col)
{
    // Stop at walls, outside the maze and at cells that were already visited.
    if (!canMoveTo(maze, size, row, col) || visited[row][col])
    {
        return false;
    }

    visited[row][col] = true;

    if (isAtExit(size, row, col))
    {
        return true;
    }

    // The function calls itself for the four neighbours.
    return canReachExit(maze, visited, size, row - 1, col)
        || canReachExit(maze, visited, size, row + 1, col)
        || canReachExit(maze, visited, size, row, col - 1)
        || canReachExit(maze, visited, size, row, col + 1);
}

bool testPathExists()
{
    // 3 random mazes of every size from 10 to 50.
    for (int size = MIN_MAZE_SIZE; size <= MAX_MAZE_SIZE; size++)
    {
        for (int round = 0; round < 3; round++)
        {
            char** maze = createMaze(size);
            generateMaze(maze, size);

            // visited[row][col] becomes true when the flood fill reaches the cell.
            bool** visited = new bool* [size];
            for (int row = 0; row < size; row++)
            {
                visited[row] = new bool[size];
                for (int col = 0; col < size; col++)
                {
                    visited[row][col] = false;
                }
            }

            bool reached = canReachExit(maze, visited, size, 0, 0);

            for (int row = 0; row < size; row++)
            {
                delete[] visited[row];
            }
            delete[] visited;
            destroyMaze(maze, size);

            if (!reached)
            {
                return false;
            }
        }
    }

    return true;
}

bool testGeneratedCells()
{
    return false;
}