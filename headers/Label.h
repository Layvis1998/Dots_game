#pragma once

class Label
{
public:
  SDL_Texture * tex;
  SDL_Rect srect, drect;
  bool IsSelected = false;

  Label (const char img_path[], int w, int h, int srecth, int srectw)
  {
    SDL_Texture * t = IMG_LoadTexture(renderer, img_path);
    tex = t;
    
    srect.h = srecth;
    srect.w = srectw;
    srect.x = 0;

    drect.h = h;
    drect.w = w;
  };
  
  void Draw()
  {
    SDL_RenderCopy(renderer, tex, &srect, &drect);
  }
};








