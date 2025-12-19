#ifndef GAME_GUI_DEF_H
#define GAME_GUI_DEF_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include "GameDefinitions.h"
#include"GameInitialization.h"
#include"EndingConditions.h"
#include"SavingLoading.h"
#include"MovingLogic.h"
#include"StackHelpers.h"


#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

// Responsive scaling factors
#define BOARDOFFSET (WINDOW_WIDTH * 0.04)  // 4% of window width
//#define SQUARE_SIZE (WINDOW_WIDTH * 0.08)   // 8% of window width (for 8x8 board)
#define BUTTON_WIDTH (WINDOW_WIDTH * 0.268)  // ~27% of window width
#define BUTTON_HEIGHT (WINDOW_HEIGHT * 0.09)  // ~9% of window height
#define BUTTON_SPACING (WINDOW_WIDTH * 0.008)  // 0.8% spacing between buttons
#define MENU_CENTER_X (WINDOW_WIDTH / 2)
#define MENU_CENTER_Y (WINDOW_HEIGHT / 2)

typedef struct 
{
    SDL_Texture *texture;
    SDL_Rect rect;
}Button;
typedef enum {
    SCREEN_MENU,
    SCREEN_GAME,
    SCREEN_QUIT
} GameState;

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
    bool running;
    GameState currentScreen;
    Game game;          
    Node *gamestack;    
    Node *redostack;





    // Dynamic dimensions for window resize support
    int windowWidth;
    int windowHeight;
    int boardOffset;
    int squareSize;
    int buttonWidth;
    int buttonHeight;
    int buttonSpacing;
} App;


#endif