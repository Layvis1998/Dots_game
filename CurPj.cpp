#include <iostream>
#include <err.h>
#include <string>
#include <queue>
#include <unordered_set>
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

class Dots
{
public:
  bool exist;
  SDL_Color clr;
  bool visited;
  bool deadend;
  bool cycle;
};

//global constants
int intrv = 20; // interval
const uint8_t red_default = 25;
const uint8_t green_default = 80;
const uint8_t blue_default = 80;
const int field_x_base = 150;
const int field_y_base = 240;
const int dots_menu_size = int(field_x_base + 2) * int(field_y_base + 2);
const int field_x_credits = 50;
const int field_y_credits = 100;
const int dots_credits_size = int(field_y_credits + 2) * int(field_x_credits + 2);

//gloabal variables
int vo;  //vertical offset
int ho;  //horizontal offset
int maxdotamount = 0;
int maxeatendots = 0;
bool classic = true;
vector <Button> ButtonVector;
vector <Label> LabelVector;
vector <ColorButton> ClrButtonVector;
Dots dots[dots_menu_size];
Dots dots_c[dots_credits_size];
Dots* dots_g;
char* field_x;
char* field_y;  

uint8_t red = 25;
uint8_t green = 80;
uint8_t blue = 80;
int x_c = 0;
int y_c = 0;
int ScWidth;
int ScHeight;
int field_x_size = field_x_base;
int field_y_size = field_y_base;
int game_x_size = 30;
int game_y_size = 30;
int rectangle_w;
int rectangle_h;

TTF_Font* my_Font; 
Mouse mouse;
int xMouse = 0;
int yMouse = 0;
uint8_t pa_back = 1;
SDL_Color white = {255, 255, 255, 255};
SDL_Color cur_color;

enum GameStateType {Menu, Credits, SAP, ONEPM, TWOPM,
                    THREEPM, FOURPM, GSM, GRM, Game};
GameStateType GameState;


int DifferentWays( Dots* dots, int current, int fx , int fy)
{
  int retval = 0;

  if (((current + 1) % fx != 0)
    && (dots[current + 1].exist == true)
    && (dots[current + 1].deadend != true))
    retval++;

  if ( (( current + 1) % fx != 0) && ( current + fx < fx * fy )
     && (dots[current + 1 + fx].exist == true)
     && (dots[current + 1 + fx].deadend != true))
    retval++;

  if (( current + fx < fx * fy ) && (dots[current + fx].exist == true)
    && (dots[current + fx].deadend != true))
    retval++;

  if (( current % fx != 0) && ( current + fx < fx * fy )
    && (dots[current - 1 + fx].exist == true)
    && (dots[current - 1 + fx].deadend != true))
    retval++;

  if ( ( current % fx != 0) && (dots[current - 1].exist == true)
    && (dots[current - 1].deadend != true))
    retval++;

  if ( ( current % fx != 0) && ( current - fx >= 0)
    && (dots[current - 1 - fx].exist == true)
    && (dots[current - 1 - fx].deadend != true))
    retval++;

  if (( current - fx >= 0) && (dots[current - fx].exist == true)
    && (dots[current - fx].deadend != true))  
    retval++;

  if ( (( current + 1) % fx != 0) && ( current - fx >= 0 )
     && (dots[current + 1 - fx].exist == true)
     && (dots[current + 1 - fx].deadend != true))
    retval++;

  return retval;
}

void FindConnectedDots
  (Dots* dots, int current, unordered_set <int> &uset, int fx, int fy)
{
  dots[current].visited = true;
  uset.insert(current);
  
  //up-right direction
  if ( (( current + 1) % fx != 0) && ( current - fx >= 0 )
     && (dots[current + 1 - fx].exist == true)
     && (dots[current + 1 - fx].deadend == false))
  {
    if (dots[current + 1 - fx].visited == false)
      FindConnectedDots(dots, current + 1 - fx, uset, fx, fy);
  }

  // right direction
  if (((current + 1) % fx != 0) && (dots[current + 1].exist == true)
    && (dots[current + 1].deadend == false))
  {                  
    if (dots[current + 1].visited == false)
      FindConnectedDots(dots, current + 1, uset, fx, fy);
  }

  //down-right direction  
  if ( (( current + 1) % fx != 0) && ( current + fx < fx * fy )
     && (dots[current + 1 + fx].exist == true)
     && (dots[current + 1 + fx].deadend == false))   
  {
    if (dots[current + 1 + fx].visited == false)
      FindConnectedDots(dots, current + 1 + fx, uset, fx, fy);  
  }

  // down direction
  if (( current + fx < fx * fy ) && (dots[current + fx].exist == true)
    && (dots[current + fx].deadend == false))
  { 
    if (dots[current + fx].visited == false)
      FindConnectedDots(dots, current + fx, uset, fx, fy);
  }

  //down-left direction
  if (( current % fx != 0) && ( current + fx < fx * fy )
     && (dots[current - 1 + fx].exist == true)
     && (dots[current - 1 + fx].deadend == false))
  { 
    if (dots[current - 1 + fx].visited == false)
      FindConnectedDots(dots, current - 1 + fx, uset, fx, fy); 
  }

  // left direction
  if ( ( current % fx != 0) && (dots[current - 1].exist == true)
    && (dots[current - 1].deadend == false))
  {  
    if (dots[current - 1].visited == false)
      FindConnectedDots(dots, current - 1, uset, fx, fy);  
  }

  //up-left direction 
  if ( ( current % fx != 0) && ( current - fx >= 0)
     && (dots[current - 1 - fx].exist == true)
     && (dots[current - 1 - fx].deadend == false))
  {
    if (dots[current - 1 - fx].visited == false)
      FindConnectedDots(dots, current - 1 - fx, uset, fx, fy); 
  }

  // up diterction
  if (( current - fx >= 0) && (dots[current - fx].exist == true)
    && (dots[current - fx].deadend == false))                                        
  { 
    if (dots[current - fx].visited == false)
      FindConnectedDots(dots, current - fx, uset, fx, fy);
  }
}

void DeleteBranches
 (Dots* dots, int current, unordered_set <int> &uset, int fx, int fy)
{
  bool branchdeleted = true;

  while ((uset.size() > 0) && (branchdeleted == true))
  {
    branchdeleted = false;
    for (auto i = uset.begin(); i != uset.end(); )
    {
      if (DifferentWays(dots, *i, fx , fy) <= 1)
      { 
        branchdeleted = true;
        dots[*i].deadend = true; 
        i = uset.erase(i);
      }
      else
        i++;
    }
  }
}

