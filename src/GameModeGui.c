

#include "../include/GameModeGui.h"
#include "../include/GuiDefinitions.h"
#include "../include/GuiHelpers.h"
#include "../include/GameDefinitions.h"
#include "../include/GameInitialization.h"
#include "../include/SavingLoading.h"
#include "../include/EndingConditions.h"
#include "../include/StackHelpers.h"
#include "../include/InputConsole.h"
#include "../include/DisplayConsole.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <SDL2/SDL_mixer.h>


int g_squareSize = 0;
int g_boardOffset = 0;



//  helper function to get the file name of the piece type  to load textures
const char* getPieceTypeName(PieceType type)
{
    switch(type)
    {
        case PAWN:   return "pawn";
        case KING:   return "king";
        case QUEEN:  return "queen";
        case BISHOP: return "bishop";
        case ROOK:   return "rook";
        case KNIGHT: return "knight";
        default:     return "";
    }
}


//  INIT GUI  --> grid , pieces , capturedpieces and buttons and has the feature of check valid textures
gamegui *initGameScreenGui(SDL_Renderer *renderer, Game *initialGame)
{
    //
    if (!renderer) {
        fprintf(stderr, "Error: NULL renderer passed to initGameScreenGui\n");
        return NULL;
    }
    


    //
    if (!initialGame) {
        fprintf(stderr, "Error: NULL game passed to initGameScreenGui\n");
        return NULL;
    }




    //
    gamegui *gui = calloc(1, sizeof(gamegui)); //allocate and initialize with 0
    if (!gui) {
        fprintf(stderr, "Error: Failed to allocate memory for GUI\n");
        return NULL;
    }
    
    // Initialize dynamic dimensions from current window size
    SDL_Window *window = SDL_RenderGetWindow(renderer);
    if (window) {
        SDL_GetWindowSize(window, &gui->windowWidth, &gui->windowHeight);
    } else {
        //Defaults
        gui->windowWidth = WINDOW_WIDTH;
        gui->windowHeight = WINDOW_HEIGHT;
    }
    gui->boardOffset = (int)(gui->windowWidth * 0.04);
    gui->squareSize = (int)(gui->windowHeight * 0.086);
    
    // Update globals
    g_squareSize = gui->squareSize;
    g_boardOffset = gui->boardOffset;

    //==========================================



    // Load textures with error checking
    gui->background = loadtexture("assets/ourfinalbg.jpg", renderer);
    if (!gui->background) {
        fprintf(stderr, "Warning: Failed to load background texture\n");
    }
    


    gui->undo.texture = loadtexture("assets/Undo.png", renderer);
    if (!gui->undo.texture) {
        fprintf(stderr, "Warning: Failed to load undo button texture\n");
    }
    

    
    gui->redo.texture = loadtexture("assets/Redo.png", renderer);
    if (!gui->redo.texture) {
        fprintf(stderr, "Warning: Failed to load redo button texture\n");
    }
    
    gui->save.texture = loadtexture("assets/Save.png", renderer);
    if (!gui->save.texture) {
        fprintf(stderr, "Warning: Failed to load save button texture\n");
    }
    
    gui->back.texture = loadtexture("assets/Back.png", renderer);
    if (!gui->back.texture) {
        fprintf(stderr, "Warning: Failed to load back button texture\n");
    }
    gui->drawAgreement.texture = loadtexture("assets/Draw.png",renderer);
    if(!gui->drawAgreement.texture)
    {
        fprintf(stderr,"Warning: Failed to load the draw agreement button texture\n");
    }
    
    gui->capBlack.texture = loadtexture("assets/capblack.png", renderer);
    if (!gui->capBlack.texture) {
        fprintf(stderr, "Warning: Failed to load black capture button texture\n");
    }
    
    gui->capWhite.texture = loadtexture("assets/capwhite.png", renderer);
    if (!gui->capWhite.texture) {
        fprintf(stderr, "Warning: Failed to load white capture button texture\n");
    }

    // Initialize button rectangles - organized layout with responsive sizing
    int bw = BUTTON_WIDTH/2;
    int bh = BUTTON_HEIGHT/2;
    int button_start_y = WINDOW_HEIGHT * 0.95;
    int button_spacing = BUTTON_SPACING;
    int margin = WINDOW_WIDTH * 0.02;  // 2% margin
    
    gui->undo.rect = (SDL_Rect){margin, button_start_y - bh * 1.2, bw, bh};
    gui->redo.rect = (SDL_Rect){margin + bw + button_spacing, button_start_y - bh * 1.2, bw, bh};
    gui->save.rect = (SDL_Rect){margin + (bw + button_spacing) * 2, button_start_y - bh * 1.2, bw, bh};
    gui->back.rect = (SDL_Rect){margin + (bw + button_spacing) * 3, button_start_y - bh * 1.2, bw, bh};
    gui->drawAgreement.rect = (SDL_Rect){margin + (bw + button_spacing) * 4,button_start_y - bh * 1.2 , bw ,bh};
    gui->capWhite.rect = (SDL_Rect){WINDOW_WIDTH - BUTTON_WIDTH - WINDOW_WIDTH * 0.20 , WINDOW_HEIGHT * 0.05, bw, bh};
    gui->capBlack.rect = (SDL_Rect){WINDOW_WIDTH - BUTTON_WIDTH - WINDOW_WIDTH * 0.00, WINDOW_HEIGHT * 0.05, bw, bh};
    
    gui->currentTurn.rect = (SDL_Rect){margin + 8 * g_squareSize / 2 - bw / 2, WINDOW_HEIGHT * 0.80, bw,bh};

    gui->whiteTurnTex = loadtexture("assets/whiteturn.png", renderer);
    if (!gui->whiteTurnTex) {
        fprintf(stderr, "Warning: Failed to load white turn texture\n");
    }
    
    gui->blackTurnTex = loadtexture("assets/blackturn.png", renderer);
    if (!gui->blackTurnTex) {
        fprintf(stderr, "Warning: Failed to load black turn texture\n");
    }

    // Set initial turn indicator based on current player
    gui->currentTurn.texture = (initialGame->currentPlayer == WHITE) ? 
                                gui->whiteTurnTex : gui->blackTurnTex;

    // Init board pieces
    int whiteCnt = 0, blackCnt = 0;
    for(int row=0; row<BOARD_SIZE; row++)
    {
        for(int col=0; col<BOARD_SIZE; col++)
        {
            Piece piece = initialGame->board[row][col];
            if(piece.type == EMPTY) continue;

            char filename[128];
            snprintf(filename, sizeof(filename), 
                             "assets/Pieces/%s_%s.png",
                             (piece.color==WHITE ? "w" : "b"), 
                             getPieceTypeName(piece.type));
            SDL_Texture *txt = loadtexture(filename, renderer);
            if(!txt) {
                fprintf(stderr, "Warning: Failed to load texture: %s\n", filename);
                continue;
            }

            SDL_Rect rc = { col*g_squareSize + BOARDOFFSET, row*g_squareSize + BOARDOFFSET, g_squareSize, g_squareSize };

            if(piece.color == WHITE)
            {
                if (whiteCnt >= 16) {
                    fprintf(stderr, "Error: Too many white pieces (max 16)\n");
                    SDL_DestroyTexture(txt);
                    continue;
                }
                gui->boardPieces[row][col].texture = txt;
                gui->boardPieces[row][col].rect = rc;
                whiteCnt++;
            }
            else
            {
                if (blackCnt >= 16) {
                    fprintf(stderr, "Error: Too many black pieces (max 16)\n");
                    SDL_DestroyTexture(txt);
                    continue;
                }
                gui->boardPieces[row][col].texture = txt;
                gui->boardPieces[row][col].rect = rc;
                blackCnt++;
            }
        }
    }

    // Init captured pieces and textures 
    int capWhiteCnt = 0, capBlackCnt = 0;
    for(int i=0; i<16; i++)
    {
        if(initialGame->capturedWhitePieces[i].type != EMPTY)
        {
            char filename[128];
            snprintf(filename, sizeof(filename), 
                             "assets/Pieces/w_%s.png", 
                             getPieceTypeName(initialGame->capturedWhitePieces[i].type));
            

            
            SDL_Texture *txt = loadtexture(filename, renderer);
            if(!txt) {
                fprintf(stderr, "Warning: Failed to load captured white piece: %s\n", filename);
                continue;
            }
            
            if (capWhiteCnt >= 16) {
                fprintf(stderr, "Error: Too many captured white pieces\n");
                SDL_DestroyTexture(txt);
                break;
            }
            
            gui->capturedPieces[0][capWhiteCnt].texture = txt;
            unsigned short col = i % 2;
            unsigned short row = i / 2;
            unsigned short baseXWhite = WINDOW_WIDTH - BUTTON_WIDTH - WINDOW_WIDTH * 0.20;
            unsigned short baseYWhite = WINDOW_HEIGHT * 0.15;
            int piece_size = g_squareSize * 0.95;
            int piece_spacing = g_squareSize * 1.00;
            gui->capturedPieces[0][capWhiteCnt].rect = (SDL_Rect){baseXWhite + col * piece_spacing, baseYWhite + row * piece_spacing, piece_size, piece_size};
            capWhiteCnt++;
        }
        
        if(initialGame->capturedBlackPieces[i].type != EMPTY)
        {
            char filename[128];
            snprintf(filename, sizeof(filename), 
                             "assets/Pieces/b_%s.png", 
                             getPieceTypeName(initialGame->capturedBlackPieces[i].type));
            SDL_Texture *txt = loadtexture(filename, renderer);
            if(!txt) {
                fprintf(stderr, "Warning: Failed to load captured black piece: %s\n", filename);
                continue;
            }
            
            if (capBlackCnt >= 16) {
                fprintf(stderr, "Error: Too many captured black pieces\n");
                SDL_DestroyTexture(txt);
                break;
            }
            
            gui->capturedPieces[1][capBlackCnt].texture = txt;
            unsigned short col = i % 2;
            unsigned short row = i / 2;
            unsigned short baseXBlack = WINDOW_WIDTH - BUTTON_WIDTH - WINDOW_WIDTH * 0.00;
            unsigned short baseYBlack = WINDOW_HEIGHT * 0.15;
            int piece_size = g_squareSize * 0.95;
            int piece_spacing = g_squareSize * 1.00;
            gui->capturedPieces[1][capBlackCnt].rect = (SDL_Rect){baseXBlack + col * piece_spacing, baseYBlack + row * piece_spacing, piece_size, piece_size};
            capBlackCnt++;
        }
    }

    // Load file labels (A-H)
    char labels[8] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
    for (int i = 0; i < 8; i++) {
        char filename[64];
        snprintf(filename, sizeof(filename), "assets/%c.png", labels[i]);
        gui->fileLabelTextures[i] = loadtexture(filename, renderer);
        if (!gui->fileLabelTextures[i]) {
            fprintf(stderr, "Warning: Failed to load file label texture: %s\n", filename);
        }
    }

    // Load rank labels (1-8)
    for (int i = 0; i < 8; i++) {
        char filename[64];
        snprintf(filename, sizeof(filename), "assets/%d.png", i + 1);
        gui->rankLabelTextures[i] = loadtexture(filename, renderer);
        if (!gui->rankLabelTextures[i]) {
            fprintf(stderr, "Warning: Failed to load rank label texture: %s\n", filename);
        }
    }


    //No moves in the beginning
    gui->fromMovingRect = (SDL_Rect){-1,-1,g_squareSize,g_squareSize};
    gui->toMovingRect = (SDL_Rect){-1,-1,g_squareSize,g_squareSize};


    // Load font once for move notation
    gui->moveFont = TTF_OpenFont("assets/fonts/SuperMaples-2vR2w.ttf", 22);
    if (!gui->moveFont) {
        fprintf(stderr, "Warning: Failed to load move font: %s\n", TTF_GetError());
    }

    //Initialize Sound Effects
    gui->sEffect.castling = Mix_LoadWAV("assets/eff/castle.mp3");
    if(!gui->sEffect.castling)
        fprintf(stderr, "Warning: Failed to load a sound effect  : %s\n", Mix_GetError());
    gui->sEffect.stalemate = Mix_LoadWAV("assets/eff/smate.wav");
    if(!gui->sEffect.stalemate)
        fprintf(stderr, "Warning: Failed to load a sound effect  : %s\n", Mix_GetError());
    gui->sEffect.check = Mix_LoadWAV("assets/eff/check.wav");
    if(!gui->sEffect.check)
        fprintf(stderr, "Warning: Failed to load a sound effect  : %s\n", Mix_GetError());
    gui->sEffect.checkmate = Mix_LoadWAV("assets/eff/mate.wav");
    if(!gui->sEffect.checkmate)
        fprintf(stderr, "Warning: Failed to load a sound effect  : %s\n", Mix_GetError());
    gui->sEffect.enpassent = Mix_LoadWAV("assets/eff/enpassant.wav");
    if(!gui->sEffect.enpassent)
        fprintf(stderr, "Warning: Failed to load a sound effect  : %s\n", Mix_GetError());
    gui->sEffect.illegalMove = Mix_LoadWAV("assets/eff/illegal.mp3");
    if(!gui->sEffect.illegalMove)
        fprintf(stderr, "Warning: Failed to load a sound effect  : %s\n", Mix_GetError());
    gui->sEffect.normalMove = Mix_LoadWAV("assets/eff/move-self.mp3");
    if(!gui->sEffect.normalMove)
        fprintf(stderr, "Warning: Failed to load a sound effect  : %s\n", Mix_GetError());
    gui->sEffect.capture = Mix_LoadWAV("assets/eff/capture.mp3");
    if(!gui->sEffect.capture)
        fprintf(stderr, "Warning: Failed to load a sound effect  : %s\n", Mix_GetError());
    gui->sEffect.promotion =Mix_LoadWAV("assets/eff/promote.mp3");
    if(!gui->sEffect.promotion)
        fprintf(stderr, "Warning: Failed to load a sound effect  : %s\n", Mix_GetError());
    gui->sEffect.draw  = Mix_LoadWAV("assets/eff/draw.wav");
    if(!gui->sEffect.draw)
        fprintf(stderr, "Warning: Failed to load a sound effect  : %s\n", Mix_GetError());
    gui->sEffect.drawOffer  = Mix_LoadWAV("assets/eff/drawoffer.mp3");
    if(!gui->sEffect.drawOffer)
        fprintf(stderr, "Warning: Failed to load a sound effect  : %s\n", Mix_GetError());

    //Initialize the threaten king
    //Important in the load not in initializing a new game
    if(initialGame->status == CHECK)
    {
        Position kingThreatened = findKingPosition(initialGame,initialGame->currentPlayer);
        gui->kingThreaten = (SDL_Rect){.x = kingThreatened.y*g_squareSize + BOARDOFFSET , .y =kingThreatened.x *g_squareSize + BOARDOFFSET ,g_squareSize,g_squareSize};
    }



    gui->lastMove.texture = NULL;
    return gui;
}

