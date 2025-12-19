




#include "../include/SavingLoading.h"
#include <string.h>


//General Helpers to save and load without issues
#define MAGIC "CHESS_SAVE_V1"
#define MAGIC_SIZE 14
#define SAVE_VERSION 1

static bool writeInt(FILE *fp, int v) {
    return fwrite(&v, sizeof(int), 1, fp) == 1;
}

static bool writeBool(FILE *fp, bool v) {
    return fwrite(&v, sizeof(bool), 1, fp) == 1;
}

static bool writeEnum(FILE *fp, int v) { // enums stored as 4 bytes
    return fwrite(&v, sizeof(int), 1, fp) == 1;
}

static bool readInt(FILE *fp, int *v) {
    return fread(v, sizeof(int), 1, fp) == 1;
}

static bool readBool(FILE *fp, bool *v) {
    return fread(v, sizeof(bool), 1, fp) == 1;
}

static bool readEnum(FILE *fp, int *v) {
    return fread(v, sizeof(int), 1, fp) == 1;
}


//===============================

bool undoMove(Node **gameStack, Node **redoStack)
{
    // Need at least 2 states to undo (current + previous)
    if (*gameStack == NULL || (*gameStack)->nextGame == NULL)
        return false;
    
    Game currentGame = pop(gameStack);
    push(redoStack, currentGame);
    
    return true;
}

bool redoMove(Node **gameStack, Node **redoStack)
{
    if (isEmptyStack(*redoStack))
        return false;
    
    // Pop from redo stack and push to game stack
    Game redoGame = pop(redoStack);
    push(gameStack, redoGame);
    
    return true;
}
bool saveGame(FILE *fp, const Game *g)
{
    if (!fp || !g) return false;

    // Write MAGIC header
    if (fwrite(MAGIC, 1, MAGIC_SIZE, fp) != MAGIC_SIZE) return false;

    // Write version number
    if (!writeInt(fp, SAVE_VERSION)) return false;

    // Save 64 board squares (each Piece)
    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++) {
            const Piece *p = &g->board[y][x];

            if (!writeEnum(fp, p->type)) return false;
            if (!writeEnum(fp, p->color)) return false;
            if (!writeBool(fp, p->hasMoved)) return false;
        }

    // Save captured pieces
    for (int i = 0; i < 16; i++) {
        if (!writeEnum(fp, g->capturedWhitePieces[i].type)) return false;
        if (!writeEnum(fp, g->capturedWhitePieces[i].color)) return false;
        if (!writeBool(fp, g->capturedWhitePieces[i].hasMoved)) return false;

        if (!writeEnum(fp, g->capturedBlackPieces[i].type)) return false;
        if (!writeEnum(fp, g->capturedBlackPieces[i].color)) return false;
        if (!writeBool(fp, g->capturedBlackPieces[i].hasMoved)) return false;
    }

    // Integers
    if (!writeInt(fp, g->halfMoveClock)) return false;

    // Enums
    if (!writeEnum(fp, g->status)) return false;
    if (!writeEnum(fp, g->currentPlayer)) return false;

    // Position
    if (!writeInt(fp, g->enPassentTarget.x)) return false;
    if (!writeInt(fp, g->enPassentTarget.y)) return false;

    // Flags
    if (!writeBool(fp, g->currentFlag.pawnPromotionMade)) return false;
    if (!writeBool(fp, g->currentFlag.castlingMade)) return false;
    if (!writeBool(fp, g->currentFlag.enpassentMade)) return false;

    // en passant availability
    if (!writeBool(fp, g->enPassentAvailable)) return false;

    return true;
}
bool loadGame(FILE *fp, Game *g)
{
    //We must load in the same way we saved the game
    // Order  matter !!
    
    if (!fp || !g) return false;

    char magicBuf[MAGIC_SIZE];
    if (fread(magicBuf, 1, MAGIC_SIZE, fp) != MAGIC_SIZE) return false;

    // Magic mismatch --> invalid file
    if (memcmp(magicBuf, MAGIC, MAGIC_SIZE) != 0) return false;

    int version;
    if (!readInt(fp, &version)) return false;
    if (version != SAVE_VERSION) return false; // wrong version

    // Load board
    for (int y = 0; y < BOARD_SIZE; y++)
        for (int x = 0; x < BOARD_SIZE; x++) {
            Piece *p = &g->board[y][x];
            if (!readEnum(fp, (int*)&p->type)) return false;
            if (!readEnum(fp, (int*)&p->color)) return false;
            if (!readBool(fp, &p->hasMoved)) return false;
        }

    // Load captured pieces
    for (int i = 0; i < 16; i++) {
        if (!readEnum(fp, (int*)&g->capturedWhitePieces[i].type)) return false;
        if (!readEnum(fp, (int*)&g->capturedWhitePieces[i].color)) return false;
        if (!readBool(fp, &g->capturedWhitePieces[i].hasMoved)) return false;

        if (!readEnum(fp, (int*)&g->capturedBlackPieces[i].type)) return false;
        if (!readEnum(fp, (int*)&g->capturedBlackPieces[i].color)) return false;
        if (!readBool(fp, &g->capturedBlackPieces[i].hasMoved)) return false;
    }

    // Integers
    if (!readInt(fp, &g->halfMoveClock)) return false;

    // Enums
    if (!readEnum(fp, (int*)&g->status)) return false;
    if (!readEnum(fp, (int*)&g->currentPlayer)) return false;

    // Position
    if (!readInt(fp, &g->enPassentTarget.x)) return false;
    if (!readInt(fp, &g->enPassentTarget.y)) return false;

    // Flags
    if (!readBool(fp, &g->currentFlag.pawnPromotionMade)) return false;
    if (!readBool(fp, &g->currentFlag.castlingMade)) return false;
    if (!readBool(fp, &g->currentFlag.enpassentMade)) return false;

    // en passant availability
    if (!readBool(fp, &g->enPassentAvailable)) return false;

    return true;
}

