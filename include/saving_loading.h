#ifndef SAVE_LOAD_H
#define SAVE_LOAD_H

#include "../include/stack.h"
#include <stdio.h>
#include <stdbool.h>

bool undoMove(Node **gameStack , Node **redoStack);
bool redoMove(Node **gameSatck, Node **redoStack);
bool loadGame(FILE *configuration,Game *game);
bool saveGame(FILE *configuration,Game *game);


#endif