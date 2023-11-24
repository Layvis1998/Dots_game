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

void GameSizeMenu()
{
  X_offset = 0;
  Y_offset = 0;
  minintrv = default_minintrv;
  SDL_Surface *surfaceText;
  SDL_Surface *surfaceText2;
  SDL_Surface *surfaceText3;
  SDL_Texture *textureText;
  SDL_Texture *textureText2;
  SDL_Texture *textureText3;
  ButtonVector[10].keytrick = false;
  ButtonVector[12].keytrick = false;

  SDL_Rect rectangle;
  SDL_Rect rectangle2;
  SDL_Rect rectangle3;
  rectangle_w = ScWidth / 32;
  rectangle_h = ScWidth / 18;

  rectangle.w = rectangle_w * 2;
  rectangle.h = rectangle_h * 2;
  rectangle.x = ScWidth * 1 / 3 - rectangle.w / 2;
  rectangle.y = vo * 3;

  rectangle2.w = rectangle_w * 2;
  rectangle2.h = rectangle_h * 2;
  rectangle2.x = ScWidth * 2 / 3 - rectangle2.w / 2;
  rectangle2.y = vo * 3;

  rectangle3.w = rectangle_w * 2;
  rectangle3.h = rectangle_h * 2;
  rectangle3.x = ScWidth * 1 / 2 - rectangle3.w / 2;
  rectangle3.y = vo * 3;

  while (GameState == GSM)
  {
    mouse.Update();
    ButtonVector[10].Update(mouse);
    ButtonVector[12].Update(mouse);

    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT))
      exit(0);

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
      GameState = Menu;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP))
      GameState = GRM;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[12].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = GRM;

    if ((event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN))
      ButtonVector[10].keytrick = true;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN))
      ButtonVector[12].keytrick = true;

    if ((event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP))
    {
      switch (player_amount)
      {
        case 1:
        {
          GameState = ONEPM;
          break;
        }  
        case 2:
        {
          GameState = TWOPM;
          break;
        } 
        case 3:
        {
          GameState = THREEPM;
          break;
        } 
        case 4:
        {
          GameState = FOURPM;
        } 
      }
    }

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[10].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
    {
      switch (player_amount)
      {
        case 1:
        {
          GameState = ONEPM;
          break;
        }  
        case 2:
        {
          GameState = TWOPM;
          break;
        } 
        case 3:
        {
          GameState = THREEPM;
          break;
        } 
        case 4:
        {
          GameState = FOURPM;
        } 
      }
    }
    
    surfaceText3 = TTF_RenderText_Solid(my_Font, "x", {255, 255, 0});
    textureText3 = SDL_CreateTextureFromSurface(renderer, surfaceText3);

    string temp_str = to_string(game_x_field);
    field_x = (char*) temp_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, field_x, {180, 255, 100});
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);

    temp_str = to_string(game_y_field);
    field_y = (char*) temp_str.c_str();
    surfaceText2 = TTF_RenderText_Solid(my_Font, field_y, {180, 255, 100});
    textureText2 = SDL_CreateTextureFromSurface(renderer, surfaceText2);
    
    if ((SDL_HasIntersection(&rectangle, &mouse.point)) &&
        (event.type == SDL_KEYDOWN) &&
        (event.key.keysym.sym == SDLK_w))
      game_x_field++;

    if ((SDL_HasIntersection(&rectangle, &mouse.point)) &&
        (event.type == SDL_KEYDOWN) &&
        (event.key.keysym.sym == SDLK_UP))
      game_x_field += 10;

    if ((SDL_HasIntersection(&rectangle, &mouse.point)) &&
       (event.type == SDL_KEYDOWN) &&
      (event.key.keysym.sym == SDLK_s))
      game_x_field--;

    if ((SDL_HasIntersection(&rectangle, &mouse.point)) &&
       (event.type == SDL_KEYDOWN) &&
       (event.key.keysym.sym == SDLK_DOWN))
      game_x_field -= 10;

    if ((SDL_HasIntersection(&rectangle2, &mouse.point)) &&
       (event.type == SDL_KEYDOWN) &&
       ((event.key.keysym.sym == SDLK_w) || (event.key.keysym.sym == SDLK_UP)))
      game_y_field++;

    if ((SDL_HasIntersection(&rectangle2, &mouse.point)) && 
       (event.type == SDL_KEYDOWN) &&
       (event.key.keysym.sym == SDLK_UP))
      game_y_field += 10;

    if ((SDL_HasIntersection(&rectangle2, &mouse.point)) && 
       (event.type == SDL_KEYDOWN) &&
       (event.key.keysym.sym == SDLK_s))
      game_y_field--;

    if ((SDL_HasIntersection(&rectangle2, &mouse.point)) && 
       (event.type == SDL_KEYDOWN) &&
       (event.key.keysym.sym == SDLK_DOWN))
      game_y_field -= 10;

    if (game_y_field < 5)
      game_y_field = 5;

    if (game_y_field > 320)
      game_y_field = 320;

    if (game_x_field < 5)
      game_x_field = 5;

    if (game_x_field > 320)
      game_x_field = 320;

    if (game_x_field >= 100)
      rectangle.w = rectangle_w * 3;
    else if (game_x_field < 10)
      rectangle.w = rectangle_w;
    else
      rectangle.w = rectangle_w * 2;

    if (game_y_field >= 100) 
      rectangle2.w = rectangle_w * 3;
    else if (game_y_field < 10)
      rectangle2.w = rectangle_w;
    else
      rectangle2.w = rectangle_w * 2;

    SDL_GetMouseState(&xMouse,&yMouse);
    Zoom();
    SDL_SetRenderDrawColor(renderer, background_red, background_green,
      background_blue, BACKGROUND_OPACITY);
    SDL_RenderClear(renderer);
    DrawField(game_x_field, game_y_field);
    if (!SDL_HasIntersection(&rectangle, &mouse.point) && 
        !SDL_HasIntersection(&rectangle2, &mouse.point))
      MoveBoard(game_x_field, game_y_field, move_game_speed);
    CheckPositionInField(game_x_field, game_y_field);

    SDL_RenderCopy(renderer, textureText, NULL, &rectangle);
    SDL_RenderCopy(renderer, textureText2, NULL, &rectangle2);
    SDL_RenderCopy(renderer, textureText3, NULL, &rectangle3);

    SDL_DestroyTexture(textureText);
    SDL_DestroyTexture(textureText2);
    SDL_DestroyTexture(textureText3);
    SDL_FreeSurface(surfaceText);
    SDL_FreeSurface(surfaceText2);
    SDL_FreeSurface(surfaceText3);

    ButtonVector[10].Draw();
    ButtonVector[12].Draw();
    LabelVector[6].Draw();
    LabelVector[7].Draw();
    mouse.DrawDot(cur_color);

    SDL_RenderPresent(renderer);
  }

}