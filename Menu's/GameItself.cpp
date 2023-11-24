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


void GameItself()
{ 
  minintrv = max(ScWidth / double(game_x_field + 4),
    ScHeight / double(game_y_field + 4));
  minintrv = max(minintrv, default_minintrv);
  intrv = minintrv;
  dot_size = round(intrv * dots_to_intrv);
  mouse.circle_size = dot_size;
  X_offset = 0;
  Y_offset = 0;

  Dots *dots_game = new Dots[game_x_field * game_y_field];
  for (int i = 0; i < game_y_field * game_x_field; i++)
  {
    dots_game[i].exist = false;
    dots_game[i].visited = false;
    dots_game[i].deadend = false;
    dots_game[i].spaceinside = false;
    dots_game[i].spaceborder = false;
  }

  switch (player_amount)
  {
    case 2:
      cur_color = ClrButtonVector[0].clr;
      break;
    case 3:
      cur_color = ClrButtonVector[2].clr;
      break;
    case 4:
      cur_color = ClrButtonVector[5].clr;
  }

  Text_num* Textures = new Text_num [game_x_field * game_y_field];
  EnumerateField(game_y_field, game_x_field, my_Font, Textures);

  while (1)
  {
    mouse.Update();

    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT))
      exit(0);

    if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
      exit(0);

    SDL_GetMouseState(&xMouse,&yMouse);
    if (GetDotInput(dots_game, cur_color, game_x_field, game_y_field))
      ChangeDotColor();
    Zoom();
    MoveBoard(game_x_field, game_y_field, move_game_speed);
    SDL_SetRenderDrawColor(renderer, background_red, background_green,
      background_blue, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    DrawField(game_x_field, game_y_field);
    DrawDots(dots_game, game_y_field * game_x_field, game_x_field);
    DrawFieldNumbers (game_y_field, game_x_field, my_Font, Textures);
    CheckPositionInField(game_x_field, game_y_field);
    ProcessDotInteraction(dots_game, game_x_field, game_y_field);

    mouse.DrawDot(cur_color);

    SDL_RenderPresent(renderer);    
  }

  delete [] Textures;
  delete dots_game;
}
