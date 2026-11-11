// Maze.h - game logic: maze memory, maze generation and player movement.
// Rule from the assignment: none of these functions use cin or cout.

#pragma once

// Smallest and largest maze size the player may choose.
const int MIN_MAZE_SIZE = 10;
const int MAX_MAZE_SIZE = 50;

// Symbols stored in the maze cells.
const char WALL_CELL = '#';
const char OPEN_CELL = '.';
const char EXIT_CELL = 'E';
const char DOOR_CELL = 'D';
const char KEY_CELL = 'K';

// Difficulty levels. NO_FOG means that the whole maze is visible.
const int DIFFICULTY_EASY = 1;
const int DIFFICULTY_MEDIUM = 2;
const int DIFFICULTY_HARD = 3;
const int NO_FOG = 0;

// Directions the player can move in.
// DIRECTION_NONE means that no movement key was pressed in this frame.
const int DIRECTION_NONE = 0;
const int DIRECTION_UP = 1;
const int DIRECTION_DOWN = 2;
const int DIRECTION_LEFT = 3;
const int DIRECTION_RIGHT = 4;

// Chance in percent that a wall outside the path becomes an open cell.
int getOpenCellChance(int difficulty);

// Number of doors in the maze. There is one key for every door.
int getDoorCount(int difficulty);

// How many cells around the player are visible, or NO_FOG.
int getFogRadius(int difficulty);

// The score is multiplied by this number.
int getScoreMultiplier(int difficulty);

// Allocates a size x size maze with new[] and fills every cell with a wall.
char** createMaze(int size);

// Releases all memory that createMaze allocated.
void destroyMaze(char** maze, int size);

// Returns true if size is between MIN_MAZE_SIZE and MAX_MAZE_SIZE.
bool isValidSize(int size);

// Builds a new random maze with an open path from (0,0) to the exit.
// When no difficulty is given, Medium is used.
void generateMaze(char** maze, int size, int difficulty = DIFFICULTY_MEDIUM);

// Returns true if (row, col) is inside the maze.
bool isInside(int size, int row, int col);

// Returns true if (row, col) is inside the maze and is not a wall.
bool canMoveTo(char** maze, int size, int row, int col);

// Calculates the cell next to (row, col) in the given direction.
void getTargetPosition(int row, int col, int direction, int& targetRow, int& targetCol);

// Moves the player one cell in the given direction if possible.
// Returns true if the player moved, false if a wall or the edge blocked the move.
bool movePlayer(char** maze, int size, int& playerRow, int& playerCol, int direction);

// Like movePlayer, but a key on the new cell goes into keyCount
// and a closed door opens if keyCount is more than 0.
bool movePlayerWithKeys(char** maze, int size, int& playerRow, int& playerCol, int direction, int& keyCount);

// Returns true if (row, col) is near enough to the player to be seen.
bool isVisible(int playerRow, int playerCol, int row, int col, int fogRadius);

// Score of a finished game: fewer moves and seconds give more points.
int calculateScore(int moveCount, int seconds, int difficulty);

// Returns true if (row, col) is the exit in the bottom-right corner.
bool isAtExit(int size, int row, int col);