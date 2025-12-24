#include "../include/DisplayConsole.h"
#include "../include/MovingLogic.h"
#include "../include/EndingConditions.h"
#include "../include/GameInitialization.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

void displayMainMenu(void)
{

    printf("||=============================================================||\n");
    printf("||                      CHESS MAIN MENU                        ||\n");
    printf("||=============================================================||\n");
    printf("||                                                             ||\n");
    printf("||     1) Play a new game                                      ||\n");
    printf("||                                                             ||\n");
    printf("||     2) Load Saved game                                      ||\n");
    printf("||                                                             ||\n");
    printf("||     3) Quit                                                 ||\n");
    printf("||=============================================================||\n");
    printf("Enter your selection (1,2,3): \n");
}
void printCapturedPieces(Game *game)
{
    short capturedWhite = findFirstEmptyCapturedSlot(game->capturedWhitePieces);
    short capturedBlack = findFirstEmptyCapturedSlot(game->capturedBlackPieces);
    printf("Captured Black Pieces\n");
    for (int i = 0; i < capturedBlack; i++) {
        char *p = " ";
        switch (game->capturedBlackPieces[i].type) {
            case PAWN:   p = " P "; break;
            case ROOK:   p = " R "; break;
            case KNIGHT: p = " N "; break;
            case BISHOP: p = " B "; break;
            case QUEEN:  p = " Q "; break;
            case KING:   p = " K "; break;
            case EMPTY: continue;
        }
        printf(" %s ",p);
    }
    printf("\n");
    printf("Captured White Pieces\n");
    for (int i = 0; i < capturedWhite; i++) {
        char *p = " ";
        switch (game->capturedWhitePieces[i].type) {
            case PAWN:   p = " p "; break;
            case ROOK:   p = " r "; break;
            case KNIGHT: p = " n "; break;
            case BISHOP: p = " b "; break;
            case QUEEN:  p = " q "; break;
            case KING:   p = " k "; break;
            case EMPTY: continue;
        }
        printf(" %s ",p);
    }
    printf("\n");
}

//  Game Status 
void printGameState(Game *game)
{
    printf("Game current hash is %llu\n",game->currentHash);
    printf("Current Turn: %s\n",game->currentPlayer == WHITE ? "WHITE" : "BLACK");

    printf( "Half move clock: %d\n", game->halfMoveClock);

    if (game->status == CHECK) {
        printf( "CHECK! %s King under attack!\n" , 
               game->currentPlayer == WHITE ? "WHITE" : "BLACK");
    }

    if (game->enPassentAvailable) printf("En Passant available\n");
    if (game->currentFlag.pawnPromotionMade) { printf("Pawn promoted!\n"); }
    if (game->currentFlag.castlingMade) { printf("Castling performed!\n");   }
    if (game->currentFlag.enpassentMade) { printf("En Passant capture!\n");   }

    printf("=============================================\n");

    printCapturedPieces(game);
}

//  Board 
void printBoard(Game *game)
{

    const char *indent = "                              ";

    printf("\n\n");
    printf("%s    A  B  C  D  E  F  G  H\n", indent);

    for (int i = 0; i < 8; i++) {
        printf("%s%d ", indent, 8 - i);
        for (int j = 0; j < 8; j++) {
            char *p = " ";
            if((i+j)%2)
                p = " . ";
            else
                p = " - ";

            if (game->board[i][j].color == WHITE) {
                switch (game->board[i][j].type) {
                    case PAWN: p = " p "; break;
                    case ROOK: p = " r "; break;
                    case KNIGHT: p = " n "; break;
                    case BISHOP: p = " b "; break;
                    case QUEEN: p = " q "; break;
                    case KING: p = " k "; break;
                    case EMPTY: continue;
                }
            } else if (game->board[i][j].color == BLACK) {
                switch (game->board[i][j].type) {
                    case PAWN: p = " P "; break;
                    case ROOK: p = " R "; break;
                    case KNIGHT: p = " N "; break;
                    case BISHOP: p = " B "; break;
                    case QUEEN: p = " Q "; break;
                    case KING: p = " K "; break;
                    case EMPTY: continue;
                }
            }
            printf("%s",p);
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
    printf( "Press Enter to Continue...");
    fflush(stdout);
    getchar();
}
void displayWelcome(void) {


    clearScreen();
    printf("\n");
    printf("||=============================================================||\n");
    printf("||                                                             ||\n");
    printf("||                  CHESS ENGINE INTERACTIVE GAME              ||\n");
    printf("||                  Supervised by Eng. Karim Alaa              ||\n");
    printf("||               CSED28++ Batch CSE212 Final Project           ||\n");
    printf("||                          Presented BY                       ||\n");
    printf("||                          Yousef Essam                       ||\n");
    printf("||                        Abdelwahab Khaled                    ||\n");
    printf("||                                                             ||\n");
    printf("||=============================================================||\n");
    printf("\n");
    printf("Welcome to the Chess Game CSED28++!\n");
    printf("Type 'help' for instructions or make your first move.\n");
}

void displayHelp(void) {
    printf("\n");
    printf("||===========================================================||\n");
    printf("||                      CHESS HELP                           ||\n");
    printf("||===========================================================||\n");
    printf("||  Commands:                                                ||\n");
    printf("||    • Move:     e2e4  or  e2 e4  or  e2-e4                 ||\n");
    printf("||    • Save:     save  or  s                                ||\n");
    printf("||    • Load:     load  or  l                                ||\n");
    printf("||    • Quit:     quit                                       ||\n");
    printf("||                                                           ||\n");
    printf("||  Special Moves:                                           ||\n");
    printf("||    • Castling: Move king 2 squares (e1g1 or e1c1)         ||\n");
    printf("||    • En Passant: Capture pawn that just moved 2 squares   ||\n");
    printf("||    • Pawn Promotion: Move pawn to last rank               ||\n");
    printf("||===========================================================||\n");
}