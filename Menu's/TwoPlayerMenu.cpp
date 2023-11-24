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

void TwoPlayerMenu()
{
  player_amount = 2; 
  X_offset = 0;
  Y_offset = 0;
  list <ColorSpace> Space;
  ButtonVector[10].keytrick = false;
  ButtonVector[12].keytrick = false;

  Text_num* Textures = new Text_num [menu_x_field * menu_y_field];
  EnumerateField(menu_y_field, menu_x_field, my_Font, Textures);

  while (GameState == TWOPM)
  {
    mouse.Update();
    ButtonVector[10].Update(mouse);
    ButtonVector[12].Update(mouse);
    ClrButtonVector[0].Update(mouse);
    ClrButtonVector[1].Update(mouse);

    SDL_PollEvent(&event);
    if ( (event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT) )
      exit(0);

    if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
      GameState = Menu;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      ButtonVector[10].keytrick = false;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[10].keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[10].IsSelected) 
      && (event.type == SDL_MOUSEBUTTONUP))
      GameState = SAP;

    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN) )
      ButtonVector[12].keytrick = true;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP) ) 
      GameState = GSM;

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[12].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP))  
      GameState = GSM;

    if ((!ClrButtonVector[0].IsSelected) && (!ClrButtonVector[1].IsSelected))
      FieldColor();

    if (ClrButtonVector[0].IsSelected)
    { 
      ClrButtonVector[0].ChangeColor();
      cur_color = ClrButtonVector[0].clr;
    }

    if (ClrButtonVector[1].IsSelected)
    { 
      ClrButtonVector[1].ChangeColor();
      cur_color = ClrButtonVector[1].clr;
    }

    SDL_GetMouseState(&xMouse,&yMouse);
    Zoom();
    MoveBoard(menu_x_field, menu_y_field, move_game_speed);
    if (GetDotInput(dots, cur_color, menu_x_field, menu_y_field))
      ChangeDotColor();
    GetDotErase(dots, menu_x_field, menu_y_field);
    SDL_SetRenderDrawColor(renderer, background_red, background_green,
      background_blue, BACKGROUND_OPACITY);
    SDL_RenderClear(renderer);
    DrawFieldNumbers (menu_y_field, menu_x_field, my_Font, Textures);
    DrawField(menu_x_field, menu_y_field);
    DrawDots(dots, menu_x_field * menu_y_field, menu_x_field);
    //EnumerateField(menu_y_field, menu_x_field, my_Font);
    CheckPositionInField(menu_x_field, menu_y_field);
    Space = ProcessDotInteraction(dots, menu_x_field, menu_y_field);

    ClrButtonVector[0].Draw();
    ClrButtonVector[1].Draw();
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