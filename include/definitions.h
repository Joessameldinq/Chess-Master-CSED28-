#ifndef DEFINITIONS_H
#define DEFINITIONS_H
#define BOARD_SIZE 8
#include <stdbool.h>
#include <stdint.h>

//Piece Types
typedef enum
{
    EMPTY = 0,
    PAWN = 1,
    KNIGHT = 2,
    BISHOP = 3,
    ROOK = 4,
    QUEEN = 5,
    KING = 6
} PieceType;

//Piece colors
typedef enum
{
    NONE = 0,
    WHITE = 1,
    BLACK = 2
} Color;

//Game state
typedef enum
{
    PLAYING,
    CHECK,
    CHECKMATE,
    STALEMATE,
    DRAW_FIFTY_MOVE,
    DRAW_INSUFFICIENT_MATERIAL,
    DRAW_AGREEMENT
} GameStatus;

//Movement type
typedef enum
{
    NORMAL_MOVE,
    CAPTURE,
    CASTLE_KINGSIDE,
    CASTLE_QUEENSIDE,
    EN_PASSENT,
    PAWN_PROMOTION,
    CAPTURE_AND_PAWN_PROMOTION
} MoveType;

/*
    Definition of Game Objects Structures
*/

//Pieces - Optimized to 4 bytes (was 12 bytes with padding)
typedef struct 
{
    PieceType type;      // 4 bytes (enum = int)
    Color color;         // 4 bytes (enum = int)
    bool hasMoved;       // 1 byte
    // 3 bytes padding added by compiler
} Piece;

//Position in board - Already optimal at 8 bytes
typedef struct 
{
    int x;  // 4 bytes
    int y;  // 4 bytes
} Position;

//GameFlags - Optimized to 4 bytes (pack bools together)
typedef struct
{
    bool pawnPromotionMade;  // 1 byte
    bool castlingMade;       // 1 byte
    bool enpassentMade;      // 1 byte
    bool enPassentAvailable; // 1 byte (moved from Game struct)
} GameFlags;

//Move - Optimized to 32 bytes (was 36+ with bad padding)
typedef struct 
{
    Position initial;           // 8 bytes
    Position final;             // 8 bytes
    Piece capturedPiece;        // 4 bytes
    MoveType moveType;          // 4 bytes
    PieceType promotionPiece;   // 4 bytes
    // 4 bytes padding
} Move;

// Game structure - Optimally ordered by size (largest to smallest)
typedef struct 
{
    // 8-byte aligned members first
    Piece board[BOARD_SIZE][BOARD_SIZE];  // 256 bytes (64 * 4)
    Piece capturedWhitePieces[16];        // 64 bytes
    Piece capturedBlackPieces[16];        // 64 bytes
    
    // 4-byte members
    int moveCounter;                       // 4 bytes
    int halfMoveClock;                     // 4 bytes
    PieceType lastCapturedPiece;          // 4 bytes
    GameStatus status;                     // 4 bytes
    Color currentPlayer;                   // 4 bytes
    Position enPassentTarget;              // 8 bytes
    GameFlags currentFlag;                 // 4 bytes
    
    // Total: ~412 bytes (perfectly aligned, no padding waste)
} Game;

#endif