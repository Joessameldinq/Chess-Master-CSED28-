
#include "../../include/GameInitialization.h"
#include "../../include/InputConsole.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



static ZobristTables globalZobrist;
static bool zobristInitialized = false;

uint64_t random64(void) {
    static uint64_t x = 123456789;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return x;
}

// Initialize zobrist tables ONCE
void initZobristTables(void) {
    if (zobristInitialized) {
        return; // Already done
    }
    
    // Initialize piece hashes
    for (int square = 0; square < 64; square++) {
        for (int piece = 0; piece < 12; piece++) {
            globalZobrist.pieces[square][piece] = random64();
        }
    }
    
    // Initialize castling hashes
    for (int i = 0; i < 4; i++) {
        globalZobrist.castling[i] = random64();
    }
    
    // Initialize en passant hashes
    for (int i = 0; i < 8; i++) {
        globalZobrist.enpassant[i] = random64();
    }
    
    // Initialize side to move hash
    globalZobrist.sideToMove = random64();
    
    zobristInitialized = true;
}

// Helper to get piece mapping 
int getPieceIndex(Piece p) {
    if (p.color == NONE || p.type == EMPTY) {
        return -1;
    }
    int base = p.type - 1;
    int colorOffset = (p.color == BLACK) ? 6 : 0;    
    return base + colorOffset;
}

uint64_t computePositionHash(Game game) {
    if (!zobristInitialized) {
        initZobristTables();
    }
    
    uint64_t hash = 0;
    
    // Hash all pieces on board
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece piece = game.board[row][col];
            
            if (piece.type != EMPTY && piece.color != NONE) {
                int pieceIndex = getPieceIndex(piece);
                if (pieceIndex >= 0) {
                    int square = row * 8 + col;
                    hash ^= globalZobrist.pieces[square][pieceIndex];
                }
            }
        }
    }
    
    // Hash castling rights
    // BLACK at row 0
    if (!game.board[0][4].hasMoved && 
        game.board[0][4].type == KING &&
        game.board[0][4].color == BLACK &&
        !game.board[0][7].hasMoved &&
        game.board[0][7].type == ROOK &&
        game.board[0][7].color == BLACK) {
        hash ^= globalZobrist.castling[0]; // Black kingside
    }
    
    if (!game.board[0][4].hasMoved && 
        game.board[0][4].type == KING &&
        game.board[0][4].color == BLACK &&
        !game.board[0][0].hasMoved &&
        game.board[0][0].type == ROOK &&
        game.board[0][0].color == BLACK) {
        hash ^= globalZobrist.castling[1]; // Black queenside
    }
    
    // WHITE at row 7
    if (!game.board[7][4].hasMoved && 
        game.board[7][4].type == KING &&
        game.board[7][4].color == WHITE &&
        !game.board[7][7].hasMoved &&
        game.board[7][7].type == ROOK &&
        game.board[7][7].color == WHITE) {
        hash ^= globalZobrist.castling[2]; // White kingside
    }
    
    if (!game.board[7][4].hasMoved && 
        game.board[7][4].type == KING &&
        game.board[7][4].color == WHITE &&
        !game.board[7][0].hasMoved &&
        game.board[7][0].type == ROOK &&
        game.board[7][0].color == WHITE) {
        hash ^= globalZobrist.castling[3]; // White queenside
    }
    
    // Hash en passant availability
    if (game.enPassentAvailable && 
        game.enPassentTarget.y >= 0 && 
        game.enPassentTarget.y < 8) {
        hash ^= globalZobrist.enpassant[game.enPassentTarget.y];
    }
    
    // Hash side to move
    if (game.currentPlayer == BLACK) {
        hash ^= globalZobrist.sideToMove;
    }
    
    return hash;
}

Game* initGame(void) {
    // Initialize zobrist tables 
    if (!zobristInitialized) {
        initZobristTables();
    }
    
    Game* game = (Game*)malloc(sizeof(Game));
    memset(game, 0, sizeof(Game));
    
    game->currentPlayer = WHITE;
    game->status = PLAYING;
    game->enPassentAvailable = false;
    game->enPassentTarget.x = -1;
    game->enPassentTarget.y = -1;
    
    for (int i = 0; i < 8; i++) {
        game->capturedWhitePieces[i].color = WHITE;
        game->capturedWhitePieces[i].hasMoved = false;
        game->capturedWhitePieces[i].type = EMPTY;
        game->capturedBlackPieces[i].hasMoved = false;
        game->capturedBlackPieces[i].color = BLACK;
        game->capturedBlackPieces[i].type = EMPTY;
    }
    
    game->currentFlag.castlingMade = false;
    game->currentFlag.enpassentMade = false;
    game->currentFlag.pawnPromotionMade = false;
    game->halfMoveClock = 0;

    // Set up board
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            game->board[i][j].hasMoved = false;
            if (i == 7 || i == 6)
                game->board[i][j].color = WHITE;
            else if (i == 0 || i == 1)
                game->board[i][j].color = BLACK;
            
            if (i == 0 || i == 7) {
                if (j == 0 || j == 7)
                    game->board[i][j].type = ROOK;
                else if (j == 1 || j == 6)
                    game->board[i][j].type = KNIGHT;
                else if (j == 2 || j == 5)
                    game->board[i][j].type = BISHOP;
                else if (j == 3)
                    game->board[i][j].type = QUEEN;
                else 
                    game->board[i][j].type = KING;
            }
            else if (i == 1 || i == 6)
                game->board[i][j].type = PAWN;
            else {
                game->board[i][j].type = EMPTY;
                game->board[i][j].color = NONE;
            }
        }
    }
    
    // Initialize hash
    game->currentHash = computePositionHash(*game);
    game->hashCount = 0;  
    game->hashHistory[game->hashCount] = game->currentHash;
    game->hashCount++;  
    
    
    return game;
}