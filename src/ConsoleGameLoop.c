#include "../include/GameDefinitions.h"
#include "../include/DisplayConsole.h"
#include "../include/InputConsole.h"
#include "../include/GameInitialization.h"
#include "../include/MovingLogic.h"
#include "../include/EndingConditions.h"
#include "../include/SavingLoading.h"
#include "../include/StackHelpers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

// Function prototypes
void runMainMenu(void);
void playGame(Game *game, Node **gameStack, Node **redoStack);
bool processMoveInput(Game *game, char *input, Node **gameStack, Node **redoStack);
void handleGameEnd(Game *game);

int main(void)
{
    displayWelcome();
    pause();
    runMainMenu();
    
    printf("\nThank you for playing Chess!\n");
    return 0;
}

/**
 * Main menu handler
 */
void runMainMenu(void)
{
    bool running = true;
    
    while (running)
    {
        clearScreen();
        displayMainMenu();
        
        char *input = readInput();
        if (!input)
        {
            fprintf(stderr, "Error reading input\n");
            continue;
        }
        
        switch (input[0])
        {
            case '1': // Play new game
            {
                Game *game = initGame();
                if (game)
                {
                    Node *gameStack = initializeStack();
                    Node *redoStack = initializeStack();
                    
                    // Push initial game state to stack
                    push(&gameStack, *game);
                    
                    playGame(game, &gameStack, &redoStack);
                    
                    // Cleanup
                    clearStack(&gameStack);
                    clearStack(&redoStack);
                    free(game);
                }
                else
                {
                    fprintf(stderr, "Failed to initialize game\n");
                }
                break;
            }
            
            case '2': // Load saved game
            {
                printf("Enter save file name: ");
                char *filename = readInput();
                if (filename)
                {
                    FILE *fp = fopen(filename, "rb");
                    if (fp)
                    {
                        Game *game = (Game*)malloc(sizeof(Game));
                        if (game && loadGame(fp, game))
                        {
                            fclose(fp);
                            printf("Game loaded successfully!\n");
                            pause();
                            
                            Node *gameStack = initializeStack();
                            Node *redoStack = initializeStack();
                            push(&gameStack, *game);
                            
                            playGame(game, &gameStack, &redoStack);
                            
                            clearStack(&gameStack);
                            clearStack(&redoStack);
                            free(game);
                        }
                        else
                        {
                            if (fp) fclose(fp);
                            printf("Failed to load game.\n");
                            pause();
                        }
                    }
                    else
                    {
                        printf("Failed to open file.\n");
                        pause();
                    }
                    free(filename);
                }
                break;
            }
            
            case '3': // Quit
                printf("Are you sure you want to quit? (y/n): ");
                char *confirm = readInput();
                if (confirm && tolower(confirm[0]) == 'y')
                {
                    running = false;
                }
                if (confirm) free(confirm);
                break;
                
            default:
                printf("Invalid selection! Please enter 1, 2, or 3.\n");
                pause();
                break;
        }
        
        free(input);
    }
}

/**
 * Main game loop
 */
