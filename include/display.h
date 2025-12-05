#ifndef DISPLAY_H
#define DISPLAY_H
#include "definitions.h"

//void printQuotes(); will be given a pointer to configuration file
void printCapturedPieces(Game *game);
void printGameState(Game *game);
void printBoard(Game *game);
void clearScreen(void);
void pause(void);
void displayHelp(void);
void displayWelcome(void);

#endif