void FindContour (Dots* dots, int fx, int fy)
{
  int current = 0;

  while ((current < fx * fy) && (dots[current].exist == false))
    current++;
  
  if (current == fx * fy)
    return;
  
  unordered_set <int> uset;
  FindConnectedDots(dots, current, uset, fx, fy);
  cout << "Connections found, uset size = " << uset.size() << endl;
  DeleteBranches(dots, current, uset, fx, fy);
  cout << "Branches deleted, uset size = "  << uset.size()  << endl; 

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
  SDL_Rect rectangle;
  rectangle.w = intrv * 47;
  rectangle.h = intrv * 2;
      
  const char *tex = "This game was written by 25-year old Karandeev"
                    " Ilia at summer 2023 during his stay in \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - x_c;
  rectangle.y = intrv * 5 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex2 = "Armenia. It was inspired by his school days of"
                     " playing the Dots game back in Russia. He\0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex2, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - x_c;
  rectangle.y = intrv * 8 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex3 = "spent a really huge amount of time playing this"
                     " game with his friends during some of\0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex3, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - x_c;
  rectangle.y = intrv * 11 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex4 = "the boring lessons. After some years including his"
                    " study in the CMC MSU faculty he \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex4, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - x_c;
  rectangle.y = intrv * 14 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex5 = "finally had the skill to implement it as a computer"
                     " games. Special thanks to his former\0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex5, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - x_c;
  rectangle.y = intrv * 17 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex6 = "fellow CMC group member Daniel Oblomov, who helped him"
                     " during his hard days of living \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex6, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - x_c;
  rectangle.y = intrv * 20 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex7 = "in Armenia. That's it. Hope you enjoy/will enjoy/enjoyed"
                     " playing this wonderfull game. \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex7, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - x_c;
  rectangle.y = intrv * 23 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  const char *tex8 = "                                   "
                     "             Respectfully yours, Karandeev Ilia       \0";
  surfaceText = TTF_RenderText_Solid(my_Font, tex8, {0, 200, 200});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangle.x = intrv * 3 - x_c;
  rectangle.y = intrv * 26 - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangle);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);
}

void EnumerateField(int dot_clmns, int dot_rows, TTF_Font *my_Font)
{
  SDL_Surface* surfaceText;
  SDL_Texture* textureText;
  SDL_Rect rectangleE;

  int number = 0;
  string num_str;
  char* num;
  rectangleE.w = intrv * 1.5;
  rectangleE.h = intrv * 1.5;

  for (int number = 0; number < dot_rows; number += 5)
  {
    num_str = to_string(number);
    num = (char*) num_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    rectangleE.x = intrv * (number + 1.3) - x_c;
    rectangleE.y = - y_c + 0.5;
    SDL_RenderCopy(renderer, textureText, NULL, &rectangleE);
    SDL_DestroyTexture(textureText);
    SDL_FreeSurface(surfaceText);
  }
  
  for (int number = 0; number < dot_clmns; number += 5)
  {
    num_str = to_string(number);
    num = (char*) num_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    rectangleE.x = - x_c + 0.5;
    rectangleE.y = intrv * ( number + 1.3) - y_c;
    SDL_RenderCopy(renderer, textureText, NULL, &rectangleE);
    SDL_DestroyTexture(textureText);
    SDL_FreeSurface(surfaceText);
  }

  for (int number = 0; number < dot_rows; number += 5)
  {
    num_str = to_string(number);
    num = (char*) num_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    rectangleE.x = intrv * ( number + 1.3) - x_c;
    rectangleE.y = intrv * (dot_clmns + 1.5) - y_c;
    SDL_RenderCopy(renderer, textureText, NULL, &rectangleE);
    SDL_DestroyTexture(textureText);
    SDL_FreeSurface(surfaceText);
  }
  
  for (int number = 0; number < dot_clmns; number += 5)
  {
    num_str = to_string(number);
    num = (char*) num_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    rectangleE.x = intrv * (dot_rows + 1.5) - x_c;
    rectangleE.y = intrv * ( number + 1.3) - y_c;
    SDL_RenderCopy(renderer, textureText, NULL, &rectangleE);
    SDL_DestroyTexture(textureText);
    SDL_FreeSurface(surfaceText);
  }

  number = dot_rows - 1;
  num_str = to_string(number);
  num = (char*) num_str.c_str();
  surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangleE.x = intrv * (number + 1.3) - x_c;
  rectangleE.y = - y_c + 0.5;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangleE);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);
  surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangleE.x = intrv * ( number + 1.3) - x_c;
  rectangleE.y = intrv * (dot_clmns + 1.5) - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangleE);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);

  number = dot_clmns - 1;
  num_str = to_string(number);
  num = (char*) num_str.c_str();
  surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangleE.x = - x_c + 0.5;
  rectangleE.y = intrv * ( number + 1.3) - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangleE);
  SDL_DestroyTexture(textureText);
  SDL_FreeSurface(surfaceText);
  surfaceText = TTF_RenderText_Solid(my_Font, num, {0, 0, 0});
  textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
  rectangleE.x = intrv * (dot_rows + 1.5) - x_c;
  rectangleE.y = intrv * ( number + 1.3) - y_c;
  SDL_RenderCopy(renderer, textureText, NULL, &rectangleE);
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
     ( (( (yMouse) % intrv) <= intrv / 3.0)
     || (((yMouse) % intrv) >= intrv * 2 / 3)) 
     && 
     ( (( (xMouse) % intrv) <= intrv / 3.0)
     || (((xMouse) % intrv) >= intrv * 2 / 3))
     && (xMouse >= 5.0 / 3 * intrv) && (yMouse >= 5.0 / 3 * intrv)
     && (xMouse + x_c <= (fxs + 5.0 / 3) * intrv)
     && (yMouse + y_c <= (fys + 5.0 / 3) * intrv))
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
       ( (( (yMouse) % intrv) <= intrv / 3.0) 
       || (((yMouse) % intrv) >= intrv * 2 / 3)) 
       && 
       ((( (xMouse) % intrv) <= intrv / 3.0)
       || (((xMouse) % intrv) >= intrv * 2 / 3))
       && (xMouse >= 5.0 / 3 * intrv) && (yMouse >= 5.0 / 3 * intrv)
       && (xMouse + x_c <= (fxs + 5.0 / 3) * intrv)
       && (yMouse + y_c <= (fys + 5.0 / 3) * intrv))
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

