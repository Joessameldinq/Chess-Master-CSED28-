#include "../include/definitions.h"
#include "../include/game.h"
#include "../include/display.h"
#include "../include/end.h"
#include "../include/move.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




bool canPieceMoveTo(Game *game, Piece piece, Move move)
{
    short int rowDirection = move.final.x - move.initial.x;
    short int colDirection = move.final.y - move.initial.y;

    if (rowDirection == 0 && colDirection == 0)
        return false;

    Piece dest = game->board[move.final.x][move.final.y];
    
    // Can't capture own piece
    if (dest.type != EMPTY && dest.color == piece.color)
        return false;

    switch (piece.type)
    {
        case PAWN: {
            short int dir = piece.color == WHITE ? -1 : +1;
            
            // Single step forward
            if (rowDirection == dir && colDirection == 0 && dest.type == EMPTY)
                return true;
            
            // Double step from starting rank
            if (rowDirection == 2 * dir && colDirection == 0 && dest.type == EMPTY && !piece.hasMoved)
            {
                Piece passed = game->board[move.initial.x + dir][move.initial.y];
                if (passed.type == EMPTY)
                    return true;
            }
            
            // Diagonal capture (including en passant square)
            if (abs(colDirection) == 1 && rowDirection == dir)
            {
                // Normal capture
                if (dest.type != EMPTY && dest.color != piece.color)
                    return true;
                
                // En passant
                if (dest.type == EMPTY && game->enPassentAvailable &&
                    game->enPassentTarget.x == move.final.x && 
                    game->enPassentTarget.y == move.final.y)
                    return true;
            }
            return false;
        }
        
        case KNIGHT:
            return (abs(rowDirection) == 2 && abs(colDirection) == 1) ||
                   (abs(rowDirection) == 1 && abs(colDirection) == 2);
        
        case BISHOP:
            return abs(rowDirection) == abs(colDirection) && isPathClear(game, move);
        
        case ROOK:
            return (rowDirection == 0 || colDirection == 0) && isPathClear(game, move);
        
        case QUEEN:
            return ((rowDirection == 0 || colDirection == 0) || 
                    (abs(rowDirection) == abs(colDirection))) && isPathClear(game, move);
        
        case KING:
            return abs(rowDirection) <= 1 && abs(colDirection) <= 1;
        
        default:
            return false;
    }
}
int findFirstEmptyCapturedSlot(Piece arr[8]) {
    for (int i = 0; i < 8; ++i) if (arr[i].type == EMPTY) return i;
    return -1; // Full
}
bool simulateMoveAndShowIfInCheck(Game *game, Move *move)
{
    // Allocate backup on HEAP
    Game *backup = (Game *)malloc(sizeof(Game));
    if (!backup) {
        fprintf(stderr, "ERROR: Memory allocation failed\n");
        return true;
    }
    Move backupMove = *move;
    // Copy entire game state
    memcpy(backup, game, sizeof(Game));
    applyMove(game,move);
    *move = backupMove;
    // //  Manually make the move 
    // Position from = move->initial;
    // Position to = move->final;
    
    // // Simple move (good enough for check detection)
    // game->board[to.x][to.y] = game->board[from.x][from.y];
    // game->board[from.x][from.y] = (Piece){.type = EMPTY, .color = NONE, .hasMoved = false};
    
    // Check if the current king is in check
    game->currentPlayer = (game->currentPlayer == WHITE) ? BLACK:WHITE;
    bool kingInCheck = inCheck(game);
    
    // Restore original game state
    memcpy(game, backup, sizeof(Game));
    free(backup);
    
    return kingInCheck;
}
/* Check whether every square between initial (exclusive) and final (exclusive)
   is empty. Works for straight and diagonal directions. */
bool isPathClear(Game *game, Move move)
{
    if (move.initial.x == move.final.x && move.initial.y == move.final.y)
        return true; // no path to clear

    short int rowDirection = move.final.x - move.initial.x;
    rowDirection = (rowDirection > 0) ? +1 : (rowDirection < 0) ? -1 : 0;
    short int colDirection = move.final.y - move.initial.y;
    colDirection = (colDirection > 0) ? +1 : (colDirection < 0) ? -1 : 0;

    short int currentRow = move.initial.x + rowDirection;
    short int currentCol = move.initial.y + colDirection;

    while (currentRow != move.final.x || currentCol != move.final.y)
    {
        Piece temp = game->board[currentRow][currentCol];
        if (temp.type != EMPTY)
            return false;
        currentRow += rowDirection;
        currentCol += colDirection;
    }

    return true; // Path is clear
}

