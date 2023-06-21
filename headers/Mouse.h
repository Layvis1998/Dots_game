#pragma once
#include <iostream>
#include <SDL2/SDL_image.h>
#include "globals.h"
using namespace std;

const char mouse_name[] = "images/mouse.png";
const char mouse_circle_name[] = "images/transparent_mouse.png";
const int MOUSE_OPACITY = 182;

class Mouse
{
public:	
  SDL_Texture * tex;
  SDL_Rect rect;
  SDL_Rect point;
  SDL_Color clr;
  uint8_t circle_size;

  Mouse()
  {
    SDL_ShowCursor(false);
    tex = IMG_LoadTexture(renderer, mouse_name);
    rect.w = 28;
    rect.h = 28;

    point.w = 1;
    point.h = 1;
    circle_size = 10;
  }

  void GiveColor(SDL_Color color)
  {
    clr.r = color.r;
    clr.g = color.g;
    clr.b = color.b;
    clr.a = MOUSE_OPACITY;
  }

  void Update()
  {
    SDL_GetMouseState(&rect.x, &rect.y);
    point.x = rect.x;
    point.y = rect.y;
  }

  void Draw()
  {
    tex = IMG_LoadTexture(renderer, mouse_name);
    SDL_ShowCursor(false);
    SDL_RenderCopy(renderer, tex, NULL, &rect);
  }

  void DrawDot(SDL_Color scolor)
  {
    SDL_SetRenderDrawColor(renderer, clr.r, clr.g, clr.b, MOUSE_OPACITY);
    for (int w = 0; w < circle_size * 2; w++)
    {
      for (int h = 0; h < circle_size * 2; h++)
      {
        int dx = circle_size - w; // horizontal offset
        int dy = circle_size - h; // vertical offset
        if ((dx * dx + dy * dy) <= (circle_size * circle_size))
          SDL_RenderDrawPoint(renderer, point.x + dx, point.y + dy);
      }
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, clr.a);
    SDL_RenderDrawPoint(renderer, point.x, point.y);
    SDL_RenderDrawPoint(renderer, point.x + 1, point.y);
    SDL_RenderDrawPoint(renderer, point.x, point.y + 1);
    SDL_RenderDrawPoint(renderer, point.x, point.y - 1);
    SDL_RenderDrawPoint(renderer, point.x - 1, point.y);
    SDL_RenderDrawPoint(renderer, point.x + 1, point.y + 1);
    SDL_RenderDrawPoint(renderer, point.x - 1, point.y + 1);
    SDL_RenderDrawPoint(renderer, point.x - 1, point.y - 1);
    SDL_RenderDrawPoint(renderer, point.x + 1, point.y - 1);
    SDL_RenderDrawPoint(renderer, point.x + 2, point.y);
    SDL_RenderDrawPoint(renderer, point.x, point.y + 2);
    SDL_RenderDrawPoint(renderer, point.x, point.y - 2);
    SDL_RenderDrawPoint(renderer, point.x - 2, point.y);
    SDL_RenderDrawPoint(renderer, point.x + 3, point.y);
    SDL_RenderDrawPoint(renderer, point.x, point.y + 3);
    SDL_RenderDrawPoint(renderer, point.x, point.y - 3);
    SDL_RenderDrawPoint(renderer, point.x - 3, point.y);

    clr = scolor;
    SDL_ShowCursor(false);
  }

  void DrawColor()
  {      
    const SDL_Rect aux_rect = rect; //This is only done for the RenderFillRect funtion to work

    SDL_SetRenderDrawColor(renderer, clr.r, clr.g, clr.b, MOUSE_OPACITY);
    SDL_RenderFillRect(renderer, &aux_rect);
    SDL_RenderDrawRect(renderer, &rect);
  }
};
