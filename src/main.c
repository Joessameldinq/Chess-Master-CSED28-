#include "../include/definitions.h"
#include "../include/game.h"
#include "../include/display.h"
#include "../include/end.h"
#include "../include/input.h"
#include "../include/move.h"
#include "../include/saving_loading.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int main()
{
    Game *game_init = initGame();
    Node *gamestack = initializeStack();
    Node *redostack = initializeStack();
    
    displayWelcome();
    pause();
    clearScreen();
    displayHelp();
    pause();
    clearScreen();
    
    // Push initial game state
    push(&gamestack, *game_init);
    free(game_init);  // We've copied it to stack, free the original
    
    char *buffer = NULL; //Moving dummy buffer
    Position *pos = NULL; // Moving dummy pos

    //Game flow flags
    bool quit = false;
    bool restart = false;
    bool undo = false;
    bool redo = false;

    do
    {
        // Work with current top of stack
        if (gamestack == NULL) break;
        
        Game *game = &(gamestack->curGame);
            
        // Display quotes
        FILE *fptr = fopen("attach/quotes.txt", "r");
        if (fptr) {
            printQuotes(fptr);
            fclose(fptr);
        }

        printBoard(game);
        printGameState(game);
        
        do
        {
            printf("Enter a move: ");
            
            if (buffer != NULL)
            {
                free(buffer);
                buffer = NULL;
            }
            
            buffer = readInput();
            
            // Handle commands
            if(strcmp(buffer, "quit") == 0 || strcmp(buffer, "Quit") == 0)
            {
                quit = true;
                break;
            }
            else if(strcmp(buffer, "restart") == 0 || strcmp(buffer, "Restart") == 0)
            {
                restart = true;
                break;
            }
            else if(strcmp(buffer, "Help") == 0 || strcmp(buffer, "help") == 0)
            {
                displayHelp();
                pause();
                clearScreen();
                printBoard(game);
                printGameState(game);
                continue;  //To validate input format in the next screen
            }
            else if(strcmp(buffer, "Undo") == 0 || strcmp(buffer, "U") == 0 || strcmp(buffer, "u") == 0)
            {
                undo = true;
                break;
            }
            else if(strcmp(buffer, "Redo") == 0 || strcmp(buffer, "R") == 0 || strcmp(buffer, "r") == 0)
            {
                redo = true;
                break;
            }
            
            if (buffer == NULL)
            {
                printf("Error reading input\n");
                continue;
            }
            
        } while (!validateInputFormat(buffer)); //Works only with a2-a4 format the rest are handled using continue and break and flags
        
        if(quit)
            {quit =false; 
            break;}
        
        if(restart)
        {
            printf("Game Restarted\n");
            pause(); //Print a pause message to show that the game has restarted
            // Clear both stacks
            clearStack(&gamestack);
            clearStack(&redostack);
            // Reinitialize the game
            Game *new_game = initGame();
            chooseFirstPlayerToBegin(new_game);
            push(&gamestack, *new_game);
            free(new_game);
            clearScreen();
            restart = false;
            continue;
        }
        
        if(undo)
        {
            if(undoMove(&gamestack, &redostack))
            {
                printf("Move undone\n");
                pause();
            }
            else
            {
                printf("Nothing to undo\n");
                pause();
            }
            clearScreen();
            undo = false;
            continue;
        }
        
        if(redo)
        {
            if(redoMove(&gamestack, &redostack))
            {
                printf("Move redone\n");
                pause();
            }
            else
            {
                printf("Nothing to redo\n");
                pause();
            }
            clearScreen();
            redo = false;
            continue;
        }
        
        // Parse and validate move
        if (pos != NULL)
        {
            free(pos);
            pos = NULL;
        }
        
        pos = parseMove(buffer);
        
        if (pos == NULL)
        {
            printf("Error parsing move\n");
            pause();
            clearScreen();
            continue;
        }
        
        // Create a copy of current game to modify
        // The most important part in our game flow
        // if we don't have this we will have in the beginning of loop
        // Game stack ----> Stack[0]
        // After modifying game ----->Stack[1]
        // And after pushing it will be Stack[1]------>Stack[1]
        // So we need to save a backup of the last snapshot of the game
        Game newGame = *game;
        
        Move move = (Move){
            .initial = pos[0],
            .final = pos[1],
            .promotionPiece = EMPTY,  
            .moveType = NORMAL_MOVE,
            .capturedPiece = {.type = EMPTY, .color = NONE, .hasMoved = false}
        };
        
        if (isValidMove(&newGame, move))
        {
            // Check for pawn promotion
            Piece movingPiece = newGame.board[move.initial.x][move.initial.y];
            if (movingPiece.type == PAWN)
            {
                int promotionRow = (movingPiece.color == WHITE) ? 0 : (BOARD_SIZE - 1);
                if (move.final.x == promotionRow)
                {
                    Piece promo = getPromotion(movingPiece.color);
                    move.promotionPiece = promo.type;
                }
            }
            
            applyMove(&newGame, &move);
            newGame.status = computeGameStatus(&newGame);
            
            // Clear redo stack when new move is made
            clearStack(&redostack);
            
            // Push new state
            push(&gamestack, newGame);
            
            clearScreen();
            
            // Check for game end
            if (fiftyMovesRule(&newGame) || isDeadPosition(&newGame) || inCheckMate(&newGame))
            {
                break;
            }
        }
        else
        {
            printf("Invalid Move\n");
            pause();
            clearScreen();
        }
        
    } while(gamestack != NULL && 
            (gamestack->curGame.status == PLAYING || gamestack->curGame.status == CHECK));
    
    // Cleanup
    if (buffer != NULL) free(buffer);
    if (pos != NULL) free(pos);
    
    // Display final game status
    if (gamestack != NULL)
    {
        clearScreen();
        printBoard(&(gamestack->curGame));
        
        switch (gamestack->curGame.status)
        {
            case CHECKMATE:
                printf("\n🏆 CHECKMATE! %s wins!\n", 
                       (gamestack->curGame.currentPlayer == WHITE) ? "BLACK" : "WHITE");
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
    }
    
    // Proper cleanup - free all nodes in the stack
    clearStack(&gamestack);
    clearStack(&redostack);
    
    return 0;
}