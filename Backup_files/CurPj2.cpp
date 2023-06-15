#include <iostream>
#include <err.h>
#include <string>
#include <stack>
#include <SDL2/SDL.h>

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "headers/Button.h"
#include "headers/Label.h"
#include "headers/ColorButton.h"
#if __linux__
  #include <X11/Xlib.h>
#endif

/*
if _WIN32
  #include "headers/wtypes.h"
#endif     
*/

//global constants
int intrv = 20; // interval
const uint8_t red_default = 25;
const uint8_t green_default = 80;
const uint8_t blue_default = 80;
const int field_x_base = 100;
const int field_y_base = 200;

//gloabal variables
uint8_t red = 25;
uint8_t green = 80;
uint8_t blue = 80;
int x_c = 0;
int y_c = 0;
int ScWidth;
int ScHeight;
SDL_Color cur_color;
int xMouse, yMouse;
int field_x_size = field_x_base;
int field_y_size = field_y_base;

class Dots
{
public:
  bool exist;
  SDL_Color clr;
  uint8_t visited;
  bool employed;
};

bool contains (int val, stack <int> st)
{
  int i = 0;
  if (st.size() == 1)
    return false;

  while (i < st.size())
  {
    if (st.top() + i == val)
      return true;
    i++;
  }
  return false;
}

bool dfs (Dots* dots, int current, stack<int>& st, int fx , int fy)
{
  st.push(current);
  int Can_move_on = 0;
  dots[current].visited = true;
  bool Cycle_found = false;

  // right direction
  if (((current + 1) % fx != 0) && (dots[current + 1].exist == true))
  {                  
    if (dots[current + 1].visited == false) 
    {
      dots[current + 1].visited = true;
      dfs(dots, current + 1, st, fx, fy);
    }
    else if ((st.size() >= 3) && (dots[current + 1].visited == true) )
    {
      st.pop();
      if (st.top() != current + 1)
        Cycle_found = true;

      st.push(current);
    }  
  }

  //down-right direction  
  if ( (( current + 1) % fx != 0) && ( current + fx < fx * fy )
     && (dots[current + 1 + fx].exist == true))   
  {
    if (dots[current + 1 + fx].visited == false)
    {
      dots[current + 1 + fx].visited = true;
      dfs(dots, current + 1 + fx, st, fx, fy);
    }
    else if ((st.size() >= 3) && (dots[current + 1 + fx].visited == true) )
    {
      st.pop();
      if (st.top() != current + 1 + fx)
        Cycle_found = true;
      st.push(current);
    }      
  }

  // down direction
  if (( current + fx < fx * fy ) && (dots[current + fx].exist == true))
  { 
    if (dots[current + fx].visited == false)
    {
      dots[current + fx].visited = true;
      dfs(dots, current + fx, st, fx, fy);
    }
    else if ((st.size() >= 3) && (dots[current + fx].visited == true) )
    {
      st.pop();
      if (st.top() != current + fx)
        Cycle_found = true;
      st.push(current);
    }  
  }

  //down-left direction
  if (( current % fx != 0) && ( current + fx < fx * fy )
     && (dots[current - 1 + fx].exist == true))
  { 
    if (dots[current - 1 + fx].visited == false)
    {
      dots[current - 1 + fx].visited = true;
      dfs(dots, current - 1 + fx, st, fx, fy);
    }
    else if ((st.size() >= 3) && (dots[current - 1 + fx].visited == true) )
    {
      st.pop();
      if (st.top() != current - 1 + fx)
        Cycle_found = true;
      st.push(current);
    }  
  }

  // left direction
  if ( ( current % fx != 0) && (dots[current - 1].exist == true))
  {  
    if (dots[current - 1].visited == false) 
    {
      dots[current - 1].visited = true;
      dfs(dots, current - 1, st, fx, fy);
    }
    else if ((st.size() >= 3) && (dots[current - 1].visited == true) )
    {
      st.pop();
      if (st.top() != current - 1)
        Cycle_found = true;
      st.push(current);
    }  
  }

  //up-left direction 
  if ( ( current % fx != 0) && ( current - fx >= 0)
     && (dots[current - 1 - fx].exist == true))
  {
    if (dots[current - 1 - fx].visited == false)
    {
      dots[current - 1 - fx].visited = true;
      dfs(dots, current - 1 - fx, st, fx, fy);
    }
    else if ((st.size() >= 3) && (dots[current - 1 - fx].visited == true) )
    {
      st.pop();
      if (st.top() != current - 1 - fx)
        Cycle_found = true;
      st.push(current);
    }  
  }

  // up diterction
  if (( current - fx >= 0) && (dots[current - fx].exist == true))                                        
  { 
    if (dots[current - fx].visited == false)
    {
      dots[current - fx].visited = true;
      dfs(dots, current - fx, st, fx, fy);
    }
    else if ((st.size() >= 3) && (dots[current - fx].visited == true) )
    {
      st.pop();
      if (st.top() != current - fx)
        Cycle_found = true;
      st.push(current);
    }  
  }  

  //up-right direction
  if ( (( current + 1) % fx != 0) && ( current - fx >= 0 )
     && (dots[current + 1 - fx].exist == true))
  {
    if (dots[current + 1 - fx].visited == false)
    {
      dots[current + 1 - fx].visited = true;
      dfs(dots, current + 1 - fx, st, fx, fy);
    }
    else if ((st.size() >= 3) && (dots[current + 1 - fx].visited == true) )
    {
      st.pop();
      if (st.top() != current + 1 - fx)
        Cycle_found = true;
      st.push(current);
    }  
  }



  /*
  if ((st.size() >= 2) && (!Can_move_on))
  {
    // down-left direction
    if (( current % fx != 0) && ( current + fx < fx * fy )
      && (dots[current + fx].exist == true)
      && (dots[current + fx].visited == true)
      && (st.top() != current - 1 + fx ) )                                      
      Can_move_on++;
    // left direction
    if ( ( current % fx != 0) && (dots[current - 1].exist == true)
      && (dots[current - 1].visited == true)
      && (st.top() != current - 1) )
      Can_move_on++;
    // up-left direction 
    if ( ( current % fx != 0) && ( current - fx >= 0)
      && (dots[current - 1 - fx].exist == true)
      && (dots[current - 1 - fx].visited == true)
      && (st.top() != current - 1 - fx) )                                      
      Can_move_on++;
    // up diterction
    if (( current - fx >= 0) && (dots[current - fx].exist == true)
      && (dots[current - fx].visited == true)
      && (st.top() != current - fx ) )                                      
      Can_move_on++;
    // up-right direction
    if ( (( current + 1) % fx != 0) && ( current - fx >= 0 )
      && (dots[current + 1 - fx].exist == true)
      && (dots[current + 1 - fx].visited == true)
      && (st.top() != current + 1 - fx ) )
      Can_move_on++;
    // right direction
    if (((current + 1) % fx != 0) && (dots[current + 1].exist == true)
      && (dots[current + 1].visited == true)
      && (st.top() != current + 1 ) )
      Can_move_on++;
    // down-right direction  
    if ( (( current + 1) % fx != 0) && ( current + fx < fx * fy )
      && (dots[current + 1 + fx].exist == true)
      && (dots[current + 1 + fx].visited == true)
      && (st.top() != current + 1 + fx ) )    
      Can_move_on++;
    // down direction
    if (( current + fx < fx * fy ) && (dots[current + fx].exist == true)
      && (dots[current + fx].visited == true)
      && (st.top() != current + fx ) )                                       
      Can_move_on++;

  }

    int temp = st.top();
    while ( (Can_move_on == 1) && (st.size() >= 2) )
    {
      st.pop();
      tem = st.top();
    }



  




  if (Cycle_found)
    cout << "\n Cycle_found \n";
  */
  return Cycle_found;
}