void playGame(Game *game, Node **gameStack, Node **redoStack)
{
    if (!game) return;
    
    bool gameRunning = true;
    
    while (gameRunning)
    {
        // Update game status
        game->status = computeGameStatus(game);
        
        clearScreen();
        printBoard(game);
        printGameState(game);
        
        // Check for game ending conditions
        if (game->status == CHECKMATE)
        {
            handleGameEnd(game);
            break;
        }
        else if (game->status == STALEMATE)
        {
            printf("\n=== STALEMATE! Game ends in a draw. ===\n");
            handleGameEnd(game);
            break;
        }
        else if (game->status == DRAW_INSUFFICIENT_MATERIAL)
        {
            printf("\n=== DRAW! Insufficient material to checkmate. ===\n");
            handleGameEnd(game);
            break;
        }
        else if (game->status == DRAW_FIFTY_MOVE)
        {
            printf("\n=== DRAW! 50-move rule. ===\n");
            handleGameEnd(game);
            break;
        }
        
        // Display whose turn it is
        printf("\n%s's turn\n", game->currentPlayer == WHITE ? "WHITE" : "BLACK");
        printf("Enter move (e.g., e2e4) or command (save/load/undo/redo/help/quit): ");
        
        // Read input
        char *input = readInput();
        if (!input)
        {
            fprintf(stderr, "Error reading input\n");
            continue;
        }
        
        // Convert to lowercase for command checking
        char *lowerInput = (char*)malloc(strlen(input) + 1);
        if (!lowerInput)
        {
            free(input);
            continue;
        }
        
        for (int i = 0; input[i]; i++)
        {
            lowerInput[i] = tolower(input[i]);
        }
        lowerInput[strlen(input)] = '\0';
        
        // Check for special commands
        if (strcmp(lowerInput, "quit") == 0 || strcmp(lowerInput, "q") == 0)
        {
            printf("Are you sure you want to quit? (y/n): ");
            char *confirm = readInput();
            if (confirm && tolower(confirm[0]) == 'y')
            {
                printf("Do you want to save before quitting? (y/n): ");
                char *saveConfirm = readInput();
                if (saveConfirm && tolower(saveConfirm[0]) == 'y')
                {
                    printf("Enter filename to save: ");
                    char *filename = readInput();
                    if (filename)
                    {
                        FILE *fp = fopen(filename, "wb");
                        if (fp)
                        {
                            if (saveGame(fp, game))
                            {
                                printf("Game saved successfully!\n");
                            }
                            else
                            {
                                printf("Failed to save game.\n");
                            }
                            fclose(fp);
                        }
                        else
                        {
                            printf("Failed to create save file.\n");
                        }
                        free(filename);
                    }
                    pause();
                }
                if (saveConfirm) free(saveConfirm);
                gameRunning = false;
            }
            if (confirm) free(confirm);
        }
        else if (strcmp(lowerInput, "save") == 0 || strcmp(lowerInput, "s") == 0)
        {
            printf("Enter filename to save: ");
            char *filename = readInput();
            if (filename)
            {
                FILE *fp = fopen(filename, "wb");
                if (fp)
                {
                    if (saveGame(fp, game))
                    {
                        printf("Game saved successfully!\n");
                    }
                    else
                    {
                        printf("Failed to save game.\n");
                    }
                    fclose(fp);
                }
                else
                {
                    printf("Failed to create save file.\n");
                }
                free(filename);
            }
            pause();
        }
        else if (strcmp(lowerInput, "load") == 0 || strcmp(lowerInput, "l") == 0)
        {
            printf("Warning: Current game will be lost. Continue? (y/n): ");
            char *confirm = readInput();
            if (confirm && tolower(confirm[0]) == 'y')
            {
                printf("Enter filename to load: ");
                char *filename = readInput();
                if (filename)
                {
                    FILE *fp = fopen(filename, "rb");
                    if (fp)
                    {
                        if (loadGame(fp, game))
                        {
                            fclose(fp);
                            printf("Game loaded successfully!\n");
                            
                            // Clear stacks and push new state
                            clearStack(gameStack);
                            clearStack(redoStack);
                            push(gameStack, *game);
                        }
                        else
                        {
                            fclose(fp);
                            printf("Failed to load game.\n");
                        }
                    }
                    else
                    {
                        printf("Failed to open file.\n");
                    }
                    free(filename);
                }
            }
            if (confirm) free(confirm);
            pause();
        }
        else if (strcmp(lowerInput, "undo") == 0 || strcmp(lowerInput, "u") == 0)
        {
            if (undoMove(gameStack, redoStack))
            {
                // Get current state from stack (peek without pop)
                if (*gameStack)
                {
                    *game = (*gameStack)->curGame;
                    printf("Move undone!\n");
                }
            }
            else
            {
                printf("No moves to undo!\n");
            }
            pause();
        }
        else if (strcmp(lowerInput, "redo") == 0 || strcmp(lowerInput, "r") == 0)
        {
            if (redoMove(gameStack, redoStack))
            {
                // Get current state from stack (peek without pop)
                if (*gameStack)
                {
                    *game = (*gameStack)->curGame;
                    printf("Move redone!\n");
                }
            }
            else
            {
                printf("No moves to redo!\n");
            }
            pause();
        }
        else if (strcmp(lowerInput, "help") == 0 || strcmp(lowerInput, "h") == 0)
        {
            displayHelp();
            pause();
        }
        else
        {
            // Process move input
            if (processMoveInput(game, input, gameStack, redoStack))
            {
                // Move was successful - game state already updated in applyMove
                printf("Move executed successfully!\n");
                pause();
            }
            else
            {
                printf("Invalid move! Try again.\n");
                pause();
            }
        }
        
        free(lowerInput);
        free(input);
    }
}