bool isValidPawn(Game *game, Piece piece, Move move)
{
    short int dir = piece.color == WHITE ? -1 : +1;
    short int rowDirection = move.final.x - move.initial.x;
    short int colDirection = move.final.y - move.initial.y;

    if (rowDirection == 0 && colDirection == 0)
        return false; // piece doesn't move

    Piece dest = game->board[move.final.x][move.final.y];

    // Prevent capturing own piece
    if (dest.type != EMPTY && dest.color == piece.color)
        return false;

    // Single step forward
    if (rowDirection == dir && colDirection == 0 && dest.type == EMPTY)
        return true;

    // Double step from starting rank
    if (rowDirection == 2 * dir && colDirection == 0 && dest.type == EMPTY && !piece.hasMoved)
    {
        Piece passed = game->board[move.initial.x + dir][move.initial.y];
        if (passed.type == EMPTY)
            return true;
    }

    // Capture or en-passant (pawn attacks only diagonally)
    if (abs(colDirection) == 1 && rowDirection == dir)
    {
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

bool isValidRook(Game *game, Piece piece, Move move)
{
    short int rowDirection = move.final.x - move.initial.x;
    short int colDirection = move.final.y - move.initial.y;

    if (rowDirection == 0 && colDirection == 0)
        return false; // piece doesn't move

    Piece dest = game->board[move.final.x][move.final.y];
    // Prevent capturing own piece
    if (dest.type != EMPTY && dest.color == piece.color)
        return false;

    if ((rowDirection == 0 || colDirection == 0) && isPathClear(game, move))
        return true;

    return false;
}

bool isValidBishop(Game *game, Piece piece, Move move)
{
    short int rowDirection = move.final.x - move.initial.x;
    short int colDirection = move.final.y - move.initial.y;

    if (rowDirection == 0 && colDirection == 0)
        return false; // piece doesn't move

    Piece dest = game->board[move.final.x][move.final.y];
    // Prevent capturing own piece
    if (dest.type != EMPTY && dest.color == piece.color)
        return false;

    if (abs(rowDirection) == abs(colDirection) && isPathClear(game, move))
        return true;

    return false;
}

bool isValidKnight(Game *game, Piece piece, Move move)
{
    short int rowDirection = move.final.x - move.initial.x;
    short int colDirection = move.final.y - move.initial.y;

    if (rowDirection == 0 && colDirection == 0)
        return false; // piece doesn't move

    Piece dest = game->board[move.final.x][move.final.y];
    // Prevent capturing own piece
    if (dest.type != EMPTY && dest.color == piece.color)
        return false;

    if ((abs(rowDirection) == 2 && abs(colDirection) == 1) ||
        (abs(rowDirection) == 1 && abs(colDirection) == 2))
        return true;

    return false;
}

bool isValidQueen(Game *game, Piece piece, Move move)
{
    return (isValidBishop(game, piece, move) || isValidRook(game, piece, move));
}

bool isValidCastling(Game *game, Piece piece, Move move)
{
    if (inCheck(game))
        return false; // king currently in check

    short int row = move.initial.x;
    short int colDiff = move.final.y - move.initial.y;

    if (abs(colDiff) != 2 || move.final.x != move.initial.x)
        return false; // king must move 2 squares horizontally

    if (piece.hasMoved)
        return false;

    bool kingSide = (colDiff > 0);
    Piece rook = kingSide ? game->board[row][7] : game->board[row][0];

    if (rook.type != ROOK || rook.hasMoved || rook.color != piece.color)
        return false;

    // Check squares between king and rook are empty
    if (kingSide) {
        if (game->board[row][5].type != EMPTY || game->board[row][6].type != EMPTY)
            return false;
    } else {
        if (game->board[row][1].type != EMPTY || game->board[row][2].type != EMPTY || game->board[row][3].type != EMPTY)
            return false;
    }

    // Check squares king passes through are not attacked
    int kingSteps = 2; // king always moves 2 squares
    int dir = kingSide ? 1 : -1;
    for (int i = 1; i <= kingSteps; i++) {
        Position pos = { .x = row, .y = move.initial.y + dir * i };
        if (isSquareAttacked(game, pos))
            return false;
    }

    return true;
}

bool isValidKing(Game *game, Piece piece, Move move)
{
    if (move.initial.x == move.final.x && move.initial.y == move.final.y)
        return false;

    Piece dest = game->board[move.final.x][move.final.y];
    // Prevent capturing own piece
    if (dest.type != EMPTY && dest.color == piece.color)
        return false;

    int dx = abs(move.final.x - move.initial.x);
    int dy = abs(move.final.y - move.initial.y);

    if (dx <= 1 && dy <= 1)
        return true;

    if (isValidCastling(game, piece, move))
        return true;

    return false;
}

/* Returns true if 'pos' is attacked by the opponent of currentPlayer. */
bool isSquareAttacked(Game *game, Position pos)
{
    Color enemy = (game->currentPlayer == WHITE) ? BLACK : WHITE;

    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            Piece enemyP = game->board[i][j];
            if (enemyP.type == EMPTY || enemyP.color != enemy) 
                continue;

            Move fake = { .initial = {i, j}, .final = pos };

            // Check if piece can legally move there (ignores king safety for now)
            if (!canPieceMoveTo(game, enemyP, fake)) 
                continue;

            return true;
        }
    }

    return false;
}
bool isLegalMove(Game *game, Piece piece, Move move)
{
    switch (piece.type)
    {
        case PAWN:   return isValidPawn(game, piece, move);
        case KNIGHT: return isValidKnight(game, piece, move);
        case BISHOP: return isValidBishop(game, piece, move);
        case ROOK:   return isValidRook(game, piece, move);
        case QUEEN:  return isValidQueen(game, piece, move);
        case KING:   return isValidKing(game, piece, move);
        default:     return false;
    }
}
bool isValidMove(Game *game, Move move)
{
    // Boundaries
    if (move.initial.x < 0 || move.initial.x >= BOARD_SIZE ||
        move.initial.y < 0 || move.initial.y >= BOARD_SIZE ||
        move.final.x   < 0 || move.final.x   >= BOARD_SIZE ||
        move.final.y   < 0 || move.final.y   >= BOARD_SIZE)
        return false;

    Piece from = game->board[move.initial.x][move.initial.y];
    Piece to   = game->board[move.final.x][move.final.y];

    // src must not be empty
    if (from.type == EMPTY) return false;

    // must move current player's piece
    if (from.color != game->currentPlayer) return false;

    // can't capture own piece
    if (to.type != EMPTY && to.color == from.color) return false;

    // must actually move
    if (move.initial.x == move.final.x && move.initial.y == move.final.y) return false;

    // piece-specific movement rules (does NOT check king safety)
    if (!isLegalMove(game, from, move)) return false;

    // Now: simulate the move and ensure our king is not left in check
    bool kingInCheckAfterMove =simulateMoveAndShowIfInCheck(game,&move);
    // If the king is in check after making the move → move is illegal
    if (kingInCheckAfterMove) return false;

    return true;
}
void applyMove(Game *game, Move *move)
{
    setFlagsFalse(game);
    Position from = move->initial;
    Position to   = move->final;
    Piece mover = game->board[from.x][from.y];
    Piece dest  = game->board[to.x][to.y];

    // Save move info
    move->moveType = NORMAL_MOVE;
    move->capturedPiece = (Piece){ .type = EMPTY, .color = NONE, .hasMoved = false };

    // Save previous en-passant state to use in en-passant detection
    bool enPassantAvailableBefore = game->enPassentAvailable;
    Position enPassantTargetBefore = game->enPassentTarget;

    // Reset en-passant for next turn
    game->enPassentAvailable = false;
    game->enPassentTarget = (Position){-1,-1};
    Piece capturedInEnpassent;
    // 1) En-passant capture detection
    if (mover.type == PAWN && from.y != to.y && dest.type == EMPTY) {
        if (enPassantAvailableBefore &&
            enPassantTargetBefore.x == to.x &&
            enPassantTargetBefore.y == to.y) 
        {
            // Remove the captured pawn
            int dir = (mover.color == WHITE) ? +1 : -1;
            int capRow = to.x + dir;
            int capCol = to.y;
            
            move->capturedPiece = game->board[capRow][capCol];
            capturedInEnpassent = game->board[capRow][capCol];
            game->board[capRow][capCol] = (Piece){ .type = EMPTY, .color = NONE, .hasMoved = false };

            move->moveType = EN_PASSENT;
            game->currentFlag.enpassentMade = true;
        }
    }

    // 2) Normal capture
    if (dest.type != EMPTY) {
        move->capturedPiece = dest;

        // Add captured piece to array
        if (dest.color == WHITE) {
            int idx = findFirstEmptyCapturedSlot(game->capturedWhitePieces);
            if (idx >= 0) game->capturedWhitePieces[idx] = dest;
        } else {
            int idx = findFirstEmptyCapturedSlot(game->capturedBlackPieces);
            if (idx >= 0) game->capturedBlackPieces[idx] = dest;
        }

        move->moveType = CAPTURE;
    }
    else if(move->moveType == EN_PASSENT)
    {
        // Add captured piece to array
        if (capturedInEnpassent.color == WHITE) {
            int idx = findFirstEmptyCapturedSlot(game->capturedWhitePieces);
            if (idx >= 0) game->capturedWhitePieces[idx] = capturedInEnpassent;
        } else {
            int idx = findFirstEmptyCapturedSlot(game->capturedBlackPieces);
            if (idx >= 0) game->capturedBlackPieces[idx] = capturedInEnpassent;
        }

    }
    

    // 3) Move the piece
    game->board[to.x][to.y] = mover;
    game->board[to.x][to.y].hasMoved = true;
    game->board[from.x][from.y] = (Piece){ .type = EMPTY, .color = NONE, .hasMoved = false };

    // 4) Pawn double-step -> set en-passant for next turn
    if (mover.type == PAWN && abs(to.x - from.x) == 2) {
        game->enPassentAvailable = true;
        game->enPassentTarget = (Position){ .x = (to.x + from.x) / 2,.y= from.y };
    }

    // 5) Castling
    if (mover.type == KING && from.x == to.x && abs(to.y - from.y) == 2) {
        if (to.y - from.y == 2) {
            // King-side
            Piece rook = game->board[from.x][7];
            rook.hasMoved = true;
            game->board[from.x][from.y + 1] = rook;
            game->board[from.x][7] = (Piece){ .type = EMPTY, .color = NONE, .hasMoved = false };
            move->moveType = CASTLE_KINGSIDE;
        } else {
            // Queen-side
            Piece rook = game->board[from.x][0];
            rook.hasMoved = true;
            game->board[from.x][from.y - 1] = rook;
            game->board[from.x][0] = (Piece){ .type = EMPTY, .color = NONE, .hasMoved = false };
            move->moveType = CASTLE_QUEENSIDE;
        }
        game->currentFlag.castlingMade =true;
    }

    // 6) Pawn promotion
    if (mover.type == PAWN) {
        bool promotionRow = (mover.color == WHITE && to.x == 0) || (mover.color == BLACK && to.x == BOARD_SIZE - 1);
        if (promotionRow) {
            //By default is Queen
            PieceType promo = (move->promotionPiece != EMPTY) ? move->promotionPiece : QUEEN;
            game->board[to.x][to.y].type = promo;
            move->moveType = PAWN_PROMOTION;
            game->currentFlag.pawnPromotionMade = true;
        }
    }

    // 7) Half-move clock for 50 move rule
    if (mover.type == PAWN || move->moveType == CAPTURE || move->moveType == EN_PASSENT) {
        game->halfMoveClock = 0;
    } else {
        game->halfMoveClock++;
    }

    // 8) Record move
    // if (game->moveCounter < 500) {
    //     game->moveHistory[game->moveCounter++] = *move;
    // }

    // 9) Toggle current player
    game->currentPlayer = (game->currentPlayer == WHITE) ? BLACK : WHITE;

    // // 10) Update status
    // game->status = computeGameStatus(game);
}
void setFlagsFalse(Game *game)
{
    game->currentFlag.pawnPromotionMade = false;
    game->currentFlag.castlingMade = false;
    game->currentFlag.enpassentMade =false;


}