//  DESTROY GUI 
void destroyGameScreenGui(gamegui *gui)
{
    if(!gui) return;

    if (gui->background) SDL_DestroyTexture(gui->background);
    if (gui->save.texture) SDL_DestroyTexture(gui->save.texture);
    if (gui->undo.texture) SDL_DestroyTexture(gui->undo.texture);
    if (gui->redo.texture) SDL_DestroyTexture(gui->redo.texture);
    if (gui->back.texture) SDL_DestroyTexture(gui->back.texture);
    if (gui->capWhite.texture) SDL_DestroyTexture(gui->capWhite.texture);
    if (gui->capBlack.texture) SDL_DestroyTexture(gui->capBlack.texture);
    if (gui->currentTurn.texture) SDL_DestroyTexture(gui->currentTurn.texture);
    if (gui->currentGameState.texture) SDL_DestroyTexture(gui->currentGameState.texture);
    if (gui->whiteTurnTex) SDL_DestroyTexture(gui->whiteTurnTex);
    if (gui->blackTurnTex) SDL_DestroyTexture(gui->blackTurnTex);
    if(gui->drawAgreement.texture) SDL_DestroyTexture(gui->drawAgreement.texture);
    if (gui->lastMove.texture) SDL_DestroyTexture(gui->lastMove.texture);

    for(int r=0; r<BOARD_SIZE; r++)
    {
        for(int c=0; c<BOARD_SIZE; c++)
        {
            if (gui->boardPieces[r][c].texture) 
                SDL_DestroyTexture(gui->boardPieces[r][c].texture);
        }
    }
    
    for(int c=0; c<2; c++)
    {
        for(int i=0; i<16; i++)
        {
            if (gui->capturedPieces[c][i].texture) 
                SDL_DestroyTexture(gui->capturedPieces[c][i].texture);
        }
    }

    // Destroy label textures
    for (int i = 0; i < 8; i++) {
        if (gui->fileLabelTextures[i]) 
            SDL_DestroyTexture(gui->fileLabelTextures[i]);
        if (gui->rankLabelTextures[i]) 
            SDL_DestroyTexture(gui->rankLabelTextures[i]);
    }

    //Destroy the sound effects
    if (gui->sEffect.capture) Mix_FreeChunk(gui->sEffect.capture);
    if (gui->sEffect.castling) Mix_FreeChunk(gui->sEffect.castling);
    if (gui->sEffect.check) Mix_FreeChunk(gui->sEffect.check);
    if (gui->sEffect.enpassent) Mix_FreeChunk(gui->sEffect.enpassent);
    if (gui->sEffect.checkmate) Mix_FreeChunk(gui->sEffect.checkmate);
    if (gui->sEffect.normalMove) Mix_FreeChunk(gui->sEffect.normalMove);
    if (gui->sEffect.promotion) Mix_FreeChunk(gui->sEffect.promotion);
    if (gui->sEffect.stalemate) Mix_FreeChunk(gui->sEffect.stalemate);
    if (gui->sEffect.draw) Mix_FreeChunk(gui->sEffect.draw);
    if (gui->sEffect.drawOffer) Mix_FreeChunk(gui->sEffect.drawOffer);

    // Destroy font
    if (gui->moveFont) TTF_CloseFont(gui->moveFont);
    
    free(gui);
    gui = NULL;
}

