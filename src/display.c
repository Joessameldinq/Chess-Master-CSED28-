#include "../include/display.h"
#include "../include/move.h"
#include "../include/end.h"
#include "../include/game.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define RESET       "\033[0m"
#define LIGHT_SQ    "\033[47m"  // Light beige square 230
#define DARK_SQ     "\033[48;5;65m"   // Dark green square
#define BLACK_PC    "\033[38;5;16m"   // Black pieces (dark gray/black)
#define WHITE_PC    "\033[38;5;255m"  // White pieces
#define BOLD        "\033[1m"
#define YELLOW "\033[93m"
#define WHITE_BG "\033[47m"
#define MAGNETA "\033[35m"

void printCapturedPieces(Game *game)
{
    short int capturedWhite = findFirstEmptyCapturedSlot(game->capturedWhitePieces);
    short int capturedBlack = findFirstEmptyCapturedSlot(game->capturedBlackPieces);
    char *piece;
    printf("Captured Black Pieces: ");
    for(int i = 0 ; i < capturedBlack ; i++)
    {
        switch (game->capturedBlackPieces[i].type)
        {
            case PAWN:
                    piece = "♟";
                    break;
            case ROOK:
                    piece = "♜";
                    break;
            case KNIGHT:
                    piece = "♞";
                    break;
            case BISHOP:
                    piece = "♝";
                    break;
            case QUEEN:
                    piece = "♛";
                    break;
            case KING:
                    piece = "♚";
                    break;
        
        }
        printf(BLACK_PC "%s "RESET,piece);
    }
    printf("\n");
    printf("Captured White Pieces: ");
    for(int i = 0 ; i < capturedWhite ; i++)
    {
        switch (game->capturedWhitePieces[i].type)
        {
            case PAWN:
                    piece = "♙";
                    break;
            case ROOK:
                    piece = "♖";
                    break;
            case KNIGHT:
                    piece = "♘";
                    break;
            case BISHOP:
                    piece = "♗";
                    break;
            case QUEEN:
                    piece = "♕";
                    break;
            case KING:
                    piece = "♔";
                    break;
        
        }
        printf(WHITE_PC "%s "RESET,piece);
    }
    printf("\n");

}
void printGameState(Game *game) {
    printf(MAGNETA BOLD);
    printf("\n");
    printf("═══════════════════════════════════════════════════════════\n");
    printf("                    CHESS GAME STATUS\n");
    printf("═══════════════════════════════════════════════════════════\n");
    
    // Current player
    printf("%s",game->currentPlayer == WHITE ? WHITE_PC : BLACK_PC);
    printf(BOLD);
    printf("Current Turn: %s%s\n", game->currentPlayer== WHITE ? "WHITE" : "BLACK",RESET);
    
    // Check status
    if(game->status == CHECK) {
        printf("⚠️  CHECK! %s King is under attack!\n", game->currentPlayer== WHITE ? "White" : "Black");
    }
    
    // Special moves indicator
    if(game->enPassentAvailable) {
        printf("ℹ️  En Passant available\n");
    }
    
    if(pawnPromotionMade) {
        printf("👑 Pawn promoted \n");
        pawnPromotionMade = false;
    }
    
    if(castlingMade) {
        printf("🏰 Castling performed!\n");
        castlingMade = false;
    }
    
    if(enpassentMade) {
        printf("⚔️  En Passant capture!\n");
        enpassentMade = false;
    }
    
    printf("═══════════════════════════════════════════════════════════\n");
    printf(RESET);
    printCapturedPieces(game);
    
}
void displayHelp(void) {
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
    printf("║  Pieces:                                                  ║\n");
    printf("║    White (lowercase): p=pawn, n=knight, b=bishop          ║\n");
    printf("║                       r=rook, q=queen, k=king             ║\n");
    printf("║    Black (UPPERCASE): P=pawn, N=knight, B=bishop          ║\n");
    printf("║                       R=rook, Q=queen, K=king             ║\n");
    printf("║                                                           ║\n");
    printf("║  Special Moves:                                           ║\n");
    printf("║    • Castling: Move king 2 squares (e1g1 or e1c1)         ║\n");
    printf("║    • En Passant: Capture pawn that just moved 2 squares   ║\n");
    printf("║    • Pawn Promotion: Move pawn to last rank               ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n" RESET);

}
void displayWelcome(void) {
    clearScreen();
    printf(YELLOW BOLD);
    printf("\n");
    printf("╔═══════════════════════════════════════════════════════════╗\n");
    printf("║                                                           ║\n");
    printf("║              ♔  CHESS ENGINE INTERACTIVE GAME  ♚          ║\n");
    printf("║           ♔  CSED28++ Batch CSE212 Final Project  ♚       ║\n");
    printf("║              ♔            Presented BY         ♚          ║\n");
    printf("║              ♔            Yousef Essam         ♚          ║\n");
    printf("║              ♔            Abdelwahab Khaled    ♚          ║\n");
    printf("║                                                           ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝\n");
    printf("\n");
    printf("Welcome to the Chess Game!\n");
    printf("Type 'help' for instructions or make your first move.\n");
    printf(RESET);
    printf("\n");
}
void printBoard(Game* game)
{
    setlocale(LC_ALL,"");
    
   
    const char *indent = "                              ";
    
   
    
    printf("\n\n\n");
    printf("%s    A  B  C  D  E  F  G  H\n", indent);
    
    for(int i = 0; i < 8; i++)
    {
        printf("%s%d ", indent, 8-i);
        
        for(int j = 0; j < 8; j++)
        {
            bool isWhiteSquare = ((i+j) % 2 == 0);
            char *backGround = isWhiteSquare ? LIGHT_SQ : DARK_SQ;
            char *fg = WHITE_PC;
            char *piece;
            
            if(game->board[i][j].color == WHITE)
            {
                fg = WHITE_PC;
                switch (game->board[i][j].type)
                {
                case PAWN:
                    piece = "♙";
                    break;
                case ROOK:
                    piece = "♖";
                    break;
                case KNIGHT:
                    piece = "♘";
                    break;
                case BISHOP:
                    piece = "♗";
                    break;
                case QUEEN:
                    piece = "♕";
                    break;
                case KING:
                    piece = "♔";
                    break;
                default:
                    piece = " ";
                    break;
                }
            }
            else if (game->board[i][j].color == BLACK)
            {
                fg= BLACK_PC;
                switch (game->board[i][j].type)
                {
                case PAWN:
                    piece = "♟";
                    break;
                case ROOK:
                    piece = "♜";
                    break;
                case KNIGHT:
                    piece = "♞";
                    break;
                case BISHOP:
                    piece = "♝";
                    break;
                case QUEEN:
                    piece = "♛";
                    break;
                case KING:
                    piece = "♚";
                    break;
                default:
                    piece = " ";
                    break;
                }
            }
            else
            {
                piece = " ";
            }
            
            printf("%s%s %s %s", backGround, fg, piece, RESET);
        }
        
        printf(" %d\n", 8-i);
    }
    
    printf("%s    A  B  C  D  E  F  G  H\n", indent);
    printf("%s", RESET);
    printf("\n\n\n\n");
}
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}
void pause(void)
{
    printf("Press Enter to Continue..");
    getchar();
}