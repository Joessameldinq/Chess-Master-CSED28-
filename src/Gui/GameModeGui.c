


#include "../../include/GameModeGui.h"
#include "../../include/GuiDefinitions.h"
#include "../../include/GuiHelpers.h"
#include "../../include/GameDefinitions.h"
#include "../../include/GameInitialization.h"
#include "../../include/SavingLoading.h"
#include "../../include/EndingConditions.h"
#include "../../include/StackHelpers.h"
#include "../../include/InputConsole.h"
#include "../../include/DisplayConsole.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <SDL2/SDL_mixer.h>


// The array of available paths
const char *fullPaths[] = {
    "assets/pieces/glass",
    "assets/pieces/gothic",
    "assets/pieces/graffiti",
    "assets/pieces/icy_sea",
    "assets/pieces/3d_chesskid",
    "assets/pieces/3d_plastic",
    "assets/pieces/3d_staunton",
    "assets/pieces/3d_wood",
    "assets/pieces/8_bit",
    "assets/pieces/alpha",
    "assets/pieces/bases",
    "assets/pieces/book",
    "assets/pieces/bubblegum",
    "assets/pieces/cases",
    "assets/pieces/classic",
    "assets/pieces/club",
    "assets/pieces/condal",
    "assets/pieces/dash",
    "assets/pieces/game_room",
    "assets/pieces/light",
    "assets/pieces/lolz",
    "assets/pieces/marble",
    "assets/pieces/maya",
    "assets/pieces/metal",
    "assets/pieces/modern",
    "assets/pieces/nature",
    "assets/pieces/neo",
    "assets/pieces/neo_wood",
    "assets/pieces/neon",
    "assets/pieces/newspaper",
    "assets/pieces/ocean",
    "assets/pieces/sky",
    "assets/pieces/space",
    "assets/pieces/tigers",
    "assets/pieces/tournament",
    "assets/pieces/vintage",
    "assets/pieces/wood"
};

const int numPieces = sizeof(fullPaths) / sizeof(fullPaths[0]);

int currentPiecesIndex= 0;
int g_squareSize = 0;
int g_boardOffset = 0;

SDL_Color lightSquare[] = {
    {238, 238, 210, 255}, // Classic Green (Light)
    {232, 235, 239, 255}, // Tournament Blue (Light)
    {240, 217, 181, 255}, // Wood Walnut (Light)
    {255, 255, 255, 255}, // High Contrast (Light)
    {204, 212, 223, 255}, // Deep Sea (Light)
    {234, 233, 210, 255}, // Forest (Light)
    {227, 193, 111, 255}, // Sandcastle (Light)
    {198, 198, 198, 255}, // Slate (Light)
    {231, 213, 185, 255}, // Maple (Light)
    {222, 227, 230, 255}, // Midnight (Light)
    {255, 220, 190, 255}, // Marble (Light)
    {240, 217, 181, 255}, // Burnt Orange (Light)
    {235, 209, 166, 255}, // Bubblegum (Light)
    {218, 223, 225, 255}, // Ice (Light)
    {245, 245, 220, 255}, // Beige (Light)
    {220, 220, 220, 255}, // Gray (Light)
    {255, 250, 240, 255}, // Ivory (Light)
    {240, 230, 215, 255}, // Tan (Light)
    {235, 235, 235, 255}, // White (Light)
    {210, 180, 140, 255}, // Tan Brown (Light)
    {245, 222, 179, 255}, // Wheat (Light)
    {230, 230, 250, 255}, // Lavender (Light)
    {240, 248, 255, 255}, // Alice Blue (Light)
    {248, 248, 255, 255}, // Ghost White (Light)
    {255, 240, 245, 255}, // Lavender Blush (Light)
    {250, 235, 215, 255}, // Antique White (Light)
    {253, 245, 230, 255}, // Old Lace (Light)
    {255, 250, 250, 255}, // Snow (Light)
    {245, 255, 250, 255}, // Mint Cream (Light)
    {240, 255, 240, 255}, // Honeydew (Light)
    {255, 255, 240, 255}, // Ivory Light (Light)
    {255, 250, 205, 255}, // Lemon Chiffon (Light)
    {250, 250, 210, 255}, // Light Goldenrod (Light)
    {255, 239, 213, 255}, // Papaya Whip (Light)
    {255, 228, 196, 255}, // Bisque (Light)
    {255, 218, 185, 255}, // Peach Puff (Light)
    {255, 222, 173, 255}, // Navajo White (Light)
    {250, 240, 230, 255}, // Linen (Light)
    {253, 245, 230, 255}  // Old Lace Alt (Light)
};