void MainMenu()
{
  ButtonVector[0].keytrick = false;
  ButtonVector[1].keytrick = false;
  ButtonVector[2].keytrick = false;
  ButtonVector[3].keytrick = false;
  ButtonVector[4].keytrick = false;

  for (int i = 0; i < dots_menu_size; i++)
  {
    dots[i].exist = false;
    dots[i].visited = false;
    dots[i].deadend = false;
  }
  
  while (GameState == Menu)
  { 
    mouse.Update();
    ButtonVector[0].Update(mouse);
    ButtonVector[1].Update(mouse);
    ButtonVector[2].Update(mouse);    
    ButtonVector[3].Update(mouse);
    ButtonVector[4].Update(mouse);

    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT))
      exit(0);

    if ( (event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[4].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      exit(0);

    if ( (event.key.keysym.sym == SDLK_e) && (event.type == SDL_KEYUP) )
      exit(0);

    if( (event.key.keysym.sym == SDLK_e) && (event.type == SDL_KEYDOWN) )
      ButtonVector[4].keytrick = true;

    if( (event.key.keysym.sym == SDLK_n) && (event.type == SDL_KEYDOWN) )
      ButtonVector[0].keytrick = true;

    if ( (event.key.keysym.sym == SDLK_n) && (event.type == SDL_KEYUP) )
      GameState = SAP;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[0].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = SAP;

    if( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN) )
      ButtonVector[3].keytrick = true;

    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP) )
      GameState = Credits;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[3].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = Credits;

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
    {
      dots[i].visited = false;
      dots[i].deadend = false;
    }
    FindContour(dots, field_x_base, field_y_base);
    
    ButtonVector[0].Draw();
    ButtonVector[1].Draw();
    ButtonVector[2].Draw();
    ButtonVector[3].Draw();
    ButtonVector[4].Draw();
    mouse.DrawCircle(cur_color);

    SDL_RenderPresent(renderer);
  }
}

void CreditsMenu()
{
  for (int i = 0; i < dots_credits_size; i++)
    dots_c[i].exist = false;

  ButtonVector[9].keytrick = false;
  x_c = 0;
  y_c = 0;
  
  while (GameState == Credits)
  {
    ButtonVector[9].Update(mouse);
    mouse.Update();

    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT))
      exit(0);

    if ((event.key.keysym.sym == SDLK_ESCAPE) && (event.type == SDL_KEYUP) )
      GameState = Menu;


    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[9].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = Menu;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[9].keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      GameState = Menu;

    SDL_GetMouseState(&xMouse, &yMouse);
    GetDotInput(dots_c, cur_color, field_x_credits, field_y_credits);
    GetDotErase(dots_c, field_x_credits, field_y_credits);
    SDL_SetRenderDrawColor(renderer, red_default, green_default,
      blue_default, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    MoveBoard(field_x_credits, field_y_credits);

    DrawField(field_x_credits, field_y_credits);
    DrawDots(dots_c, dots_credits_size, field_x_credits);
    EnumerateField(field_y_credits, field_x_credits, my_Font);
    Zoom();
    mouse.DrawCircle(cur_color);
    FillCredits(my_Font);

    ButtonVector[9].Draw();
    SDL_RenderPresent(renderer);
  }
}

void SelectAmountOfPlayersMenu()
{
  ButtonVector[5].keytrick = false;
  ButtonVector[6].keytrick = false;
  ButtonVector[7].keytrick = false;
  ButtonVector[8].keytrick = false;
  ButtonVector[9].keytrick = false;
  for (int i = 0; i < dots_menu_size; i++)
  {
    dots[i].exist = false;
    dots[i].visited = false;
    dots[i].deadend = false;
  }

  while (GameState == SAP)
  {
    ButtonVector[5].Update(mouse);
    ButtonVector[6].Update(mouse);
    ButtonVector[7].Update(mouse);
    ButtonVector[8].Update(mouse);
    ButtonVector[9].Update(mouse);
    mouse.Update();
    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT) )
      exit(0);

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[9].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = Menu;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[9].keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      GameState = Menu;

    if ((event.key.keysym.sym == SDLK_ESCAPE) && (event.type == SDL_KEYUP) )
      GameState = Menu;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[5].IsSelected) 
      && (event.type == SDL_MOUSEBUTTONUP))
      GameState = ONEPM;

    if ( (event.key.keysym.sym == SDLK_1) && (event.type == SDL_KEYUP) )
      GameState = ONEPM;

    if( (event.key.keysym.sym == SDLK_1) && (event.type == SDL_KEYDOWN) )
      ButtonVector[5].keytrick = true;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[6].IsSelected) 
      && (event.type == SDL_MOUSEBUTTONUP))  
      GameState = TWOPM;

    if ( (event.key.keysym.sym == SDLK_2) && (event.type == SDL_KEYUP) )
      GameState = TWOPM;

    if( (event.key.keysym.sym == SDLK_2) && (event.type == SDL_KEYDOWN) )
      ButtonVector[6].keytrick = true;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[7].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP))
      GameState = THREEPM;

    if ( (event.key.keysym.sym == SDLK_3) && (event.type == SDL_KEYUP) )
      GameState = THREEPM;

    if( (event.key.keysym.sym == SDLK_3) && (event.type == SDL_KEYDOWN) )
      ButtonVector[7].keytrick = true;

    if ((event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[8].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = FOURPM;

    if ( (event.key.keysym.sym == SDLK_4) && (event.type == SDL_KEYUP) )
      GameState = FOURPM;

    if( (event.key.keysym.sym == SDLK_4) && (event.type == SDL_KEYDOWN) )
      ButtonVector[8].keytrick = true;

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

    LabelVector[0].Draw();
    ButtonVector[5].Draw();
    ButtonVector[6].Draw();
    ButtonVector[7].Draw();
    ButtonVector[8].Draw();
    ButtonVector[9].Draw();
    mouse.DrawCircle(cur_color);
    
    SDL_RenderPresent(renderer);
  }
}  

void OnePlayerMenu()
{
  ButtonVector[10].keytrick = false;
  for (int i = 0; i < dots_menu_size; i++)
  {
    dots[i].exist = false;
    dots[i].visited = false;
    dots[i].deadend = false;
  }

  while (GameState == ONEPM)
  {
    ButtonVector[10].Update(mouse);
    mouse.Update();
    SDL_PollEvent(&event);
    if ((event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT))
      exit(0);

    if (event.key.keysym.sym == SDLK_ESCAPE)
      GameState = Menu;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[10].keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      GameState = SAP;

    if ( (ButtonVector[10].IsSelected)
       && (event.button.button == SDL_BUTTON_LEFT)
       && (event.type == SDL_MOUSEBUTTONUP))
      GameState = SAP;
    
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

    LabelVector[1].Draw();
    LabelVector[2].Draw();
    ButtonVector[10].Draw();
    mouse.DrawCircle(cur_color);
    
    SDL_RenderPresent(renderer);
  }
}

