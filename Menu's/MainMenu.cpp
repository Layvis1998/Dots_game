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


void MainMenu()
{
  X_offset = 0;
  Y_offset = 0;
  
  ButtonVector[0].keytrick = false;
  ButtonVector[1].keytrick = false;
  ButtonVector[2].keytrick = false;
  ButtonVector[3].keytrick = false;
  ButtonVector[4].keytrick = false;

  Text_num* Textures = new Text_num [menu_x_field * menu_y_field];
  EnumerateField(menu_y_field, menu_x_field, my_Font, Textures);

  list <ColorSpace> Spaces;

  while (GameState == Menu)
  { 
    mouse.Update();
    ButtonVector[0].Update(mouse);
    ButtonVector[1].Update(mouse);
    ButtonVector[2].Update(mouse);    
    ButtonVector[3].Update(mouse);
    ButtonVector[4].Update(mouse);

    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT))
      exit(0);

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[4].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      exit(0);

    if ((event.key.keysym.sym == SDLK_e) && (event.type == SDL_KEYUP))
      exit(0);

    if ((event.key.keysym.sym == SDLK_e) && (event.type == SDL_KEYDOWN))
      ButtonVector[4].keytrick = true;

    if ((event.key.keysym.sym == SDLK_n) && (event.type == SDL_KEYDOWN))
      ButtonVector[0].keytrick = true;

    if ((event.key.keysym.sym == SDLK_n) && (event.type == SDL_KEYUP))
      GameState = SAP;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[0].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = SAP;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN))
      ButtonVector[3].keytrick = true;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP))
      GameState = Credits;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[3].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = Credits;

    SDL_GetMouseState(&xMouse,&yMouse);
    Zoom();
    if (GetDotInput(dots, cur_color, menu_x_field, menu_y_field))
      ChangeDotColor();
    GetDotErase(dots, menu_x_field, menu_y_field);
    MoveBoard(menu_x_field, menu_y_field, move_game_speed);

    SDL_SetRenderDrawColor(renderer, red_default, green_default,
      blue_default, BACKGROUND_OPACITY);
    SDL_RenderClear(renderer);

    DrawFieldNumbers (menu_y_field, menu_x_field, my_Font, Textures);
    DrawField(menu_x_field, menu_y_field);
    Spaces = ProcessDotInteraction(dots, menu_x_field, menu_y_field);

    for (auto i = Spaces.begin(); i != Spaces.end(); i++ )
      ColorColorSpace(i->BorderDots, dots, menu_x_field);

    DrawDots(dots, menu_x_field * menu_y_field, menu_x_field);
    CheckPositionInField(menu_x_field, menu_y_field);

    ButtonVector[0].Draw();
    ButtonVector[1].Draw();
    ButtonVector[2].Draw();
    ButtonVector[3].Draw();
    ButtonVector[4].Draw();
    mouse.DrawDot(cur_color);

    SDL_RenderPresent(renderer);
  }

  delete [] Textures;
}