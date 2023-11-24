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

void OnePlayerMenu()
{
  X_offset = 0;
  Y_offset = 0;
  ButtonVector[10].keytrick = false;

  Text_num* Textures = new Text_num [menu_x_field * menu_y_field];
  EnumerateField(menu_y_field, menu_x_field, my_Font, Textures);

  while (GameState == ONEPM)
  {
    ButtonVector[10].Update(mouse);
    mouse.Update();

    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT))
      exit(0);

    if ((event.key.keysym.sym == SDLK_ESCAPE) && (event.type == SDL_KEYDOWN))
      GameState = Menu;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[10].keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      GameState = SAP;

    if ( (ButtonVector[10].IsSelected)
       && (event.button.button == SDL_BUTTON_LEFT)
       && (event.type == SDL_MOUSEBUTTONUP))
      GameState = SAP;
    
    SDL_GetMouseState(&xMouse,&yMouse);
    Zoom();
    MoveBoard(menu_x_field, menu_y_field, move_game_speed);
    SDL_SetRenderDrawColor(renderer, background_red, background_green,
      background_blue, BACKGROUND_OPACITY);
    SDL_RenderClear(renderer);
    DrawFieldNumbers (menu_y_field, menu_x_field, my_Font, Textures);
    DrawField(menu_x_field, menu_y_field);
    if (GetDotInput(dots, cur_color, menu_x_field, menu_y_field))
      ChangeDotColor();
    GetDotErase(dots, menu_x_field, menu_y_field);
    DrawDots(dots, menu_x_field * menu_y_field, menu_x_field);
    CheckPositionInField(menu_x_field, menu_y_field);
    ProcessDotInteraction(dots, menu_x_field, menu_y_field);

    LabelVector[1].Draw();
    LabelVector[2].Draw();
    ButtonVector[10].Draw();
    mouse.DrawDot(cur_color);
    
    SDL_RenderPresent(renderer);
  }

  delete [] Textures;
}