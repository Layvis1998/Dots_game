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

void ThreePlayerMenu()
{
  X_offset = 0;
  Y_offset = 0;
  ButtonVector[10].keytrick = false;
  ButtonVector[12].keytrick = false;
  player_amount = 3; 

  Text_num* Textures = new Text_num [menu_x_field * menu_y_field];
  EnumerateField(menu_y_field, menu_x_field, my_Font, Textures);

  while (GameState == THREEPM)
  {
    mouse.Update();
    ButtonVector[10].Update(mouse);
    ButtonVector[12].Update(mouse);
    ClrButtonVector[2].Update(mouse);
    ClrButtonVector[3].Update(mouse);
    ClrButtonVector[4].Update(mouse);

    SDL_PollEvent(&event);
    if ( (event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT) )
      exit(0);

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
      GameState = Menu;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      ButtonVector[10].keytrick = false;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[10].keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[10].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP))
      GameState = SAP;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP)) 
      GameState = GSM;

    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN))
      ButtonVector[12].keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[12].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = GSM;

    if ((!ClrButtonVector[2].IsSelected) && (!ClrButtonVector[3].IsSelected)
      && (!ClrButtonVector[4].IsSelected))
      FieldColor();

    if (ClrButtonVector[2].IsSelected)
    { 
      ClrButtonVector[2].ChangeColor();
      cur_color = ClrButtonVector[2].clr;
    }

    if (ClrButtonVector[3].IsSelected)
    { 
      ClrButtonVector[3].ChangeColor();
      cur_color = ClrButtonVector[3].clr;
    }

    if (ClrButtonVector[4].IsSelected)
    { 
      ClrButtonVector[4].ChangeColor();
      cur_color = ClrButtonVector[4].clr;
    }

    SDL_GetMouseState(&xMouse,&yMouse);
    MoveBoard(menu_x_field, menu_y_field, move_game_speed);
    if (GetDotInput(dots, cur_color, menu_x_field, menu_y_field))
      ChangeDotColor();
    GetDotErase(dots, menu_x_field, menu_y_field);
    Zoom();
    SDL_SetRenderDrawColor(renderer, background_red, background_green,
      background_blue, BACKGROUND_OPACITY);
    SDL_RenderClear(renderer);
    DrawField(menu_x_field, menu_y_field);
    DrawDots(dots, menu_x_field * menu_y_field, menu_x_field);
    DrawFieldNumbers (menu_y_field, menu_x_field, my_Font, Textures);
    CheckPositionInField(menu_x_field, menu_y_field);
    ProcessDotInteraction(dots, menu_x_field, menu_y_field);

    ClrButtonVector[2].Draw();
    ClrButtonVector[3].Draw();
    ClrButtonVector[4].Draw();
    LabelVector[3].Draw();
    LabelVector[4].Draw();
    LabelVector[5].Draw();
    ButtonVector[10].Draw();
    ButtonVector[12].Draw();
    mouse.DrawDot(cur_color);
    
    SDL_RenderPresent(renderer);
  }
  delete [] Textures;
}