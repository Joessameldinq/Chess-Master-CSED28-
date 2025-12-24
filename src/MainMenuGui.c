


#include "../include/MainMenuGui.h"
#include "../include/GuiDefinitions.h"
#include "../include/GuiHelpers.h"
#include "../include/DisplayConsole.h"
#include <stdlib.h>
mainmenugui *initMainMenuGui(SDL_Renderer *renderer)
{
    //Caller must free it after moving to another screen
    mainmenugui *gui = (mainmenugui*)malloc(sizeof(mainmenugui));

    ///LOADING TEXTURES USING loadtexuter helper function
    gui->background = loadtexture("assets/ourfinalbg.jpg",renderer);
    gui->loadgame.texture = loadtexture("assets/button_load.bmp",renderer);
    gui->newgame.texture = loadtexture("assets/button_new_game.bmp",renderer);
    gui->quit.texture = loadtexture("assets/button_quit.bmp",renderer);

    //Rectangles - centered horizontally, positioned vertically and is dynamica according to the window size
    int button_width = BUTTON_WIDTH;
    int button_height = BUTTON_HEIGHT;
    int center_x = (WINDOW_WIDTH - button_width) / 2;
    
    gui->loadgame.rect = (SDL_Rect){.x = center_x, .y = WINDOW_HEIGHT * 0.25, .w = button_width, .h = button_height};
    gui->newgame.rect = (SDL_Rect){.x = center_x, .y = WINDOW_HEIGHT * 0.50, .w = button_width, .h = button_height};
    gui->quit.rect = (SDL_Rect){.x = center_x, .y = WINDOW_HEIGHT * 0.75, .w = button_width, .h = button_height};




    return gui; //CallerMustFree it

}
void renderMainMenu(SDL_Renderer *renderer,mainmenugui *gui)
{
    //Clear any screens before this screen
    SDL_RenderClear(renderer);


    //Render the background
    SDL_RenderCopy(renderer,gui->background,NULL,NULL);


    //Render the buttons
    SDL_RenderCopy(renderer,gui->loadgame.texture,NULL,&gui->loadgame.rect);
    SDL_RenderCopy(renderer,gui->newgame.texture,NULL,&gui->newgame.rect);
    SDL_RenderCopy(renderer,gui->quit.texture,NULL,&gui->quit.rect);



    //Render the whole screen
    SDL_RenderPresent(renderer);
    
}

void destroyMainMenuGui(mainmenugui *mainMenuGui)
{


    SDL_DestroyTexture(mainMenuGui->background);


    SDL_DestroyTexture(mainMenuGui->loadgame.texture);
    SDL_DestroyTexture(mainMenuGui->newgame.texture);
    SDL_DestroyTexture(mainMenuGui->quit.texture);



    if(mainMenuGui)
        free(mainMenuGui);
    mainMenuGui = NULL; //set it to null to avoid double free problems
}
void Menu_HandleEvent(mainmenugui *mainMenuGui, SDL_Event *event, App *app)
{

    //i optimized handle events and used wait event instead of poll event !



    if (!mainMenuGui || !event || !app) {
        fprintf(stderr, "Error: NULL parameter in Menu_HandleEvent\n");
        return;
    }

    if(event->type == SDL_QUIT) {
        app->running = false;
        return;
    }
    
    if(event->type == SDL_KEYDOWN)
    {
        if(event->key.keysym.sym == SDLK_ESCAPE) {
            app->running = false;
            return;
        }
        
    }
    
    if(event->type == SDL_MOUSEBUTTONDOWN && event->button.button == SDL_BUTTON_LEFT)
    {
        int mx = event->button.x;
        int my = event->button.y;
        
        
        // Validate coordinates
        if (mx < 0 || my < 0) {
            fprintf(stderr, "Warning: Negative mouse coordinates\n");
            return;
        }

        if(isButtonClicked(mx, my, mainMenuGui->loadgame)) {

            highlightClickedButton(app->renderer,mainMenuGui->loadgame);


            char fileName[60] = {0};
            int slot = showSaveSlotDialog(app->window);
            
            if (slot < 1 || slot > 8) {
                printf("Load cancelled (invalid slot: %d)\n", slot);
                return;
            }


            
            sprintf(fileName, "cfg/slot_%d.bin", slot);
            printf("Attempting to load game from file: %s\n", fileName);
                
            FILE *fptr = fopen(fileName, "rb");
            if (!fptr) {
                    fprintf(stderr, "Error: Could not open file '%s' for reading\n", fileName);
                    printf("Load failed: File not found or cannot be opened\n");
                    showGameMessage(app->window, "Load Failed", "Could not open save file");
                    return;
            }
            
            // Load the game data
            Game loadedGame = {0};
            if (!loadGame(fptr, &loadedGame)) {
                    fprintf(stderr, "Error: Failed to read Game struct from file\n");
                    printf("Load failed: Could not read game data\n");
                    fclose(fptr);
                    showGameMessage(app->window, "Load Failed", "Could not read game data from file");
                    return;
            }
            fclose(fptr);
            pushGameToApp(app,loadedGame);
            
        }
        else if(isButtonClicked(mx, my, mainMenuGui->newgame)) {
            // printf("New Game clicked\n");
            highlightClickedButton(app->renderer,mainMenuGui->newgame);

            //And later in another place we will load a new game
            Game *newGame = initGame();
            pushGameToApp(app,*newGame);
            free(newGame);

        }
        else if(isButtonClicked(mx, my, mainMenuGui->quit)) {
            // printf("Quit clicked\n");
            highlightClickedButton(app->renderer,mainMenuGui->quit);
            app->running = false;
        }
    }
}
void pushGameToApp(App *app , Game game){

    // Clear old stacks and prepare new game
    clearStack(&app->gamestack);
    clearStack(&app->redostack);

    

    app->game = game;

    //Push the game to the stack
    if (!push(&app->gamestack, app->game)) {
                    fprintf(stderr, "Error: Failed to push loaded game to stack\n");
                    printf("Load failed: Could not store game in stack\n");
                    showGameMessage(app->window, "Load Failed", "Could not initialize game stack");
                    return;
    }
    //Change to screen game 
    app->currentScreen = SCREEN_GAME;    

}

void runMainMenuGui(App *app, mainmenugui *gui)
{
    if (!app) {
        fprintf(stderr, "Error: NULL app in runMainMenuGui\n");
        return;
    }
    
    if (!gui) {
        fprintf(stderr, "Error: NULL gui in runMainMenuGui\n");
        return;
    }
    
    if (!app->renderer) {
        fprintf(stderr, "Error: NULL renderer in runMainMenuGui\n");
        return;
    }

    SDL_Event e;
    while(app->running && app->currentScreen == SCREEN_MENU)
    {
        renderMainMenu(app->renderer, gui);
        if(SDL_WaitEvent(&e))
            Menu_HandleEvent(gui,&e,app);
        
    }
}