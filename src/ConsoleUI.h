// ConsoleUI.h - everything the player sees and types.
// All cin and cout code of the game is in ConsoleUI.cpp.

#pragma once

// Main menu options.
const int MENU_EXIT = 0;
const int MENU_NEW_GAME = 1;
const int MENU_HELP = 2;
const int MENU_SELF_TEST = 3;

// Returned by readDirection when the player presses Q.
// The other return values are the DIRECTION_ constants from Maze.h.
const int COMMAND_QUIT = 0;

// Prints the game title and a short welcome text.
void showIntro();

// Prints the numbered main menu options.
void showMenu();

// Prints the goal of the game, the map symbols and the controls.
void showHelp();

// Reads a whole number from minValue to maxValue and asks again on bad input.
int readIntInRange(int minValue, int maxValue);

// Asks for the maze size and returns a number from MIN_MAZE_SIZE to MAX_MAZE_SIZE.
int readMazeSize();

// Waits for an arrow key, W, A, S, D or Q.
// Returns DIRECTION_UP, DIRECTION_DOWN, DIRECTION_LEFT, DIRECTION_RIGHT or COMMAND_QUIT.
int readDirection();

// Draws the maze from the top row to the bottom row with the player as P.
void printMaze(char** maze, int size, int playerRow, int playerCol);

// Explains the symbols used on the map.
void printLegend();

// Prints the player's row, column and number of moves.
void printPlayerInfo(int playerRow, int playerCol, int moveCount);

// Describes the cell at (row, col): wall, open cell, exit or outside the maze.
void printCellInfo(char** maze, int size, int row, int col);

// Shows the controls before every move.
void printPrompt();

// Congratulates the player for reaching the exit.
void printWinMessage(int moveCount);

// Clears the console window.
void clearScreen();