#include <stdbool.h>
#include "../include/definitions.h"
#include "../include/move.h"


// Find the king belonging to a given color
Position findKingPosition(Game *game, Color color)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            Piece p = game->board[i][j];
            if (p.type == KING && p.color == color)
                return (Position){ .x = i, .y = j };
        }
    }
    return (Position){ .x = -1, .y = -1 }; // Should never happen
}

// Checks if the current player's king is in check
bool inCheck(Game *game)
{
    Color kingColor = game->currentPlayer;
    Position kingPos = findKingPosition(game, kingColor);

    return isSquareAttacked(game, kingPos);
}
bool inCheckMate(Game *game)
{
    if (!inCheck(game))
        return false;

    Color color = game->currentPlayer;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            Piece piece = game->board[i][j];
            if (piece.type == EMPTY || piece.color != color) continue;

            for (int x = 0; x < BOARD_SIZE; x++)
            {
                for (int y = 0; y < BOARD_SIZE; y++)
                {
                    Move mv = { .initial = {i,j}, .final = {x,y} };
                    if (!isValidMove(game, mv)) continue;

                    if (!simulateMoveAndShowIfInCheck(game, &mv))
                        return false; // found a legal move to escape check
                }
            }
        }
    }

    return true; // no escape -> checkmate
}
GameStatus computeGameStatus(Game *game)
{
    if (inCheckMate(game)) return CHECKMATE;
    if (inStaleMate(game)) return STALEMATE;
    return inCheck(game) ? CHECK : PLAYING;
}


bool isStalemate(Game *game)
{
    if (inCheck(game))
        return false; // king in check -> not stalemate

    Color color = game->currentPlayer;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            Piece piece = game->board[i][j];
            if (piece.type == EMPTY || piece.color != color) continue;

            for (int x = 0; x < BOARD_SIZE; x++)
            {
                for (int y = 0; y < BOARD_SIZE; y++)
                {
                    Move mv = { .initial = {i,j}, .final = {x,y} };
                    if (!isValidMove(game, mv)) continue;

                    if (!simulateMoveAndShowIfInCheck(game, &mv))
                        return false; // found a legal move -> not stalemate
                }
            }
        }
    }

    return true; // no legal moves, king not in check → stalemate
}