void FindContour (Dots* dots, int fx, int fy)
{
  int nnvisited = 0;
  while ((nnvisited < fx * fy) && (dots[nnvisited].exist == false))
    nnvisited++;
  
  if (nnvisited == fx * fy)
    return;

  stack <int> st;

  int current = nnvisited;
  cout << std::boolalpha << "Cycle found is " << dfs(dots, current, st, fx, fy)
    << endl;
  cout << " Stack size = " << st.size() << endl;
}

void MoveBoard(int fx, int fy)
{
  int CurScWidth;
  int CurScHeight;
  SDL_GetWindowSize(window, &CurScWidth, &CurScHeight);

  if (xMouse <= intrv)
    x_c -= 14;
  if (xMouse >= CurScWidth - intrv)
    x_c += 14;
  if (yMouse <=  intrv)
    y_c -= 14;
  if (yMouse >= CurScHeight - intrv)
    y_c += 14;

  if (event.key.keysym.sym == SDLK_s)
    y_c += 14;
  if (event.key.keysym.sym == SDLK_w)
    y_c -= 14;
  if (event.key.keysym.sym == SDLK_d)
    x_c += 14;
  if (event.key.keysym.sym == SDLK_a)
    x_c -= 14;
  if (event.key.keysym.sym == SDLK_DOWN)
    y_c += 14;
  if (event.key.keysym.sym == SDLK_UP)
    y_c -= 14;
  if (event.key.keysym.sym == SDLK_LEFT)
    x_c -= 14;
  if (event.key.keysym.sym == SDLK_RIGHT)
    x_c += 14;

  if (x_c > intrv * fx - CurScWidth + intrv * 3)
    x_c = intrv * fx - CurScWidth + intrv * 3;
  if (y_c > intrv * fy - CurScHeight + intrv * 3) 
    y_c = intrv * fy - CurScHeight + intrv * 3;

  if (x_c <= 0)
    x_c = 0;
  if (y_c <= 0)
    y_c = 0;
}

void DrawCircle(SDL_Renderer *renderer, int x, int y, int radius, 
  SDL_Color clr)
{
  SDL_SetRenderDrawColor(renderer, clr.r, clr.g, clr.b, clr.a);
  for (int w = 0; w < radius * 2; w++)
  {
    for (int h = 0; h < radius * 2; h++)
    {
      int dx = radius - w; // horizontal offset
      int dy = radius - h; // vertical offset
      if ((dx * dx + dy * dy) <= (radius * radius))
        SDL_RenderDrawPoint(renderer, x + dx, y + dy);
    }
  }
}

void DrawField(int dot_rows, int dot_clmns)
{
  for (int i = 2; i <= dot_rows + 1; i++)
  {  
    SDL_RenderDrawLine(renderer, intrv * i - 0 - x_c, 2 * intrv - y_c,
      intrv * i - 0 - x_c, intrv * (dot_clmns + 1) - y_c);
    SDL_RenderDrawLine(renderer, intrv * i - 1 - x_c, 2 * intrv - y_c,
      intrv * i - 1 - x_c, intrv * (dot_clmns + 1) - y_c);
    SDL_RenderDrawLine(renderer, intrv * i - 2 - x_c, 2 * intrv - y_c,
      intrv * i - 2 - x_c, intrv * (dot_clmns + 1) - y_c);
  }

  for (int i = 2; i <= dot_clmns + 1; i++)
  {  
    SDL_RenderDrawLine(renderer, 2 * intrv - x_c, intrv * i - 0 - y_c,
      intrv * (dot_rows + 1) - x_c, intrv * i - 0 - y_c);
    SDL_RenderDrawLine(renderer, 2 * intrv - x_c, intrv * i - 1 - y_c,
      intrv * (dot_rows + 1) - x_c, intrv * i - 1 - y_c);
    SDL_RenderDrawLine(renderer, 2 * intrv - x_c, intrv * i - 2 - y_c,
      intrv * (dot_rows + 1) - x_c, intrv * i - 2 - y_c);
  }
}

void FillCredits(TTF_Font *my_Font)
{
  SDL_Surface* surfaceText;
  SDL_Texture* textureText;
  SDL_Rect rectangle4;
  rectangle4.w = intrv * 47;
  rectangle4.h = intrv * 2;
      
  const char *tex = "This game was written by 25-year old Karandeev Ilia at summer 2023 during his stay in \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle4.x = intrv * 3 - x_c;
  rectangle4.y = intrv * 5 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex2 = "Armenia. It was inspired by his school days of"
                     " playing the Dots game back in Russia. He\0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex2, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle4.x = intrv * 3 - x_c;
  rectangle4.y = intrv * 8 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex3 = "spent a really huge amount of time playing this"
                     " game with his friends during some of\0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex3, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle4.x = intrv * 3 - x_c;
  rectangle4.y = intrv * 11 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex4 = "the boring lessons. After some years including his"
                    " study in the CMC MSU faculty he \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex4, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle4.x = intrv * 3 - x_c;
  rectangle4.y = intrv * 14 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex5 = "finally had the skill to implement it as a computer"
                     " games. Special thanks to his former\0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex5, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle4.x = intrv * 3 - x_c;
  rectangle4.y = intrv * 17 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex6 = "fellow CMC group member Daniel Oblomov, who helped him"
                     " during his hard days of living \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex6, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle4.x = intrv * 3 - x_c;
  rectangle4.y = intrv * 20 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex7 = "in Armenia. That's it. Hope you enjoy/will enjoy/enjoyed"
                     " playing this wonderfull game. \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex7, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle4.x = intrv * 3 - x_c;
  rectangle4.y = intrv * 23 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex8 = "                                   "
                     "             Respectfully yours, Karandeev Ilia       \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex8, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle4.x = intrv * 3 - x_c;
  rectangle4.y = intrv * 26 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);
}

