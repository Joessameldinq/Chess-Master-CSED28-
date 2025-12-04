#include "../include/definitions.h"
#include "../include/game.h"
#include "../include/display.h"
#include "../include/end.h"
#include <stdbool.h>
#include <math.h>
#include <stdio.h>
int findFirstEmptyCapturedSlot(Piece arr[8]) {
    for (int i = 0; i < 8; ++i) if (arr[i].type == EMPTY) return i;
    return -1;
}
bool simulateMoveAndShowIfInCheck(Game *game, Move *move)
{
    // Save current state
    Game backup = *game;

    // Try making the move on the board
    applyMove(game, move);

    Color sideThatMoved = (game->currentPlayer == WHITE) ? BLACK : WHITE;
    Color savedCurrent = game->currentPlayer;
    game->currentPlayer = sideThatMoved;
    bool kingInCheck = inCheck(game);
    game->currentPlayer = savedCurrent;
    // Restore the original board state
    *game = backup;

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
    short int rowDirection = move.final.x - move.initial.x;
    short int colDirection = move.final.y - move.initial.y;

    if (rowDirection == 0 && colDirection == 0)
        return false; // piece doesn't move

    if (abs(colDirection) != 2 || abs(rowDirection) != 0)
        return false; // must be king moving exactly 2 squares horizontally

    // King mustn't have moved
    if (piece.hasMoved)
        return false;

    bool kingSide = (colDirection > 0);
    Piece rook = kingSide ? game->board[move.initial.x][7] : game->board[move.initial.x][0];

    // Check rook exists and hasn't moved and is same color
    if (rook.type != ROOK || rook.hasMoved == true || rook.color != piece.color)
        return false;

    // Check squares between king and rook are empty
    if (kingSide)
    {
        if ((game->board[move.initial.x][5].type != EMPTY) || (game->board[move.initial.x][6].type != EMPTY))
            return false;
    }
    else
    {
        if ((game->board[move.initial.x][1].type != EMPTY) || (game->board[move.initial.x][2].type != EMPTY) || (game->board[move.initial.x][3].type != EMPTY))
            return false;
    }

    // TODO: check king is not currently in check and that king does not pass through a square under attack
    // e.g. if (isSquareAttacked(game, move.initial)) return false;
    // and check intermediate squares (move.initial.y +/- 1) depending on side

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
            if (enemyP.type == EMPTY || enemyP.color != enemy) continue;

            // Create a move from that enemy piece to pos
            Move fake = { .initial = {i, j}, .final = pos };

            switch (enemyP.type)
            {
                case PAWN:
                {
                    // Pawns attack diagonally only
                    short int dir = (enemyP.color == WHITE) ? -1 : +1;
                    if ((pos.x == i + dir) && (abs(pos.y - j) == 1))
                        return true;
                    break;
                }
                case KNIGHT:
                    if (isValidKnight(game, enemyP, fake)) return true;
                    break;
                case BISHOP:
                    if (isValidBishop(game, enemyP, fake)) return true;
                    break;
                case QUEEN:
                    if (isValidQueen(game, enemyP, fake)) return true;
                    break;
                case ROOK:
                    if (isValidRook(game, enemyP, fake)) return true;
                    break;
                case KING:
                {
                    // king attacks adjacent squares only
                    int dx = abs(pos.x - i);
                    int dy = abs(pos.y - j);
                    if (dx <= 1 && dy <= 1) return true;
                    break;
                }
                default:
                    break;
            }
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
    Position from = move->initial;
    Position to   = move->final;

    Piece mover = game->board[from.x][from.y];
    Piece dest  = game->board[to.x][to.y];

    // Default: set moveType to NORMAL_MOVE, will adjust below.
    move->moveType = NORMAL_MOVE;
    move->capturedPiece = (Piece){ .type = EMPTY, .color = NONE, .hasMoved = false };

    // Reset en-passant availability by default; may be set if pawn double-step occurs
    game->enPassentAvailable = false;
    game->enPassentTarget = (Position){-1,-1};

    // 1) En-passant capture detection:
    // If pawn moved diagonally into empty square and enPassentTarget matches
    if (mover.type == PAWN && from.y != to.y && dest.type == EMPTY) {
        // en-passant capture
        if (game->enPassentAvailable &&
            game->enPassentTarget.x == to.x &&
            game->enPassentTarget.y == to.y) 
        {
            // captured pawn sits on the row that the pawn jumped from
            int dir = (mover.color == WHITE) ? +1 : -1; // captured pawn row is opposite direction
            int capRow = to.x + dir; // the pawn that was captured
            int capCol = to.y;

            // Save captured piece in move record
            move->capturedPiece = game->board[capRow][capCol];

            // Remove captured pawn
            game->board[capRow][capCol].type = EMPTY;
            game->board[capRow][capCol].color = NONE;
            game->board[capRow][capCol].hasMoved = false;

            move->moveType = EN_PASSENT;
        }
    }

    //  
    // 2) Normal capture (if destination occupied)
    //  
    if (dest.type != EMPTY) {
        // Save captured piece in move record
        move->capturedPiece = dest;

        // Add to captured arrays
        if (dest.color == WHITE) {
            int idx = findFirstEmptyCapturedSlot(game->capturedWhitePieces);
            if (idx >= 0) game->capturedWhitePieces[idx] = dest;
        } else if (dest.color == BLACK) {
            int idx = findFirstEmptyCapturedSlot(game->capturedBlackPieces);
            if (idx >= 0) game->capturedBlackPieces[idx] = dest;
        }

        move->moveType = CAPTURE;
    }

    //  
    // 3) Move the piece on board (basic)
    //  
    game->board[to.x][to.y] = mover;
    game->board[to.x][to.y].hasMoved = true;

    // clear source
    game->board[from.x][from.y].type = EMPTY;
    game->board[from.x][from.y].color = NONE;
    game->board[from.x][from.y].hasMoved = false;

    //  
    // 4) Pawn double-step -> enable en-passant target
    //  
    if (mover.type == PAWN && abs(to.x - from.x) == 2) {
        game->enPassentAvailable = true;
        int midRow = (to.x + from.x) / 2;
        game->enPassentTarget = (Position){ midRow, from.y };
    }

    //  
    // 5) Castling: if king moved 2 squares horizontally, move the rook
    //  
    if (mover.type == KING && from.x == to.x && abs(to.y - from.y) == 2) {
        if (to.y - from.y == 2) {
            // king-side castle: rook from col 7 to col from.y+1
            Piece rook = game->board[from.x][7];
            game->board[from.x][from.y + 1] = rook;
            game->board[from.x][7].type = EMPTY;
            game->board[from.x][7].color = NONE;
            game->board[from.x][7].hasMoved = false;
            move->moveType = CASTLE_KINGSIDE;
        } else {
            // queen-side castle: rook from col 0 to col from.y-1
            Piece rook = game->board[from.x][0];
            game->board[from.x][from.y - 1] = rook;
            game->board[from.x][0].type = EMPTY;
            game->board[from.x][0].color = NONE;
            game->board[from.x][0].hasMoved = false;
            move->moveType = CASTLE_QUEENSIDE;
        }
    }

    //  
    // 6) Promotion
    //  
    if (mover.type == PAWN) {
        bool whitePromotionRow = (mover.color == WHITE && to.x == 0);
        bool blackPromotionRow = (mover.color == BLACK && to.x == BOARD_SIZE - 1);
        if (whitePromotionRow || blackPromotionRow) {
            // Use move->promotionPiece if set; else default to QUEEN
            PieceType promo = (move->promotionPiece != EMPTY) ? move->promotionPiece : QUEEN;
            game->board[to.x][to.y].type = promo;
            move->moveType = PAWN_PROMOTION;
        }
    }

    //  
    // 7) half-move clock: reset on pawn move or capture, else increment
    //  
    if (mover.type == PAWN || move->moveType == CAPTURE || move->moveType == EN_PASSENT) {
        game->halfMoveClock = 0;
    } else {
        game->halfMoveClock++;
    }

    //  
    // 8) record move in history
    //  
    if (game->moveCounter < 500) {
        game->moveHistory[game->moveCounter] = *move;
        game->moveCounter++;
    }

    //  
    // 9) update position hash / hashCount if i use it
    //    (Not implemented here — i will place hashing call if exist)
    //  
    // example: game->positionHashes[game->hashCount++] = computeZobristHash(game);

    //  
    // 10) toggle player (change player turn)
    //  
    game->currentPlayer = (game->currentPlayer == WHITE) ? BLACK : WHITE;

    //  
    // 11) update status (basic): update to CHECK if opponent is in check
    //     For full correctness, compute CHECK, CHECKMATE, STALEMATE, etc.
    //  
    // Note: in many engines computation of status is done separately.
    game->status = computeGameStatus(game);
}