SDL_Color darkSquare[] = {
    {118, 150, 86, 255},  // Classic Green (Dark)
    {125, 135, 150, 255}, // Tournament Blue (Dark)
    {181, 136, 99, 255},  // Wood Walnut (Dark)
    {80, 80, 80, 255},    // High Contrast (Dark)
    {75, 115, 153, 255},  // Deep Sea (Dark)
    {75, 117, 81, 255},   // Forest (Dark)
    {184, 139, 74, 255},  // Sandcastle (Dark)
    {100, 111, 128, 255}, // Slate (Dark)
    {151, 103, 63, 255},  // Maple (Dark)
    {58, 87, 122, 255},   // Midnight (Dark)
    {130, 130, 60, 255},  // Marble (Dark)
    {165, 105, 63, 255},  // Burnt Orange (Dark)
    {170, 122, 101, 255}, // Bubblegum (Dark)
    {119, 149, 182, 255}, // Ice (Dark)
    {139, 115, 85, 255},  // Beige (Dark)
    {128, 128, 128, 255}, // Gray (Dark)
    {160, 140, 120, 255}, // Ivory (Dark)
    {140, 120, 100, 255}, // Tan (Dark)
    {150, 150, 150, 255}, // White (Dark)
    {139, 90, 60, 255},   // Tan Brown (Dark)
    {189, 154, 122, 255}, // Wheat (Dark)
    {147, 112, 219, 255}, // Lavender (Dark)
    {135, 154, 180, 255}, // Alice Blue (Dark)
    {140, 140, 170, 255}, // Ghost White (Dark)
    {176, 140, 160, 255}, // Lavender Blush (Dark)
    {160, 130, 100, 255}, // Antique White (Dark)
    {180, 150, 120, 255}, // Old Lace (Dark)
    {170, 160, 150, 255}, // Snow (Dark)
    {130, 170, 140, 255}, // Mint Cream (Dark)
    {140, 170, 140, 255}, // Honeydew (Dark)
    {180, 170, 130, 255}, // Ivory Light (Dark)
    {200, 180, 120, 255}, // Lemon Chiffon (Dark)
    {170, 160, 100, 255}, // Light Goldenrod (Dark)
    {190, 150, 110, 255}, // Papaya Whip (Dark)
    {180, 140, 100, 255}, // Bisque (Dark)
    {190, 130, 90, 255},  // Peach Puff (Dark)
    {180, 135, 90, 255},  // Navajo White (Dark)
    {160, 140, 120, 255}, // Linen (Dark)
    {175, 145, 115, 255}  // Old Lace Alt (Dark)
};

const int numColors = sizeof(lightSquare) / sizeof(lightSquare[0]);
int currentColorIndex = 0;
SDL_Color currentLight;
SDL_Color currentDark;
int colorCounter = 0;

Mix_Music *bgMusic;
bool musicPlaying = false;
//  helper function to get the file name of the piece type  to load textures
const char* getPieceTypeName(PieceType type)
{
    switch(type)
    {
        case PAWN:   return "p";
        case KING:   return "k";
        case QUEEN:  return "q";
        case BISHOP: return "b";
        case ROOK:   return "r";
        case KNIGHT: return "n";
        default:     return "";
    }
}
//Updated one
void get_piece_path(Piece piece, char *outputBuffer) {
    // We use currentPieces as the base directory
    snprintf(outputBuffer, 100, 
             "%s/%s%s.png", 
             fullPaths[currentPiecesIndex],
             (piece.color == WHITE ? "w" : "b"),
             getPieceTypeName(piece.type)
    );
}

//Draw a pieces counter for captured pieces
int countPiecesByType(Piece capturedPieces[16], int counts[6])
{
    // Initialize counts
    for (int i = 0; i < 6; i++) {
        counts[i] = 0;
    }
    
    int total = 0;
    for (int i = 0; i < 16; i++) {
        if (capturedPieces[i].type != EMPTY) {
            // Map piece type to array index
            int index = -1;
            switch (capturedPieces[i].type) {
                case PAWN:   index = 0; break;
                case KNIGHT: index = 1; break;
                case BISHOP: index = 2; break;
                case ROOK:   index = 3; break;
                case QUEEN:  index = 4; break;
                case KING:   index = 5; break;
                default: break;
            }
            
            if (index >= 0) {
                counts[index]++;
                total++;
            }
        }
    }
    
    return total;
}
int calculateMaterialValue(int counts[6])
{
    // Standard chess piece values
    int values[] = {1, 3, 3, 5, 9, 0}; // Pawn, Knight, Bishop, Rook, Queen, King
    
    int total = 0;
    for (int i = 0; i < 6; i++) {
        total += counts[i] * values[i];
    }
    
    return total;
}

