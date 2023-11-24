#pragma once
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

#include "globals.h"

using std::unordered_set;

enum GameStateType {Menu, Credits, SAP, ONEPM, TWOPM,
                    THREEPM, FOURPM, GSM, GRM, Game};

struct Dots
{
  SDL_Color clr;
  bool exist;
  bool visited;
  bool deadend;
  uint16_t cycle;
  bool spaceborder;
  bool spaceinside;
};

struct ColorSpace
{
  unordered_set <int> Cycle;
  unordered_set <int> InnerDots;
  unordered_set <int> BorderDots;   
};

struct Text_num
{
  SDL_Texture* txtr;
};


const char mouse_name[] = "images/mouse.png";
const char mouse_circle_name[] = "images/transparent_mouse.png";
const int MOUSE_OPACITY = 170;

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