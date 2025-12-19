


#include <stdbool.h>
#include "../include/GameDefinitions.h"
#include "../include/MovingLogic.h"
#include "../include/EndingConditions.h"


// Find the king belonging to a given color
Position findKingPosition(Game *game, Color color) //Find the position of king of specific color
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
bool inCheck(Game *game) //detect if the king of current game turn is attacked by enemy
{
    Color kingColor = game->currentPlayer;
    Position kingPos = findKingPosition(game, kingColor);

    return isSquareAttacked(game, kingPos); //must answer ONE question only: Is the king's square currently attacked by any enemy piece? Only geometrically
    //It doesn't matter wheter the attack leave the other king in check or not
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

                    // Only consider legal moves and king safety
                    if (!isValidMove(game, mv))
                        continue;
                    else
                        return false;
                }
            }
        }
    }

    return true; // No legal moves to escape check  -----> checkmate
}
GameStatus computeGameStatus(Game *game)
{
    if (inCheckMate(game)) return CHECKMATE;
    if (isStalemate(game)) return STALEMATE;
    if(isDeadPosition(game))return DRAW_INSUFFICIENT_MATERIAL;
    if(fiftyMovesRule(game)) return DRAW_FIFTY_MOVE;
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

                    if (!canPieceMoveTo(game, piece,mv))
                        continue;

                    // If any legal move exists --> not stalemate
                    if (!simulateMoveAndShowIfInCheck(*game, mv))
                        return false;
                }
            }
        }
    }

    return true; // No legal moves and king is not in check  -----> stalemate
}
bool fiftyMovesRule(Game *game)
{
    if(game->halfMoveClock >= 100)
    {
        game->status = DRAW_FIFTY_MOVE;
        return true;
    }
    return false;

}
Color getSquareColor(int row,int col)
{
    return ((row+col) %2 == 0 )? WHITE:BLACK;
}
//check for suffecient pieces for the player to win if both doesnt have that suffecient pieces we declare a draw.
bool isDeadPosition(Game *game)
{
    short int knightW = 0, knightB = 0, bishopW = 0, bishopB = 0;
    short int whiteBishopOnWhiteSquares = 0, whiteBishopOnBlackSquares = 0;
    short int blackBishopOnWhiteSquares = 0, blackBishopOnBlackSquares = 0;  // ✓ Fixed
    
    for(int i = 0; i < BOARD_SIZE; i++)
    {
        for(int j = 0; j < BOARD_SIZE; j++)
        {
            Piece p = game->board[i][j];
            if(p.type == PAWN || p.type == QUEEN || p.type == ROOK)
                return false; // Sufficient material
            else if(p.type == KNIGHT)
            {
                if(p.color == WHITE)
                    knightW++;
                else
                    knightB++;
            }
            else if(p.type == BISHOP)
            {
                if(getSquareColor(i,j) == WHITE && p.color == WHITE)
                {
                    bishopW++;
                    whiteBishopOnWhiteSquares++;
                }
                else if(getSquareColor(i,j) == WHITE && p.color == BLACK)
                {
                    bishopB++;
                    blackBishopOnWhiteSquares++;
                }
                else if(getSquareColor(i,j) == BLACK && p.color == WHITE)
                {
                    bishopW++;
                    whiteBishopOnBlackSquares++;
                }
                else
                {
                    bishopB++;
                    blackBishopOnBlackSquares++;
                }
            }
        }
    }
    
    short int totalKnights = knightW + knightB;
    short int totalBishops = bishopW + bishopB;
    short int totalMinorPieces = totalBishops + totalKnights;
    
    // 1. King vs King
    if(totalMinorPieces == 0)
        return true;
    
    // 2. (King + Bishop) vs King
    if(totalBishops == 1 && totalKnights == 0)
        return true;
    
    // 3. (King + Knight) vs King
    if(totalKnights == 1 && totalBishops == 0)
        return true;
    
    // 4. (King + Bishop) vs (King + Bishop) (same colored squares)
    //  both bishops on same color squares
    if(totalBishops == 2 && totalKnights == 0 && bishopW == 1 && bishopB == 1)
    {
        if((whiteBishopOnWhiteSquares == 1 && blackBishopOnWhiteSquares == 1) || 
        (whiteBishopOnBlackSquares == 1 && blackBishopOnBlackSquares == 1))
            return true;
    }
    
    // 5. (King + Knight) vs (King + Knight)
    // i found that this case can end with checkmate and not a dead position under the FIDE rules
    //While extremely rare, checkmate is possible with K+N vs K+N through forced moves. This should not be a dead position according to FIDE rules.
    // if(knightW == 1 && knightB == 1 && totalBishops == 0)
    //     return true;
    
    // 6. (King + Knight + Knight) vs King
    if(totalKnights == 2 && totalBishops == 0)
    {
        if((knightB == 2 && knightW == 0) || (knightW == 2 && knightB == 0))
            return true;
    }
    
    // 7. (King + Bishop(s)) vs (King + Bishop(s)) (all same color)
    if(totalBishops >= 2 && totalKnights == 0)
    {
        if((totalBishops == whiteBishopOnWhiteSquares + blackBishopOnWhiteSquares) || 
           (totalBishops == whiteBishopOnBlackSquares + blackBishopOnBlackSquares))
            return true;
    }
    
    // 8. (King + Knight(s)) vs (King + Knight(s))
    // After watching some games i found that this case can lead to a dead position especially when KNN vs KN
    //With multiple knights, checkmate becomes increasingly possible
    // if(totalBishops == 0 && totalKnights >= 2 && knightW != 0 && knightB != 0)
        // return true;
    
    // 9. (King + Bishop(s)) (same color) vs (King + Knight(s))
    //A bishop and knight can work together to deliver checkmate, even against knights.
    // if((bishopW > 0 && bishopB == 0 && knightB > 0 && knightW == 0) || 
    //    (bishopB > 0 && bishopW == 0 && knightW > 0 && knightB == 0))
    // {
    //     // All bishops must be on same color
    //     if((totalBishops == whiteBishopOnWhiteSquares + blackBishopOnWhiteSquares) ||
    //        (totalBishops == whiteBishopOnBlackSquares + blackBishopOnBlackSquares))
    //         return true;
    // }
    
    return false;
}