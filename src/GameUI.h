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

// Returned by readDifficultyChoice when no difficulty key was pressed in this frame.
const int DIFFICULTY_NONE = 0;

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

// Draws the three difficulty levels and what they change.
void drawDifficultyScreen();

// Returns DIFFICULTY_EASY, DIFFICULTY_MEDIUM or DIFFICULTY_HARD from Maze.h
// for keys 1, 2 and 3, or DIFFICULTY_NONE.
int readDifficultyChoice();

// Reads the arrow keys, W, A, S, D and Q. Does not wait for a key.
// Returns a DIRECTION_ constant from Maze.h, DIRECTION_NONE or COMMAND_QUIT.
int readDirection();

// Draws the maze from the top row to the bottom row in the colors of the difficulty level.
// Cells that are hidden by the fog are not drawn.
void drawMaze(char** maze, int size, int playerRow, int playerCol, int fogRadius, int difficulty);

// Draws the player at its position.
void drawPlayer(int size, int playerRow, int playerCol, int difficulty);

// Explains the icons used on the map, in the colors of the difficulty level.
void drawLegend(int difficulty);

// Draws the player's row, column and number of moves.
void drawPlayerInfo(int playerRow, int playerCol, int moveCount);

// Draws the inventory: how many keys the player has.
void drawInventory(int keyCount);

// Describes the cell at (row, col): wall, open cell, exit or outside the maze.
void drawCellInfo(char** maze, int size, int row, int col);

// Draws the time of the game as minutes and seconds.
void drawTimer(int seconds);

// Congratulates the player and shows the moves, the time, the score and the best score.
void drawWinScreen(int moveCount, int seconds, int score, int bestScore);

// Draws PASS or FAIL for every test. results has TEST_COUNT elements.
void drawTestScreen(const bool results[]);