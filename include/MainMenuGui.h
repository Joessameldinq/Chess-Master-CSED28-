#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "GuiDefinitions.h"
typedef struct 
{
    SDL_Texture *background;
    Button newgame;
    Button loadgame;
    Button quit;
    
}mainmenugui;


void renderMainMenu(SDL_Renderer *renderer,mainmenugui *gui);
mainmenugui *initMainMenuGui(SDL_Renderer *renderer);
void destroyMainMenuGui(mainmenugui *mainMenuGui);
void Menu_HandleEvent(mainmenugui *mainMenuGui,SDL_Event *event,App *app);
void runMainMenuGui(App *app,mainmenugui *gui);
void pushGameToApp(App *app , Game game);


#endif