#include <iostream>
#include <err.h>
#include <string>
#include <queue>
#include <climits>
#include <unordered_set>
#include <stack>
#include <algorithm>
#include <list>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <set>
#include <stdint.h>

#include "../headers/Globcnst.h"
#include "../headers/Globvars.h"
#include "../headers/OverloadedOperators.h"
#include "../headers/MenuFunc.h"

void SelectAmountOfPlayersMenu()
{
  X_offset = 0;
  Y_offset = 0;

  ButtonVector[5].keytrick = false;
  ButtonVector[6].keytrick = false;
  ButtonVector[7].keytrick = false;
  ButtonVector[8].keytrick = false;
  ButtonVector[9].keytrick = false;

  Text_num* Textures = new Text_num [menu_x_field * menu_y_field];
  EnumerateField(menu_y_field, menu_x_field, my_Font, Textures);

  while (GameState == SAP)
  {
    ButtonVector[5].Update(mouse);
    ButtonVector[6].Update(mouse);
    ButtonVector[7].Update(mouse);
    ButtonVector[8].Update(mouse);
    ButtonVector[9].Update(mouse);
    mouse.Update();

    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT) )
      exit(0);

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[9].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = Menu;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[9].keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      GameState = Menu;

    if ((event.key.keysym.sym == SDLK_ESCAPE) && (event.type == SDL_KEYUP) )
      GameState = Menu;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[5].IsSelected) 
      && (event.type == SDL_MOUSEBUTTONUP))
      GameState = ONEPM;

    if ( (event.key.keysym.sym == SDLK_1) && (event.type == SDL_KEYUP) )
      GameState = ONEPM;

    if( (event.key.keysym.sym == SDLK_1) && (event.type == SDL_KEYDOWN) )
      ButtonVector[5].keytrick = true;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[6].IsSelected) 
      && (event.type == SDL_MOUSEBUTTONUP))  
      GameState = TWOPM;

    if ( (event.key.keysym.sym == SDLK_2) && (event.type == SDL_KEYUP) )
      GameState = TWOPM;

    if( (event.key.keysym.sym == SDLK_2) && (event.type == SDL_KEYDOWN) )
      ButtonVector[6].keytrick = true;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[7].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP))
      GameState = THREEPM;

    if ( (event.key.keysym.sym == SDLK_3) && (event.type == SDL_KEYUP) )
      GameState = THREEPM;

    if( (event.key.keysym.sym == SDLK_3) && (event.type == SDL_KEYDOWN) )
      ButtonVector[7].keytrick = true;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[8].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = FOURPM;

    if ( (event.key.keysym.sym == SDLK_4) && (event.type == SDL_KEYUP) )
      GameState = FOURPM;

    if( (event.key.keysym.sym == SDLK_4) && (event.type == SDL_KEYDOWN) )
      ButtonVector[8].keytrick = true;

    SDL_GetMouseState(&xMouse,&yMouse);
    Zoom();
    if (GetDotInput(dots, cur_color, menu_x_field, menu_y_field))
      ChangeDotColor();
    GetDotErase(dots, menu_x_field, menu_y_field);
    SDL_SetRenderDrawColor(renderer, red_default, green_default, 
      blue_default, BACKGROUND_OPACITY);
    SDL_RenderClear(renderer);
    MoveBoard(menu_x_field, menu_y_field, move_game_speed);
    DrawField(menu_x_field, menu_y_field);
    DrawDots(dots, menu_x_field * menu_y_field, menu_x_field);
    DrawFieldNumbers (menu_y_field, menu_x_field, my_Font, Textures);
    CheckPositionInField(menu_x_field, menu_y_field);
    ProcessDotInteraction(dots, menu_x_field, menu_y_field);

    LabelVector[0].Draw();
    ButtonVector[5].Draw();
    ButtonVector[6].Draw();
    ButtonVector[7].Draw();
    ButtonVector[8].Draw();
    ButtonVector[9].Draw();
    mouse.DrawDot(cur_color);
    
    SDL_RenderPresent(renderer);
  }
  delete [] Textures;
}  