void EnumerateField(int dot_clmns, int dot_rows, TTF_Font *my_Font)
{
  SDL_Surface* surfaceText;
  SDL_Texture* textureText;
  SDL_Rect rectangle4;

  int number = 0;
  string num_str;
  char* num;
  rectangle4.w = intrv * 1.5;
  rectangle4.h = intrv * 1.5;
      
  for (int number = 0; number < dot_rows; number += 5)
  {
    num_str = to_string(number);
    num = (char*) num_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    rectangle4.x = intrv * (number + 1.3) - x_c;
    rectangle4.y = - y_c + 0.5;
    SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
    SDL_DestroyTexture(textureText);
    SDL_FreeSurface(surfaceText);
  }
  
  for (int number = 0; number < dot_clmns; number += 5)
  {
    num_str = to_string(number);
    num = (char*) num_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    rectangle4.x = - x_c + 0.5;
    rectangle4.y = intrv * ( number + 1.3) - y_c;
    SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
    SDL_DestroyTexture(textureText);
    SDL_FreeSurface(surfaceText);
  }

  for (int number = 0; number < dot_rows; number += 5)
  {
    num_str = to_string(number);
    num = (char*) num_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    rectangle4.x = intrv * ( number + 1.3) - x_c;
    rectangle4.y = intrv * (dot_clmns + 1.5) - y_c;
    SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
    SDL_DestroyTexture(textureText);
    SDL_FreeSurface(surfaceText);
  }
  
  for (int number = 0; number < dot_clmns; number += 5)
  {
    num_str = to_string(number);
    num = (char*) num_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    rectangle4.x = intrv * (dot_rows + 1.5) - x_c;
    rectangle4.y = intrv * ( number + 1.3) - y_c;
    SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
    SDL_DestroyTexture(textureText);
    SDL_FreeSurface(surfaceText);
  }

  number = dot_rows - 1;
  num_str = to_string(number);
  num = (char*) num_str.c_str();
  surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle4.x = intrv * (number + 1.3) - x_c;
  rectangle4.y = - y_c + 0.5;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);
  surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle4.x = intrv * ( number + 1.3) - x_c;
  rectangle4.y = intrv * (dot_clmns + 1.5) - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  number = dot_clmns - 1;
  num_str = to_string(number);
  num = (char*) num_str.c_str();
  surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle4.x = - x_c + 0.5;
  rectangle4.y = intrv * ( number + 1.3) - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);
  surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle4.x = intrv * (dot_rows + 1.5) - x_c;
  rectangle4.y = intrv * ( number + 1.3) - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle4);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);  
}

void FieldColor()
{
  if (event.type == SDL_KEYDOWN)
  {  
    switch(event.key.keysym.sym)
    {
      case SDLK_g:
        green += 1;
        break;
      case SDLK_r:
        red += 1;
        break;
      case SDLK_b:
        blue += 1;
        break;
      case SDLK_o:
      case SDLK_0:
      {
        red = red_default;
        green = green_default;
        blue = blue_default;
        break;
      }
      case SDLK_w:
      {
        red = 255;
        green = 255;
        blue = 255;
      }
    }
  }
}

void DrawDots(Dots* dots, int size, int fx)
{
  for (int i = 0; i < size; i++)
  { 
    if (dots[i].exist == true) 
      DrawCircle(renderer, (i % fx + 2) * intrv - x_c,
        (i / fx + 2) * intrv - y_c, 7, dots[i].clr);  
  }
}

void ScreenShot()
{
  int w;
  int h;

  SDL_GetWindowSize(window, &w, &h);
  SDL_Surface *sshot = SDL_CreateRGBSurface
    (0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
  SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888,
    sshot->pixels, sshot->pitch);
  SDL_SaveBMP(sshot, "screenshot.bmp");
  SDL_FreeSurface(sshot);
}

void Zoom()
{
  if(event.type == SDL_MOUSEWHEEL)
  {
    if(event.wheel.y > 0) // Zoom up
      intrv++;
    else if(event.wheel.y < 0) // Zoom down
      intrv--;

    if (intrv > 75)
      intrv = 75;

    if (intrv < 15)
      intrv = 15;
  }
}

int GetDotInput(Dots *dots, SDL_Color clr, int fxs, int fys)
{
  if ( (event.button.button == SDL_BUTTON_LEFT) &&
     (event.type == SDL_MOUSEBUTTONUP) &&
     ( (( (yMouse) % intrv) <= intrv / 3.0) || (((yMouse) % intrv) >= intrv * 2 / 3)) 
     && 
     ( (( (xMouse) % intrv) <= intrv / 3.0) || (((xMouse) % intrv) >= intrv * 2 / 3))
     
     && (xMouse >= 5.0 / 3 * intrv) && (yMouse >= 5.0 / 3 * intrv)
     && (xMouse + x_c <= (fxs + 5.0 / 3) * intrv)
     && (yMouse + y_c <= (fys + 5.0 / 3) * intrv)
     )
    {  
      int coeff = round((xMouse + x_c) / float(intrv)) - 2 +
        + (round((yMouse + y_c) / float(intrv)) - 2) * fxs;
      
      if (dots[coeff].exist == false)
      {  
        dots[coeff].exist = true;
        dots[coeff].clr = clr;
        return 1;
      }      
    }
    return 0;
}

void GetDotErase(Dots *dots, int fxs, int fys)
{
  if ( (event.button.button == SDL_BUTTON_RIGHT) &&
     (event.type == SDL_MOUSEBUTTONUP) &&
     ( (( (yMouse) % intrv) <= intrv / 3.0) || (((yMouse) % intrv) >= intrv * 2 / 3)) 
     && 
     ( (( (xMouse) % intrv) <= intrv / 3.0) || (((xMouse) % intrv) >= intrv * 2 / 3))
     
     && (xMouse >= 5.0 / 3 * intrv) && (yMouse >= 5.0 / 3 * intrv)
     && (xMouse + x_c <= (fxs + 5.0 / 3) * intrv)
     && (yMouse + y_c <= (fys + 5.0 / 3) * intrv)
     )
    {  
      int coeff = round((xMouse + x_c) / float(intrv)) - 2 +
        + (round((yMouse + y_c) / float(intrv)) - 2) * fxs;
      dots[coeff].exist = false;      
    }
}

bool operator==(SDL_Color a, SDL_Color b)
{
  if ( (a.r == b.r) && (a.g == b.g) && (a.b == b.b) && (a.a == b.a) ) 
    return true;
  else
    return false;
}