void TwoPlayerMenu()
{
  ButtonVector[10].keytrick = false;
  ButtonVector[12].keytrick = false;
  for (int i = 0; i < dots_menu_size; i++)
  {
    dots[i].exist = false;
    dots[i].visited = false;
    dots[i].deadend = false;
  }

  while (GameState == TWOPM)
  {
    mouse.Update();
    ButtonVector[10].Update(mouse);
    ButtonVector[12].Update(mouse);
    ClrButtonVector[0].Update(mouse);
    ClrButtonVector[1].Update(mouse);
    SDL_PollEvent(&event);
    if ( (event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT) )
      exit(0);

    if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
      GameState = Menu;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      ButtonVector[10].keytrick = false;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[10].keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[10].IsSelected) 
      && (event.type == SDL_MOUSEBUTTONUP))
      GameState = SAP;


    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN) )
      ButtonVector[12].keytrick = true;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP) )
    {
      pa_back = 2;  
      GameState = GSM;
    }

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[12].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
    {
      pa_back = 2;  
      GameState = GSM;
    }

    if ((!ClrButtonVector[0].IsSelected) && (!ClrButtonVector[1].IsSelected))
      FieldColor();

    if (ClrButtonVector[0].IsSelected)
    { 
      ClrButtonVector[0].ChangeColor();
      cur_color = ClrButtonVector[0].clr;
    }

    if (ClrButtonVector[1].IsSelected)
    { 
      ClrButtonVector[1].ChangeColor();
      cur_color = ClrButtonVector[1].clr;
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

    ClrButtonVector[0].Draw();
    ClrButtonVector[1].Draw();

    LabelVector[3].Draw();
    LabelVector[4].Draw();
    LabelVector[5].Draw();
    ButtonVector[10].Draw();
    ButtonVector[12].Draw();
    mouse.DrawCircle(cur_color);
    
    SDL_RenderPresent(renderer);
  }
}

void ThreePlayerMenu()
{
  ButtonVector[10].keytrick = false;
  ButtonVector[12].keytrick = false;
  for (int i = 0; i < dots_menu_size; i++)
  {
    dots[i].exist = false;
    dots[i].visited = false;
    dots[i].deadend = false;
  }

  while (GameState == THREEPM)
  {
    mouse.Update();
    ButtonVector[10].Update(mouse);
    ButtonVector[12].Update(mouse);
    ClrButtonVector[2].Update(mouse);
    ClrButtonVector[3].Update(mouse);
    ClrButtonVector[4].Update(mouse);
    SDL_PollEvent(&event);
    if ( (event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT) )
      exit(0);

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
      GameState = Menu;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      ButtonVector[10].keytrick = false;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[10].keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[10].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP))
      GameState = SAP;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP) )
    {
      pa_back = 3;  
      GameState = GSM;
    }

    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN) )
      ButtonVector[12].keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[12].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
    {
      pa_back = 3;  
      GameState = GSM;
    }

    if ((!ClrButtonVector[2].IsSelected) && (!ClrButtonVector[3].IsSelected)
      && (!ClrButtonVector[4].IsSelected)  )
      FieldColor();

    if (ClrButtonVector[2].IsSelected)
    { 
      ClrButtonVector[2].ChangeColor();
      cur_color = ClrButtonVector[2].clr;
    }

    if (ClrButtonVector[3].IsSelected)
    { 
      ClrButtonVector[3].ChangeColor();
      cur_color = ClrButtonVector[3].clr;
    }

    if (ClrButtonVector[4].IsSelected)
    { 
      ClrButtonVector[4].ChangeColor();
      cur_color = ClrButtonVector[4].clr;
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

    ClrButtonVector[2].Draw();
    ClrButtonVector[3].Draw();
    ClrButtonVector[4].Draw();
    LabelVector[3].Draw();
    LabelVector[4].Draw();
    LabelVector[5].Draw();
    ButtonVector[10].Draw();
    ButtonVector[12].Draw();
    mouse.DrawCircle(cur_color);
    
    SDL_RenderPresent(renderer);
  }
}

void FourPlayerMenu()
{
  ButtonVector[10].keytrick = false;
  ButtonVector[12].keytrick = false;
  for (int i = 0; i < dots_menu_size; i++)
  {
    dots[i].exist = false;
    dots[i].visited = false;
    dots[i].deadend = false;
  }

  while (GameState == FOURPM)
  {
    mouse.Update();
    ButtonVector[10].Update(mouse);
    ButtonVector[12].Update(mouse);
    ClrButtonVector[5].Update(mouse);
    ClrButtonVector[6].Update(mouse);
    ClrButtonVector[7].Update(mouse);
    ClrButtonVector[8].Update(mouse);

    SDL_PollEvent(&event);
    if ( (event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT) )
      exit(0);

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
      GameState = Menu;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      ButtonVector[10].keytrick = false;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[10].keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[10].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP))
      GameState = SAP;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP) )
    {
      pa_back = 4;  
      GameState = GSM;
    }

    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN) )
      ButtonVector[12].keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[12].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
    {
      pa_back = 4;  
      GameState = GSM;
    }

    if ((!ClrButtonVector[5].IsSelected) && (!ClrButtonVector[6].IsSelected)
      && (!ClrButtonVector[7].IsSelected))
      FieldColor();

    if (ClrButtonVector[5].IsSelected)
    { 
      ClrButtonVector[5].ChangeColor();
      cur_color = ClrButtonVector[5].clr;
    }

    if (ClrButtonVector[6].IsSelected)
    { 
      ClrButtonVector[6].ChangeColor();
      cur_color = ClrButtonVector[6].clr;
    }

    if (ClrButtonVector[7].IsSelected)
    { 
      ClrButtonVector[7].ChangeColor();
      cur_color = ClrButtonVector[7].clr;
    }

    if (ClrButtonVector[8].IsSelected)
    { 
      ClrButtonVector[8].ChangeColor();
      cur_color = ClrButtonVector[8].clr;
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

    LabelVector[3].Draw();
    LabelVector[4].Draw();
    LabelVector[5].Draw();
    ButtonVector[10].Draw();
    ButtonVector[12].Draw();
    ClrButtonVector[5].Draw();
    ClrButtonVector[6].Draw();
    ClrButtonVector[7].Draw();
    ClrButtonVector[8].Draw();
    mouse.DrawCircle(cur_color);
    
    SDL_RenderPresent(renderer);
  }
}

