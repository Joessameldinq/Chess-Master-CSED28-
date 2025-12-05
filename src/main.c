#include "../include/definitions.h"
#include "../include/game.h"
#include "../include/display.h"
#include "../include/end.h"
#include "../include/input.h"
#include "../include/move.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main()
{
    // 1. Start the music in the background (using mpg123)
    // system("mpg123 'Erik Satie - Gnossienne No.1.mp3' &");
   
    
    Game *game = initGame();
    displayWelcome();
    pause();
    clearScreen();
    displayHelp();
    pause();
    clearScreen();
    
    

    
    char *buffer = NULL;
    Position *pos = NULL;
    bool quit = false;
    bool restart = false;
    while (game->status == PLAYING || game->status == CHECK)
    {
        //Displaying Quotes
         FILE *fptr = fopen("attach/quotes.txt", "r");

        if (!fptr) {
            printf("Error: could not open quotes file!\n");
            return 1;
        }
        printQuotes(fptr);
        fclose(fptr);


        printBoard(game);
        printGameState(game);
        
        do
        {
            printf("Enter a move: ");
            
            // Free previous buffer if exists
            if (buffer != NULL)
            {
                free(buffer);
                buffer = NULL;
            }
            
            buffer = readInput();
            if(strcmp(buffer,"quit")==0 || strcmp(buffer,"Quit") == 0)
                {quit = true;
                break;}
            else if(strcmp(buffer,"restart")==0 || strcmp(buffer,"Restart") == 0)
                {restart = true;
                break;}
            else if(strcmp(buffer,"Help")==0 || strcmp(buffer,"help") == 0)
                {displayHelp();
                pause();
                }
            else
            {
                quit = false;
                restart = false;
            }
       
            
            if (buffer == NULL)
            {
                printf("Error reading input\n");
                continue;
            }
            
        } while (!validateInputFormat(buffer));
        if(quit)break;
        else if(restart)
        {
            printf("Game Restarted\n");
            free(game);
            game = initGame();
            continue;
        }
        // Free previous position if exists
        if (pos != NULL)
        {
            free(pos);
            pos = NULL;
        }
        
        pos = parseMove(buffer);
        
        if (pos == NULL)
        {
            printf("Error parsing move\n");
            continue;
        }
        
        printf("Parse move working\n");
        
        //   Initialize all fields of Move struct
        Move move = (Move){
            .initial = pos[0],
            .final = pos[1],
            .promotionPiece = EMPTY,  
            .moveType = NORMAL_MOVE,
            .capturedPiece = {.type = EMPTY, .color = NONE, .hasMoved = false}
        };
        printf("Before is Valid Move\n");
        
        if (isValidMove(game, move))
        {
            
            // Check if pawn promotion is needed
            Piece movingPiece = game->board[move.initial.x][move.initial.y];
            if (movingPiece.type == PAWN)
            {
                int promotionRow = (movingPiece.color == WHITE) ? 0 : (BOARD_SIZE - 1);
                if (move.final.x == promotionRow)
                {
                    // Ask for promotion piece
                    Piece promo = getPromotion(movingPiece.color);
                    move.promotionPiece = promo.type;
                }
            }
            
            applyMove(game, &move);
            game->status= computeGameStatus(game);
            if (fiftyMovesRule(game) || isDeadPosition(game) || inCheckMate(game))
            {
                break;
            }
        }
        else
        {
            printf("Invalid Move\n");
        }
        clearScreen();

    }
    
    // Cleanup
    if (buffer != NULL)
        free(buffer);
    if (pos != NULL)
        free(pos);
    
    // Display final game status
    clearScreen();
    printBoard(game);
    
    switch (game->status)
    {
        case CHECKMATE:
            printf("\n🏆 CHECKMATE! %s wins!\n", 
                   (game->currentPlayer == WHITE) ? "BLACK" : "WHITE");
            break;
        case STALEMATE:
            printf("\n🤝 STALEMATE! Game is a draw.\n");
            break;
        case DRAW_FIFTY_MOVE:
            printf("\n🤝 DRAW by fifty-move rule!\n");
            break;
        default:
            printf("\n🤝 Game ended in a draw.\n");
            break;
    }
    
    // Stop the music process
    // printf("Stopping music...\n");
    // system("pkill mpg123");
    
    free(game);
    return 0;
}