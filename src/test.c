#include <stdio.h>
#include <stdbool.h>
#include "../include/definitions.h"
#include "../include/game.h"
#include "../include/display.h"
#include <string.h> // for strlen
#include <stdlib.h> // for abs


// Assume these functions exist in your project:
// bool isValidPawn(Game*, Piece, Move);
// bool isValidRookMove(Game*, Piece, Move);
// bool isValidKnight(Game*, Piece, Move);
// bool isValidBishop(Game*, Piece, Move);
// bool isValidQueen(Game*, Piece, Move);
// bool isValidKing(Game*, Piece, Move);
// void printBoard(Game*);
// void loadStandardPosition(Game*);
// void applyMoveToGame(Game*, Move);
void loadStandardPosition(Game *game) {
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
            game->board[r][c] = (Piece){EMPTY, WHITE, false};

    // Pawns
    for (int c = 0; c < 8; ++c) {
        game->board[6][c] = (Piece){PAWN, WHITE, false};
        game->board[1][c] = (Piece){PAWN, BLACK, false};
    }

    // Rooks
    game->board[7][0] = (Piece){ROOK, WHITE, false};
    game->board[7][7] = (Piece){ROOK, WHITE, false};
    game->board[0][0] = (Piece){ROOK, BLACK, false};
    game->board[0][7] = (Piece){ROOK, BLACK, false};

    // Knights
    game->board[7][1] = (Piece){KNIGHT, WHITE, false};
    game->board[7][6] = (Piece){KNIGHT, WHITE, false};
    game->board[0][1] = (Piece){KNIGHT, BLACK, false};
    game->board[0][6] = (Piece){KNIGHT, BLACK, false};

    // Bishops
    game->board[7][2] = (Piece){BISHOP, WHITE, false};
    game->board[7][5] = (Piece){BISHOP, WHITE, false};
    game->board[0][2] = (Piece){BISHOP, BLACK, false};
    game->board[0][5] = (Piece){BISHOP, BLACK, false};

    // Queens
    game->board[7][3] = (Piece){QUEEN, WHITE, false};
    game->board[0][3] = (Piece){QUEEN, BLACK, false};

    // Kings
    game->board[7][4] = (Piece){KING, WHITE, false};
    game->board[0][4] = (Piece){KING, BLACK, false};

    game->currentPlayer = WHITE;
}
void applyMoveToGame(Game *game, Move move) {
    Piece p = game->board[move.initial.x][move.initial.y];
    game->board[move.final.x][move.final.y] = p;
    game->board[move.final.x][move.final.y].hasMoved = true;
    game->board[move.initial.x][move.initial.y] = (Piece){EMPTY, WHITE, false};
    game->currentPlayer = (game->currentPlayer == WHITE) ? BLACK : WHITE;
}
// Minimal helper to parse move like "A2A4"
bool parseSquare(const char *s, Position *pos) {
    if (!s || strlen(s) < 2) return false;
    char file = s[0], rank = s[1];
    if (file >= 'a' && file <= 'h') file -= 'a'-'A';
    if (file < 'A' || file > 'H') return false;
    if (rank < '1' || rank > '8') return false;
    pos->y = file - 'A';
    pos->x = 8 - (rank - '0');
    return true;
}

bool parseMove(const char *s, Move *m) {
    Position from, to;
    if (!parseSquare(s, &from)) return false;
    if (!parseSquare(s+2, &to)) return false;
    m->initial = from;
    m->final = to;
    return true;
}
void runMoves(Game *game, const char *moves[], int moveCount, const char *title) {
    printf("\n=== %s ===\n", title);
    for (int i = 0; i < moveCount; ++i) {
        Move m;
        if (!parseMove(moves[i], &m)) {
            printf("Invalid move format: %s\n", moves[i]);
            continue;
        }
        Piece p = game->board[m.initial.x][m.initial.y];
        if (p.type == EMPTY) {
            printf("No piece at source: %s\n", moves[i]);
            continue;
        }
        applyMoveToGame(game, m);
        printf("\nAfter move %s:\n", moves[i]);
        printBoard(game);
    }
}

int main() {
    Game game;

    // ------------------ 1. First Moves ------------------
    loadStandardPosition(&game);
    const char *firstMoves[] = {"A2A4", "A7A6", "B1A3", "H1H8"}; // include invalid to test
    runMoves(&game, firstMoves, sizeof(firstMoves)/sizeof(firstMoves[0]), "First Moves");

    // ------------------ 2. Check/Checkmate ------------------
    loadStandardPosition(&game);
    const char *checkmateWhite[] = {"E2E4","H7H6","D1H5","A7A6","F1C4","B7B6","H5F7"};
    runMoves(&game, checkmateWhite, sizeof(checkmateWhite)/sizeof(checkmateWhite[0]), "Checkmate - White");

    const char *checkmateBlack[] = {"E7E5","H2H3","D8H4","A2A3","F8C5","B2B3","H4F2"};
    runMoves(&game, checkmateBlack, sizeof(checkmateBlack)/sizeof(checkmateBlack[0]), "Checkmate - Black");

    // ------------------ 3. Stalemate ------------------
    loadStandardPosition(&game);
    const char *stalemateMoves[] = {
        "C2C4","H7H5","H2H4","A7A5","D1A4","A8A6","A4A5","A6H6",
        "A5C7","F7F6","C7D7","E8F7","D7B7","D8D3","B7B8","D3H7",
        "B8C8","F7G6","C8E6"
    };
    runMoves(&game, stalemateMoves, sizeof(stalemateMoves)/sizeof(stalemateMoves[0]), "Stalemate Scenario");

    // ------------------ 4. Promotion ------------------
    loadStandardPosition(&game);
    const char *promotionWhite[] = {"E2E4","D7D5","E4D5","G8F6","F1B5","C7C6","D5C6","D8B6","C6B7","B6B5","B7C8"};
    runMoves(&game, promotionWhite, sizeof(promotionWhite)/sizeof(promotionWhite[0]), "Promotion - White");

    // You can add more sections:
    // - Undo/Redo tests
    // - Invalid move tests
    // - Specific piece moves (Pawn, Rook, Knight, etc.)
    // - Check only scenarios

    return 0;
}