void GameSizeMenu()
{
  SDL_Surface *surfaceText;
  SDL_Surface *surfaceText2;
  SDL_Surface *surfaceText3;
  SDL_Texture *textureText;
  SDL_Texture *textureText2;
  SDL_Texture *textureText3;
  ButtonVector[10].keytrick = false;
  ButtonVector[12].keytrick = false;

  SDL_Rect rectangle;
  SDL_Rect rectangle2;
  SDL_Rect rectangle3;
  rectangle_w = ScWidth / 32;
  rectangle_h = ScWidth / 18;

  rectangle.w = rectangle_w * 2;
  rectangle.h = rectangle_h * 2;
  rectangle.x = ScWidth * 1 / 3 - rectangle.w / 2;
  rectangle.y = vo * 3;

  rectangle2.w = rectangle_w * 2;
  rectangle2.h = rectangle_h * 2;
  rectangle2.x = ScWidth * 2 / 3 - rectangle2.w / 2;
  rectangle2.y = vo * 3;

  rectangle3.w = rectangle_w * 2;
  rectangle3.h = rectangle_h * 2;
  rectangle3.x = ScWidth * 1 / 2 - rectangle3.w / 2;
  rectangle3.y = vo * 3;

  while (GameState == GSM)
  {
    mouse.Update();
    ButtonVector[10].Update(mouse);
    ButtonVector[12].Update(mouse);

    SDL_PollEvent(&event);
    if ( (event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT) )
      exit(0);

    if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
      GameState = Menu;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP) )
      GameState = GRM;

    if ( (event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[12].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = GRM;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[10].keytrick = true;

    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN) )
      ButtonVector[12].keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
    {
      switch (pa_back)
      {
        case 1:
        {
          GameState = ONEPM;
          break;
        }  
        case 2:
        {
          GameState = TWOPM;
          break;
        } 
        case 3:
        {
          GameState = THREEPM;
          break;
        } 
        case 4:
        {
          GameState = FOURPM;
        } 
      }
    }

    if ( (event.button.button == SDL_BUTTON_LEFT) && (ButtonVector[10].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
   {
      switch (pa_back)
      {
        case 1:
        {
          GameState = ONEPM;
          break;
        }  
        case 2:
        {
          GameState = TWOPM;
          break;
        } 
        case 3:
        {
          GameState = THREEPM;
          break;
        } 
        case 4:
        {
          GameState = FOURPM;
        } 
      }
    }
    
    surfaceText3 = TTF_RenderText_Solid(my_Font, "x", {255, 255, 0});
    textureText3 = SDL_CreateTextureFromSurface(renderer, surfaceText3);

    string temp_str = to_string(game_x_size);
    field_x = (char*) temp_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, field_x, {180, 255, 100});
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);

    temp_str = to_string(game_y_size);
    field_y = (char*) temp_str.c_str();
    surfaceText2 = TTF_RenderText_Solid(my_Font, field_y, {180, 255, 100});
    textureText2 = SDL_CreateTextureFromSurface(renderer, surfaceText2);
    
    if ( (SDL_HasIntersection(&rectangle, &mouse.point)) &&
        (event.type == SDL_KEYDOWN) &&
        (event.key.keysym.sym == SDLK_w))
      game_x_size++;

    if ( (SDL_HasIntersection(&rectangle, &mouse.point)) &&
        (event.type == SDL_KEYDOWN) &&
        (event.key.keysym.sym == SDLK_UP))
      game_x_size += 10;

    if ( (SDL_HasIntersection(&rectangle, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      (event.key.keysym.sym == SDLK_s))
      game_x_size--;

    if ( (SDL_HasIntersection(&rectangle, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      (event.key.keysym.sym == SDLK_DOWN))
      game_x_size -= 10;

    if ( (SDL_HasIntersection(&rectangle2, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      ((event.key.keysym.sym == SDLK_w) || (event.key.keysym.sym == SDLK_UP)))
      game_y_size++;

    if ( (SDL_HasIntersection(&rectangle2, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      (event.key.keysym.sym == SDLK_UP))
      game_y_size += 10;

    if ( (SDL_HasIntersection(&rectangle2, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      (event.key.keysym.sym == SDLK_s))
      game_y_size--;

    if ( (SDL_HasIntersection(&rectangle2, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      (event.key.keysym.sym == SDLK_DOWN))
      game_y_size -= 10;

    if (game_y_size < 5)
      game_y_size = 5;

    if (game_y_size > 320)
      game_y_size = 320;

    if (game_x_size < 5)
      game_x_size = 5;

    if (game_x_size > 320)
      game_x_size = 320;

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
    SDL_DestroyTexture(textureText3);
    SDL_FreeSurface(surfaceText3);

    ButtonVector[10].Draw();
    ButtonVector[12].Draw();
    LabelVector[6].Draw();
    LabelVector[7].Draw();
    mouse.DrawCircle(cur_color);
    SDL_RenderPresent(renderer);
  }


}

void GameRuleMenu()
{
  SDL_Surface *surfaceText4;
  SDL_Surface *surfaceText5;
  SDL_Texture *textureText4;
  SDL_Texture *textureText5;
  SDL_Rect rectangle4;
  SDL_Rect rectangle5;
  rectangle4.w = LabelVector[9].drect.h;
  rectangle4.h = LabelVector[9].drect.h;
  rectangle4.x = ScWidth * 4 / 9 + LabelVector[9].drect.w;
  rectangle4.y = vo * 4;

  rectangle5.w = LabelVector[10].drect.h;
  rectangle5.h = LabelVector[10].drect.h;
  rectangle5.x = ScWidth * 4 / 9 + LabelVector[10].drect.w;
  rectangle5.y = vo * 5;

  ButtonVector[11].keytrick = false;
  ButtonVector[13].keytrick = false;
  ButtonVector[14].keytrick = false;

  while (GameState == GRM)
  {
    mouse.Update();
    ButtonVector[11].Update(mouse);
    ButtonVector[13].Update(mouse);
    ButtonVector[14].Update(mouse); 

    SDL_PollEvent(&event);
    if ( (event.type == SDL_KEYDOWN) && (event.type == SDL_QUIT) )
      exit(0);

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )
      GameState = GSM;

    if( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYDOWN) )
      ButtonVector[11].keytrick = true;

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[11].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = GSM;

    if ((event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
      GameState = Menu;

    if ((event.key.keysym.sym == SDLK_t) && (event.type == SDL_KEYUP) )
    { 
      classic = false; 
      GameState = Game;
    }

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[14].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
    { 
      classic = false; 
      GameState = Game;
    }

    if ( (event.key.keysym.sym == SDLK_t) && (event.type == SDL_KEYDOWN) )
      ButtonVector[14].keytrick = true;

    if ((event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYUP) )
      GameState = Game;

    if ( (event.button.button == SDL_BUTTON_LEFT)
      && (ButtonVector[13].IsSelected)
      && (event.type == SDL_MOUSEBUTTONUP)) 
      GameState = Game;

    if ( (event.key.keysym.sym == SDLK_c) && (event.type == SDL_KEYDOWN) )
      ButtonVector[13].keytrick = true;

    if ( (event.key.keysym.sym == SDLK_b) && (event.type == SDL_KEYUP) )  
      GameState = GSM;

    string temp_str;
    if (maxdotamount <= 0)
      temp_str = "~";
    else 
      temp_str = to_string(maxdotamount);
    field_x = (char*) temp_str.c_str();
    surfaceText4 = TTF_RenderText_Solid(my_Font, field_x, {180, 255, 100});
    textureText4 = SDL_CreateTextureFromSurface(renderer, surfaceText4);

    if (maxeatendots <= 0)
      temp_str = "~";
    else 
      temp_str = to_string(maxeatendots);
    field_y = (char*) temp_str.c_str();
    surfaceText5 = TTF_RenderText_Solid(my_Font, field_y, {180, 255, 100});
    textureText5 = SDL_CreateTextureFromSurface(renderer, surfaceText5);
    
    if ( (SDL_HasIntersection(&rectangle4, &mouse.point)) &&
        (event.type == SDL_KEYDOWN) &&
        ((event.key.keysym.sym == SDLK_w) || (event.key.keysym.sym == SDLK_UP)))
      maxdotamount++;

    if ( (SDL_HasIntersection(&rectangle4, &mouse.point)) &&
        (event.type == SDL_KEYDOWN) &&
        (event.key.keysym.sym == SDLK_UP))
      maxdotamount += 10;

    if ( (SDL_HasIntersection(&rectangle4, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      (event.key.keysym.sym == SDLK_s))
      maxdotamount--;

    if ( (SDL_HasIntersection(&rectangle4, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      (event.key.keysym.sym == SDLK_DOWN))
      maxdotamount -= 10 ;

    if ( (SDL_HasIntersection(&rectangle5, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      (event.key.keysym.sym == SDLK_w))
      maxeatendots++;

    if ( (SDL_HasIntersection(&rectangle5, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      (event.key.keysym.sym == SDLK_UP))
      maxeatendots += 10;

    if ( (SDL_HasIntersection(&rectangle5, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      (event.key.keysym.sym == SDLK_s))
      maxeatendots--;

    if ( (SDL_HasIntersection(&rectangle5, &mouse.point))
      && (event.type == SDL_KEYDOWN) &&
      (event.key.keysym.sym == SDLK_DOWN))
      maxeatendots -= 10;


    if (maxdotamount < 0)
      maxdotamount = 0;

    if (maxdotamount > 5000)
      maxdotamount = 5000;

    if (maxeatendots < 0)
      maxeatendots = 0;

    if (maxeatendots > 700)
      maxeatendots = 700;

    if (maxdotamount >= 100)
      rectangle4.w = rectangle_w * 3;
    else if (maxdotamount < 10)
      rectangle4.w = rectangle_w;
    else
      rectangle4.w = rectangle_w * 2;

    if (maxeatendots >= 100) 
      rectangle5.w = rectangle_w * 3;
    else if (maxeatendots < 10)
      rectangle5.w = rectangle_w;
    else
      rectangle5.w = rectangle_w * 2;

    SDL_GetMouseState(&xMouse,&yMouse);
    SDL_SetRenderDrawColor(renderer, red, green, blue, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
    DrawField(game_x_size, game_y_size);
    if (!SDL_HasIntersection(&rectangle4, &mouse.point) && 
      !SDL_HasIntersection(&rectangle5, &mouse.point))
      MoveBoard(game_x_size, game_y_size);
    Zoom();
    EnumerateField(game_y_size, game_x_size, my_Font);

    SDL_RenderCopy(renderer, textureText4, NULL, &rectangle4);
    SDL_RenderCopy(renderer, textureText5, NULL, &rectangle5);

    SDL_DestroyTexture(textureText4);
    SDL_FreeSurface(surfaceText4);
    SDL_DestroyTexture(textureText5);
    SDL_FreeSurface(surfaceText5);

    ButtonVector[11].Draw();
    ButtonVector[13].Draw();
    ButtonVector[14].Draw();
    LabelVector[7].Draw();
    LabelVector[8].Draw();
    LabelVector[9].Draw();
    LabelVector[10].Draw();
    mouse.DrawCircle(cur_color);
    SDL_RenderPresent(renderer);
  }
}

void GameItself()
{
  int dots_game_size = (game_y_size + 2) * (game_x_size + 2);
  Dots *dots_g = new Dots[dots_game_size];
  for (int i = 0; i < dots_game_size; i++)
  {
    dots_g[i].exist = false;
    dots_g[i].visited = 0;
    dots_g[i].deadend = false;
  }
  x_c = 0;
  y_c = 0;

  switch (pa_back)
  {
    case 2:
      cur_color = ClrButtonVector[0].clr;
      break;
    case 3:
      cur_color = ClrButtonVector[2].clr;
      break;
    case 4:
      cur_color = ClrButtonVector[5].clr;
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
          if (cur_color == ClrButtonVector[0].clr)
            cur_color = ClrButtonVector[1].clr;
          else
            cur_color = ClrButtonVector[0].clr;
          break;
        }
        case 3:
        {
          if (cur_color == ClrButtonVector[2].clr)
            cur_color = ClrButtonVector[3].clr;
          else if (cur_color == ClrButtonVector[3].clr)
            cur_color = ClrButtonVector[4].clr;
          else
            cur_color = ClrButtonVector[2].clr;
          break;
        }
        case 4:
        {
          if (cur_color == ClrButtonVector[5].clr)
            cur_color = ClrButtonVector[6].clr;
          else if (cur_color == ClrButtonVector[6].clr)
            cur_color = ClrButtonVector[7].clr;
          else if (cur_color == ClrButtonVector[7].clr)
            cur_color = ClrButtonVector[8].clr;
          else
            cur_color = ClrButtonVector[5].clr;
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
  delete dots_g;
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

/////////////////////////////////////Some variables/////////////////////////////
  cur_color = white;
  mouse.GiveColor(white);
  mouse.circle_size = 9;
  vo = ScHeight / 8;  //vertical offset
  ho = ScWidth / 5;  //horizontal offset

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

//////////////////////////////Initializing SDL_TTF//////////////////////////////
  if (TTF_Init() == -1)
    cout << "Could not initialize SDL2_ttf, error:" << TTF_GetError() << endl;
  else
    cout << "SDL2_ttf system ready to go!";

  my_Font = TTF_OpenFont("DroidSans.ttf", 32);
  if (my_Font == nullptr)
  {
    cout << "could not load font" << endl;
    exit(1);
  }

/////////////////All buttons and labels are here////////////////////////////////  
  const char* New = "images/new.png";
  Button NewButton(New, ho, vo, 100, 400);
  NewButton.srect.y = 0;
  NewButton.drect.x = ScWidth / 2 - NewButton.drect.w / 2; 
  NewButton.drect.y = vo;
  ButtonVector.push_back(NewButton);  //0 - index
  NewButton.~Button(); 

  const char* load = "images/load.png";
  Button LoadButton(load, ho, vo, 100, 400);
  LoadButton.srect.y = 0;
  LoadButton.drect.x = ScWidth / 2 - LoadButton.drect.w / 2; 
  LoadButton.drect.y = vo * 2;
  ButtonVector.push_back(LoadButton);  //1 - index
  LoadButton.~Button(); 

  const char* helpB = "images/help.png";
  Button HelpButton(helpB, ho, vo, 100, 400);
  HelpButton.srect.y = 0;
  HelpButton.drect.x = ScWidth / 2 - HelpButton.drect.w / 2; 
  HelpButton.drect.y = vo * 3;
  ButtonVector.push_back(HelpButton);  //2 - index
  HelpButton.~Button(); 

  const char* credits = "images/credits.png";
  Button CreditsButton(credits, ho, vo, 100, 400);
  CreditsButton.srect.y = 0;
  CreditsButton.drect.x = ScWidth / 2 - CreditsButton.drect.w / 2; 
  CreditsButton.drect.y = vo * 4;
  ButtonVector.push_back(CreditsButton);  //3 - index
  CreditsButton.~Button(); 

  const char* exitB = "images/exit.png";
  Button ExitButton(exitB, ho, vo, 100, 400);
  ExitButton.srect.y = 0;
  ExitButton.drect.x = ScWidth / 2 - ExitButton.drect.w / 2; 
  ExitButton.drect.y = vo * 5;
  ButtonVector.push_back(ExitButton);  //4 - index
  ExitButton.~Button(); 

  const char* oneB = "images/one.png";
  Button OneButton(oneB, ho, vo, 100, 200);
  OneButton.srect.y = 0;
  OneButton.drect.x = ScWidth / 2 - OneButton.drect.w / 2; 
  OneButton.drect.y = vo * 2;
  ButtonVector.push_back(OneButton);  //5 - index
  OneButton.~Button(); 

  const char* twoB = "images/two.png";
  Button TwoButton(twoB, ho, vo, 100, 400);
  TwoButton.srect.y = 0;
  TwoButton.drect.x = ScWidth / 2 - TwoButton.drect.w / 2; 
  TwoButton.drect.y = vo * 3;
  ButtonVector.push_back(TwoButton);  //6 - index
  TwoButton.~Button(); 

  const char* threeB = "images/three.png";
  Button ThreeButton(threeB, ho, vo, 100, 400);
  ThreeButton.srect.y = 0;
  ThreeButton.drect.x = ScWidth / 2 - ThreeButton.drect.w / 2; 
  ThreeButton.drect.y = vo * 4;
  ButtonVector.push_back(ThreeButton);  //7 - index
  ThreeButton.~Button(); 

  const char* fourB = "images/four.png";
  Button FourButton(fourB, ho, vo, 100, 400);
  FourButton.srect.y = 0;
  FourButton.drect.x = ScWidth / 2 - FourButton.drect.w / 2; 
  FourButton.drect.y = vo * 5;
  ButtonVector.push_back(FourButton);  //8 - index
  FourButton.~Button(); 

  const char* backB = "images/back.png";
  Button BackButton(backB, ho, vo, 100, 400);
  BackButton.srect.y = 0;
  BackButton.drect.x = ScWidth / 2 - BackButton.drect.w / 2; 
  BackButton.drect.y = vo * 6;
  ButtonVector.push_back(BackButton);  //9 - index
  BackButton.~Button(); 

  Button Back2Button(backB, ho, vo, 100, 400);
  Back2Button.srect.y = 0;
  Back2Button.drect.x = ScWidth / 2 - Back2Button.drect.w / 2; 
  Back2Button.drect.y = vo * 6;
  ButtonVector.push_back(Back2Button);  //10 - index
  Back2Button.~Button(); 

  Button Back3Button(backB, ho, vo, 100, 400);
  Back3Button.srect.y = 0;
  Back3Button.drect.x = ScWidth / 2 - Back3Button.drect.w / 2; 
  Back3Button.drect.y = vo * 7;
  ButtonVector.push_back(Back3Button);  //11 - index
  Back3Button.~Button(); 

  const char* continueB = "images/continue.png";
  Button ContinueButton(continueB, ho, vo, 100, 400);
  ContinueButton.srect.y = 0;
  ContinueButton.drect.x = ScWidth / 2 - ContinueButton.drect.w / 2; 
  ContinueButton.drect.y = vo * 5;
  ButtonVector.push_back(ContinueButton);  //12 - index
  ContinueButton.~Button(); 

  const char* dcw = "images/classic.png";
  Button ClassicButton(dcw, ho, vo, 100, 400);
  ClassicButton.srect.y = 0;
  ClassicButton.drect.x = ScWidth / 2 - ClassicButton.drect.w / 2; 
  ClassicButton.drect.y = vo * 2;
  ButtonVector.push_back(ClassicButton);  //13 - index
  ClassicButton.~Button(); 

  const char* terB = "images/territory.png";
  Button TerritoryButton(terB, ho, vo, 100, 400);
  TerritoryButton.srect.y = 0;
  TerritoryButton.drect.x = ScWidth / 2 - TerritoryButton.drect.w / 2; 
  TerritoryButton.drect.y = vo * 3; 
  ButtonVector.push_back(TerritoryButton);  //14 - index
  TerritoryButton.~Button();  

  const char* paL = "images/pa.png";
  Label PaLabel(paL, ho * 1.5, vo, 100, 600);
  PaLabel.srect.y = 0;
  PaLabel.drect.x = ScWidth / 2 - PaLabel.drect.w / 2; 
  PaLabel.drect.y = vo;
  LabelVector.push_back(PaLabel); //0 - index
  PaLabel.~Label();

  const char* GvsAIL = "images/gvai.png";
  Label GvsAILabel(GvsAIL, ho * 1.5, vo, 100, 600);
  GvsAILabel.srect.y = 0;
  GvsAILabel.drect.x = ScWidth / 2 - GvsAILabel.drect.w / 2; 
  GvsAILabel.drect.y = vo;
  LabelVector.push_back(GvsAILabel); //1 - index
  GvsAILabel.~Label();

  const char* udL = "images/ud.png";
  Label Udlabel(udL, ho * 1.5, vo, 100, 600);
  Udlabel.srect.y = 0;
  Udlabel.drect.x = ScWidth / 2 - Udlabel.drect.w / 2; 
  Udlabel.drect.y = vo * 2;
  LabelVector.push_back(Udlabel); //2 - index
  Udlabel.~Label();

  const char* sgL = "images/sg.png";
  Label SgLabel(sgL, ho * 1.5, vo, 100, 600);
  SgLabel.srect.y = 0;
  SgLabel.drect.x = ScWidth / 2 - SgLabel.drect.w / 2; 
  SgLabel.drect.y = vo;
  LabelVector.push_back(SgLabel); //3 - index
  SgLabel.~Label();

  const char* colorsL = "images/colors.png";
  Label ColorsLabel(colorsL, ho * 1.5, vo, 100, 600);
  ColorsLabel.srect.y = 0;
  ColorsLabel.drect.x = ScWidth / 2 - ColorsLabel.drect.w / 2; 
  ColorsLabel.drect.y = vo * 2;
  LabelVector.push_back(ColorsLabel); //4 - index
  ColorsLabel.~Label();

  const char* rgbL = "images/rgb.png";
  Label RgbLabel(rgbL, ho * 1.5, vo, 100, 600);
  RgbLabel.srect.y = 0;
  RgbLabel.drect.x = ScWidth / 2 - RgbLabel.drect.w / 2; 
  RgbLabel.drect.y = vo * 3;
  LabelVector.push_back(RgbLabel); //5 - index
  RgbLabel.~Label();

  const char* sgsL = "images/sgs.png";
  Label SgsLabel(sgsL, ho * 1.5, vo, 100, 600);
  SgsLabel.srect.y = 0;
  SgsLabel.drect.x = ScWidth / 2 - SgsLabel.drect.w / 2; 
  SgsLabel.drect.y = 0; 
  LabelVector.push_back(SgsLabel); //6 - index
  SgsLabel.~Label();

  const char* useL = "images/use.png";
  Label UseLabel(useL, ho * 1.5, vo, 100, 600);
  UseLabel.srect.y = 0;
  UseLabel.drect.x = ScWidth / 2 - UseLabel.drect.w / 2; 
  UseLabel.drect.y = vo;
  LabelVector.push_back(UseLabel); //7 - index
  UseLabel.~Label();

  const char* sgrL = "images/sgr.png";
  Label SgrLabel(sgrL, ho * 1.5, vo, 100, 600);
  SgrLabel.srect.y = 0;
  SgrLabel.drect.x = ScWidth / 2 - SgrLabel.drect.w / 2; 
  SgrLabel.drect.y = 0;
  LabelVector.push_back(SgrLabel); //8 - index
  SgrLabel.~Label();

  const char* mdaL = "images/mda.png";
  Label MaxdotamountLabel(mdaL, ho * 1.5, vo, 100, 800);
  MaxdotamountLabel.srect.y = 0;
  MaxdotamountLabel.drect.x = ScWidth * 4 / 9 - MaxdotamountLabel.drect.w / 2; 
  MaxdotamountLabel.drect.y = vo * 4;
  LabelVector.push_back(MaxdotamountLabel); //9 - index
  MaxdotamountLabel.~Label();

  const char* medL = "images/med.png";
  Label MaxeatendotsLabel(medL, ho * 1.5, vo, 100, 800);
  MaxeatendotsLabel.srect.y = 0;
  MaxeatendotsLabel.drect.x = ScWidth * 4 / 9 - MaxeatendotsLabel.drect.w / 2; 
  MaxeatendotsLabel.drect.y = vo * 5;
  LabelVector.push_back(MaxeatendotsLabel); //10 - index
  MaxeatendotsLabel.~Label();

  ColorButton P21(25, 170, 100);
  P21.rect.x = ScWidth / 3 - P21.rect.w / 2; 
  P21.rect.y = vo * 4;
  ClrButtonVector.push_back(P21); //0 - index
  P21.~ColorButton();

  ColorButton P22(100, 0, 150);
  P22.rect.x = ScWidth * 2 / 3 - P22.rect.w / 2; 
  P22.rect.y = vo * 4;
  ClrButtonVector.push_back(P22); //1 - index
  P22.~ColorButton();

  ColorButton P31(255, 0, 0);
  P31.rect.x = ScWidth / 4 - P31.rect.w / 2; 
  P31.rect.y = vo * 4;
  ClrButtonVector.push_back(P31); //2 - index
  P31.~ColorButton();
  
  ColorButton P32(0, 255, 0);
  P32.rect.x = ScWidth * 2 / 4 - P32.rect.w / 2; 
  P32.rect.y = vo * 4;
  ClrButtonVector.push_back(P32); //3 - index
  P32.~ColorButton();

  ColorButton P33(0, 0, 255);
  P33.rect.x = ScWidth * 3 / 4 - P33.rect.w / 2; 
  P33.rect.y = vo * 4;
  ClrButtonVector.push_back(P33); //4 - index
  P33.~ColorButton();

  ColorButton P41(0, 255, 10);
  P41.rect.x = ScWidth * 1 / 5 - P41.rect.w / 2; 
  P41.rect.y = vo * 4;
  ClrButtonVector.push_back(P41); //5 - index
  P41.~ColorButton();

  ColorButton P42(255, 10, 0);
  P42.rect.x = ScWidth * 2 / 5 - P42.rect.w / 2; 
  P42.rect.y = vo * 4;
  ClrButtonVector.push_back(P42); //6 - index
  P42.~ColorButton();

  ColorButton P43(0, 10, 255);
  P43.rect.x = ScWidth * 3 / 5 - P43.rect.w / 2; 
  P43.rect.y = vo * 4;
  ClrButtonVector.push_back(P43); //7 - index
  P43.~ColorButton();

  ColorButton P44(240, 240, 0);
  P44.rect.x = ScWidth * 4 / 5 - P44.rect.w / 2; 
  P44.rect.y = vo * 4;
  ClrButtonVector.push_back(P44); //8 - index
  P44.~ColorButton();

/////////////////////////////////Main cycle/////////////////////////////////////
  GameState = Menu;

  while(1)
  {
    switch(GameState)
    {
      case Menu:
        MainMenu();
        break;
      case SAP:
        SelectAmountOfPlayersMenu();
        break;
      case Credits:
        CreditsMenu();
        break;
      case ONEPM:
        OnePlayerMenu();
        break;
      case TWOPM:
        TwoPlayerMenu();
        break;
      case THREEPM:
        ThreePlayerMenu();
        break;
      case FOURPM:
        FourPlayerMenu();
        break;
      case GSM:
        GameSizeMenu();
        break;
      case GRM:
        GameRuleMenu();
        break;
      case Game:
        GameItself();  
    }
  }
//////////////////////////Exiting program///////////////////////////////////////
  TTF_CloseFont(my_Font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 666;
}