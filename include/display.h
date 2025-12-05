#ifndef DISPLAY_H
#define DISPLAY_H
#include "definitions.h"
#include <stdlib.h>
 #include <string.h>
#include <stdio.h> 
void printQuotes(FILE *configuration);
void printCapturedPieces(Game *game);
void printGameState(Game *game);
void printBoard(Game *game);
void clearScreen(void);
void pause(void);
void displayHelp(void);
void displayWelcome(void);

#endif