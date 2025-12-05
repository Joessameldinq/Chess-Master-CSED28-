#include "../include/display.h"
#include "../include/move.h"
#include "../include/end.h"
#include "../include/game.h"
#include "../include/utilities.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <locale.h>

#ifdef _WIN32
    #include <windows.h>
#endif

// Colors
#define RESET       "\033[0m"
#define LIGHT_SQ    "\033[47m"       // Light square
#define DARK_SQ     "\033[48;5;65m"  // Dark green square
#define BLACK_PC    "\033[38;5;16m"
#define WHITE_PC    "\033[38;5;255m"
#define BOLD        "\033[1m"
#define YELLOW      "\033[93m"
#define MAGENTA     "\033[35m"
#define BLUE        "\033[34m"
#define RED_BG      "\033[41m"
#define GREEN_BG    "\033[42m"

// ------------------ Initialization ------------------
void initDisplay() {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // Windows CMD UTF-8
    #endif
    setlocale(LC_ALL, "");          // WSL/Linux UTF-8
    srand((unsigned int)time(NULL)); // Seed random for quotes depending on current time
}

// ------------------ Main Menu ------------------
void displayMainMenu(void)
{
    printf(BLUE BOLD);
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                      CHESS MAIN MENU                      ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║                                                           ║\n");
    printf("║     1) Play a new game                                    ║\n");
    printf("║                                                           ║\n");
    printf("║     2) Load Saved game                                    ║\n");
    printf("║                                                           ║\n");
    printf("║     3) Quit                                               ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf(RESET);
}

// ------------------ Quotes ------------------
void printQuotes(FILE *configuration)
{
    if (!configuration) {
        printf(RED_BG BOLD "Error: Could not open quotes file!\n" RESET);
        return;
    }

    int randomQuote = (rand() % 15) + 1;
    char target[10];
    snprintf(target, sizeof(target), "%d.", randomQuote);

    char buffer[256];
    printf(GREEN_BG BOLD);
    printf("===========================================================================\n");
    printf("🧩 Life Lessons from the Chessboard ♟️\n");
    printf("===========================================================================\n" RESET);

    while (fgets(buffer, sizeof(buffer), configuration)) {
        if (strncmp(buffer, target, strlen(target)) == 0) break;
    }

    if (feof(configuration)) {
        printf(RED_BG "Error: Quote number %d not found!\n" RESET, randomQuote);
        return;
    }

    printf("%s", buffer);
    for (int i = 0; i < 2; i++) {
        if (fgets(buffer, sizeof(buffer), configuration))
            printf("%s", buffer);
    }
    printf("\n");
}

// ------------------ Captured Pieces ------------------
void printCapturedPieces(Game *game)
{
    short capturedWhite = findFirstEmptyCapturedSlot(game->capturedWhitePieces);
    short capturedBlack = findFirstEmptyCapturedSlot(game->capturedBlackPieces);

    printf(RED_BG BOLD "Captured Black Pieces: " RESET);
    for (int i = 0; i < capturedBlack; i++) {
        char *p = " ";
        switch (game->capturedBlackPieces[i].type) {
            case PAWN:   p = "♟"; break;
            case ROOK:   p = "♜"; break;
            case KNIGHT: p = "♞"; break;
            case BISHOP: p = "♝"; break;
            case QUEEN:  p = "♛"; break;
            case KING:   p = "♚"; break;
        }
        printf(BLACK_PC LIGHT_SQ " %s " RESET, p);
    }
    printf("\n");

    printf(GREEN_BG BOLD "Captured White Pieces: " RESET);
    for (int i = 0; i < capturedWhite; i++) {
        char *p = " ";
        switch (game->capturedWhitePieces[i].type) {
            case PAWN:   p = "♙"; break;
            case ROOK:   p = "♖"; break;
            case KNIGHT: p = "♘"; break;
            case BISHOP: p = "♗"; break;
            case QUEEN:  p = "♕"; break;
            case KING:   p = "♔"; break;
        }
        printf(WHITE_PC DARK_SQ " %s " RESET, p);
    }
    printf("\n");
}

// ------------------ Game Status ------------------
void printGameState(Game *game)
{
    printf(MAGENTA BOLD "════════════════ CHESS GAME STATUS ════════════════\n" RESET);

    printf("%sCurrent Turn: %s%s\n", 
           game->currentPlayer == WHITE ? WHITE_PC : BLACK_PC, 
           game->currentPlayer == WHITE ? "WHITE" : "BLACK", RESET);

    printf("⏳ Half move clock: %d\n", game->halfMoveClock);

    if (game->status == CHECK) {
        printf(RED_BG BOLD "⚠️  CHECK! %s King under attack!\n" RESET, 
               game->currentPlayer == WHITE ? "WHITE" : "BLACK");
    }

    if (game->enPassentAvailable) printf("ℹ️  En Passant available\n");
    if (game->currentFlag.pawnPromotionMade) { printf("👑 Pawn promoted!\n"); setFlagsFalse(game); }
    if (game->currentFlag.castlingMade) { printf("🏰 Castling performed!\n"); setFlagsFalse(game); }
    if (game->currentFlag.enpassentMade) { printf("⚔️  En Passant capture!\n"); setFlagsFalse(game); }

    printf(MAGENTA BOLD "═════════════════════════════════════════════════\n" RESET);

    printCapturedPieces(game);
}

// ------------------ Board ------------------
void printBoard(Game *game)
{

    const char *indent = "                              ";

    printf("\n\n");
    printf("%s    A  B  C  D  E  F  G  H\n", indent);

    for (int i = 0; i < 8; i++) {
        printf("%s%d ", indent, 8 - i);
        for (int j = 0; j < 8; j++) {
            bool isWhiteSquare = (i + j) % 2 == 0;
            char *bg = isWhiteSquare ? LIGHT_SQ : DARK_SQ;
            char *fg = WHITE_PC;
            char *p = " ";

            if (game->board[i][j].color == WHITE) {
                fg = WHITE_PC;
                switch (game->board[i][j].type) {
                    case PAWN: p = "♙"; break;
                    case ROOK: p = "♖"; break;
                    case KNIGHT: p = "♘"; break;
                    case BISHOP: p = "♗"; break;
                    case QUEEN: p = "♕"; break;
                    case KING: p = "♔"; break;
                }
            } else if (game->board[i][j].color == BLACK) {
                fg = BLACK_PC;
                switch (game->board[i][j].type) {
                    case PAWN: p = "♟"; break;
                    case ROOK: p = "♜"; break;
                    case KNIGHT: p = "♞"; break;
                    case BISHOP: p = "♝"; break;
                    case QUEEN: p = "♛"; break;
                    case KING: p = "♚"; break;
                }
            }
            printf("%s%s %s %s", bg, fg, p, RESET);
        }
        printf(" %d\n", 8 - i);
    }

    printf("%s    A  B  C  D  E  F  G  H\n", indent);
    printf("\n");
}

// ------------------ Utility ------------------
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause(void) {
    printf(BOLD LIGHT_SQ "Press Enter to Continue..." RESET);
    fflush(stdout);
    getchar();
}
void displayWelcome(void) {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // Enable UTF-8 in Windows console
    #endif

    clearScreen();
    printf(YELLOW BOLD);
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                                                           ║\n");
    printf("║              ♔  CHESS ENGINE INTERACTIVE GAME  ♚          ║\n");
    printf("║              ♔  Supervised by Eng. Karim Alaa  ♚          ║\n");
    printf("║           ♔  CSED28++ Batch CSE212 Final Project  ♚       ║\n");
    printf("║              ♔            Presented BY         ♚          ║\n");
    printf("║              ♔            Yousef Essam         ♚          ║\n");
    printf("║              ♔            Abdelwahab Khaled    ♚          ║\n");
    printf("║                                                           ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("Welcome to the Chess Game!\n");
    printf("Type 'help' for instructions or make your first move.\n");
    printf(RESET "\n");
}

// Display the help menu
void displayHelp(void) {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // Enable UTF-8 in Windows console
    #endif

    printf(BOLD MAGENTA);
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                      CHESS HELP                           ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║  Commands:                                                ║\n");
    printf("║    • Move:     e2e4  or  e2 e4  or  e2-e4                 ║\n");
    printf("║    • Help:     help  or  h                                ║\n");
    printf("║    • Restart:  restart  or  r                             ║\n");
    printf("║    • Quit:     quit  or  q                                ║\n");
    printf("║                                                           ║\n");
    printf("║  Special Moves:                                           ║\n");
    printf("║    • Castling: Move king 2 squares (e1g1 or e1c1)         ║\n");
    printf("║    • En Passant: Capture pawn that just moved 2 squares   ║\n");
    printf("║    • Pawn Promotion: Move pawn to last rank               ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf(RESET "\n");
}