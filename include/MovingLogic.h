#ifndef MOVE_H
#define MOVE_H

#include "GameDefinitions.h"
#include <stdbool.h>

//A struct for backup   i will use it in simulateMove
/*
==================
*/

void setFlagsFalse(Game *game);
bool isPathClear(Game *game,Move move);
bool isValidPawn(Game *game,Piece piece,Move move);
bool isValidRook(Game *game,Piece piece,Move move);
bool isValidBishop(Game *game,Piece piece,Move move);
bool isValidKnight(Game *game,Piece piece,Move move);
bool isValidQueen(Game *game,Piece piece,Move move);
bool isValidCastling(Game *game,Piece piece,Move move);
bool isValidKing(Game *game, Piece piece, Move move);
bool isSquareAttacked(Game *game, Position pos);
void applyMove(Game *game, Move *move);
bool isValidMove(Game *game, Move move);
bool isLegalMove(Game *game, Piece piece, Move move);
int findFirstEmptyCapturedSlot(Piece arr[]);
bool simulateMoveAndShowIfInCheck(Game game, Move move);
bool canPieceMoveTo(Game *game, Piece piece, Move move);
#endif
