#ifndef GAME_GUI_H
#define GAME_GUI_H



#include "GuiDefinitions.h"
typedef struct
{
    Mix_Chunk * normalMove;
    Mix_Chunk * promotion;
    Mix_Chunk * castling;
    Mix_Chunk * enpassent;
    Mix_Chunk * capture;
    Mix_Chunk *illegalMove;
    Mix_Chunk * checkmate;
    Mix_Chunk * check;
    Mix_Chunk * stalemate;
    Mix_Chunk * draw;
    Mix_Chunk * drawOffer;
}SoundEffects;
typedef struct 
{
    SoundEffects sEffect;
    Button save;
    Button undo;
    Button redo;
    Button back;
    Button drawAgreement;
    Button capWhite;
    Button capBlack;
    Button currentTurn;           
    SDL_Texture *whiteTurnTex;    
    SDL_Texture *blackTurnTex;    
    Button capturedPieces[2][16];
    Button boardPieces[8][8];
    Button lastMove;
    Button currentGameState;      // check/checkmate/enpassant etc.
    SDL_Texture *fileLabelTextures[8];  // A.png through H.png
    SDL_Texture *rankLabelTextures[8];  // 1.png through 8.png
    SDL_Texture *background;
    SDL_Rect fromMovingRect;
    SDL_Rect toMovingRect;
    SDL_Rect kingThreaten;
    TTF_Font *moveFont;  // Font for displaying move notation (loaded once at init)
    // Dynamic dimensions for resize support
    int squareSize;
    int boardOffset;
    int windowWidth;
    int windowHeight;
} gamegui;




gamegui *initGameScreenGui(SDL_Renderer *renderer, Game *initialGame);
void renderGameScreenGui(SDL_Renderer *renderer, gamegui *gui, Game *game, 
                         bool dragging, SDL_Texture *dragTexture, SDL_Rect dragRect,Position draggedInitialPosition);
void destroyGameScreenGui(gamegui *gui);
void runGame(App *app, gamegui *gui);
int pieceIndex(gamegui *gui, Piece piece, int row, int col);
void updateGameGui(gamegui *gui, Game *game, SDL_Renderer *renderer);
int getPromotionDialog(SDL_Window *window);
bool offerDrawDialog(SDL_Window *window);
void clearHighlight(gamegui *gui);
void highlightValidMoves(Game *game,Position from,SDL_Renderer *renderer);
void updateLastMoveTexture(gamegui *gui,SDL_Renderer *renderer, Move move);
#endif