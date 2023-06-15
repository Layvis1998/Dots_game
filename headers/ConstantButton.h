
#pragma once
#include <SDL2/SDL.h>
#include "Mouse.h"

class Button 
{
  public:
    SDL_Texture * tex;
    SDL_Rect srect, drect;
    bool IsSelected = false;
    bool mousetrick = false;

    Button (const char img_path[], int w, int h)
    {
      SDL_Texture * t = IMG_LoadTexture(renderer, img_path);
      tex = t;

      srect.h = srecth;
      srect.w = srectw;
      srect.x = 0;

      drect.h = h;
      drect.w = w;
    };

  void update (Mouse& mouse)
  {
    if ((SDL_HasIntersection(&drect, &mouse.point)) || (mousetrick == true) )
      IsSelected = true;
    else
      IsSelected = false;
    
    if (IsSelected == true)
      srect.x = srect.w;
    else
      srect.x = 0;
  } 

  void draw()
  {
    SDL_RenderCopy(renderer, tex, &srect, &drect);
  }
};