int main(int argc, char *argv[])
{
  using std::cout;
  using std::cin;
  using std::endl;
  using std::max;
  using std::to_string;
  using std::string;
  using std::pair;

///////////////////////////////Finding out display parameters///////////////////
  #if __linux__
  char *wid; Display *dpy; Window w;
  if(!(dpy = XOpenDisplay(0)))
    errx(1, "cannot open display '%s'", XDisplayName(0));
  int snum = DefaultScreen(dpy);
  ScWidth = DisplayWidth(dpy, snum);
  ScHeight = DisplayHeight(dpy, snum);
  cout << "Device screen width: " << ScWidth << ", Device screen height: "
       << ScHeight << endl;
  #endif

  #if _WIN32
    GetDesktopResolution(ScWidth, ScHeight);
    cout << ScWidth << '\n' << ScHeight << '\n';
  #endif     
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////Some variables/////////////////////////////
  SDL_Color white = {255, 255, 255, 255};
  cur_color = white;
  Mouse mouse;
  mouse.GiveColor(white);
  mouse.circle_size = 6;

  const int vo = ScHeight / 8;  //vertical offset
  const int ho = ScWidth / 5;  //horizontal offset
  uint8_t pa_back = 1;
  bool dots_conqured_victory = false;
  
  const int dots_menu_size = int(field_x_base + 2) * int(field_y_base + 2);
  Dots dots[dots_menu_size];

  for (int i = 0; i < dots_menu_size; i++)
  {
    dots[i].exist = false;
    dots[i].clr = white;
  }

  int game_x_size = 30;
  int game_y_size = 30;

  int field_x_credits = 50;
  int field_y_credits = 100;
  int dots_credits_size = (field_y_credits + 2) * (field_x_credits + 2);
  Dots *dots_c = new Dots[dots_credits_size];
  for (int i = 0; i < dots_credits_size; i++)
    dots_c[i].exist = false;
////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////Initializing SDL///////////////////////////
  window = SDL_CreateWindow("Dots", 0, 0, ScWidth, ScHeight,
    SDL_WINDOW_FULLSCREEN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED); 

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
      SDL_GetError());
    return 3;
  }
//////////////////////////////////////////////////////////////////////////////// 


//////////////////////////////SDL_TTF for GSM///////////////////////////////////
  if (TTF_Init() == -1)
    cout << "Could not initialize SDL2_ttf, error:" << TTF_GetError() << endl;
  else
    cout << "SDL2_ttf system ready to go!";

  TTF_Font* my_Font = TTF_OpenFont("DroidSans.ttf", 32);
  if (my_Font == nullptr)
  {
    cout << "could not load font" << endl;
    exit(1);
  }

  string temp_str = to_string(field_x_size);
  char* field_x = (char*) temp_str.c_str();
  int rectangle_w = ScWidth / 32;
  int rectangle_h = ScWidth / 18;
  SDL_Surface* surfaceText = TTF_RenderText_Solid(my_Font, field_x,
    {255, 255, 0});
  SDL_Texture* textureText = 
    SDL_CreateTextureFromSurface(renderer, surfaceText);
  SDL_Rect rectangle;
  rectangle.w = rectangle_w * 2;
  rectangle.h = rectangle_h * 2;
  rectangle.x = ScWidth * 1 / 3 - rectangle.w / 2;
  rectangle.y = vo * 3;

  temp_str = to_string(field_y_size);
  char* field_y = (char*) temp_str.c_str();
  SDL_Surface* surfaceText2 = TTF_RenderText_Solid(my_Font, field_y,
    {255, 255, 0});
  SDL_Texture* textureText2 = SDL_CreateTextureFromSurface
    (renderer, surfaceText2);
  SDL_Rect rectangle2;
  rectangle2.w = rectangle_w * 2;
  rectangle2.h = rectangle_h * 2;
  rectangle2.x = ScWidth * 2 / 3 - rectangle2.w / 2;
  rectangle2.y = vo * 3;

  SDL_Surface* surfaceText3 = TTF_RenderText_Solid(my_Font, "x",
    {255, 255, 0});
  SDL_Texture* textureText3 =
    SDL_CreateTextureFromSurface(renderer, surfaceText3);
  SDL_FreeSurface(surfaceText3);
  SDL_Rect rectangle3;

  rectangle3.w = rectangle_w * 2;
  rectangle3.h = rectangle_h * 2;
  rectangle3.x = ScWidth * 1 / 2 - rectangle3.w / 2;
  rectangle3.y = vo * 3;
//////////////////////////////////////////////////////////////////////////////// 


