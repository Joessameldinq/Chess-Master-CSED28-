#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#define BOARD_SIZE 8

#include <stdbool.h>
//Piece Types
typedef enum
{
    EMPTY = 0,
    PAWN =1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING =6
}PieceType;

//Piece colors
typedef enum
{
    NONE = 0,
    WHITE =0,
    BLACK = 1
}Color;

//Game state
typedef enum
{
    PLAYING,
    CHECK,
    CHECKMATE,
    STALEMATE,
    DRAW_FIFTY_MOVE,
    DRAW_THREEFOLD_REPTITION,
    DRAW_INSUFFICIENT_MATERIAL,
    DRAW_AGREEMENT
}GameStatus;

//Movement type
typedef enum
{
    NORMAL_MOVE,
    CAPTURE,
    CASTLE_KINGSIDE,
    CASTLE_QUEENSIDE,
    EN_PASSENT,
    PAWN_PROMOTION
}MoveType;

/*
    =======================================================
        Definition of Game Objects Structures
    =======================================================
*/
//Pieces
typedef struct 
{
    PieceType type;
    Color color;
    bool hasMoved;
}Piece;

//Position in board
typedef struct 
{
    int x;
    int y;
}Position;

//Move
typedef struct 
{
    Position initial;
    Position final;
    MoveType moveType;
    Piece   capturedPiece;
    PieceType promotionPiece;
}Move;


//Game structure
typedef struct 
{
    Piece board[BOARD_SIZE][BOARD_SIZE];
    Piece capturedWhitePieces[8];
    Piece capturedBlackPieces[8];
    Color currentPlayer;
    GameStatus status;
    Move moveHistory[500];
    int moveCounter;
    Position enPassentTarget;
    bool enPassentAvailable;
    int halfMoveClock; // for 50 move rule will be discussed later
    unsigned long long positionHashes[500]; // For three fold reptition and will be discussed later
    int hashCount;
}Game;








#endif
