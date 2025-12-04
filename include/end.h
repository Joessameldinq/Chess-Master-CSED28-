#ifndef END_H
#define END_H

#include "definitions.h"
#include <stdbool.h>
Position findKingPosition(Game *game, Color color);
bool inCheck(Game *game);
bool inCheckMate(Game *game);

GameStatus computeGameStatus(Game *game);


#endif