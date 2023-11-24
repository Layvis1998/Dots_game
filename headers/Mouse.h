#pragma once
#include <iostream>
#include <SDL2/SDL_image.h>
#include "globals.h"
using namespace std;

const char* mouse_name;
const char* mouse_circle_name;
int MOUSE_OPACITY;

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
  }

  void GiveColor(SDL_Color color)
  {
  }

  void Update()
  {
  }

  void Draw()
  {
  }

  void DrawDot(SDL_Color scolor)
  {
  }

  void DrawColor()
  {      
  }

};