//  RENDER 
void renderGameScreenGui(SDL_Renderer *renderer, gamegui *gui, Game *game,
                         bool dragging, SDL_Texture *dragTexture, SDL_Rect dragRect,Position draggedInitialPosition)
{
    if(!renderer || !gui || !game) return;



    //Clear the screen
    SDL_RenderClear(renderer);

    

    // Draw background
    if(gui->background) SDL_RenderCopy(renderer, gui->background, NULL, NULL);
    //Update halfmove clock
    renderHalfMoveClock(game->halfMoveClock,gui->moveFont,renderer);

    

    // Draw chessboard with light and dark squares
    SDL_Color light={235, 236, 208, 255};
    SDL_Color dark={119, 149, 86, 255};

    for(int row=0; row<BOARD_SIZE; row++)
    {
        for(int col=0; col<BOARD_SIZE; col++)
        {
            SDL_Rect r={col*g_squareSize + BOARDOFFSET, row*g_squareSize + BOARDOFFSET, g_squareSize, g_squareSize};
            if((row+col)%2==0)
                SDL_SetRenderDrawColor(renderer, light.r, light.g, light.b, light.a);
            else
                SDL_SetRenderDrawColor(renderer, dark.r, dark.g, dark.b, dark.a);
            SDL_RenderFillRect(renderer, &r);
        }
    }




    //Highlight the valid moves with green and it take under account that moves making the king inCheck aren't valid
    if(dragging)
        highlightValidMoves(game,draggedInitialPosition,renderer);

    //Highlight the king in check
    SDL_Color redThreatened = (SDL_Color){255,10,10,255*0.60};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); //Blendmode for transperancy
    SDL_SetRenderDrawColor(renderer,redThreatened.r,redThreatened.g,redThreatened.b,redThreatened.a);
    SDL_RenderFillRect(renderer,&gui->kingThreaten);
    // Highlight from ---> to last move
    SDL_Color highlightMovingPositions = (SDL_Color){255,255,0,255 * 0.38}; // value of a = transperancy percenatge * 255
    if(gui->fromMovingRect.x != -1 && gui->toMovingRect.x !=-1 && !dragging)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer,highlightMovingPositions.r,highlightMovingPositions.g,highlightMovingPositions.b,highlightMovingPositions.a);
        SDL_RenderFillRect(renderer,&gui->fromMovingRect);
        SDL_RenderFillRect(renderer,&gui->toMovingRect);

    }
    // Draw board pieces ( and skip dragged pieces)
    for(int row=0; row<BOARD_SIZE; row++)
    {
        for(int col=0; col<BOARD_SIZE; col++)
        {
            SDL_Texture *t = gui->boardPieces[row][col].texture;
            if(t && (!dragging || t != dragTexture))
                SDL_RenderCopy(renderer, t, NULL, &gui->boardPieces[row][col].rect);
        }
    }




    // Draw file labels (A-H) at bottom and top of board
    int label_size = g_squareSize * 0.2;
    for (int i = 0; i < 8; i++) {
        if (gui->fileLabelTextures[i]) {
            SDL_Rect labelRect = {i * g_squareSize + BOARDOFFSET + g_squareSize/2 - label_size/2, BOARDOFFSET + 8*g_squareSize + g_squareSize/4, label_size, label_size};
            SDL_RenderCopy(renderer, gui->fileLabelTextures[i], NULL, &labelRect);
            SDL_Rect labelRectU = {i * g_squareSize + BOARDOFFSET + g_squareSize/2 - label_size/2, BOARDOFFSET - g_squareSize/3, label_size, label_size};
            SDL_RenderCopy(renderer, gui->fileLabelTextures[i], NULL, &labelRectU);
        }
    }





    // Draw rank labels (1-8) on left and right side of board (reversed so 8 is at top, 1 is at bottom)
    for (int i = 0; i < 8; i++) {
        if (gui->rankLabelTextures[i]) {
            // rankLabelTextures[0] is "1.png", rankLabelTextures[7] is "8.png"
            // We want 8 at top (row 0), 1 at bottom (row 7)
            // So for i=0 (texture "1.png"), draw at row 7; for i=7 (texture "8.png"), draw at row 0
            int rowPos = 7 - i;  // Reverse mapping: i=0 -> row 7, i=7 -> row 0
            SDL_Rect labelRect = {BOARDOFFSET - g_squareSize/3, rowPos * g_squareSize + BOARDOFFSET + g_squareSize/2 - label_size/2, label_size, label_size};
            SDL_RenderCopy(renderer, gui->rankLabelTextures[i], NULL, &labelRect);
            SDL_Rect labelRectR = {BOARDOFFSET + 8*g_squareSize + g_squareSize/4, rowPos * g_squareSize + BOARDOFFSET + g_squareSize/2 - label_size/2, label_size, label_size};
            SDL_RenderCopy(renderer, gui->rankLabelTextures[i], NULL, &labelRectR);
        }
    }



    // Draw captured pieces
    for(int c=0; c<2; c++)
    {
        for(int i=0; i<16; i++)
        {
            SDL_Texture *t = gui->capturedPieces[c][i].texture;
            if(t) SDL_RenderCopy(renderer, t, NULL, &gui->capturedPieces[c][i].rect);
        }
    }

    //Last move
    if(gui->lastMove.texture)
        SDL_RenderCopy(renderer,gui->lastMove.texture,NULL,&gui->lastMove.rect);



    // Draw buttons
    SDL_RenderCopy(renderer, gui->back.texture, NULL, &gui->back.rect);
    SDL_RenderCopy(renderer, gui->undo.texture, NULL, &gui->undo.rect);
    SDL_RenderCopy(renderer, gui->redo.texture, NULL, &gui->redo.rect);
    SDL_RenderCopy(renderer, gui->save.texture, NULL, &gui->save.rect);
    SDL_RenderCopy(renderer,gui->drawAgreement.texture,NULL,&gui->drawAgreement.rect);
    SDL_RenderCopy(renderer, gui->capWhite.texture, NULL, &gui->capWhite.rect);
    SDL_RenderCopy(renderer, gui->capBlack.texture, NULL, &gui->capBlack.rect);

    // Draw turn indicator
    if(gui->currentTurn.texture)
        SDL_RenderCopy(renderer, gui->currentTurn.texture, NULL, &gui->currentTurn.rect);



    
    
    
    // Draw dragged piece on top of the board above everything
    if(dragging && dragTexture)
    {
        SDL_RenderCopy(renderer, dragTexture, NULL, &dragRect);
    }


    

    

    SDL_RenderPresent(renderer);
}


