// GameUI.h - everything the player sees and presses in the raylib window.
// All drawing and all keyboard code of the game is in GameUI.cpp.

#pragma once

// Size of the game window in pixels.
const int WINDOW_WIDTH = 900;
const int WINDOW_HEIGHT = 700;

// Main menu options. MENU_NONE means that no menu key was pressed in this frame.
const int MENU_NONE = -1;
const int MENU_EXIT = 0;
const int MENU_NEW_GAME = 1;
const int MENU_HELP = 2;
const int MENU_SELF_TEST = 3;

// Returned by readDirection when the player presses Q.
// The other return values are the DIRECTION_ constants from Maze.h.
const int COMMAND_QUIT = 5;

// Loads the game font. Call it once after InitWindow.
void loadGameFont();

// Releases the game font. Call it once before CloseWindow.
void unloadGameFont();

// Draws the game title and the numbered menu options.
void drawMenuScreen();

// Returns the menu option of the pressed key, or MENU_NONE.
int readMenuChoice();

// Draws the goal of the game, the map symbols and the controls.
void drawHelpScreen();

// Returns true in the frame when Q is pressed on the help, win or test screen.
bool readBackKey();

// Adds pressed digits to typedSize and removes the last digit on Backspace.
// Returns true in the frame when Enter is pressed.
bool readSizeInput(int& typedSize);

// Draws the typed maze size and an error message if showError is true.
void drawSizeScreen(int typedSize, bool showError);

// Reads the arrow keys, W, A, S, D and Q. Does not wait for a key.
// Returns a DIRECTION_ constant from Maze.h, DIRECTION_NONE or COMMAND_QUIT.
int readDirection();

// Draws the text maze from the top row to the bottom row.
void drawMaze(char** maze, int size);

// Draws the symbol @ at the position of the player.
void drawPlayer(int size, int playerRow, int playerCol);

// Explains the symbols used on the map.
void drawLegend();

// Draws the player's row, column and number of moves.
void drawPlayerInfo(int playerRow, int playerCol, int moveCount);

// Describes the cell at (row, col): wall, open cell, exit or outside the maze.
void drawCellInfo(char** maze, int size, int row, int col);

// Congratulates the player for reaching the exit.
void drawWinScreen(int moveCount);

// Draws PASS or FAIL for every test. results has TEST_COUNT elements.
void drawTestScreen(const bool results[]);