// Tests.h - self-tests for the game logic in Maze.cpp.
// Every test returns true if it passed and false if it failed.

#pragma once

// Runs every test and prints PASS or FAIL for each one and a summary.
void runAllTests();

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