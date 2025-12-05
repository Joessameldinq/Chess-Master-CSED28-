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
#include <time.h>
#ifdef _WIN32
#include <windows.h> //TO handle windows issues cmd
#endif
#define RED "\033[41m"
#define GREEN "\033[42m"
#define BOLD "\033[1m"
#define RESET "\033[0m"

void load_new_game(void);
void game_loop(Game *game);
void load_saved_game(void);
void show_saved_games(void);
void append_saved_games(char *fileName);

int main()
{
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); //To show the utf8 on the cmd
    #endif
    displayWelcome();
    pause();
    clearScreen();
    displayHelp();
    pause();
    clearScreen();
    displayMainMenu();

    int option;
    scanf("%d", &option);
    (void)getchar();

    switch (option)
    {
        case 1: load_new_game(); break;
        case 2: load_saved_game(); break;
        case 3: return 0;
        default: return 0;
    }

    return 0;
}

void load_new_game(void)
{
    Game *game_init = initGame();
    game_loop(game_init); // game_loop frees game_init
}

void game_loop(Game *game_init)
{
    char fileName[50] = {0};
    Node *gamestack = initializeStack();
    Node *redostack = initializeStack();

    // Push initial game state safely
    Game tempGame;
    memset(&tempGame, 0, sizeof(Game));
    tempGame = *game_init;
    push(&gamestack, tempGame);
    free(game_init);

    char *buffer = NULL;
    Position *pos = NULL;

    bool quit = false, restart = false, undo = false, redo = false;
    bool savedAs = false, saved = false;

    do
    {
        printf(saved ? GREEN BOLD "SAVED\n" RESET : RED BOLD "SAVED\n" RESET);

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

            // Commands
            if (strcasecmp(buffer, "saveas") == 0)
            {
                printf("Enter the configuration file name to save your gamestate: ");
                fgets(fileName, sizeof(fileName), stdin);
                fileName[strcspn(fileName, "\n")] = 0;
                append_saved_games(fileName);
                savedAs = true;

                FILE *fptr = fopen(fileName, "wb");
                if (fptr) { saveGame(fptr, game); fclose(fptr); }
                printf("Game saved as %s\n", fileName);
                pause();
                continue;
            }
            else if (strcasecmp(buffer, "save") == 0)
            {
                if (!savedAs)
                {
                    printf("Enter the configuration file name to save your gamestate: ");
                    fgets(fileName, sizeof(fileName), stdin);
                    fileName[strcspn(fileName, "\n")] = 0;
                    append_saved_games(fileName);
                    savedAs = true;
                }

                FILE *fptr = fopen(fileName, "wb");
                if (fptr) { saveGame(fptr, game); fclose(fptr); }
                printf("Game saved as %s\n", fileName);
                saved = true;
                pause();
                continue;
            }
            else if (strcasecmp(buffer, "quit") == 0) { quit = true; break; }
            else if (strcasecmp(buffer, "restart") == 0) { restart = true; break; }
            else if (strcasecmp(buffer, "help") == 0) { displayHelp(); pause(); clearScreen(); printBoard(game); printGameState(game); continue; }
            else if (strcasecmp(buffer, "undo") == 0 || strcmp(buffer,"u") == 0) { undo = true; break; }
            else if (strcasecmp(buffer, "redo") == 0 || strcmp(buffer,"r") == 0) { redo = true; break; }

            if (!validateInputFormat(buffer)) continue;

        } while (false);

        if (quit) break;

        if (restart)
        {
            printf("Game Restarted\n");
            pause();
            clearStack(&gamestack);
            clearStack(&redostack);

            Game *new_game = initGame();
            Game temp;
            memset(&temp, 0, sizeof(Game));
            temp = *new_game;
            push(&gamestack, temp);
            free(new_game);

            clearScreen();
            restart = false;
            continue;
        }

        if (undo)
        {
            if (undoMove(&gamestack, &redostack)) printf("Move undone\n"); else printf("Nothing to undo\n");
            pause(); clearScreen(); undo = false; continue;
        }

        if (redo)
        {
            if (redoMove(&gamestack, &redostack)) printf("Move redone\n"); else printf("Nothing to redo\n");
            pause(); clearScreen(); redo = false; continue;
        }

        if (pos != NULL) { free(pos); pos = NULL; }
        pos = parseMove(buffer);
        if (!pos) { printf("Error parsing move\n"); pause(); clearScreen(); continue; }

        Game newGame;
        memset(&newGame, 0, sizeof(Game));
        newGame = *game;

        Move move = { .initial = pos[0], .final = pos[1], .promotionPiece = EMPTY, .moveType = NORMAL_MOVE, .capturedPiece = {.type = EMPTY, .color = NONE, .hasMoved = false} };

        if (isValidMove(&newGame, move))
        {
            Piece movingPiece = newGame.board[move.initial.x][move.initial.y];
            if (movingPiece.type == PAWN)
            {
                int promotionRow = (movingPiece.color == WHITE) ? 0 : (BOARD_SIZE - 1);
                if (move.final.x == promotionRow) move.promotionPiece = getPromotion(movingPiece.color).type;
            }

            applyMove(&newGame, &move);
            saved = false;
            newGame.status = computeGameStatus(&newGame);
            clearStack(&redostack);
            push(&gamestack, newGame);
            clearScreen();

            if (fiftyMovesRule(&newGame) || isDeadPosition(&newGame) || inCheckMate(&newGame)) break;
        }
        else
        {
            printf("Invalid Move\n");
            pause(); clearScreen();
        }

    } while (gamestack != NULL && (gamestack->curGame.status == PLAYING || gamestack->curGame.status == CHECK));

    // Cleanup
    if (buffer) free(buffer);
    if (pos) free(pos);

    // Display final status
    if (gamestack != NULL)
    {
        clearScreen();
        printBoard(&(gamestack->curGame));

        switch (gamestack->curGame.status)
        {
            case CHECKMATE:
                printf("\n🏆 CHECKMATE! %s wins!\n", (gamestack->curGame.currentPlayer == WHITE) ? "BLACK" : "WHITE");
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

    clearStack(&gamestack);
    clearStack(&redostack);
}

void load_saved_game()
{
    show_saved_games();

    char f[256], fullPath[300];
    FILE *fptr = NULL;

    printf("Enter the saved game you want to continue (or 'quit' to exit):\n");

    while (1)
    {
        if (!fgets(f, sizeof(f), stdin)) continue;

        // Remove trailing newline
        f[strcspn(f, "\n")] = 0;

        // Allow user to quit
        if (strcasecmp(f, "quit") == 0) return;

        // Build full path
        if (strstr(f, ".bin") == NULL) snprintf(fullPath, sizeof(fullPath), "%s.bin", f);
        else strncpy(fullPath, f, sizeof(fullPath)-1);

        fptr = fopen(fullPath, "rb");
        if (!fptr)
        {
            printf("File not found! Enter a valid name or 'quit' to exit:\n");
            continue;
        }

        // Load game
        Game *game = malloc(sizeof(Game));
        if (!game)
        {
            printf("Memory allocation failed!\n");
            fclose(fptr);
            return;
        }
        memset(game, 0, sizeof(Game));
        if (!loadGame(fptr, game))
        {
            printf("Failed to load game from file!\n");
            free(game);
            fclose(fptr);
            continue;
        }

        fclose(fptr);
        game_loop(game);
        break;
    }
}
void show_saved_games()
{
    FILE *fptr = fopen("attach/saved_meta.txt", "r");
    if (!fptr) { printf("No Saved Games\n"); return; }

    char line[200];
    bool found = false;

    while (fgets(line, sizeof(line), fptr))
    {
        found = true;
        line[strcspn(line, "\n")] = 0;
        printf("%s\n", line);
    }

    if (!found) printf("No Saved Games\n");
    fclose(fptr);
}

void append_saved_games(char *fileName)
{
    FILE *fptr = fopen("attach/saved_meta.txt", "a");
    if (!fptr) { printf("Could not open saved_meta.txt!\n"); return; }

    time_t rawtime;
    time(&rawtime);
    char *time_string = ctime(&rawtime);
    time_string[strcspn(time_string, "\n")] = 0;

    fprintf(fptr, "%s %s\n", fileName, time_string);
    fclose(fptr);
}