//  HANDLE EVENTS // The most important part
bool gameScreenHandleEvents(gamegui *gui, SDL_Event *event, App *app,
                            bool *dragging, SDL_Rect *dragRect, Position *draggedPiece, SDL_Renderer *renderer)
{
    if(!gui || !event || !app || !dragging || !dragRect || !draggedPiece || !renderer) return false;

    int mx, my;

    if(event->type == SDL_QUIT)
    {
        app->currentScreen = SCREEN_QUIT;
        app->running = false;
        return true;
    }

    if(event->type == SDL_KEYDOWN)
    {
        switch(event->key.keysym.sym)
        {
            case SDLK_ESCAPE:
                app->currentScreen = SCREEN_QUIT;
                app->running = false;
                return true;

            case SDLK_u:case SDLK_z:
                //Accept both u and z and ctrl u and ctrl z
                if(undoMove(&app->gamestack, &app->redostack))
                {
                    if(app->gamestack != NULL)
                    {
                        memcpy(&app->game, &app->gamestack->curGame, sizeof(Game));
                        updateGameGui(gui, &app->game, renderer);
                        printf("Move undone\n");
                    }
                    else printf("Error: Game stack is empty after undo\n");
                }
                else printf("Nothing to undo\n");
                return true;

            case SDLK_r:
                //Accept both 
                if(redoMove(&app->gamestack, &app->redostack))
                {
                    if(app->gamestack != NULL)
                    {
                        memcpy(&app->game, &app->gamestack->curGame, sizeof(Game));
                        updateGameGui(gui, &app->game, renderer);
                        printf("Move redone\n");
                    }
                    else printf("Error: Game stack is empty after redo\n");
                }
                else printf("Nothing to redo\n");
                return true;
        }
    }

    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT && !(*dragging))
    {
        //Drag a piece or click on controloers
        
        mx = event->button.x;
        my = event->button.y;

    if(isButtonClicked(mx,my,gui->save))
    {
        highlightClickedButton(app->renderer,gui->save);
        char fileName[50] = {0};
        int slot = showSaveSlotDialog(app->window);
        
        if (slot < 1 || slot > 5) {
            printf("Save cancelled (invalid slot: %d)\n", slot);
            return true;
        }
        
        sprintf(fileName, "cfg/slot_%d.bin", slot);
        printf("Attempting to save game to file: %s\n", fileName);
        
        FILE *fptr = fopen(fileName, "wb");
        if (!fptr) {
            fprintf(stderr, "Error: Could not open file '%s' for writing\n", fileName);
            printf("Save failed: Cannot write to file\n");
            showGameMessage(app->window, "Save Failed", "Could not write to save file");
            return true;
        }
        
        if (!saveGame(fptr, &app->gamestack->curGame)) {
            fprintf(stderr, "Error: Failed to write Game struct to file\n");
            printf("Save failed: Could not write game data\n");
            fclose(fptr);
            showGameMessage(app->window, "Save Failed", "Could not write game data to file");
            return true;
        }
        
        fclose(fptr);
        printf("Game successfully saved to slot %d (%s)\n", slot, fileName);
        showGameMessage(app->window, "Save Successful", "Game saved successfully");
        return true;
    }
    else if(isButtonClicked(mx,my,gui->back))
    {
        highlightClickedButton(app->renderer,gui->back);
        app->currentScreen = SCREEN_MENU;

        // Clear all game stacks to delete current game
        clearStack(&app->gamestack);
        clearStack(&app->redostack);

        printf("Returned to main menu. Game cleared.\n");
        return true;
    }
    else if(isButtonClicked(mx,my,gui->drawAgreement))
    {
        playSoundEffect(gui->sEffect.drawOffer);
        highlightClickedButton(app->renderer,gui->drawAgreement);
        int drawAgreement = offerDrawDialog(app->window);
        if(drawAgreement)
            {
                playSoundEffect(gui->sEffect.draw);
                app->currentScreen = SCREEN_MENU;
                app->gamestack->curGame.status = DRAW_AGREEMENT;
                // Clear all game stacks to delete current game
                clearStack(&app->gamestack);
                clearStack(&app->redostack);

                printf("Returned to main menu. Game cleared.\n");
            }
            return true;
    }
    else if(isButtonClicked(mx,my,gui->undo))
        {
            highlightClickedButton(app->renderer,gui->undo);
            if(undoMove(&app->gamestack, &app->redostack))
            {
                if(app->gamestack != NULL)
                {
                    memcpy(&app->game, &app->gamestack->curGame, sizeof(Game));
                    updateGameGui(gui, &app->game, renderer);
                    // printBoard(&app->game);
                    printf("Move undone\n");
                }
                else printf("Error: Game stack is empty after undo\n");
            }
            else printf("Nothing to undo\n");
            return true;
        }
    else if(isButtonClicked(mx,my,gui->redo))
        {
            highlightClickedButton(app->renderer,gui->redo);
            if(redoMove(&app->gamestack, &app->redostack))
            {
                if(app->gamestack != NULL)
                {
                    memcpy(&app->game, &app->gamestack->curGame, sizeof(Game));
                    updateGameGui(gui, &app->game, renderer);
                    // printBoard(&app->game);
                    printf("Move redone\n");
                }
                else printf("Error: Game stack is empty after redo\n");
            }
            else printf("Nothing to redo\n");
            return true;
        }

        // Check board
        int row =( my-BOARDOFFSET) / g_squareSize;
        int col = (mx-BOARDOFFSET) / g_squareSize;
        //Check Boundaries 
        if(row>=0 && row<BOARD_SIZE && col>=0 && col<BOARD_SIZE)
        {
            Piece piece = app->game.board[row][col];
            if(piece.type != EMPTY && piece.color == app->game.currentPlayer)
            {
                *dragging = true;
                *draggedPiece = (Position){row, col};
                *dragRect = (SDL_Rect){col*g_squareSize + BOARDOFFSET, row*g_squareSize + BOARDOFFSET, g_squareSize, g_squareSize};
            }
        }
    }

    else if(event->type == SDL_MOUSEMOTION && *dragging)
    {
        mx = event->motion.x; 
        my = event->motion.y;
        dragRect->x = mx - g_squareSize / 2; //To center the piece at the mouse
        dragRect->y = my - g_squareSize / 2; //to center the piece at the mouse 
    }

    else if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT && *dragging)
    {
        *dragging = false;
        mx = event->button.x; 
        my = event->button.y;

        Position initial = *draggedPiece;
        Position final = { (my-BOARDOFFSET)/g_squareSize, (mx-BOARDOFFSET)/g_squareSize };

        if(final.x<0 || final.x>=BOARD_SIZE || final.y<0 || final.y>=BOARD_SIZE) return false;

        // Ensure we have a valid game stack
        if(app->gamestack == NULL)
        {
            printf("Error: Game stack is NULL during move attempt\n");
            return false;
        }

        Move move = { .initial = initial, .final = final, .promotionPiece = EMPTY, .moveType = NORMAL_MOVE };
        move.capturedPiece.type = EMPTY; move.capturedPiece.color = NONE;

        Game newGame; 
        // Always copy from app->game which is kept in sync
        memcpy(&newGame, &app->game, sizeof(Game));

        if(isValidMove(&newGame, move))
        {
            Piece movingPiece = newGame.board[move.initial.x][move.initial.y];
            if(movingPiece.type==PAWN)
            {
                int promoRow = (movingPiece.color==WHITE) ? 0 : BOARD_SIZE-1;
                if(move.final.x == promoRow) move.promotionPiece = getPromotionDialog(app->window);
            }
            

            applyMove(&newGame, &move);
            updateLastMoveTexture(gui,app->renderer,move);
            newGame.status = computeGameStatus(&newGame);

            //For console part can be ignored
            printGameState(&newGame);
            printBoard(&newGame);
            // printBoard(&newGame);


            

            // Show Message Box show current state
            if(move.moveType == PAWN_PROMOTION)
                {//showGameMessage(app->window,"Move Type","👑 Pawn promoted!");
                playSoundEffect(gui->sEffect.promotion);}
            else if(move.moveType == CASTLE_KINGSIDE)
                {//showGameMessage(app->window,"Move Type","🏰 Castling King Side performed!");
                playSoundEffect(gui->sEffect.castling);}
            else if(move.moveType == CASTLE_QUEENSIDE)
                {//showGameMessage(app->window,"Move Type","🏰 Castling Queen Side performed!");
                playSoundEffect(gui->sEffect.castling);}
            else if(move.moveType == EN_PASSENT)
                {//showGameMessage(app->window,"Move Type","⚔️  En Passant capture!");
                playSoundEffect(gui->sEffect.enpassent);}
            else if(move.moveType == NORMAL_MOVE)
                    playSoundEffect(gui->sEffect.normalMove);
            else if(move.moveType == CAPTURE)
                    playSoundEffect(gui->sEffect.capture);


            switch (newGame.status)
            {
            case CHECK:
                playSoundEffect(gui->sEffect.check);
                if(newGame.currentPlayer == WHITE)
                    showGameMessage(app->window,"Warning","⚠️  CHECK! White King under attack!");
                else
                    showGameMessage(app->window,"Warning","⚠️  CHECK! Black King under attack!");
                break;
            case CHECKMATE:
                playSoundEffect(gui->sEffect.checkmate);
                SDL_Texture *mate = NULL;
                if(newGame.currentPlayer == WHITE)
                    {mate = loadtexture("assets/wmate.png",renderer);
                    SDL_RenderCopy(renderer,mate,NULL,NULL);
                    SDL_RenderPresent(renderer);
                    showGameMessage(app->window,"End of game","🏆 CHECKMATE! Black wins!");
                    SDL_DestroyTexture(mate);
                    }
                else
                    {mate = loadtexture("assets/bmate.png",renderer);
                    SDL_RenderCopy(renderer,mate,NULL,NULL);
                    SDL_RenderPresent(renderer);
                    if(!mate)
                        printf("ERROR");
                    showGameMessage(app->window,"End of game","🏆 CHECKMATE! White wins!");
                    SDL_DestroyTexture(mate);
                    }
                printf("\n🏆 CHECKMATE! %s wins!\n", (newGame.currentPlayer == WHITE) ? "BLACK" : "WHITE");
                app->currentScreen = SCREEN_MENU;
                break;
            case STALEMATE:
                SDL_Texture *stalemate = NULL;
                stalemate = loadtexture("assets/stale.png",renderer);
                SDL_RenderCopy(renderer,stalemate,NULL,NULL);
                SDL_RenderPresent(renderer);
                playSoundEffect(gui->sEffect.stalemate);
                showGameMessage(app->window,"End of game","🤝 STALEMATE! Game is a draw.");
                printf("\n🤝 STALEMATE! Game is a draw.\n");
                app->currentScreen = SCREEN_MENU;
                SDL_DestroyTexture(stalemate);
                break;
            case DRAW_FIFTY_MOVE:
                playSoundEffect(gui->sEffect.draw);
                showGameMessage(app->window,"End of game","🤝 DRAW by fifty-move rule!");
                printf("\n🤝 DRAW by fifty-move rule!\n");
                app->currentScreen = SCREEN_MENU;
                break;
            case DRAW_AGREEMENT:
                playSoundEffect(gui->sEffect.draw);
                showGameMessage(app->window,"End of game","🤝 Game ended in a draw."); //TODO this logic
                printf("\n🤝 Game ended in a draw.\n");
                app->currentScreen = SCREEN_MENU;
                break;
            
            case DRAW_INSUFFICIENT_MATERIAL:
                playSoundEffect(gui->sEffect.draw);
                showGameMessage(app->window,"End of game","🤝 Game ended in a dead position.");
                printf("\n 🤝 Game ended in a dead position\n");
                app->currentScreen = SCREEN_MENU;
                break;
            }

            
            // Clear redo stack before pushing new move
            clearStack(&app->redostack);
            
            if(push(&app->gamestack, newGame))
            {
                // Sync app->game with the new state
                app->game = newGame;
                updateGameGui(gui, &app->game, renderer);
                //Highlight last moved piece
                gui->fromMovingRect = (SDL_Rect){BOARDOFFSET + move.initial.y * g_squareSize,BOARDOFFSET + move.initial.x * g_squareSize,g_squareSize,g_squareSize};
                gui->toMovingRect = (SDL_Rect){BOARDOFFSET + move.final.y * g_squareSize,BOARDOFFSET + move.final.x * g_squareSize,g_squareSize,g_squareSize};
                printf("Move executed successfully\n");
            }
            else printf("Error: Failed to push game state\n");
        }
        else 
        {
            playSoundEffect(gui->sEffect.illegalMove);
            printf("Invalid move attempted\n");
        }
    }

    return false;
}


