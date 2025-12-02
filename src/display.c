#include "../include/display.h"
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

void printGameState(Game *game) {
    printf("\n");
    printf("═══════════════════════════════════════════════════════════\n");
    printf("                    CHESS GAME STATUS\n");
    printf("═══════════════════════════════════════════════════════════\n");
    
    // Current player
    printf("Current Turn: %s\n", game->currentPlayer== WHITE ? "WHITE" : "BLACK");
    
    // Check status
    if(game->status == CHECK) {
        printf("⚠️  CHECK! %s King is under attack!\n", game->currentPlayer== WHITE ? "White" : "Black");
    }
    
    // Special moves indicator
    if(game->enPassentAvailable) {
        printf("ℹ️  En Passant available\n");
    }
    
    // if(pawnPromotionMade) {
    //     printf("👑 Pawn promoted to %c\n", pawnPromotedTo);
    //     pawnPromotionMade = false;
    // }
    
    // if(castlingMade) {
    //     printf("🏰 Castling performed!\n");
    //     castlingMade = false;
    // }
    
    // if(enpassentMade) {
    //     printf("⚔️  En Passant capture!\n");
    //     enpassentMade = false;
    // }
    
    printf("═══════════════════════════════════════════════════════════\n");
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
    printf("╚═══════════════════════════════════════════════════════════╝\n");
}
void displayWelcome(void) {
    clearScreen();
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
    printf("\n");
}
void printBoard(Game* game)
{
    setlocale(LC_CTYPE,"");
    
   
    const char *indent = "                              ";
    
    const char *bg_light = "\x1b[48;5;230m";
    const char *bg_dark  = "\x1b[48;5;64m";
    
    const char *fg_black = "\x1b[38;5;16m";
    const char *fg_white = "\x1b[38;5;231m";
    const char *reset    = "\x1b[0m";
    
    printf("\n\n\n");
    printf("%s    A  B  C  D  E  F  G  H\n", indent);
    
    for(int i = 0; i < 8; i++)
    {
        printf("%s%d ", indent, 8-i);
        
        for(int j = 0; j < 8; j++)
        {
            bool isWhiteSquare = ((i+j) % 2 == 0);
            const char *backGround = isWhiteSquare ? bg_light : bg_dark;
            const char *fg = fg_white;
            const char *piece;
            
            if(game->board[i][j].color == WHITE)
            {
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
            
            if (piece[0] == (unsigned char)0xE2 &&
                piece[1] == (unsigned char)0x99 &&
                (unsigned char)piece[2] >= 0x9A) {
                fg = fg_black;
            }
            
            printf("%s%s %s %s", backGround, fg, piece, reset);
        }
        
        printf(" %d\n", 8-i);
    }
    
    printf("%s    A  B  C  D  E  F  G  H\n", indent);
    printf("%s", reset);
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