// Tests.h - self-tests for the game logic in Maze.cpp.
// Every test returns true if it passed and false if it failed.
// The results are shown on the test screen, so no test uses cout.

#pragma once

// Number of tests in the list. GameUI draws one line for each of them.
const int TEST_COUNT = 10;

// Returns the name of the test with this number, from 0 to TEST_COUNT - 1.
const char* getTestName(int index);

// Runs the test with this number and returns true if it passed.
bool runTest(int index);

// Checks that 10 and 50 are accepted and that 9, 51, 0 and -5 are rejected.
bool testValidSizes();

// Checks that destroyMaze frees all memory that createMaze allocated.
bool testCreateAndDestroy();

// Checks that the player cannot move into a wall.
bool testWallBlocksMove();

// Checks that the player can move into an open cell.
bool testValidMove();

// Checks that the player cannot move outside the maze.
bool testMoveOutsideMaze();

// Checks that the exit is detected in the bottom-right corner.
bool testExitReached();

// Checks that every generated maze has a path from the start to the exit.
bool testPathExists();

// Checks that the start is open and the exit is in the bottom-right corner.
bool testGeneratedCells();

// Checks that a door blocks the player without a key and opens with a key.
bool testDoorNeedsKey();

// Checks that a key goes into the inventory when the player steps on it.
bool testKeyIsPickedUp();