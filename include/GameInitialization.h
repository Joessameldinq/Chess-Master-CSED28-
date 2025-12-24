#ifndef GAME_H
#define GAME_H

#include "GameDefinitions.h"

Game* initGame(void);
void initZobristTables(void);
uint64_t random64(void);
int getPieceIndex(Piece p);
uint64_t computePositionHash(Game game);


#endif