#include "../include/definitions.h"
#include "../include/game.h"
#include "../include/display.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>

bool isPathClear(Game *game,Move move)
{
    if (move.initial.x == move.final.x && move.initial.y == move.final.y)
        return true; // no path to clear
    short int rowDirection = move.final.x - move.initial.x;
    rowDirection = (rowDirection>0) ? +1 : (rowDirection<0) ? -1 : 0;
    short int colDirection = move.final.y - move.initial.y;
    colDirection = (colDirection>0) ? +1 : (colDirection<0) ? -1 : 0;

    short int currentRow = move.initial.x + rowDirection;
    short int currentCol = move.initial.y + colDirection;
    Piece temp;

    while(currentCol != move.final.y || currentRow != move.final.x)
    {
        temp = game->board[currentRow][currentCol];
        if(temp.type != EMPTY)
            return false;
        currentCol += colDirection;
        currentRow += rowDirection;
    }

    return true; // Path is clear

}
bool isValidPawn(Game *game,Piece piece,Move move)
{
if (piece.color != game->currentPlayer)        return false; // Try to move enemy piece
    short int  dir = piece.color == WHITE ? -1 : +1;
    short int rowDirection = move.final.x - move.initial.x;
    short int colDirection = move.final.y - move.initial.y;

    if(rowDirection ==  0&& colDirection == 0) 
        return false; // piece don't move
    Piece dest = game->board[move.final.x][move.final.y];
    if(dest.color== game->currentPlayer)
        return false;

    //Single step
    if(rowDirection == dir && dest.type == EMPTY&& colDirection == 0)
        return true;
    
    //Double step
    if((rowDirection == 2 * dir) && (dest.type == EMPTY) && (colDirection == 0) && (!piece.hasMoved) )
        {
            Piece  passed = game->board[move.initial.x + dir][move.initial.y];
            if(passed.type == EMPTY)
            return true;
        }

    //Capture and Enpassent
    if (abs(colDirection) == 1 && rowDirection == dir) {
            // Normal capture
            if (dest.type != EMPTY && dest.color != piece.color) 
                return true;
            // En passant
            if (dest.type == EMPTY && game->enPassentAvailable &&
                game->enPassentTarget.x == move.final.x && game->enPassentTarget.y == move.final.y)
                return true;
    }
    return false;
}
bool isValidRookMove(Game *game,Piece piece,Move move)
{
if (piece.color != game->currentPlayer)        return false;
    //Moves horizontally and vertically
    short int rowDirection = move.final.x - move.initial.x;
    short int colDirection = move.final.y - move.initial.y;
    Piece dest = game->board[move.final.x][move.final.y];
    if(dest.color== game->currentPlayer)
        return false;
    if(rowDirection ==  0&& colDirection == 0) 
        return false; // piece don't move    
    if((rowDirection == 0 || colDirection == 0) && isPathClear(game,move))
        return true;

    return false;
}
bool isValidBishop(Game *game,Piece piece,Move move)
{
if (piece.color != game->currentPlayer)        return false;
    //Moves Diagonally
    short int rowDirection = move.final.x - move.initial.x;
    short int colDirection = move.final.y - move.initial.y;
    Piece dest = game->board[move.final.x][move.final.y];
    if(dest.color== game->currentPlayer)
        return false;
    if(rowDirection ==  0&& colDirection == 0) 
        return false; // piece don't move    

    if (abs(rowDirection) == abs(colDirection) && isPathClear(game,move));
        return true;

    return false;
    
}
bool isValidKnight(Game *game,Piece piece,Move move)
{
    //Moves in L shape
    //Path shouldn't be clear
if (piece.color != game->currentPlayer)        return false;
    short int rowDirection = move.final.x - move.initial.x;
    short int colDirection = move.final.y - move.initial.y;
    Piece dest = game->board[move.final.x][move.final.y];
    if(dest.color== game->currentPlayer)
        return false;
    if(rowDirection ==  0&& colDirection == 0) 
        return false; // piece don't move    

    if (abs(rowDirection) == 2 && abs(colDirection) == 1)
        return true;
    if(abs(rowDirection) == 1 && abs(colDirection) == 2)
        return true;
    return false;

    
}
bool isValidQueen(Game *game,Piece piece,Move move)
{
    return (isValidBishop(game,piece,move) || isValidRookMove(game,piece,move));
    
}
bool isValidCastling(Game *game,Piece piece,Move move)
{
    
if (piece.color != game->currentPlayer)        return false;
    short int rowDirection = move.final.x - move.initial.x;
    short int colDirection = move.final.y - move.initial.y;
    if(rowDirection ==  0&& colDirection == 0) 
        return false; // piece don't move 
    if(abs(colDirection) != 2 || abs(rowDirection)!=0)
        return false; // Must be king moving exactly 2 squares horizontally

    //King mustn't have moved
    if(piece.hasMoved)
        return false;
    

    //Determine king side or queen side
    bool kingSide = (colDirection>0) ? true:false;
    Piece rook = kingSide? game->board[move.initial.x][7] : game->board[move.initial.x][0];
    // Check rook exists and hasn't moved
    if(rook.type != ROOK || rook.hasMoved == true || rook.color != piece.color)
        return false;

    // Check squares between king and rook are empty
    if(kingSide)
    {
        if((game->board[move.initial.x][5].type != EMPTY) || (game->board[move.initial.x][6].type != EMPTY))
            return false;
    }else
    {
        if((game->board[move.initial.x][1].type != EMPTY) || (game->board[move.initial.x][2].type != EMPTY) || ( game->board[move.initial.x][3].type != EMPTY))
            return false;
    }
    // King must not be in check

    
    // if inCheck(Game *game)
    //     return false;
    
    // King must not pass through check
    // FOR each square king passes through:
    //     IF square_is_under_attack(board, square, enemy_color):
    //         RETURN FALSE
    
    // King must not land in check (checked later in validation)

    return true;
    
}
bool isValidKing(Game *game, Piece piece, Move move)
{
    if (piece.color != game->currentPlayer) return false;
    if (move.initial.x == move.final.x && move.initial.y == move.final.y) return false;

    Piece dest = game->board[move.final.x][move.final.y];
    if (dest.color == game->currentPlayer) return false;

    int dx = abs(move.final.x - move.initial.x);
    int dy = abs(move.final.y - move.initial.y);

    if (dx <= 1 && dy <= 1)
        return true;

    if (isValidCastling(game, piece, move))
        return true;

    return false;  
}