//  INIT GUI  --> grid , pieces , capturedpieces and buttons and has the feature of check valid textures
gamegui *initGameScreenGui(SDL_Renderer *renderer, Game *initialGame)
{
    //Initalize square colors
    currentLight = lightSquare[colorCounter];
    currentDark = darkSquare[colorCounter];
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

    // Load background music
    bgMusic = Mix_LoadMUS("assets/eff/Erik Satie - Gnossienne No.1.mp3");
    if (!bgMusic) {
        fprintf(stderr, "Failed to load mp3: %s\n", Mix_GetError());
    } else {
        Mix_VolumeMusic(MIX_MAX_VOLUME / 4);
        Mix_PlayMusic(bgMusic, -1); 
        musicPlaying = true;
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
    
    // gui->capBlack.texture = loadtexture("assets/capblack(2).png", renderer);
    // if (!gui->capBlack.texture) {
    //     fprintf(stderr, "Warning: Failed to load black capture button texture\n");
    // }
    
    // gui->capWhite.texture = loadtexture("assets/capwhite(2).png", renderer);
    // if (!gui->capWhite.texture) {
    //     fprintf(stderr, "Warning: Failed to load white capture button texture\n");
    // }
    //Set initial music texture is running
    gui->runMusic.texture = loadtexture("assets/musicrun.png",renderer);
    if(!gui->runMusic.texture){
         fprintf(stderr, "Warning: Failed to load music  texture\n");

    }

    gui->arrowForward.texture = loadtexture("assets/arrowforward.png",renderer);
    if(!gui->arrowForward.texture){
         fprintf(stderr, "Warning: Failed to load arrowforward  texture\n");

    }

    gui->arrowBack.texture = loadtexture("assets/arrowback.png",renderer);
    if(!gui->arrowForward.texture){
         fprintf(stderr, "Warning: Failed to load arrowback  texture\n");

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
    // gui->capWhite.rect = (SDL_Rect){WINDOW_WIDTH - BUTTON_WIDTH - WINDOW_WIDTH * 0.20 , WINDOW_HEIGHT * 0.05, bw, bh};
    // gui->capBlack.rect = (SDL_Rect){WINDOW_WIDTH - BUTTON_WIDTH - WINDOW_WIDTH * 0.00, WINDOW_HEIGHT * 0.05, bw, bh};
    gui->runMusic.rect = (SDL_Rect){margin + (bw + button_spacing) * 5.5,button_start_y - bh * 1.2,bw,bh};
    gui->arrowForward.rect = (SDL_Rect){g_boardOffset + g_squareSize *9.50, 0 , 45,45};
    gui->arrowBack.rect = (SDL_Rect){g_boardOffset + g_squareSize *8.50, 0, 45,45};
    
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

            char filename[100];
            get_piece_path(piece,filename);
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
            char filename[100];
            get_piece_path(initialGame->capturedWhitePieces[i],filename);
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
            int piece_size = g_squareSize * 0.60;
            int piece_spacing = g_squareSize * 1.00;
            gui->capturedPieces[0][capWhiteCnt].rect = (SDL_Rect){baseXWhite + col * piece_spacing, baseYWhite + row * piece_spacing, piece_size, piece_size};
            capWhiteCnt++;
        }
        
        if(initialGame->capturedBlackPieces[i].type != EMPTY)
        {
            char filename[100];
            get_piece_path(initialGame->capturedBlackPieces[i],filename);
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

    gui->moveHistoryFont = TTF_OpenFont("assets/fonts/SuperMaples-2vR2w.ttf", 18);
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


    gui->validMove = loadtexture("assets/dot.png",renderer);
    gui->lastMove.texture = NULL;
    return gui;
}

//  DESTROY GUI 
void destroyGameScreenGui(gamegui *gui)
{
    if(!gui) return;
    if (gui->validMove) SDL_DestroyTexture(gui->validMove);
    if (gui->background) SDL_DestroyTexture(gui->background);
    if (gui->save.texture) SDL_DestroyTexture(gui->save.texture);
    if (gui->undo.texture) SDL_DestroyTexture(gui->undo.texture);
    if (gui->redo.texture) SDL_DestroyTexture(gui->redo.texture);
    if (gui->back.texture) SDL_DestroyTexture(gui->back.texture);
    // if (gui->capWhite.texture) SDL_DestroyTexture(gui->capWhite.texture);
    // if (gui->capBlack.texture) SDL_DestroyTexture(gui->capBlack.texture);
    if (gui->currentTurn.texture) SDL_DestroyTexture(gui->currentTurn.texture);
    if (gui->whiteTurnTex) SDL_DestroyTexture(gui->whiteTurnTex);
    if (gui->blackTurnTex) SDL_DestroyTexture(gui->blackTurnTex);
    if(gui->drawAgreement.texture) SDL_DestroyTexture(gui->drawAgreement.texture);
    if (gui->lastMove.texture) SDL_DestroyTexture(gui->lastMove.texture);
    if(gui->runMusic.texture)SDL_DestroyTexture(gui->runMusic.texture);

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

    //Destroy Background music
    if (bgMusic) Mix_FreeMusic(bgMusic);

    // Destroy font
    if (gui->moveFont) TTF_CloseFont(gui->moveFont);    // Destroy font
    if (gui->moveHistoryFont) TTF_CloseFont(gui->moveHistoryFont);
    
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
    // if(gui->background) SDL_RenderCopy(renderer, gui->background, NULL, NULL);

    SDL_Color c = {255,255,255,255};
    SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a);
    SDL_RenderFillRect(renderer, NULL);

    //Update halfmove clock
    renderHalfMoveClock(game->halfMoveClock,gui->moveFont,renderer);

    


    

    // Draw chessboard with light and dark squares based on the counter
    SDL_Color light=currentLight;
    SDL_Color dark = currentDark;

    for(int row=0; row<BOARD_SIZE; row++)
    {
        for(int col=0; col<BOARD_SIZE; col++)
        {
            SDL_Rect r={col*g_squareSize + BOARDOFFSET, row*g_squareSize + BOARDOFFSET, g_squareSize, g_squareSize};
            if((row+col)%2==0)//even are light and odd are black
                SDL_SetRenderDrawColor(renderer, light.r, light.g, light.b, light.a);
            else
                SDL_SetRenderDrawColor(renderer, dark.r, dark.g, dark.b, dark.a);
            SDL_RenderFillRect(renderer, &r);
        }
    }




    //Highlight the valid moves with green and it take under account that moves making the king inCheck aren't valid
    if(dragging)
        highlightValidMoves(game,draggedInitialPosition,renderer,gui->validMove);

    //Highlight the king in check
    SDL_Color redThreatened = (SDL_Color){255,10,10,255*0.60};
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); //Blendmode for transperancy
    SDL_SetRenderDrawColor(renderer,redThreatened.r,redThreatened.g,redThreatened.b,redThreatened.a);
    SDL_RenderFillRect(renderer,&gui->kingThreaten);
    // Highlight from ---> to last move

    SDL_Color highlightMovingPositions = (SDL_Color){255,255,0,255 * 0.38}; // value of a = transperancy percenatge * 255
    if(gui->fromMovingRect.x != -1 && gui->toMovingRect.x !=-1 && !dragging)
    {
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); //Blend mode for transperancy
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
    // for(int c=0; c<2; c++)
    // {
    //     for(int i=0; i<16; i++)
    //     {
    //         SDL_Texture *t = gui->capturedPieces[c][i].texture;
    //         if(t) SDL_RenderCopy(renderer, t, NULL, &gui->capturedPieces[c][i].rect);
    //     }
    // }
    //Draw move history

int counter = game->hashCount;

// Draw Panel
int moveHistoryBaseX = WINDOW_WIDTH * 0.80;
int moveHistoryBaseY = 10;
int moveHistoryWidth = WINDOW_WIDTH * 0.2;
int moveHistoryHeight = g_squareSize * 10;
SDL_Rect panel = {moveHistoryBaseX, moveHistoryBaseY, moveHistoryWidth, moveHistoryHeight};

// Draw background
SDL_Color bgColor = {30, 30, 30, 255};
SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
SDL_RenderFillRect(renderer, &panel);

// Draw border
SDL_Color borderColor = {100, 100, 100, 255};
SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
SDL_RenderDrawRect(renderer, &panel);

// Add title
int titleHeight = 0;
if (gui->moveHistoryFont) {
    SDL_Color titleColor = {200, 200, 200, 255};
    SDL_Surface *titleSurf = TTF_RenderText_Blended(gui->moveHistoryFont, "Moves History:", titleColor);
    if (titleSurf) {
        SDL_Texture *titleTex = SDL_CreateTextureFromSurface(renderer, titleSurf);
        SDL_Rect titleRect = {moveHistoryBaseX + 5, moveHistoryBaseY + 5, titleSurf->w, titleSurf->h};
        SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);
        titleHeight = titleSurf->h;
        if(titleTex)
            SDL_DestroyTexture(titleTex);
        SDL_FreeSurface(titleSurf);
    }
}

