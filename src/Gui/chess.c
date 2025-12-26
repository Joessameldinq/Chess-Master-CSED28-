

#include "../../include/GuiDefinitions.h"
#include "../../include/MainMenuGui.h"
#include "../../include/GameModeGui.h"
#include "../../include/GuiHelpers.h"
#include "../../include/GameInitialization.h"
#include "../../include/StackHelpers.h"
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



int main(int argc, char *argv[])
{
    // 
    // SDL INITIALIZATION
    // 
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // 
    // SDL_mixer initialization
    // 
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
        fprintf(stderr, "SDL_mixer init failed: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }
    Mix_AllocateChannels(16);  

    // // Load background music
    // Mix_Music *bgMusic = Mix_LoadMUS("assets/eff/theme.mp3");
    // if (!bgMusic) {
    //     fprintf(stderr, "Failed to load mp3: %s\n", Mix_GetError());
    // }
    // Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

    // 
    // SDL_image initialization
    // 
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        fprintf(stderr, "IMG_Init Error: %s\n", IMG_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        return 1;
    }
    //
    //SDL ttf initialization
    //
    //TTF_Init return zero on sucess -1 on failure
    if(TTF_Init()){
        fprintf(stderr,"TTF_Init Error: %s\n",TTF_GetError());
        Mix_CloseAudio();
        SDL_Quit();
        IMG_Quit();
        return 1;
    }

    
    // 
    // Create fullscreen window + renderer
    // 
    App app = {0};

    app.window = SDL_CreateWindow("Chess Game",
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        WINDOW_WIDTH,
                                        WINDOW_HEIGHT,
                                        SDL_WINDOW_SHOWN);
    if (!app.window) {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        Mix_CloseAudio();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    app.renderer = SDL_CreateRenderer(app.window, -1,
                                      SDL_RENDERER_ACCELERATED |
                                      SDL_RENDERER_PRESENTVSYNC);
    if (!app.renderer) {
        fprintf(stderr, "SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(app.window);
        Mix_CloseAudio();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }



    // Initialize dynamic dimensions from window size
    SDL_GetWindowSize(app.window, &app.windowWidth, &app.windowHeight);
    app.boardOffset = (int)(app.windowWidth * 0.04);
    app.squareSize = (int)(app.windowWidth * 0.08);
    app.buttonWidth = (int)(app.windowWidth * 0.268);
    app.buttonHeight = (int)(app.windowHeight * 0.09);
    app.buttonSpacing = (int)(app.windowWidth * 0.008);
    //=================================================
    
    // 
    // Initialize GUI and stacks
    // 
    app.running = true;
    app.currentScreen = SCREEN_MENU;
    app.gamestack = initializeStack();
    app.redostack = initializeStack();



    mainmenugui    *mainMenuGui   = initMainMenuGui(app.renderer);
    gamegui *gameGui = NULL; // It will be initialized later
    // bool musicPlaying = false;
    

    // 
    // Main game loop
    // 
    while (app.running)
    {
        GameState prev = app.currentScreen;

        switch (app.currentScreen)
        {
            case SCREEN_MENU:
                if (gameGui) {
                    destroyGameScreenGui(gameGui);
                    gameGui = NULL;  // Prevent use after free or double free
                }
                if(!mainMenuGui)
                    mainMenuGui = initMainMenuGui(app.renderer);
                
                // Clear renderer to prevent graphics artifacts
                SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
                SDL_RenderClear(app.renderer);
                //================================================

                // Stop music when returning to menu
                // if (musicPlaying) {
                //     Mix_HaltMusic();
                //     musicPlaying = false;
                // }

                runMainMenuGui(&app, mainMenuGui);
                break;

            case SCREEN_GAME:
                if (!gameGui) {
                    gameGui = initGameScreenGui(app.renderer, &app.game);
                }
                // Clear renderer
                SDL_SetRenderDrawColor(app.renderer, 0, 0, 0, 255);
                SDL_RenderClear(app.renderer);
                //===========================
                
                // Start music only once
                // if (!musicPlaying && bgMusic) {
                //     Mix_PlayMusic(bgMusic, -1);
                //     musicPlaying = true;
                // }
                runGame(&app, gameGui);
                break;
        }

        // Quit confirm
        if (!app.running ) {
            if (showExitConfirm(app.window) == 2)
                {app.running = true;
                app.currentScreen = prev;}
        }
    }

    // 
    // CLEANUP
    // 
    if(mainMenuGui) {
        destroyMainMenuGui(mainMenuGui);
        mainMenuGui = NULL;
    }
    if (gameGui) {
        destroyGameScreenGui(gameGui);
        gameGui = NULL;
    }

    clearStack(&app.gamestack);
    clearStack(&app.redostack);

    // if (bgMusic) Mix_FreeMusic(bgMusic);
    Mix_CloseAudio();
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();




    // To handle unsued parameters warning
    (void)argc;
    (void)argv;
    return 0;
}