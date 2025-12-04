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

    // Try every possible move for the current player
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            Piece piece = game->board[i][j];
            if (piece.type == EMPTY || piece.color != color)
                continue;

            Position from = { .x = i, .y = j };

            for (int x = 0; x < BOARD_SIZE; x++)
            {
                for (int y = 0; y < BOARD_SIZE; y++)
                {
                    Position to = { .x = x, .y = y };
                    Move mv = { .initial = from, .final = to };

                    // Only consider legal moves
                    if (!isValidMove(game, mv))
                        continue;

                    // If king is safe after move  -> not checkmate
                    if (!simulateMoveAndShowIfInCheck(game, &mv))
                        return false;
                }
            }
        }
    }

    return true; // No legal moves to escape check  -> checkmate
}
GameStatus computeGameStatus(Game *game)
{
    if (inCheckMate(game)) return CHECKMATE;
    if (inStaleMate(game)) return STALEMATE;
    return inCheck(game) ? CHECK : PLAYING;
}


// Stalemate logic:
// 1) King is NOT in check
// 2) No legal moves available
bool isStalemate(Game *game)
{
    if (inCheck(game)) // If in check, cannot be stalemate
        return false;

    Color color = game->currentPlayer;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            Piece piece = game->board[i][j];
            if (piece.type == EMPTY || piece.color != color)
                continue;

            Position from = { .x = i, .y = j };

            for (int x = 0; x < BOARD_SIZE; x++)
            {
                for (int y = 0; y < BOARD_SIZE; y++)
                {
                    Position to = { .x = x, .y = y };
                    Move mv = { .initial = from, .final = to };

                    if (!isValidMove(game, mv))
                        continue;

                    // If any legal move exists -> not stalemate
                    if (!simulateMoveAndShowIfInCheck(game, &mv))
                        return false;
                }
            }
        }
    }

    return true; // No legal moves and king is not in check  -> stalemate
}