// Calculate scrolling parameters
int startY = moveHistoryBaseY + titleHeight + 10; // Offset for title
int lineHeight = 18;  // Height per move
int availableHeight = moveHistoryHeight - titleHeight - 15; // Space for moves
int maxVisibleMoves = availableHeight / lineHeight;

// Calculate which moves to show (auto-scroll to bottom)
int startIndex = 0;
if (counter - 1 > maxVisibleMoves) {
    startIndex = (counter - 1) - maxVisibleMoves;
}

// Draw move history with scrolling
for(int i = startIndex; i < counter; i++) {

    int displayIndex = i - startIndex;
    
    // Highlight last move with different background
    bool isLastMove = (i == counter - 1); // Last move in history
    
    if (isLastMove) {
        // Draw highlight background for last move
        SDL_Color highlightBg = {50, 50, 70, 255}; // Darker blue
        SDL_SetRenderDrawColor(renderer, highlightBg.r, highlightBg.g, highlightBg.b, highlightBg.a);
        SDL_Rect highlightRect = {
            moveHistoryBaseX + 2,
            startY + displayIndex * lineHeight,
            moveHistoryWidth - 4,
            lineHeight
        };
        SDL_RenderFillRect(renderer, &highlightRect);
    }
    
    SDL_Rect moveRect = {
        moveHistoryBaseX + 5,
        startY + displayIndex * lineHeight,
        moveHistoryWidth - 10,
        16
    };
    
    // Use different color for last move
    SDL_Color textColor;
    if (isLastMove) {
        textColor = (SDL_Color){255, 255, 100, 255}; // Yellow for last move
    } else {
        if(i %2 == 1) //white
            textColor = (SDL_Color){180, 180, 180, 255}; // Slightly darker gray for white moves
        else //black
            textColor = (SDL_Color){240, 240, 240,250};
    }
    
    SDL_Surface *moveSur = TTF_RenderUTF8_Blended(
        gui->moveHistoryFont, 
        game->moveHistory[i], 
        textColor
    );
    
    if (moveSur) {
        moveRect.w = moveSur->w;
        moveRect.h = moveSur->h;
        SDL_Texture *moveTexture = SDL_CreateTextureFromSurface(renderer, moveSur);
        SDL_FreeSurface(moveSur);
        SDL_RenderCopy(renderer, moveTexture, NULL, &moveRect);
        if(moveTexture)
            SDL_DestroyTexture(moveTexture);
    }
}

