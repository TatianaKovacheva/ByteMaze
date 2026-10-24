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

// Directions the player can move in.
const int DIRECTION_UP = 1;
const int DIRECTION_DOWN = 2;
const int DIRECTION_LEFT = 3;
const int DIRECTION_RIGHT = 4;

// Allocates a size x size maze with new[] and fills every cell with a wall.
char** createMaze(int size);

// Releases all memory that createMaze allocated.
void destroyMaze(char** maze, int size);

// Returns true if size is between MIN_MAZE_SIZE and MAX_MAZE_SIZE.
bool isValidSize(int size);

// Builds a new random maze with an open path from (0,0) to the exit.
void generateMaze(char** maze, int size);

// Returns true if (row, col) is inside the maze.
bool isInside(int size, int row, int col);

// Returns true if (row, col) is inside the maze and is not a wall.
bool canMoveTo(char** maze, int size, int row, int col);

// Calculates the cell next to (row, col) in the given direction.
void getTargetPosition(int row, int col, int direction, int& targetRow, int& targetCol);

// Moves the player one cell in the given direction if possible.
// Returns true if the player moved, false if a wall or the edge blocked the move.
bool movePlayer(char** maze, int size, int& playerRow, int& playerCol, int direction);

// Returns true if (row, col) is the exit in the bottom-right corner.
bool isAtExit(int size, int row, int col);