//  RUN GAME LOOP 
void runGame(App *app, gamegui *gui)
{
    if(!app || !gui) return;

    SDL_Event e;
    bool dragging = false;
    SDL_Rect dragRect = {0};
    Position draggedPiece = {-1, -1};

    while(app->running && app->currentScreen == SCREEN_GAME)
    {
        // Determine dragged piece texture if dragging
        SDL_Texture *dragTex = NULL;
        if(dragging && draggedPiece.x >=0 && draggedPiece.y >=0)
        dragTex = gui->boardPieces[draggedPiece.x][draggedPiece.y].texture;
        // Render GUI using app->game
        renderGameScreenGui(app->renderer, gui, &app->game, dragging, dragTex, dragRect,draggedPiece);
        if(SDL_WaitEvent(&e))
        {gameScreenHandleEvents(gui,&e,app,&dragging,&dragRect,&draggedPiece,app->renderer);
        // Exit loop if screen changed
        if(app->currentScreen != SCREEN_GAME) break;
        }
        
    }
}

void updateGameGui(gamegui *gui, Game *game, SDL_Renderer *renderer)
{
    if (!gui || !game || !renderer) return;

    //  Update board pieces and destroy them before
    for(int row=0; row<BOARD_SIZE; row++)
    {
        for(int col=0; col<BOARD_SIZE; col++)
        {
            Piece piece = game->board[row][col];

            // Destroy old texture if exists
            if(gui->boardPieces[row][col].texture)
            {
                SDL_DestroyTexture(gui->boardPieces[row][col].texture);
                gui->boardPieces[row][col].texture = NULL;
            }

            if(piece.type == EMPTY) continue;

            char filename[128];
            snprintf(filename, sizeof(filename), "assets/Pieces/%s_%s.png",
                     (piece.color==WHITE ? "w" : "b"),
                     getPieceTypeName(piece.type));
            


            SDL_Texture *txt = loadtexture(filename, renderer);
            if(!txt)
            {
                fprintf(stderr, "Warning: Failed to load texture: %s\n", filename);
                continue;
            }

            gui->boardPieces[row][col].texture = txt;
            gui->boardPieces[row][col].rect = (SDL_Rect){col*g_squareSize + BOARDOFFSET, row*g_squareSize + BOARDOFFSET, g_squareSize, g_squareSize};
        }
    }

    //  Update captured pieces and destroy them before
    for(int i=0; i<16; i++)
    {
        // White captured
        if(gui->capturedPieces[0][i].texture) SDL_DestroyTexture(gui->capturedPieces[0][i].texture);
        gui->capturedPieces[0][i].texture = NULL;
        if(game->capturedWhitePieces[i].type != EMPTY)
        {
            char filename[128];
            snprintf(filename, sizeof(filename), "assets/Pieces/w_%s.png", getPieceTypeName(game->capturedWhitePieces[i].type));
            SDL_Texture *txt = loadtexture(filename, renderer);
            if(txt)
            {
                gui->capturedPieces[0][i].texture = txt;
                int baseXWhite = WINDOW_WIDTH - BUTTON_WIDTH - WINDOW_WIDTH * 0.20;
                int baseYWhite = WINDOW_HEIGHT * 0.15;
                int col = i % 2;
                int row = i / 2;
                int piece_size = g_squareSize * 0.95;
                int piece_spacing = g_squareSize * 1.00;
                gui->capturedPieces[0][i].rect = (SDL_Rect){baseXWhite + col * piece_spacing, baseYWhite + row * piece_spacing, piece_size, piece_size};

            }
        }

        // Black captured
        if(gui->capturedPieces[1][i].texture) SDL_DestroyTexture(gui->capturedPieces[1][i].texture);
        gui->capturedPieces[1][i].texture = NULL;
        if(game->capturedBlackPieces[i].type != EMPTY)
        {
            char filename[128];
            snprintf(filename, sizeof(filename), "assets/Pieces/b_%s.png", getPieceTypeName(game->capturedBlackPieces[i].type));
            SDL_Texture *txt = loadtexture(filename, renderer);
            if(txt)
            {
                gui->capturedPieces[1][i].texture = txt;
                int col = i % 2;
                int row = i / 2;
                int baseX_black = WINDOW_WIDTH - BUTTON_WIDTH - WINDOW_WIDTH * 0.00;
                int baseY_black = WINDOW_HEIGHT * 0.15;
                int piece_size = g_squareSize * 0.95;
                int piece_spacing = g_squareSize * 1.00;

                gui->capturedPieces[1][i].rect = (SDL_Rect){
                    baseX_black + col * piece_spacing,
                    baseY_black + row * piece_spacing,
                    piece_size,
                    piece_size
                };
                

            }
        }
    }

    //  Update turn indicator 
    gui->currentTurn.texture = (game->currentPlayer == WHITE) ? gui->whiteTurnTex : gui->blackTurnTex;

    //clear highlighting will only enabled after applying a new move
    clearHighlight(gui);
    if(game->status != CHECK)
        gui->kingThreaten = (SDL_Rect){.x = -1,.y = -1,.w = 0,.h = 0};
    else
    {
        Position kingThreatened = findKingPosition(game,game->currentPlayer);
        gui->kingThreaten = (SDL_Rect){.x = kingThreatened.y*g_squareSize + BOARDOFFSET , .y =kingThreatened.x *g_squareSize + BOARDOFFSET ,g_squareSize,g_squareSize};
    }

    

}