// Optional: Draw scroll indicator if there are more moves above
if (startIndex > 0) {
    SDL_Color scrollColor = {150, 150, 150, 255};
    SDL_SetRenderDrawColor(renderer, scrollColor.r, scrollColor.g, scrollColor.b, scrollColor.a);
    // Draw up arrow or indicator
    int arrowY = startY - 5;
    for (int i = 0; i < 3; i++) {
        SDL_RenderDrawLine(renderer, 
            moveHistoryBaseX + moveHistoryWidth/2 - i, arrowY + i,
            moveHistoryBaseX + moveHistoryWidth/2 + i, arrowY + i);
    }
}
    // Draw captured pieces with MATERIAL COUNT DISPLAY
    for(int c=0; c<2; c++)
    {
        // Count pieces by type
        int counts[6] = {0}; // [PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING]
        Piece *capturedArray = (c == 0) ? game->capturedWhitePieces : game->capturedBlackPieces;
        int totalCaptured = countPiecesByType(capturedArray, counts);
        
        if (totalCaptured > 0) {
            // Calculate panel position
            int baseX = (c == 0) ? 
                (WINDOW_WIDTH - BUTTON_WIDTH - WINDOW_WIDTH * 0.15 - 100) :
                (WINDOW_WIDTH - BUTTON_WIDTH - WINDOW_WIDTH * 0.00 - 100);
            int baseY = WINDOW_HEIGHT * 0.15 - 20;
            int panelWidth = g_squareSize * 2;
            int panelHeight = g_squareSize * 5;
            
            SDL_Rect panel = {baseX, baseY, panelWidth, panelHeight};
            
            // // Draw background panel
            // SDL_Color bgColor = {40, 40, 40, 255};
            // SDL_SetRenderDrawColor(renderer, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
            // SDL_RenderFillRect(renderer, &panel);
            
            // Draw border
            SDL_Color borderColor = {100, 100, 100, 255};
            SDL_SetRenderDrawColor(renderer, borderColor.r, borderColor.g, borderColor.b, borderColor.a);
            SDL_RenderDrawRect(renderer, &panel);
            
            // Draw title
            if (gui->moveFont) {
                char title[32];
                snprintf(title, sizeof(title), "%s Lost:", c == 0 ? "White" : "Black");
                SDL_Color titleColor = {200, 200, 200, 255};
                
                SDL_Surface *titleSurf = TTF_RenderText_Blended(gui->moveFont, title, titleColor);
                if (titleSurf) {
                    SDL_Texture *titleTex = SDL_CreateTextureFromSurface(renderer, titleSurf);
                    SDL_Rect titleRect = {
                        panel.x + 10,
                        panel.y + 10,
                        titleSurf->w,
                        titleSurf->h
                    };
                    SDL_RenderCopy(renderer, titleTex, NULL, &titleRect);
                    SDL_DestroyTexture(titleTex);
                    SDL_FreeSurface(titleSurf);
                }
            }
            
            // Draw piece counts
            int yOffset = panel.y + 45;
            int iconSize = g_squareSize * 0.50;
            int lineSpacing = g_squareSize * 0.70;
            
            // Piece type names for creating filenames
            const char* pieceNames[] = {"p", "n", "b", "r", "q", "k"};            
            for (int pieceType = 0; pieceType < 6; pieceType++) {
                if (counts[pieceType] > 0) {
                    // Load piece texture
                    char filename[100];
                    snprintf(filename, sizeof(filename), 
                            "%s/%s%s.png", 
                            fullPaths[currentPiecesIndex],
                            (c == 0 ? "w" : "b"),
                            pieceNames[pieceType]);
                    
                    SDL_Texture *pieceIcon = loadtexture(filename, renderer);
                    if (pieceIcon) {
                        // Draw piece icon
                        SDL_Rect iconRect = {panel.x + 15, yOffset, iconSize, iconSize};
                        SDL_RenderCopy(renderer, pieceIcon, NULL, &iconRect);
                        SDL_DestroyTexture(pieceIcon);
                    }
                    
                    // Draw count text
                    if (gui->moveFont) {
                        char countText[16];
                        snprintf(countText, sizeof(countText), "x%d", counts[pieceType]);
                        SDL_Color countColor = {0, 0, 0, 255};
                        
                        SDL_Surface *countSurf = TTF_RenderText_Blended(gui->moveFont, countText, countColor);
                        if (countSurf) {
                            SDL_Texture *countTex = SDL_CreateTextureFromSurface(renderer, countSurf);
                            SDL_Rect countRect = {
                                panel.x + 20 + iconSize,
                                yOffset + (iconSize - countSurf->h) / 2,
                                countSurf->w,
                                countSurf->h
                            };
                            SDL_RenderCopy(renderer, countTex, NULL, &countRect);
                            SDL_DestroyTexture(countTex);
                            SDL_FreeSurface(countSurf);
                        }
                    }
                    
                    yOffset += lineSpacing;
                }
            }
            
            // Draw separator line
            yOffset += 10;
            SDL_SetRenderDrawColor(renderer, 80, 80, 80, 255);
            SDL_RenderDrawLine(renderer, panel.x + 10, yOffset, panel.x + panel.w - 10, yOffset);
            yOffset += 15;
            
            // Draw material value
            if (gui->moveFont) {
                int materialValue = calculateMaterialValue(counts);
                char materialText[32];
                snprintf(materialText, sizeof(materialText), "%d pts", materialValue);
                SDL_Color materialColor = {0, 0, 0, 255};
                
                SDL_Surface *materialSurf = TTF_RenderText_Blended(gui->moveFont, materialText, materialColor);
                if (materialSurf) {
                    SDL_Texture *materialTex = SDL_CreateTextureFromSurface(renderer, materialSurf);
                    SDL_Rect materialRect = {
                        panel.x + 10,
                        yOffset,
                        materialSurf->w,
                        materialSurf->h
                    };
                    SDL_RenderCopy(renderer, materialTex, NULL, &materialRect);
                    SDL_DestroyTexture(materialTex);
                    SDL_FreeSurface(materialSurf);
                }
            }
        }
    }

    //Last move
    if(gui->lastMove.texture)
        SDL_RenderCopy(renderer,gui->lastMove.texture,NULL,&gui->lastMove.rect);



    // Draw buttons
    SDL_RenderCopy(renderer,gui->runMusic.texture,NULL,&gui->runMusic.rect);
    SDL_RenderCopy(renderer, gui->back.texture, NULL, &gui->back.rect);
    SDL_RenderCopy(renderer, gui->undo.texture, NULL, &gui->undo.rect);
    SDL_RenderCopy(renderer, gui->redo.texture, NULL, &gui->redo.rect);
    SDL_RenderCopy(renderer, gui->save.texture, NULL, &gui->save.rect);
    SDL_RenderCopy(renderer,gui->drawAgreement.texture,NULL,&gui->drawAgreement.rect);
    // SDL_RenderCopy(renderer, gui->capWhite.texture, NULL, &gui->capWhite.rect);
    // SDL_RenderCopy(renderer, gui->capBlack.texture, NULL, &gui->capBlack.rect);
    SDL_RenderCopy(renderer,gui->arrowBack.texture,NULL,&gui->arrowBack.rect);
    SDL_RenderCopy(renderer,gui->arrowForward.texture,NULL,&gui->arrowForward.rect);
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