/////////////////All buttons and labels are here////////////////////////////////  
  const char* New = "images/new.png";
  Button NewButton(New, ho, vo, 100, 400);
  NewButton.srect.y = 0;
  NewButton.drect.x = ScWidth / 2 - NewButton.drect.w / 2; 
  NewButton.drect.y = vo;

  const char* load = "images/load.png";
  Button loadButton(load, ho, vo, 100, 400);
  loadButton.srect.y = 0;
  loadButton.drect.x = ScWidth / 2 - loadButton.drect.w / 2; 
  loadButton.drect.y = vo * 2;

  const char* helpB = "images/help.png";
  Button helpButton(helpB, ho, vo, 100, 400);
  helpButton.srect.y = 0;
  helpButton.drect.x = ScWidth / 2 - helpButton.drect.w / 2; 
  helpButton.drect.y = vo * 3;

  const char* credits = "images/credits.png";
  Button creditsButton(credits, ho, vo, 100, 400);
  creditsButton.srect.y = 0;
  creditsButton.drect.x = ScWidth / 2 - creditsButton.drect.w / 2; 
  creditsButton.drect.y = vo * 4;

  const char* exitB = "images/exit.png";
  Button exitButton(exitB, ho, vo, 100, 400);
  exitButton.srect.y = 0;
  exitButton.drect.x = ScWidth / 2 - exitButton.drect.w / 2; 
  exitButton.drect.y = vo * 5;

  const char* paB = "images/pa.png";
  Label paLabel(paB, ho * 1.5, vo, 100, 600);
  paLabel.srect.y = 0;
  paLabel.drect.x = ScWidth / 2 - paLabel.drect.w / 2; 
  paLabel.drect.y = vo;

  const char* oneB = "images/one.png";
  Button oneButton(oneB, ho, vo, 100, 200);
  oneButton.srect.y = 0;
  oneButton.drect.x = ScWidth / 2 - oneButton.drect.w / 2; 
  oneButton.drect.y = vo * 2;

  const char* twoB = "images/two.png";
  Button twoButton(twoB, ho, vo, 100, 400);
  twoButton.srect.y = 0;
  twoButton.drect.x = ScWidth / 2 - twoButton.drect.w / 2; 
  twoButton.drect.y = vo * 3;

  const char* threeB = "images/three.png";
  Button threeButton(threeB, ho, vo, 100, 400);
  threeButton.srect.y = 0;
  threeButton.drect.x = ScWidth / 2 - threeButton.drect.w / 2; 
  threeButton.drect.y = vo * 4;

  const char* fourB = "images/four.png";
  Button fourButton(fourB, ho, vo, 100, 400);
  fourButton.srect.y = 0;
  fourButton.drect.x = ScWidth / 2 - fourButton.drect.w / 2; 
  fourButton.drect.y = vo * 5;

  const char* backB = "images/back.png";
  Button backButton(backB, ho, vo, 100, 400);
  backButton.srect.y = 0;
  backButton.drect.x = ScWidth / 2 - backButton.drect.w / 2; 
  backButton.drect.y = vo * 6;

  const char* gvaiL = "images/GVAI.png";
  Label gvaiLabel(gvaiL, ho * 1.5, vo, 100, 600);
  gvaiLabel.srect.y = 0;
  gvaiLabel.drect.x = ScWidth / 2 - gvaiLabel.drect.w / 2; 
  gvaiLabel.drect.y = vo;

  const char* udL = "images/ud.png";
  Label udLabel(udL, ho * 1.5, vo, 100, 600);
  udLabel.srect.y = 0;
  udLabel.drect.x = ScWidth / 2 - udLabel.drect.w / 2; 
  udLabel.drect.y = vo * 2;

  Button back2Button(backB, ho, vo, 100, 400);
  back2Button.srect.y = 0;
  back2Button.drect.x = ScWidth / 2 - back2Button.drect.w / 2; 
  back2Button.drect.y = vo * 6;

  Button back3Button(backB, ho, vo, 100, 400);
  back3Button.srect.y = 0;
  back3Button.drect.x = ScWidth / 2 - back3Button.drect.w / 2; 
  back3Button.drect.y = vo * 6;

  const char* continueB = "images/continue.png";
  Button continueButton(continueB, ho, vo, 100, 400);
  continueButton.srect.y = 0;
  continueButton.drect.x = ScWidth / 2 - continueButton.drect.w / 2; 
  continueButton.drect.y = vo * 5;

  const char* sgL = "images/sg.png";
  Label sgLabel(sgL, ho * 1.5, vo, 100, 600);
  sgLabel.srect.y = 0;
  sgLabel.drect.x = ScWidth / 2 - sgLabel.drect.w / 2; 
  sgLabel.drect.y = vo;

  const char* colorsL = "images/colors.png";
  Label colorsLabel(colorsL, ho * 1.5, vo, 100, 600);
  colorsLabel.srect.y = 0;
  colorsLabel.drect.x = ScWidth / 2 - colorsLabel.drect.w / 2; 
  colorsLabel.drect.y = vo * 2;

  const char* rgbL = "images/rgb.png";
  Label rgbLabel(rgbL, ho * 1.5, vo, 100, 600);
  rgbLabel.srect.y = 0;
  rgbLabel.drect.x = ScWidth / 2 - rgbLabel.drect.w / 2; 
  rgbLabel.drect.y = vo * 3;

  ColorButton P21(25, 170, 100);
  P21.rect.x = ScWidth / 3 - P21.rect.w / 2; 
  P21.rect.y = vo * 4;

  ColorButton P22(100, 0, 150);
  P22.rect.x = ScWidth * 2 / 3 - P22.rect.w / 2; 
  P22.rect.y = vo * 4;

  ColorButton P31(255, 0, 0);
  P31.rect.x = ScWidth / 4 - P21.rect.w / 2; 
  P31.rect.y = vo * 4;

  ColorButton P32(0, 255, 0);
  P32.rect.x = ScWidth * 2 / 4 - P22.rect.w / 2; 
  P32.rect.y = vo * 4;

  ColorButton P33(0, 0, 255);
  P33.rect.x = ScWidth * 3 / 4 - P22.rect.w / 2; 
  P33.rect.y = vo * 4;

  ColorButton P41(0, 255, 10);
  P41.rect.x = ScWidth * 1 / 5 - P22.rect.w / 2; 
  P41.rect.y = vo * 4;

  ColorButton P42(255, 10, 0);
  P42.rect.x = ScWidth * 2 / 5 - P21.rect.w / 2; 
  P42.rect.y = vo * 4;

  ColorButton P43(0, 10, 255);
  P43.rect.x = ScWidth * 3 / 5 - P22.rect.w / 2; 
  P43.rect.y = vo * 4;

  ColorButton P44(240, 240, 0);
  P44.rect.x = ScWidth * 4 / 5 - P22.rect.w / 2; 
  P44.rect.y = vo * 4;

  const char* sgsL = "images/sgs.png";
  Label sgsLabel(sgsL, ho * 1.5, vo, 100, 600);
  sgsLabel.srect.y = 0;
  sgsLabel.drect.x = ScWidth / 2 - sgsLabel.drect.w / 2; 
  sgsLabel.drect.y = vo; 

  const char* useL = "images/use.png";
  Label useLabel(useL, ho * 1.5, vo, 100, 600);
  useLabel.srect.y = 0;
  useLabel.drect.x = ScWidth / 2 - useLabel.drect.w / 2; 
  useLabel.drect.y = vo * 2;

  const char* sgrL = "images/sgr.png";
  Label sgrLabel(sgrL, ho * 1.5, vo, 100, 600);
  sgrLabel.srect.y = 0;
  sgrLabel.drect.x = ScWidth / 2 - sgrLabel.drect.w / 2; 
  sgrLabel.drect.y = vo;

  const char* dcw = "images/dcw.png";
  Button dcwButton(dcw, ho * 4.5, vo, 100, 900);
  dcwButton.srect.y = 0;
  dcwButton.drect.x = ScWidth / 2 - dcwButton.drect.w / 2; 
  dcwButton.drect.y = vo * 3;

  const char* tvB = "images/tv.png";
  Button tvButton(tvB, ho * 3.5, vo, 100, 700);
  tvButton.srect.y = 0;
  tvButton.drect.x = ScWidth / 2 - tvButton.drect.w / 2; 
  tvButton.drect.y = vo * 4;   
