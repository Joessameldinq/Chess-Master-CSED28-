




#include "../../include/SavingLoading.h"
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
static bool writeU64(FILE *fp,uint64_t v){
    return fwrite(&v, sizeof(uint64_t), 1, fp) == 1;
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
static bool readU64(FILE *fp,uint64_t *v){
    return fread(v, sizeof(uint64_t), 1, fp) == 1;
}

static bool writeString(FILE *fp, char *str)
{
    uint64_t len = str ? strlen(str) : 0;

    // write length
    if (fwrite(&len, sizeof(uint64_t), 1, fp) != 1)
        return false;

    // write characters (without '\0')
    if (len > 0)
        return fwrite(str, sizeof(char), len, fp) == len;

    return true;
}
static bool readString(FILE *fp, char **str)
{
    uint64_t len;

    // read length
    if (fread(&len, sizeof(uint64_t), 1, fp) != 1)
        return false;

    // allocate memory (+1 for null terminator)
    *str = malloc(len + 1);
    if (!*str)
        return false;

    // read characters
    if (len > 0 && fread(*str, sizeof(char), len, fp) != len)
    {
        free(*str);
        return false;
    }

    (*str)[len] = '\0'; // null-terminate
    return true;
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
bool saveGame(FILE *fp, const Game *g, Node *stack)
{
    if (!fp || !g) return false;

    // Write MAGIC header
    if (fwrite(MAGIC, 1, MAGIC_SIZE, fp) != MAGIC_SIZE) return false;
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
    if (!writeBool(fp, g->enPassentAvailable)) return false;

    // Save hashing
    if(!writeU64(fp, g->currentHash)) return false;
    if(!writeInt(fp, g->hashCount)) return false;

    for(int hash = 0; hash < g->hashCount; hash++) {
        if(!writeU64(fp, g->hashHistory[hash])) return false;
    }

    for(int hash = 0; hash < g->hashCount; hash++) {
        if(!writeString(fp, g->moveHistory[hash])) return false;
    }

    // ✅ Count and WRITE stack size BEFORE writing data
    int stackCount = 0;
    Node *temp = stack;
    while(temp) {
        stackCount++;
        temp = temp->nextGame;
    }
    if (!writeInt(fp, stackCount)) return false;  // ✅ Write count HERE

    // Save each game in the stack
    Node *current = stack;
    while(current) {
        // Save board
        for (int y = 0; y < BOARD_SIZE; y++)
            for (int x = 0; x < BOARD_SIZE; x++) {
                const Piece *p = &current->curGame.board[y][x];
                if (!writeEnum(fp, p->type)) return false;
                if (!writeEnum(fp, p->color)) return false;
                if (!writeBool(fp, p->hasMoved)) return false;
            }

        // Save captured pieces
        for (int j = 0; j < 16; j++) {  // ✅ Use 'j' instead of 'i'
            if (!writeEnum(fp, current->curGame.capturedWhitePieces[j].type)) return false;
            if (!writeEnum(fp, current->curGame.capturedWhitePieces[j].color)) return false;
            if (!writeBool(fp, current->curGame.capturedWhitePieces[j].hasMoved)) return false;

            if (!writeEnum(fp, current->curGame.capturedBlackPieces[j].type)) return false;
            if (!writeEnum(fp, current->curGame.capturedBlackPieces[j].color)) return false;
            if (!writeBool(fp, current->curGame.capturedBlackPieces[j].hasMoved)) return false;
        }

        if (!writeInt(fp, current->curGame.halfMoveClock)) return false;
        if (!writeEnum(fp, current->curGame.status)) return false;
        if (!writeEnum(fp, current->curGame.currentPlayer)) return false;
        if (!writeInt(fp, current->curGame.enPassentTarget.x)) return false;
        if (!writeInt(fp, current->curGame.enPassentTarget.y)) return false;
        if (!writeBool(fp, current->curGame.currentFlag.pawnPromotionMade)) return false;
        if (!writeBool(fp, current->curGame.currentFlag.castlingMade)) return false;
        if (!writeBool(fp, current->curGame.currentFlag.enpassentMade)) return false;
        if (!writeBool(fp, current->curGame.enPassentAvailable)) return false;
        if (!writeU64(fp, current->curGame.currentHash)) return false;
        if (!writeInt(fp, current->curGame.hashCount)) return false;

        for(int hash = 0; hash < current->curGame.hashCount; hash++) {
            if(!writeU64(fp, current->curGame.hashHistory[hash])) return false;
        }

        for(int hash = 0; hash < current->curGame.hashCount; hash++) {
            if(!writeString(fp, current->curGame.moveHistory[hash])) return false;
        }

        current = current->nextGame;
    }

    return true;
}

bool loadGame(FILE *fp, Game *g, Node **stack)
{
    if (!fp || !g) return false;

    char magicBuf[MAGIC_SIZE];
    if (fread(magicBuf, 1, MAGIC_SIZE, fp) != MAGIC_SIZE) return false;
    if (memcmp(magicBuf, MAGIC, MAGIC_SIZE) != 0) return false;

    int version;
    if (!readInt(fp, &version)) return false;
    if (version != SAVE_VERSION) return false;

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

    if (!readInt(fp, &g->halfMoveClock)) return false;
    if (!readEnum(fp, (int*)&g->status)) return false;
    if (!readEnum(fp, (int*)&g->currentPlayer)) return false;
    if (!readInt(fp, &g->enPassentTarget.x)) return false;
    if (!readInt(fp, &g->enPassentTarget.y)) return false;
    if (!readBool(fp, &g->currentFlag.pawnPromotionMade)) return false;
    if (!readBool(fp, &g->currentFlag.castlingMade)) return false;
    if (!readBool(fp, &g->currentFlag.enpassentMade)) return false;
    if (!readBool(fp, &g->enPassentAvailable)) return false;

    // Load hashing
    if(!readU64(fp, &g->currentHash)) return false;
    if(!readInt(fp, &g->hashCount)) return false;

    for(int hash = 0; hash < g->hashCount; hash++) {
        if(!readU64(fp, &g->hashHistory[hash])) return false;
    }

    for(int i = 0; i < g->hashCount; i++) {
        if(!readString(fp, &g->moveHistory[i])) return false;
    }

    // ✅ Read stack count (now in correct position)
    int stackCount;
    if (!readInt(fp, &stackCount)) return false;
    
    // Clear existing stack
    while(*stack != NULL) {
        pop(stack);
    }
    
    // Load each game state from stack
    for (int i = 0; i < stackCount; i++) {
        Game stackGame;
        
        // Load board
        for (int y = 0; y < BOARD_SIZE; y++)
            for (int x = 0; x < BOARD_SIZE; x++) {
                Piece *p = &stackGame.board[y][x];
                if (!readEnum(fp, (int*)&p->type)) return false;
                if (!readEnum(fp, (int*)&p->color)) return false;
                if (!readBool(fp, &p->hasMoved)) return false;
            }

        // Load captured pieces - ✅ Use 'j' to avoid shadowing
        for (int j = 0; j < 16; j++) {
            if (!readEnum(fp, (int*)&stackGame.capturedWhitePieces[j].type)) return false;
            if (!readEnum(fp, (int*)&stackGame.capturedWhitePieces[j].color)) return false;
            if (!readBool(fp, &stackGame.capturedWhitePieces[j].hasMoved)) return false;

            if (!readEnum(fp, (int*)&stackGame.capturedBlackPieces[j].type)) return false;
            if (!readEnum(fp, (int*)&stackGame.capturedBlackPieces[j].color)) return false;
            if (!readBool(fp, &stackGame.capturedBlackPieces[j].hasMoved)) return false;
        }

        if (!readInt(fp, &stackGame.halfMoveClock)) return false;
        if (!readEnum(fp, (int*)&stackGame.status)) return false;
        if (!readEnum(fp, (int*)&stackGame.currentPlayer)) return false;
        if (!readInt(fp, &stackGame.enPassentTarget.x)) return false;
        if (!readInt(fp, &stackGame.enPassentTarget.y)) return false;
        if (!readBool(fp, &stackGame.currentFlag.pawnPromotionMade)) return false;
        if (!readBool(fp, &stackGame.currentFlag.castlingMade)) return false;
        if (!readBool(fp, &stackGame.currentFlag.enpassentMade)) return false;
        if (!readBool(fp, &stackGame.enPassentAvailable)) return false;
        if (!readU64(fp, &stackGame.currentHash)) return false;
        if (!readInt(fp, &stackGame.hashCount)) return false;

        for(int hash = 0; hash < stackGame.hashCount; hash++) {
            if(!readU64(fp, &stackGame.hashHistory[hash])) return false;
        }

        for(int hash = 0; hash < stackGame.hashCount; hash++) {
            if(!readString(fp, &stackGame.moveHistory[hash])) return false;
        }
        
        push(stack, stackGame);
    }
    
    // Reverse the stack to restore original order
    Node *reversedStack = NULL;
    while(*stack != NULL) {
        Game temp = pop(stack);
        push(&reversedStack, temp);
    }
    *stack = reversedStack;

    return true;
}
