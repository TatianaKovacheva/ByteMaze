// Tests.cpp - self-tests for the game logic.
// Every test returns true if it passed. GameUI shows the results on the test screen.
// Tests that are still empty return false until they get their code.

#include "Tests.h"
#include "Maze.h"

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
    return false;
}

bool testWallBlocksMove()
{
    return false;
}

bool testValidMove()
{
    return false;
}

bool testMoveOutsideMaze()
{
    return false;
}

bool testExitReached()
{
    return false;
}

bool testPathExists()
{
    return false;
}

bool testGeneratedCells()
{
    return false;
}