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



//Check if piece can move to pos only geometrically
bool canPieceMoveTo(Game *game, Piece piece, Move move)
{
    short int rowDirection = move.final.x - move.initial.x;
    short int colDirection = move.final.y - move.initial.y;

    //If final == initial
    if (rowDirection == 0 && colDirection == 0)
        return false;

    Piece dest = game->board[move.final.x][move.final.y];
    
    // Can't capture own piece
    if (dest.type != EMPTY && dest.color == piece.color)
        return false;

    //Test all pieces 
    switch (piece.type)
    {
        case PAWN: {
            //White pices move up and black pieces moves down
            short int dir = piece.color == WHITE ? -1 : +1;
            
            // Single step forward
            if (rowDirection == dir && colDirection == 0 && dest.type == EMPTY)
                return true;
            
            // Double step from starting rank
            //The pawn shouldn't have moved before
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
            //L shape move 2 h 1 v or 2 v 1 h
            return (abs(rowDirection) == 2 && abs(colDirection) == 1) ||
                   (abs(rowDirection) == 1 && abs(colDirection) == 2);
        
        case BISHOP:
            //Diagonal move 
            return abs(rowDirection) == abs(colDirection) && isPathClear(game, move);
        
        case ROOK:
            //Straight move only change in row or col
            return (rowDirection == 0 || colDirection == 0) && isPathClear(game, move);
        
        case QUEEN:
            //Mix the rules of bishop and rook
            return ((rowDirection == 0 || colDirection == 0) || 
                    (abs(rowDirection) == abs(colDirection))) && isPathClear(game, move);
        
        case KING:
            //Move to adjacent squares
            return abs(rowDirection) <= 1 && abs(colDirection) <= 1;
        
        default:
            return false;
    }
}
//Find the count of captured pieces to push to capturedPieces Arrays
int findFirstEmptyCapturedSlot(Piece arr[]) {
    for (int i = 0; i < 16; ++i) if (arr[i].type == EMPTY) return i;
    return -1; // Full
}
bool simulateMoveAndShowIfInCheck(Game *game, Move *move)
{
    // Allocate backup on HEAP
    //If in stack it might make stack overflow
    //Backup must be freed
    //Backup is used to undo the move
    Game *backup = (Game *)malloc(sizeof(Game));
    if (!backup) {
        fprintf(stderr, "ERROR: Memory allocation for game backub failed\n");
        return true;
    }
    //as the move is changed after applyMove function we should keep it's backup
    Move backupMove = *move;
    // Copy entire game state
    //Memcpy copy n bytes from src ---> destination
    memcpy(backup, game, sizeof(Game));
    applyMove(game,move);
    //Return the original move 
    *move = backupMove;
    // //  Manually make the move 
    // Position from = move->initial;
    // Position to = move->final;
    
    // // Simple move (good enough for check detection)
    //I Replaced it with applyMove now
    // game->board[to.x][to.y] = game->board[from.x][from.y];
    // game->board[from.x][from.y] = (Piece){.type = EMPTY, .color = NONE, .hasMoved = false};
    
    // Check if the current king is in check
    //for example we moved white so the next turn will be black and  i want to check after moving white is white king in check so we should change the turn
    //kingInCheck (Recommended fix) later i will think of giving inCheck color parameter 
    game->currentPlayer = (game->currentPlayer == WHITE) ? BLACK:WHITE;
    bool kingInCheck = inCheck(game);
    
    // Restore original game state and free backup
    memcpy(game, backup, sizeof(Game));
    free(backup);
    
    return kingInCheck;
}
/* Check whether every square between initial (exclusive) and final (exclusive)
   is empty. Works for straight and diagonal directions. */
bool isPathClear(Game *game, Move move)
{
    if (move.initial.x == move.final.x && move.initial.y == move.final.y)
        return true; // no path to clear initial == final

    short int rowDirection = move.final.x - move.initial.x;
    rowDirection = (rowDirection > 0) ? +1 : (rowDirection < 0) ? -1 : 0; //Find the unit rowDirection (Greater than absoulte function as it might cause zerodivision error)
    short int colDirection = move.final.y - move.initial.y;
    colDirection = (colDirection > 0) ? +1 : (colDirection < 0) ? -1 : 0;//Find the unit colDirection (Greater than absoulte function as it might cause zerodivision error)

    short int currentRow = move.initial.x + rowDirection;
    short int currentCol = move.initial.y + colDirection;
    //Iterate untill we reach the final position
    //It doesn't check boundaries as it's guaranteed that there is a valid path 
    while (currentRow != move.final.x || currentCol != move.final.y)
    {
        Piece temp = game->board[currentRow][currentCol];
        //If piece found at the path direction
        if (temp.type != EMPTY)
            return false;
        currentRow += rowDirection;
        currentCol += colDirection;
    }

    return true; // Path is clear
}

//Check if it's valid pawn movement
//It check single step , double step and enpassant capture and diagonal capture
//Pawn promotion is just the same as single step
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
/*  It ignores king safety for the moved piece*/
bool isSquareAttacked(Game *game, Position pos)
{
    //Enemy color is the opposite of current turn player
    Color enemy = (game->currentPlayer == WHITE) ? BLACK : WHITE;
    //Iterate on all board pieces
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            //check if there is enemy piece at position i,j
            Piece enemyP = game->board[i][j];
            if (enemyP.type == EMPTY || enemyP.color != enemy) 
                continue;

            //Make facke move from i,j to the passed position to the function
            Move fake = { .initial = {i, j}, .final = pos };

            // Check if piece can legally move there (ignores king safety for now)
            if (!canPieceMoveTo(game, enemyP, fake)) 
                continue;

            return true;
        }
    }

    return false;
}
// piece-specific movement rules from square to another (does NOT check king safety)
// Large switch replacement
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
//The last step before make move
//So it can be used in simulate moves or applying moves
//But we shouldn't use it in simulate move as it will lead to infinite recursion
//So in simulate moves we manually apply move --> check if king isInCheck ---> redo the move and game state
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
    // Only geometric check
    if (!isLegalMove(game, from, move)) return false;

    // Now: simulate the move and ensure our king is not left in check
    // Apply the move --> check the king state --> redo the move
    bool kingInCheckAfterMove = simulateMoveAndShowIfInCheck(game,&move);
    // If the king is in check after making the move ----> move is illegal
    if (kingInCheckAfterMove) return false;

    return true;
}
// This function doesn't check if the movid valid or no
// It just apply the move
// So isValidMove should call and if returns true we apply the move
void applyMove(Game *game, Move *move)
{
    // As only one of the special moves can happen
    // Vary rare condition is pawnpromotion + capture
    setFlagsFalse(game);
    Position from = move->initial;
    Position to   = move->final;
    Piece mover = game->board[from.x][from.y];
    Piece dest  = game->board[to.x][to.y];

    // Save move info
    // We change it later 
    // The passed move to the function has only two data
    // The initial and final position
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
    // Pawn move diagonally to a position that was just passed from the enemy pawn who moved 2 steps
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
            rook.hasMoved = true; //We changed the moved piece out this block so only remaining is the rook to be changed
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
            if(dest.type != EMPTY && dest.color != mover.color)
                move->moveType = CAPTURE_AND_PAWN_PROMOTION;
            else
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

    // 8) Toggle current player
    game->currentPlayer = (game->currentPlayer == WHITE) ? BLACK : WHITE;

    // // 10) Update status
    // game->status = computeGameStatus(game);
    // Will be called in main as compute gameStatus will make stack overflow
}
void setFlagsFalse(Game *game)
{
    game->currentFlag.pawnPromotionMade = false;
    game->currentFlag.castlingMade = false;
    game->currentFlag.enpassentMade =false;
}
