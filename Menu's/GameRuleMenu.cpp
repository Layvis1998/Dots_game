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

void GameRuleMenu()
{
  minintrv = max(ScWidth / double(game_x_field + 4),
    ScHeight / double(game_y_field + 4));
  minintrv = max(minintrv, default_minintrv);
  intrv = minintrv;
  X_offset = 0;
  Y_offset = 0;

  SDL_Surface *surfaceText4;
  SDL_Surface *surfaceText5;
  SDL_Texture *textureText4;
  SDL_Texture *textureText5;
  SDL_Rect rectangle4;
  SDL_Rect rectangle5;

  rectangle_w = LabelVector[9].drect.h * 4 / 5;
  rectangle_h = LabelVector[9].drect.h;

  rectangle4.h = rectangle_h;
  rectangle4.x = LabelVector[9].drect.x
    + LabelVector[9].drect.w + ScWidth / 13;
  rectangle4.y = vo * 4;

  rectangle5.h = rectangle_h;
  rectangle5.x = LabelVector[10].drect.x 
    + LabelVector[10].drect.w + ScWidth / 13;
  rectangle5.y = vo * 5;

  ButtonVector[11].keytrick = false;
  ButtonVector[13].keytrick = false;
  ButtonVector[14].keytrick = false;

  Text_num* Textures = new Text_num [game_x_field * game_y_field];
  EnumerateField(game_y_field, game_x_field, my_Font, Textures);

  while (GameState == GRM)
  {
    mouse.Update();
    ButtonVector[11].Update(mouse);
    ButtonVector[13].Update(mouse);
    ButtonVector[14].Update(mouse); 

    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT))
      exit(0);

    if ((event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP))
      GameState = GSM;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[11].keytrick = true;

    if ((event.button.button == SDL_BUTTON_LEFT) && 
       (ButtonVector[11].IsSelected) && 
       (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = GSM;

    if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
      GameState = Menu;

    if ((event.key.keysym.sym == SDLK_t) && (event.type == SDL_KEYUP))
    { 
      classic = false; 
      GameState = Game;
    }

    if ((event.button.button == SDL_BUTTON_LEFT) &&
       (ButtonVector[14].IsSelected) && 
       (event.type == SDL_MOUSEBUTTONUP)) 
    { 
      classic = false; 
      GameState = Game;
    }

    if ((event.key.keysym.sym == SDLK_t) && (event.type == SDL_KEYDOWN))
      ButtonVector[14].keytrick = true;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP))
      GameState = Game;

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[13].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = Game;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN))
      ButtonVector[13].keytrick = true;

    if ((event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP))  
      GameState = GSM;

    string temp_str;
    if (maxdotamount <= 0)
      temp_str = "~";
    else 
      temp_str = to_string(maxdotamount);
    field_x = (char*) temp_str.c_str();
    surfaceText4 = TTF_RenderText_Solid(my_Font, field_x, {180, 255, 100});
    textureText4 = SDL_CreateTextureFromSurface(renderer, surfaceText4);

    if (maxeatendots <= 0)
      temp_str = "~";
    else 
      temp_str = to_string(maxeatendots);
    field_y = (char*) temp_str.c_str();
    surfaceText5 = TTF_RenderText_Solid(my_Font, field_y, {180, 255, 100});
    textureText5 = SDL_CreateTextureFromSurface(renderer, surfaceText5);
    
    if ((SDL_HasIntersection(&rectangle4, &mouse.point)) &&
       (event.type == SDL_KEYDOWN) &&
       ((event.key.keysym.sym == SDLK_w) || (event.key.keysym.sym == SDLK_UP)))
      maxdotamount++;

    if ((SDL_HasIntersection(&rectangle4, &mouse.point)) &&
       (event.type == SDL_KEYDOWN) &&
       (event.key.keysym.sym == SDLK_UP))
      maxdotamount += 10;

    if ((SDL_HasIntersection(&rectangle4, &mouse.point)) &&
       (event.type == SDL_KEYDOWN) &&
       (event.key.keysym.sym == SDLK_s))
      maxdotamount--;

    if ((SDL_HasIntersection(&rectangle4, &mouse.point)) &&
       (event.type == SDL_KEYDOWN) &&
       (event.key.keysym.sym == SDLK_DOWN))
      maxdotamount -= 10 ;

    if ((SDL_HasIntersection(&rectangle5, &mouse.point)) &&
       (event.type == SDL_KEYDOWN) &&
       (event.key.keysym.sym == SDLK_w))
      maxeatendots++;

    if ((SDL_HasIntersection(&rectangle5, &mouse.point)) &&
       (event.type == SDL_KEYDOWN) &&
       (event.key.keysym.sym == SDLK_UP))
      maxeatendots += 10;

    if ((SDL_HasIntersection(&rectangle5, &mouse.point)) &&
       (event.type == SDL_KEYDOWN) &&
       (event.key.keysym.sym == SDLK_s))
      maxeatendots--;

    if ((SDL_HasIntersection(&rectangle5, &mouse.point)) &&
       (event.type == SDL_KEYDOWN) &&
       (event.key.keysym.sym == SDLK_DOWN))
      maxeatendots -= 10;

    if (maxdotamount < 0)
      maxdotamount = 0;

    if (maxdotamount > 5000)
      maxdotamount = 5000;

    if (maxeatendots < 0)
      maxeatendots = 0;

    if (maxeatendots > 700)
      maxeatendots = 700;

    if (maxdotamount >= 1000)
      rectangle4.w = rectangle_w * 4;
    else if (maxdotamount >= 100)
      rectangle4.w = rectangle_w * 3;
    else if (maxdotamount >= 10)
      rectangle4.w = rectangle_w * 2;
    else
      rectangle4.w = rectangle_w;

    if (maxeatendots >= 100) 
      rectangle5.w = rectangle_w * 3;
    else if (maxeatendots < 10)
      rectangle5.w = rectangle_w;
    else
      rectangle5.w = rectangle_w * 2;

    SDL_GetMouseState(&xMouse,&yMouse);
    Zoom();
    if (!SDL_HasIntersection(&rectangle4, &mouse.point) && 
      !SDL_HasIntersection(&rectangle5, &mouse.point))
      MoveBoard(game_x_field, game_y_field, move_game_speed);
    SDL_SetRenderDrawColor(renderer, background_red, background_green,
      background_blue, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    DrawField(game_x_field, game_y_field);
    CheckPositionInField(game_x_field, game_y_field);
    DrawFieldNumbers (game_y_field, game_x_field, my_Font, Textures);

    SDL_RenderCopy(renderer, textureText4, NULL, &rectangle4);
    SDL_RenderCopy(renderer, textureText5, NULL, &rectangle5);

    SDL_DestroyTexture(textureText4);
    SDL_FreeSurface(surfaceText4);
    SDL_DestroyTexture(textureText5);
    SDL_FreeSurface(surfaceText5);

    ButtonVector[11].Draw();
    ButtonVector[13].Draw();
    ButtonVector[14].Draw();
    LabelVector[7].Draw();
    LabelVector[8].Draw();
    LabelVector[9].Draw();
    LabelVector[10].Draw();
    mouse.DrawDot(cur_color);

    SDL_RenderPresent(renderer);
  }

  delete [] Textures;
}