/**
 * Process move input and execute if valid
 */
bool processMoveInput(Game *game, char *input, Node **gameStack, Node **redoStack)
{
    if (!input || !game) return false;
    
    // Validate input format
    if (!validateInputFormat(input))
    {
        printf("Invalid input format! Use format like: e2e4\n");
        return false;
    }
    
    // Parse move
    Position *positions = parseMove(input);
    if (!positions)
    {
        printf("Failed to parse move.\n");
        return false;
    }
    
    Position from = positions[0];
    Position to = positions[1];
    free(positions);
    
    // Create move structure
    Move move = {
        .initial = from,
        .final = to,
        .promotionPiece = EMPTY
    };
    
    // Check for pawn promotion (if pawn reaches last rank)
    Piece piece = game->board[from.x][from.y];
    if (piece.type == PAWN)
    {
        bool isPromotionRank = (piece.color == WHITE && to.x == 0) || 
                               (piece.color == BLACK && to.x == 7);
        if (isPromotionRank)
        {
            // Ask for promotion piece
            Piece promotionPiece = getPromotion(piece.color);
            move.promotionPiece = promotionPiece.type;
        }
    }
    
    // Validate move
    if (!isValidMove(game, move))
    {
        printf("Illegal move!\n");
        
        // Give helpful feedback
        if (piece.type == EMPTY)
        {
            printf("There is no piece at that position.\n");
        }
        else if (piece.color != game->currentPlayer)
        {
            printf("That's not your piece!\n");
        }
        else
        {
            printf("That piece cannot move there.\n");
        }
        
        return false;
    }
    
    // Execute the move
    applyMove(game, &move);
    
    // Push new state to stack and clear redo stack
    push(gameStack, *game);
    clearStack(redoStack);
    
    printf("Move: %c%d -> %c%d\n", 
           'a' + from.y, 8 - from.x,
           'a' + to.y, 8 - to.x);
    
    return true;
}

/**
 * Handle game ending conditions
 */
void handleGameEnd(Game *game)
{
    if (!game) return;
    
    printf("\n");
    printf("||=============================================================||\n");
    printf("||                        GAME OVER                            ||\n");
    printf("||=============================================================||\n");
    
    if (game->status == CHECKMATE)
    {
        Color winner = (game->currentPlayer == WHITE) ? BLACK : WHITE;
        printf("||                     CHECKMATE!                              ||\n");
        printf("||                  %s WINS THE GAME!                      ||\n", 
               winner == WHITE ? "WHITE" : "BLACK");
    }
    else if (game->status == STALEMATE)
    {
        printf("||                     STALEMATE!                              ||\n");
        printf("||                   GAME ENDS IN DRAW                         ||\n");
    }
    else if (game->status == DRAW_INSUFFICIENT_MATERIAL)
    {
        printf("||              DRAW - INSUFFICIENT MATERIAL!                  ||\n");
        printf("||                   GAME ENDS IN DRAW                         ||\n");
    }
    else if (game->status == DRAW_FIFTY_MOVE)
    {
        printf("||                 DRAW - 50 MOVE RULE!                        ||\n");
        printf("||                   GAME ENDS IN DRAW                         ||\n");
    }
    
    printf("||=============================================================||\n");
    
    printf("\nDo you want to save this game? (y/n): ");
    char *saveInput = readInput();
    if (saveInput && tolower(saveInput[0]) == 'y')
    {
        printf("Enter filename: ");
        char *filename = readInput();
        if (filename)
        {
            FILE *fp = fopen(filename, "wb");
            if (fp)
            {
                if (saveGame(fp, game))
                {
                    printf("Game saved successfully!\n");
                }
                else
                {
                    printf("Failed to save game.\n");
                }
                fclose(fp);
            }
            else
            {
                printf("Failed to create save file.\n");
            }
            free(filename);
        }
    }
    if (saveInput) free(saveInput);
    
    pause();
<<<<<<< HEAD

    return 0;
=======
>>>>>>> Abdelwahab-branch
}