//  HANDLE EVENTS // The most important part it almost do every thing in the game loop
bool gameScreenHandleEvents(gamegui *gui, SDL_Event *event, App *app,
                            bool *dragging, SDL_Rect *dragRect, Position *draggedPiece, SDL_Renderer *renderer)
{
    if(!gui || !event || !app || !dragging || !dragRect || !draggedPiece || !renderer) return false;

    int mx, my;

    if(event->type == SDL_QUIT)
    {
        app->running = false;
        return true;
    }

    if(event->type == SDL_KEYDOWN)
    {
        switch(event->key.keysym.sym)
        {
            case SDLK_ESCAPE:
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
            case SDLK_s:
                {
                    char fileName[50] = {0};
                    int slot = showSaveSlotDialog(app->window,"Save A GAME");
                    
                    if (slot < 1 || slot > 8) {
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
                    
                    if (!saveGame(fptr, &app->gamestack->curGame,app->gamestack)) {
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
                case SDLK_RIGHT:
                {
                    highlightClickedButton(renderer,gui->arrowForward);
                    if(colorCounter < numColors-1){
                        colorCounter++;
                    }
                    if(currentPiecesIndex < numPieces -1){
                        currentPiecesIndex++;
                    }
                    currentLight = lightSquare[colorCounter];
                    currentDark  = darkSquare[colorCounter];
                    updateGameGui(gui,&app->game,app->renderer);
                    break;

                }
                case SDLK_LEFT:
                {
                    highlightClickedButton(renderer,gui->arrowBack);
                    if(colorCounter >0){
                        colorCounter--;
                    }
                    if(currentPiecesIndex >0){
                        currentPiecesIndex--;
                    }
                    currentLight = lightSquare[colorCounter];
                    currentDark  = darkSquare[colorCounter];
                    updateGameGui(gui,&app->game,app->renderer);
                    break;

                }
        }
    }

    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT && !(*dragging))
    {
        //Drag a piece or click on controloers buttons like undo redo switch background themes draw agreement switch off bg music etc
        
        //Taking the cooridnates of the current mouse position
        mx = event->button.x;
        my = event->button.y;
    if (isButtonClicked(mx, my, gui->runMusic)) {
    musicPlaying = !musicPlaying; //Toogle run---->not run      not run---->run
    highlightClickedButton(renderer, gui->runMusic);

    if (musicPlaying) {
        // If music was paused, resume. If it was never started, play.
        if (Mix_PausedMusic()) {
                Mix_ResumeMusic();
        } else {
                Mix_PlayMusic(bgMusic, -1);
            }
        SDL_DestroyTexture(gui->runMusic.texture);
        gui->runMusic.texture = loadtexture("assets/musicrun.png", renderer);
        } else {
            Mix_PauseMusic();
            SDL_DestroyTexture(gui->runMusic.texture);
            gui->runMusic.texture = loadtexture("assets/musichalted.png", renderer);
        }
    }
    else if(isButtonClicked(mx, my, gui->arrowForward)) {
        highlightClickedButton(renderer,gui->arrowForward);
        if(colorCounter < numColors-1){
            colorCounter++;
        }
        if(currentPiecesIndex < numPieces -1){
            currentPiecesIndex++;
        }
        currentLight = lightSquare[colorCounter];
        currentDark  = darkSquare[colorCounter];
        updateGameGui(gui,&app->game,app->renderer);
    }
    else if(isButtonClicked(mx, my, gui->arrowBack)) {
        highlightClickedButton(renderer,gui->arrowBack);
        if(colorCounter >0){
            colorCounter--;
        }
        if(currentPiecesIndex >0){
            currentPiecesIndex--;
        }
        currentLight = lightSquare[colorCounter];
        currentDark  = darkSquare[colorCounter];
        updateGameGui(gui,&app->game,app->renderer);
    }

    else if(isButtonClicked(mx,my,gui->save))
    {
        highlightClickedButton(app->renderer,gui->save);
        char fileName[50] = {0};
        int slot = showSaveSlotDialog(app->window,"Save A GAME");
        
        if (slot < 1 || slot > 8) {
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
        
        if (!saveGame(fptr, &app->gamestack->curGame,app->gamestack)) {
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

        // Check board and get current row and col NOTE THAT in SDL x is the column and y is the row
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
            case PLAYING:
            break;
            case CHECK:
                playSoundEffect(gui->sEffect.check);
                //if(newGame.currentPlayer == WHITE)
                    //howGameMessage(app->window,"Warning","⚠️  CHECK! White King under attack!");
                //else
                    //showGameMessage(app->window,"Warning","⚠️  CHECK! Black King under attack!");
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
                printf("\n CHECKMATE! %s wins!\n", (newGame.currentPlayer == WHITE) ? "BLACK" : "WHITE");
                app->currentScreen = SCREEN_MENU;
                break;
            case STALEMATE:
                SDL_Texture *stalemate = NULL;
                stalemate = loadtexture("assets/smate.png",renderer);
                SDL_RenderCopy(renderer,stalemate,NULL,NULL);
                SDL_RenderPresent(renderer);
                playSoundEffect(gui->sEffect.stalemate);
                showGameMessage(app->window,"End of game","🤝 STALEMATE! Game is a draw.");
                 printf("\n STALEMATE! Game is a draw.\n");
                app->currentScreen = SCREEN_MENU;
                SDL_DestroyTexture(stalemate);
                break;
            case DRAW_FIFTY_MOVE:
                SDL_Texture *fiftymove = NULL;
                fiftymove = loadtexture("assets/50.png",renderer);
                SDL_RenderCopy(renderer,fiftymove,NULL,NULL);
                SDL_RenderPresent(renderer);
                playSoundEffect(gui->sEffect.draw);
                showGameMessage(app->window,"End of game","🤝 DRAW by fifty-move rule!");
                 printf("\n DRAW by fifty-move rule!\n");
                app->currentScreen = SCREEN_MENU;
                SDL_DestroyTexture(fiftymove);
                break;
            case DRAW_AGREEMENT:
                SDL_Texture *drawagreement = NULL;
                drawagreement = loadtexture("assets/agree.png",renderer);
                SDL_RenderCopy(renderer,drawagreement,NULL,NULL);
                SDL_RenderPresent(renderer);
                playSoundEffect(gui->sEffect.draw);
                showGameMessage(app->window,"End of game","🤝 Game ended in a draw."); //TODO this logic
                 printf("\n Game ended in a draw.\n");
                app->currentScreen = SCREEN_MENU;
                SDL_DestroyTexture(drawagreement);
                break;
            
            case DRAW_INSUFFICIENT_MATERIAL:
                SDL_Texture *insuff = NULL;
                insuff = loadtexture("assets/insuff.png",renderer);
                SDL_RenderCopy(renderer,insuff,NULL,NULL);
                SDL_RenderPresent(renderer);
                playSoundEffect(gui->sEffect.draw);
                showGameMessage(app->window,"End of game","🤝 Game ended in an Insufficent material position.");
                printf("\n   Game ended in an Insufficent material position.\n");
                app->currentScreen = SCREEN_MENU;
                SDL_DestroyTexture(insuff);
                break;
            
             case DRAW_THREEFOLD_REPTITION:
                SDL_Texture *repti = NULL;
                repti = loadtexture("assets/rep.png",renderer);
                SDL_RenderCopy(renderer,repti,NULL,NULL);
                SDL_RenderPresent(renderer);
                playSoundEffect(gui->sEffect.draw);
                showGameMessage(app->window,"End of game","🤝 Game ended in a threefold reptition position.");
                printf("\n   Game ended in a threefold reptition position.\n");
                app->currentScreen = SCREEN_MENU;
                SDL_DestroyTexture(repti);
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
            }
            else printf("Error: Failed to push game state\n");
        }
        else 
        {
            playSoundEffect(gui->sEffect.illegalMove);
        }
    }

    return false;
}


//  RUN GAME LOOP 
void runGame(App *app, gamegui *gui)
{
    if(!app || !gui) return;
    if (!musicPlaying && bgMusic) {
        Mix_PlayMusic(bgMusic, -1);
        musicPlaying = true;
    }
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

            char filename[100];
            get_piece_path(piece,filename);
            


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
            char filename[100];
            get_piece_path(game->capturedWhitePieces[i],filename);
            SDL_Texture *txt = loadtexture(filename, renderer);
            if(txt)
            {
                gui->capturedPieces[0][i].texture = txt;
                int baseXWhite = WINDOW_WIDTH - BUTTON_WIDTH - WINDOW_WIDTH * 0.20;
                int baseYWhite = WINDOW_HEIGHT * 0.15;
                int col = i % 2;
                int row = i / 2;
                int piece_size = g_squareSize * 0.60;
                int piece_spacing = g_squareSize * 1.00;
                gui->capturedPieces[0][i].rect = (SDL_Rect){baseXWhite + col * piece_spacing, baseYWhite + row * piece_spacing, piece_size, piece_size};

            }
        }

        // Black captured
        if(gui->capturedPieces[1][i].texture) SDL_DestroyTexture(gui->capturedPieces[1][i].texture);
        gui->capturedPieces[1][i].texture = NULL;
        if(game->capturedBlackPieces[i].type != EMPTY)
        {
            char filename[100];
            get_piece_path(game->capturedBlackPieces[i],filename);
            SDL_Texture *txt = loadtexture(filename, renderer);
            if(txt)
            {
                gui->capturedPieces[1][i].texture = txt;
                int col = i % 2;
                int row = i / 2;
                int baseX_black = WINDOW_WIDTH - BUTTON_WIDTH - WINDOW_WIDTH * 0.00;
                int baseY_black = WINDOW_HEIGHT * 0.15;
                int piece_size = g_squareSize * 0.60;
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

    int buttonid = QUEEN;
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
void highlightValidMoves(Game *game,Position from,SDL_Renderer *renderer,SDL_Texture *point)
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
                SDL_RenderCopy(renderer,point,NULL,&validPlace);
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
    sprintf(text, "Half Move Clock : %d",halfMoveClock);
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

