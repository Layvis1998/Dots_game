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

void FillCredits(TTF_Font *my_Font, Text_num* textureText)
{
  SDL_Color CreditsColor = {205, 80, 205, SDL_ALPHA_OPAQUE};
  SDL_Surface* surfaceText;
  SDL_Rect rectangle;
  rectangle.w = intrv * 47;
  rectangle.h = intrv * 2;
      
  const char *tex = "This game was written by 25-year old Karandeev"
                    " Ilia at summer 2023 during his stay in \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex, CreditsColor);
  textureText[0].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 5 - Y_offset;
  SDL_RenderCopy(renderer, textureText[0].txtr, NULL, &rectangle);
  SDL_FreeSurface(surfaceText);

  const char *tex2 = "Armenia. It was inspired by his school days of"
                     " playing the Dots game back in Russia. He\0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex2, CreditsColor);
  textureText[1].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 8 - Y_offset;
  SDL_RenderCopy(renderer, textureText[1].txtr, NULL, &rectangle);
  SDL_FreeSurface(surfaceText);

  const char *tex3 = "spent a really huge amount of time playing this"
                     " game with his friends during some of\0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex3, CreditsColor);
  textureText[2].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 11 - Y_offset;
  SDL_RenderCopy(renderer, textureText[2].txtr, NULL, &rectangle);
  SDL_FreeSurface(surfaceText);

  const char *tex4 = "the boring lessons. After some years including his"
                    " study in the CMC MSU faculty he \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex4, CreditsColor);
  textureText[3].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 14 - Y_offset;
  SDL_RenderCopy(renderer, textureText[3].txtr, NULL, &rectangle);
  SDL_FreeSurface(surfaceText);

  const char *tex5 = "finally had the skill to implement it as a computer"
                     " game. Special thanks to his former\0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex5, CreditsColor);
  textureText[4].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 17 - Y_offset;
  SDL_RenderCopy(renderer, textureText[4].txtr, NULL, &rectangle);
  SDL_FreeSurface(surfaceText);

  const char *tex6 = "fellow CMC group member Daniel Oblomov, who helped him"
                     " during his hard days of living \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex6, CreditsColor);
  textureText[5].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 20 - Y_offset;
  SDL_RenderCopy(renderer, textureText[5].txtr, NULL, &rectangle);
  SDL_FreeSurface(surfaceText);

  const char *tex7 = "in Armenia. That's it. Hope you enjoy/will enjoy/enjoyed"
                     " playing this wonderfull game. \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex7, CreditsColor);
  textureText[6].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 23 - Y_offset;
  SDL_RenderCopy(renderer, textureText[6].txtr, NULL, &rectangle);
  SDL_FreeSurface(surfaceText);

  const char *tex8 = "                                   "
                     "             Respectfully yours, Karandeev Ilia       \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex8, CreditsColor);
  textureText[7].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 26 - Y_offset;
  SDL_RenderCopy(renderer, textureText[7].txtr, NULL, &rectangle);
  SDL_FreeSurface(surfaceText);
}

void DrawCredits( Text_num* textureText)
{
  SDL_Color CreditsColor = {205, 80, 205, SDL_ALPHA_OPAQUE};
  SDL_Rect rectangle;
  rectangle.w = intrv * 47;
  rectangle.h = intrv * 2;
      
  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 5 - Y_offset;
  SDL_RenderCopy(renderer, textureText[0].txtr, NULL, &rectangle);

  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 8 - Y_offset;
  SDL_RenderCopy(renderer, textureText[1].txtr, NULL, &rectangle);

  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 11 - Y_offset;
  SDL_RenderCopy(renderer, textureText[2].txtr, NULL, &rectangle);

  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 14 - Y_offset;
  SDL_RenderCopy(renderer, textureText[3].txtr, NULL, &rectangle);

  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 17 - Y_offset;
  SDL_RenderCopy(renderer, textureText[4].txtr, NULL, &rectangle);

  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 20 - Y_offset;
  SDL_RenderCopy(renderer, textureText[5].txtr, NULL, &rectangle);

  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 23 - Y_offset;
  SDL_RenderCopy(renderer, textureText[6].txtr, NULL, &rectangle);

  rectangle.x = intrv * 3 - X_offset;
  rectangle.y = intrv * 26 - Y_offset;
  SDL_RenderCopy(renderer, textureText[7].txtr, NULL, &rectangle);
}

void CreditsMenu()
{
  X_offset = 0;
  Y_offset = 0;

  ButtonVector[10].keytrick = false;

  int previous_minintrv = minintrv;
  minintrv = max(ScWidth / double(credits_x_field + 2),
    ScHeight / double(credits_y_field + 2));
  minintrv = max(default_minintrv, minintrv);
  intrv = minintrv;
  dot_size = round(intrv * dots_to_intrv);
  mouse.circle_size = dot_size + 1;

  Text_num* Textures = new Text_num [credits_x_field * credits_y_field];
  EnumerateField(credits_y_field, credits_x_field, my_Font, Textures);

  Text_num* TexturesC = new Text_num [8];
  FillCredits(my_Font, TexturesC);
  
  while (GameState == Credits)
  {
    ButtonVector[11].Update(mouse);
    mouse.Update();

    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT))
      exit(0);

    if ((event.key.keysym.sym == SDLK_ESCAPE) && (event.type == SDL_KEYUP) )
      GameState = Menu;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[11].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = Menu;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[11].keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      GameState = Menu;

    SDL_GetMouseState(&xMouse, &yMouse);
    Zoom();

    if (GetDotInput(dots_credits, cur_color, credits_x_field, credits_y_field))  
      ChangeDotColor();
    GetDotErase(dots_credits, credits_x_field, credits_y_field);
    SDL_SetRenderDrawColor(renderer, red_default, green_default,
      blue_default, BACKGROUND_OPACITY);
    SDL_RenderClear(renderer);
    MoveBoard(credits_x_field, credits_y_field, move_game_speed);
    DrawField(credits_x_field, credits_y_field);
    DrawDots(dots_credits, credits_y_field * credits_x_field, credits_x_field);
    DrawFieldNumbers (credits_y_field, credits_x_field, my_Font, Textures);
    DrawCredits(TexturesC);
    CheckPositionInField(credits_x_field, credits_y_field);
    ProcessDotInteraction(dots_credits, credits_x_field, credits_y_field);

    ButtonVector[11].Draw();
    mouse.DrawDot(cur_color);
    
    SDL_RenderPresent(renderer);
  }
  
  minintrv = previous_minintrv;
  delete [] Textures;
  delete [] TexturesC;

}