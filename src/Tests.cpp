// Tests.cpp - self-tests for the game logic.
// Every test returns true if it passed. GameUI shows the results on the test screen.

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
    case 8:
        return "A door needs a key";
    case 9:
        return "A key goes into the inventory";
    case 10:
        return "Fog hides the far cells";
    case 11:
        return "Score formula and multiplier";
    case 12:
        return "Keys can be reached before the doors";
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
    case 8:
        return testDoorNeedsKey();
    case 9:
        return testKeyIsPickedUp();
    case 10:
        return testFogVisibility();
    case 11:
        return testScoreCalculation();
    case 12:
        return testKeysBeforeDoors();
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
// Doors count as open: there is a key for every door, and testKeysBeforeDoors checks
// that all keys can be reached before the doors.
bool canReachExit(char** maze, bool** visited, int size, int row, int col)
{
    // Stop at walls, outside the maze and at cells that were already visited.
    if (!isInside(size, row, col) || maze[row][col] == WALL_CELL || visited[row][col])
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
    int size = MIN_MAZE_SIZE;
    char** maze = createMaze(size);
    generateMaze(maze, size);

    bool startOpen = maze[0][0] == OPEN_CELL;
    bool exitInCorner = maze[size - 1][size - 1] == EXIT_CELL;

    destroyMaze(maze, size);
    return startOpen && exitInCorner;
}

bool testDoorNeedsKey()
{
    char** maze = createTestMaze();
    maze[0][3] = DOOR_CELL;
    maze[0][4] = OPEN_CELL;
    int row = 0;
    int col = 2;
    int keyCount = 0;

    // Without a key the door at (0,3) works like a wall.
    bool movedWithoutKey = movePlayerWithKeys(maze, MIN_MAZE_SIZE, row, col, DIRECTION_RIGHT, keyCount);

    // With one key the door opens and the key is used.
    keyCount = 1;
    bool movedWithKey = movePlayerWithKeys(maze, MIN_MAZE_SIZE, row, col, DIRECTION_RIGHT, keyCount);
    bool doorOpened = maze[0][3] == OPEN_CELL;

    destroyMaze(maze, MIN_MAZE_SIZE);
    return !movedWithoutKey && movedWithKey && doorOpened && keyCount == 0 && col == 3;
}

bool testKeyIsPickedUp()
{
    char** maze = createTestMaze();
    maze[0][1] = KEY_CELL;
    int row = 0;
    int col = 0;
    int keyCount = 0;

    bool moved = movePlayerWithKeys(maze, MIN_MAZE_SIZE, row, col, DIRECTION_RIGHT, keyCount);
    bool keyRemoved = maze[0][1] == OPEN_CELL;

    destroyMaze(maze, MIN_MAZE_SIZE);
    return moved && keyCount == 1 && keyRemoved;
}

bool testFogVisibility()
{
    // The player is at (5,5) and sees 3 cells in every direction.
    bool nearVisible = isVisible(5, 5, 5, 8, 3) && isVisible(5, 5, 2, 2, 3);
    bool farHidden = !isVisible(5, 5, 5, 9, 3) && !isVisible(5, 5, 1, 5, 3);

    // Without fog even the farthest cell is visible.
    bool noFogVisible = isVisible(0, 0, MAX_MAZE_SIZE - 1, MAX_MAZE_SIZE - 1, NO_FOG);

    return nearVisible && farHidden && noFogVisible;
}

bool testScoreCalculation()
{
    // 1000 - 2 * 10 - 3 * 20 = 920
    bool easyScore = calculateScore(10, 20, DIFFICULTY_EASY) == 920;
    bool hardScore = calculateScore(10, 20, DIFFICULTY_HARD) == 920 * 3;

    // A very slow game still gets the minimum of 100 points, times 2 on Medium.
    bool minimumScore = calculateScore(1000, 1000, DIFFICULTY_MEDIUM) == 200;

    return easyScore && hardScore && minimumScore;
}

bool testKeysBeforeDoors()
{
    for (int difficulty = DIFFICULTY_EASY; difficulty <= DIFFICULTY_HARD; difficulty++)
    {
        // Sizes 10, 15, 20 ... 50, three mazes of every size.
        for (int size = MIN_MAZE_SIZE; size <= MAX_MAZE_SIZE; size += 5)
        {
            for (int round = 0; round < 3; round++)
            {
                char** maze = createMaze(size);
                generateMaze(maze, size, difficulty);

                bool** reached = new bool* [size];
                for (int row = 0; row < size; row++)
                {
                    reached[row] = new bool[size];
                    for (int col = 0; col < size; col++)
                    {
                        reached[row][col] = false;
                    }
                }
                markReachable(maze, size, 0, 0, reached);

                // Count the doors, the keys and the keys that the flood fill reached.
                int doors = 0;
                int keys = 0;
                int keysReached = 0;
                for (int row = 0; row < size; row++)
                {
                    for (int col = 0; col < size; col++)
                    {
                        if (maze[row][col] == DOOR_CELL)
                        {
                            doors++;
                        }
                        if (maze[row][col] == KEY_CELL)
                        {
                            keys++;
                            if (reached[row][col])
                            {
                                keysReached++;
                            }
                        }
                    }
                }

                for (int row = 0; row < size; row++)
                {
                    delete[] reached[row];
                }
                delete[] reached;
                destroyMaze(maze, size);

                bool countsRight = doors == getDoorCount(difficulty) && keys == doors;
                if (!countsRight || keysReached != keys)
                {
                    return false;
                }
            }
        }
    }

    return true;
}