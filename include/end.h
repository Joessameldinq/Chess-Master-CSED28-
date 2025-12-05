#ifndef END_H
#define END_H

#include "definitions.h"
#include <stdbool.h>
Position findKingPosition(Game *game, Color color);
bool inCheck(Game *game);
bool inCheckMate(Game *game);
bool isDeadPosition(Game *game);
Color getSquareColor(int row,int col);
GameStatus computeGameStatus(Game *game);
bool fiftyMovesRule(Game *game);
bool isStalemate(Game *game);
#endif