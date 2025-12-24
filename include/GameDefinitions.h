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
    DRAW_AGREEMENT,
    DRAW_THREEFOLD_REPTITION
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
typedef struct {
    uint64_t pieces[64][12];      // [square][piece_type + color_offset]
    uint64_t castling[4];         // BKingSide , BQueenSide , WKingSide , WQueenSide
    uint64_t enpassant[8];       // for each column for enpassant available
    uint64_t sideToMove;        // toggle for black's turn
} ZobristTables; //Only helper struct and won't be saved

/*
    Definition of Game Objects Structures
*/

typedef struct 
{
    PieceType type;     
    Color color;         
    bool hasMoved;       
} Piece;

typedef struct 
{
    int x; 
    int y; 
} Position;

typedef struct
{
    bool pawnPromotionMade;  
    bool castlingMade;       
    bool enpassentMade;     
} GameFlags;

typedef struct 
{
    Position initial;            
    Position final;              
    Piece capturedPiece;       
    MoveType moveType;         
    PieceType promotionPiece;  
} Move;

typedef struct 
{
    Piece board[BOARD_SIZE][BOARD_SIZE];    
    Piece capturedWhitePieces[16];         
    Piece capturedBlackPieces[16];         
    int halfMoveClock;                    
    GameStatus status;                    
    Color currentPlayer;                  
    Position enPassentTarget;               
    GameFlags currentFlag;
    bool enPassentAvailable; 
    uint64_t currentHash;
    uint64_t hashHistory[1024];
    int hashCount;                
} Game;

#endif