// Dialog Helpers
int getPromotionDialog(SDL_Window *window)
{
    const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, QUEEN, "Queen" },
        { 0, ROOK, "Rook" },
        { 0, BISHOP, "Bishop" },
        { 0, KNIGHT, "Knight" },
    };

    const SDL_MessageBoxColorScheme colorScheme = {
        {
            { 255, 0, 0 },   // background
            { 0, 255, 0 },   // text
            { 255, 255, 0 }, // button border
            { 0, 0, 255 },   // button background
            { 255, 255, 255 } // button selected
        }
    };

    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION,
        window,
        "Promotion",
        "Choose piece to promote:",
        SDL_arraysize(buttons),
        buttons,
        &colorScheme
    };

    int buttonid = 0;
    SDL_ShowMessageBox(&messageboxdata, &buttonid);

    return buttonid;  // 5=Queen, 4=Rook, 3=Bishop, 2=Knight and default is Queen

}
bool offerDrawDialog(SDL_Window *window)
{
    //Default will be no
    const SDL_MessageBoxButtonData buttons[] = {
        {0,1,"Yes"},
        {SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT,0,"NO"}
    };

    const SDL_MessageBoxColorScheme colorScheme = {
        {
            { 255, 0, 0 },   // background
            { 0, 255, 0 },   // text
            { 255, 255, 0 }, // button border
            { 0, 0, 255 },   // button background
            { 255, 255, 255 } // button selected
        }
    };

    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION,
        window,
        "Draw Agreement",
        "Are you sure you want to offer draw ?",
        SDL_arraysize(buttons),
        buttons,
        &colorScheme
    };

    int buttonId = 0;
    SDL_ShowMessageBox(&messageboxdata,&buttonId);

    return buttonId;

}
void clearHighlight(gamegui *gui)
{
    gui->fromMovingRect.x = -1;
    gui->toMovingRect.x = -1;
}
void highlightValidMoves(Game *game,Position from,SDL_Renderer *renderer)
{
    
    for(int i = 0;i<BOARD_SIZE;i++)
    {
        for(int j =0;j< BOARD_SIZE;j++)
        {
            Position final = {.x = i , .y = j};
            Move move;
            move.initial = from;
            move.final = final;
            if(isValidMove(game,move))
            {

                SDL_Color greenH = (SDL_Color){0,255,0,255 * 0.38};
                SDL_Rect validPlace = (SDL_Rect){.x = BOARDOFFSET + j * g_squareSize,.y = BOARDOFFSET + i * g_squareSize,.w = g_squareSize,.h =g_squareSize};
                SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); // for transperancy
                SDL_SetRenderDrawColor(renderer,greenH.r,greenH.g,greenH.b,greenH.a);
                SDL_RenderFillRect(renderer,&validPlace);
            }
        }
    }

}
void updateLastMoveTexture(gamegui *gui, SDL_Renderer *renderer, Move move)
{
    if (!gui || !gui->moveFont) {
        fprintf(stderr, "Error: Font not loaded in updateLastMoveTexture\n");
        return;
    }

    char text[32];
    TTF_Font *font = gui->moveFont;  // Use pre-loaded font instead of loading it every time

    SDL_Color textColor = {255, 100, 100, 255};

    /* Base move text */
    snprintf(text, sizeof(text),
             "%c%d%c%d",
             move.initial.y + 'A',
             8 - move.initial.x,
             move.final.y + 'A',
             8 - move.final.x);

    /* Handle promotion */
    if (move.moveType == PAWN_PROMOTION)
    {
        char promoChar = 'Q';

        switch (move.promotionPiece)
        {
            case QUEEN:  promoChar = 'Q'; break;
            case ROOK:   promoChar = 'R'; break;
            case BISHOP: promoChar = 'B'; break;
            case KNIGHT: promoChar = 'N'; break;
            default: break;
        }

        char promoText[8];
        snprintf(promoText, sizeof(promoText), "%c", promoChar);
        strcat(text,promoText);
    }

    /* Destroy old texture */
    if (gui->lastMove.texture) {
        SDL_DestroyTexture(gui->lastMove.texture);
        gui->lastMove.texture = NULL;
    }

    /* Render text */
    SDL_Surface *sur = TTF_RenderUTF8_Blended(font, text, textColor);
    if (!sur) {
        fprintf(stderr, "TTF render error: %s\n", TTF_GetError());
        return;  // i don't close  the font here - it's managed by the gui structure
    }

    gui->lastMove.texture = SDL_CreateTextureFromSurface(renderer, sur);

    /* KEEP x & y, FIX width & height */
    gui->lastMove.rect.x = WINDOW_WIDTH * 0.02 + 4 * g_squareSize / 2 - BUTTON_WIDTH/4;
    gui->lastMove.rect.y = WINDOW_HEIGHT * 0.80;
    gui->lastMove.rect.w = sur->w;   //  text natural width
    gui->lastMove.rect.h = sur->h;   //  text natural height

    SDL_FreeSurface(sur);
    // Font is managed by gui structure and destroyed in destroyGameScreenGui()
}
void renderHalfMoveClock(int halfMoveClock,TTF_Font *font,SDL_Renderer *renderer){

    char text[50];
    SDL_Color textColor = {255, 100, 100, 255};
    sprintf(text, "⏰ Half Move Clock : %d",halfMoveClock);
    /* Render text */
    SDL_Surface *sur = TTF_RenderUTF8_Blended(font, text, textColor);
    if (!sur) {
        fprintf(stderr, "TTF render error: %s\n", TTF_GetError());
        return;  // i don't close  the font here - it's managed by the gui structure
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer,sur);
    if(!texture){
        fprintf(stderr, "Texture loading error: %s\n", SDL_GetError());
        return;  //
    }
    SDL_Rect rec = {WINDOW_WIDTH * 0.02 + 15 * g_squareSize / 2 - BUTTON_WIDTH/4,WINDOW_HEIGHT * 0.80,sur->w,sur->h};
    SDL_FreeSurface(sur);

    SDL_RenderCopy(renderer,texture,NULL,&rec);
    SDL_DestroyTexture(texture);

}

