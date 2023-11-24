#pragma once
#include <SDL2/SDL.h>
#include "Mouse.h"
#include <stdint.h>

class ColorButton 
{
public:
  SDL_Color clr;
  SDL_Rect rect;
  bool IsSelected;

  ColorButton (uint8_t r, uint8_t g, uint8_t b)
  {
  };

  void Update (Mouse& mouse)
  {
  } 

  void ChangeColor()
  {
  }
  
  void Draw()
  {      
  }
};
