#pragma once
#include <SDL2/SDL.h>
#include "Mouse.h"
#include <stdint.h>

class ColorButton 
{
public:
  SDL_Color clr;
  SDL_Rect rect;
  bool IsSelected = false;

  ColorButton (uint8_t r, uint8_t g, uint8_t b)
  {
    rect.h = 150;
    rect.w = 150;
    clr.r = r;
    clr.g = g;
    clr.b = b;
    clr.a = 255;
  };

  void Update (Mouse& mouse)
  {
    if (SDL_HasIntersection(&rect, &mouse.point))
      IsSelected = true;
    else
      IsSelected = false;
  } 

  void ChangeColor()
  {
    switch(event.key.keysym.sym)
    {
      case SDLK_g:
        clr.g += 2;
        break;
      case SDLK_r:
        clr.r += 2;
        break;
      case SDLK_b:
        clr.b += 2;
        break;
      case SDLK_o:
      case SDLK_0:
      {
        clr.r = 0;
        clr.g = 0;
        clr.b = 0;
        clr.a = 255;
        break;
      }
      case SDLK_w:
      {
        clr.r = 255;
        clr.g = 255;
        clr.b = 255;
        clr.a = 255;
      }
    }
  }
  
  void Draw()
  {      
    const SDL_Rect aux_rect = rect; //This is only done for the RenderFillRect funtion to work
    SDL_SetRenderDrawColor(renderer, clr.r, clr.g, clr.b, clr.a);
    SDL_RenderFillRect(renderer, &aux_rect);
    SDL_RenderDrawRect(renderer, &rect);
  }
};
