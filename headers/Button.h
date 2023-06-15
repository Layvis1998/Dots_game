
#pragma once
#include <SDL2/SDL.h>
#include "Mouse.h"

class Button 
{
  public:
    SDL_Texture * tex;
    SDL_Rect srect, drect;
    bool IsSelected = false;
    bool keytrick = false;

    Button (const char img_path[], int w, int h, int srecth, int srectw)
    {
      SDL_Texture * t = IMG_LoadTexture(renderer, img_path);
      tex = t;

      srect.h = srecth;
      srect.w = srectw;
      srect.x = 0;

      drect.h = h;
      drect.w = w;
    };

  void Update (Mouse& mouse)
  {
    if ((SDL_HasIntersection(&drect, &mouse.point)) || (keytrick == true) )
      IsSelected = true;
    else
      IsSelected = false;
    
    if (IsSelected == true)
      srect.x = srect.w;
    else
      srect.x = 0;
  } 

  void Draw()
  {
    SDL_RenderCopy(renderer, tex, &srect, &drect);
  }
};