////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////Menu cycle///////////////////////////////////
  Menu:
  NewButton.keytrick = false;
  exitButton.keytrick = false;
  creditsButton.keytrick = false;
  helpButton.keytrick = false;
  loadButton.keytrick = false;

  for (int i = 0; i < dots_menu_size; i++)
    dots[i].exist = false;
  
  while (1)
  { 
    mouse.Update();
    NewButton.Update(mouse);
    loadButton.Update(mouse);
    helpButton.Update(mouse);    
    exitButton.Update(mouse);
    creditsButton.Update(mouse);

    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT))
      exit(0);

    if ( (event.button.button == SDL_BUTTON_LEFT) && (exitButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      exit(0);

    if ( (event.key.keysym.sym == SDLK_e) && (event.type == SDL_KEYUP) )
      exit(0);

    if( (event.key.keysym.sym == SDLK_e) && (event.type == SDL_KEYDOWN) )
      exitButton.keytrick = true;

    if( (event.key.keysym.sym == SDLK_n) && (event.type == SDL_KEYDOWN) )
      NewButton.keytrick = true;

    if ( (event.key.keysym.sym == SDLK_n) && (event.type == SDL_KEYUP) )
      goto SAP;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (NewButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      goto SAP;

    if( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN) )
      creditsButton.keytrick = true;

    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP) )
      goto Credits;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (creditsButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      goto Credits;

    SDL_GetMouseState(&xMouse,&yMouse);
    GetDotInput(dots, cur_color, field_x_base, field_y_base);
    GetDotErase(dots, field_x_base, field_y_base);
    SDL_SetRenderDrawColor(renderer, red_default, green_default,
      blue_default, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    MoveBoard(field_x_base, field_y_base);

    DrawField(field_x_base, field_y_base);
    DrawDots(dots, dots_menu_size, field_x_size);
    EnumerateField(field_y_base, field_x_base, my_Font);
    Zoom();
    for (int i = 0; i < dots_menu_size; i++)
      dots[i].visited = 0;
    FindContour(dots, field_x_base, field_y_base);
    
    NewButton.Draw();
    loadButton.Draw();
    helpButton.Draw();
    exitButton.Draw();
    creditsButton.Draw();
    mouse.DrawCircle(cur_color);

    SDL_RenderPresent(renderer);
  }
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////Credits///////////////////////////////////////////
  Credits: 
  backButton.keytrick = false;
  x_c = 0;
  y_c = 0;
  
  while (1)
  {
    backButton.Update(mouse);
    mouse.Update();

    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT))
      exit(0);

    if ((event.key.keysym.sym == SDLK_ESCAPE) && (event.type == SDL_KEYUP) )
      goto Menu;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (backButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      goto Menu;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      backButton.keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      goto Menu;

    SDL_GetMouseState(&xMouse,&yMouse);
    GetDotInput(dots_c, cur_color, field_x_credits, field_y_credits);
    GetDotErase(dots_c, field_x_credits, field_y_credits);
    SDL_SetRenderDrawColor(renderer, red_default, green_default,
      blue_default, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    MoveBoard(field_x_credits, field_y_credits);

    DrawField(field_x_credits, field_y_credits);
    DrawDots(dots_c, dots_menu_size, field_x_credits);
    EnumerateField(field_y_credits, field_x_credits, my_Font);
    Zoom();
    mouse.DrawCircle(cur_color);
    FillCredits(my_Font);

    backButton.Draw();
    SDL_RenderPresent(renderer);
  }
///////////////////////////////////////////////////////////////////////////////


/////////////////////////////Select amount of players menu//////////////////////
  SAP:
  backButton.keytrick = false;
  oneButton.keytrick = false;
  twoButton.keytrick = false;
  threeButton.keytrick = false;
  fourButton.keytrick = false;
  for (int i = 0; i < dots_menu_size; i++)
    dots[i].exist = false;

  while (1)
  {
    backButton.Update(mouse);
    oneButton.Update(mouse);
    twoButton.Update(mouse);
    threeButton.Update(mouse);
    fourButton.Update(mouse);
    mouse.Update();
    SDL_PollEvent(&event);
    if (event.type == SDL_KEYDOWN && event.type == SDL_QUIT)
      exit(0);

    if ( (event.button.button == SDL_BUTTON_LEFT) && (backButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      goto Menu;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      backButton.keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      goto Menu;

    if ((event.key.keysym.sym == SDLK_ESCAPE) && (event.type == SDL_KEYUP) )
      goto Menu;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (oneButton.IsSelected) 
      && (event.type == SDL_MOUSEBUTTONUP))
      goto OPM;

    if ( (event.key.keysym.sym == SDLK_1) && (event.type == SDL_KEYUP) )
      goto OPM;

    if( (event.key.keysym.sym == SDLK_1) && (event.type == SDL_KEYDOWN) )
      oneButton.keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (twoButton.IsSelected) 
      && (event.type == SDL_MOUSEBUTTONUP))  
      goto TWOPM;

    if ( (event.key.keysym.sym == SDLK_2) && (event.type == SDL_KEYUP) )
      goto TWOPM;

    if( (event.key.keysym.sym == SDLK_2) && (event.type == SDL_KEYDOWN) )
      twoButton.keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (threeButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP))
      goto THPM;

    if ( (event.key.keysym.sym == SDLK_3) && (event.type == SDL_KEYUP) )
      goto THPM;

    if( (event.key.keysym.sym == SDLK_3) && (event.type == SDL_KEYDOWN) )
      threeButton.keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (fourButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      goto FPM;

    if ( (event.key.keysym.sym == SDLK_4) && (event.type == SDL_KEYUP) )
      goto FPM;

    if( (event.key.keysym.sym == SDLK_4) && (event.type == SDL_KEYDOWN) )
      fourButton.keytrick = true;

    SDL_GetMouseState(&xMouse,&yMouse);
    GetDotInput(dots, cur_color, field_x_base, field_y_base);
    GetDotErase(dots, field_x_base, field_y_base);
    SDL_SetRenderDrawColor(renderer, red_default, green_default, blue_default,
      SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    MoveBoard(field_x_base, field_y_base);
    DrawField(field_x_base, field_y_base);
    DrawDots(dots, dots_menu_size, field_x_size);
    Zoom();
    EnumerateField(field_y_base, field_x_base, my_Font);

    paLabel.Draw();
    backButton.Draw();
    oneButton.Draw();
    twoButton.Draw();
    threeButton.Draw();
    fourButton.Draw();
    mouse.DrawCircle(cur_color);
    
    SDL_RenderPresent(renderer);
  }
////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////One player menu//////////////////////////
  OPM:
  back2Button.keytrick = false;
  for (int i = 0; i < dots_menu_size; i++)
    dots[i].exist = false;

  while (1)
  {
    back2Button.Update(mouse);
    mouse.Update();
    SDL_PollEvent(&event);
    if (event.type == SDL_KEYDOWN && event.type == SDL_QUIT)
      exit(0);

    if (event.key.keysym.sym == SDLK_ESCAPE)
      goto Menu;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      back2Button.keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      goto SAP;

    if ( (back2Button.IsSelected) && (event.button.button == SDL_BUTTON_LEFT)
       && (event.type == SDL_MOUSEBUTTONUP))
      goto SAP;
    
    SDL_GetMouseState(&xMouse,&yMouse);

    MoveBoard(field_x_base, field_y_base);
    SDL_SetRenderDrawColor(renderer, 25, 80, 80, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    DrawField(field_x_base, field_y_base);
    GetDotInput(dots, cur_color, field_x_base, field_y_base);
    GetDotErase(dots, field_x_base, field_y_base);
    DrawDots(dots, dots_menu_size, field_x_size);
    EnumerateField(field_y_base, field_x_base, my_Font);
    Zoom();

    gvaiLabel.Draw();
    udLabel.Draw();
    back2Button.Draw();
    mouse.DrawCircle(cur_color);
    
    SDL_RenderPresent(renderer);
  }
////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////Two player menu//////////////////////////
  TWOPM:
  back2Button.keytrick = false;
  continueButton.keytrick = false;
  for (int i = 0; i < dots_menu_size; i++)
    dots[i].exist = false;

  while (1)
  {
    mouse.Update();
    continueButton.Update(mouse);
    back2Button.Update(mouse);
    P21.Update(mouse);
    P22.Update(mouse);
    SDL_PollEvent(&event);
    if (event.type == SDL_KEYDOWN && event.type == SDL_QUIT)
      exit(0);

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
      goto Menu;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      back2Button.keytrick = false;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      back2Button.keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (back2Button.IsSelected) 
      && (event.type == SDL_MOUSEBUTTONUP))
      goto SAP;

    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN) )
      continueButton.keytrick = true;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP) )
    {
      pa_back = 2;  
      goto GSM;
    }

    if ( (event.button.button == SDL_BUTTON_LEFT) && (continueButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
    {
      pa_back = 2;  
      goto GSM;
    }

    if ( (!P21.IsSelected) && (!P22.IsSelected) )
      FieldColor();

    if (P21.IsSelected)
    { 
      P21.ChangeColor();
      cur_color = P21.clr;
    }

    if (P22.IsSelected)
    { 
      P22.ChangeColor();
      cur_color = P22.clr;
    }

    SDL_GetMouseState(&xMouse,&yMouse);
    MoveBoard(field_x_base, field_y_base);
    SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    DrawField(field_x_base, field_y_base);
    EnumerateField(field_y_base, field_x_base, my_Font);
    Zoom();
    GetDotInput(dots, cur_color, field_x_base, field_y_base);
    GetDotErase(dots, field_x_base, field_y_base);
    DrawDots(dots, dots_menu_size, field_x_size);

    P21.Draw();
    P22.Draw();
    continueButton.Draw();
    sgLabel.Draw();
    colorsLabel.Draw();
    rgbLabel.Draw();
    back2Button.Draw();
    mouse.DrawCircle(cur_color);
    
    SDL_RenderPresent(renderer);
  }
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////Three player menu/////////////////////////////
  THPM:
  back2Button.keytrick = false;
  continueButton.keytrick = false;
  for (int i = 0; i < dots_menu_size; i++)
    dots[i].exist = false;

  while (1)
  {
    mouse.Update();
    continueButton.Update(mouse);
    back2Button.Update(mouse);
    P31.Update(mouse);
    P32.Update(mouse);
    P33.Update(mouse);
    SDL_PollEvent(&event);
    if (event.type == SDL_KEYDOWN && event.type == SDL_QUIT)
      exit(0);

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
      goto Menu;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      back2Button.keytrick = false;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      back2Button.keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (back2Button.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP))
      goto SAP;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP) )
    {
      pa_back = 3;  
      goto GSM;
    }

    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN) )
      continueButton.keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (continueButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
    {
      pa_back = 3;  
      goto GSM;
    }

    if ( (!P31.IsSelected) && (!P32.IsSelected) && (!P33.IsSelected)  )
      FieldColor();

    if (P31.IsSelected)
    { 
      P31.ChangeColor();
      cur_color = P31.clr;
    }

    if (P32.IsSelected)
    { 
      P32.ChangeColor();
      cur_color = P32.clr;
    }

    if (P33.IsSelected)
    { 
      P33.ChangeColor();
      cur_color = P33.clr;
    }

    SDL_GetMouseState(&xMouse,&yMouse);
    MoveBoard(field_x_base, field_y_base);
    SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    DrawField(field_x_base, field_y_base);
    EnumerateField(field_y_base, field_x_base, my_Font);
    Zoom();
    GetDotInput(dots, cur_color, field_x_base, field_y_base);
    GetDotErase(dots, field_x_base, field_y_base);
    DrawDots(dots, dots_menu_size, field_x_size);

    P31.Draw();
    P32.Draw();
    P33.Draw();
    sgLabel.Draw();
    colorsLabel.Draw();
    rgbLabel.Draw();
    back2Button.Draw();
    continueButton.Draw();
    mouse.DrawCircle(cur_color);
    
    SDL_RenderPresent(renderer);
  }
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////Four player menu//////////////////////////////
  FPM:
  back2Button.keytrick = false;
  continueButton.keytrick = false;
  for (int i = 0; i < dots_menu_size; i++)
    dots[i].exist = false;


  while (1)
  {
    mouse.Update();
    continueButton.Update(mouse);
    back2Button.Update(mouse);
    P41.Update(mouse);
    P42.Update(mouse);
    P43.Update(mouse);
    P44.Update(mouse);

    SDL_PollEvent(&event);
    if (event.type == SDL_KEYDOWN && event.type == SDL_QUIT)
      exit(0);

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
      goto Menu;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      back2Button.keytrick = false;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      back2Button.keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (back2Button.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      goto SAP;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP) )
    {
      pa_back = 4;  
      goto GSM;
    }

    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN) )
      continueButton.keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (continueButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
    {
      pa_back = 4;  
      goto GSM;
    }

    if (  (!P41.IsSelected) && (!P42.IsSelected) && (!P43.IsSelected))
      FieldColor();

    if (P41.IsSelected)
    { 
      P41.ChangeColor();
      cur_color = P41.clr;
    }

    if (P42.IsSelected)
    { 
      P42.ChangeColor();
      cur_color = P42.clr;
    }

    if (P43.IsSelected)
    { 
      P43.ChangeColor();
      cur_color = P43.clr;
    }

    if (P44.IsSelected)
    { 
      P44.ChangeColor();
      cur_color = P44.clr;
    }

    SDL_GetMouseState(&xMouse,&yMouse);
    MoveBoard(field_x_base, field_y_base);
    SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    DrawField(field_x_base, field_y_base);
    EnumerateField(field_y_base, field_x_base, my_Font);
    Zoom();
    GetDotInput(dots, cur_color, field_x_base, field_y_base);
    GetDotErase(dots, field_x_base, field_y_base);
    DrawDots(dots, dots_menu_size, field_x_size);

    sgLabel.Draw();
    colorsLabel.Draw();
    back2Button.Draw();
    rgbLabel.Draw();
    continueButton.Draw();
    P41.Draw();
    P42.Draw();
    P43.Draw();
    P44.Draw();
    mouse.DrawCircle(cur_color);
    
    SDL_RenderPresent(renderer);
  }
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////Game size menu////////////////////////////////
  GSM:
  game_x_size = 30;
  game_y_size = 30;
  back3Button.keytrick = false;
  continueButton.keytrick = false;
  for (int i = 0; i < dots_menu_size; i++)
    dots[i].exist = false;

  while (1)
  {
    mouse.Update();
    continueButton.Update(mouse);
    back3Button.Update(mouse);

    SDL_PollEvent(&event);
    if (event.type == SDL_KEYDOWN && event.type == SDL_QUIT)
      exit(0);

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
      goto Menu;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP) )
      goto GRM;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (continueButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      goto GRM;

    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN) )
      continueButton.keytrick = true;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      back3Button.keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
    {
      switch (pa_back)
      {
        case 1:
          goto OPM;
        case 2:
          goto TWOPM;
        case 3:
          goto THPM;
        case 4:
          goto FPM;
      }
    }

    if ( (event.button.button == SDL_BUTTON_LEFT) && (back3Button.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
   {
      switch (pa_back)
      {
        case 1:
          goto OPM;
        case 2:
          goto TWOPM;
        case 3:
          goto THPM;
        case 4:
          goto FPM;
      }
    }

    temp_str = to_string(game_x_size);
    field_x = (char*) temp_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, field_x, {255, 255, 0});
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    
    if ( (SDL_HasIntersection(&rectangle, &mouse.point)) &&
        (event.type == SDL_KEYDOWN) &&
        ((event.key.keysym.sym == SDLK_w) || (event.key.keysym.sym == SDLK_UP)))
      game_x_size++;

    if ( (SDL_HasIntersection(&rectangle, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      ((event.key.keysym.sym == SDLK_s) || (event.key.keysym.sym == SDLK_DOWN)))
      game_x_size--;

    temp_str = to_string(game_y_size);
    field_y = (char*) temp_str.c_str();
    surfaceText2 = TTF_RenderText_Solid(my_Font, field_y, {255, 255, 0});
    textureText2 = SDL_CreateTextureFromSurface(renderer, surfaceText2);
    
    if ( (SDL_HasIntersection(&rectangle2, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      ((event.key.keysym.sym == SDLK_w) || (event.key.keysym.sym == SDLK_UP)))
      game_y_size++;

    if ( (SDL_HasIntersection(&rectangle2, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      ((event.key.keysym.sym == SDLK_s) || (event.key.keysym.sym == SDLK_DOWN)))
      game_y_size--;

    if (game_y_size < 5)
      game_y_size = 5;

    if (game_y_size > 300)
      game_y_size = 300;

    if (game_x_size < 5)
      game_x_size = 5;

    if (game_x_size > 300)
      game_x_size = 300;

    if (game_x_size >= 100)
      rectangle.w = rectangle_w * 3;
    else if (game_x_size < 10)
      rectangle.w = rectangle_w;
    else
      rectangle.w = rectangle_w * 2;

    if (game_y_size >= 100) 
      rectangle2.w = rectangle_w * 3;
    else if (game_y_size < 10)
      rectangle2.w = rectangle_w;
    else
      rectangle2.w = rectangle_w * 2;

    SDL_GetMouseState(&xMouse,&yMouse);
    SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    DrawField(game_x_size, game_y_size);
    if (!SDL_HasIntersection(&rectangle, &mouse.point) && 
      !SDL_HasIntersection(&rectangle2, &mouse.point))
      MoveBoard(game_x_size, game_y_size);
    Zoom();
    EnumerateField(game_y_size, game_x_size, my_Font);

    SDL_RenderCopy(renderer, textureText, NULL, &rectangle);
    SDL_RenderCopy(renderer, textureText2, NULL, &rectangle2);
    SDL_RenderCopy(renderer, textureText3, NULL, &rectangle3);

    SDL_DestroyTexture(textureText);
    SDL_FreeSurface(surfaceText);
    SDL_DestroyTexture(textureText2);
    SDL_FreeSurface(surfaceText2);

    back3Button.Draw();
    sgsLabel.Draw();
    useLabel.Draw();
    continueButton.Draw();
    mouse.DrawCircle(cur_color);
    SDL_RenderPresent(renderer);
  }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////Game rule menu//////////////////////////
  GRM:
  back2Button.keytrick = false;
  dcwButton.keytrick = false;
  tvButton.keytrick = false;
  back2Button.keytrick = false;

  while (1)
  {
    mouse.Update();
    back2Button.Update(mouse);
    dcwButton.Update(mouse);
    tvButton.Update(mouse); 

    SDL_PollEvent(&event);
    if (event.type == SDL_KEYDOWN && event.type == SDL_QUIT)
      exit(0);

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      goto GSM;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      back2Button.keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (back2Button.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      goto GSM;

    if (event.key.keysym.sym == SDLK_ESCAPE)
      goto Menu;

    if ((event.key.keysym.sym == SDLK_t) && (event.type == SDL_KEYUP) )
      goto Game;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (tvButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      goto Game;

    if ( (event.key.keysym.sym == SDLK_t) && (event.type == SDL_KEYDOWN) )
      tvButton.keytrick = true;

    if ((event.key.keysym.sym == SDLK_d) && (event.type == SDL_KEYUP) )
      goto Game;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (dcwButton.IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      goto Game;

    if ( (event.key.keysym.sym == SDLK_d) && (event.type == SDL_KEYDOWN) )
      dcwButton.keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      goto GSM;

    SDL_GetMouseState(&xMouse,&yMouse);
    SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    DrawField(game_x_size, game_y_size);
    MoveBoard(game_x_size, game_y_size);
    Zoom();
    EnumerateField(game_y_size, game_x_size, my_Font);

    tvButton.Draw();
    dcwButton.Draw();
    back2Button.Draw();
    sgrLabel.Draw();
    mouse.DrawCircle(cur_color);
    SDL_RenderPresent(renderer);
  }
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////Main game cycle/////////////////////////
  Game:
  int dots_game_size = (game_y_size + 2) * (game_x_size + 2);
  Dots *dots_g = new Dots[dots_game_size];
  for (int i = 0; i < dots_game_size; i++)
  {
    dots_g[i].exist = false;
  }
  x_c = 0;
  y_c = 0;

  switch (pa_back)
  {
    case 2:
      cur_color = P21.clr;
      break;
    case 3:
      cur_color = P31.clr;
    case 4:
      cur_color = P41.clr;
  }

  while (1)
  {
    mouse.Update();
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT)
      break;

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
      exit(0);

    SDL_GetMouseState(&xMouse,&yMouse);
    if (GetDotInput(dots_g, cur_color, game_x_size, game_y_size))
    {
      switch (pa_back)
      {
        case 2:
        {
          if (cur_color == P21.clr)
            cur_color = P22.clr;
          else
            cur_color = P21.clr;
          break;
        }
        case 3:
        {
          if (cur_color == P31.clr)
            cur_color = P32.clr;
          else if (cur_color == P32.clr)
            cur_color = P33.clr;
          else
            cur_color = P31.clr;
          break;
        }
        case 4:
        {
          if (cur_color == P41.clr)
            cur_color = P42.clr;
          else if (cur_color == P42.clr)
            cur_color = P43.clr;
          else if (cur_color == P43.clr)
            cur_color = P44.clr;
          else
            cur_color = P41.clr;
          break;
        }
      }
    }
    SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    MoveBoard(game_x_size, game_y_size);
    DrawField(game_x_size, game_y_size);
    DrawDots(dots_g, dots_game_size, game_x_size);
    Zoom();
    EnumerateField(game_y_size, game_x_size, my_Font);

    mouse.DrawCircle(cur_color);
    SDL_RenderPresent(renderer);    
  }
////////////////////////////////////////////////////////////////////////////////


//////////////////////////Exiting program///////////////////////////////////////
  delete dots_g;
  SDL_DestroyTexture(textureText);
  SDL_DestroyTexture(textureText2);
  SDL_DestroyTexture(textureText3);
  TTF_CloseFont(my_Font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 666;
////////////////////////////////////////////////////////////////////////////////
}

