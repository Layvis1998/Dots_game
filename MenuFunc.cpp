#include <iostream>
#include <err.h>
#include <string>
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

#include "headers/Globcnst.h"
#include "headers/Globvars.h"
#include "headers/OverloadedOperators.h"

using std::cout;
using std::endl;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::string;
using std::list;
using std::to_string;
using std::max;

int Min (unordered_set <int> uset)
{
  int min = INT_MAX;
  for (auto i = uset.begin(); i != uset.end(); i++ )
  {
    if (min > *i)
      min = *i;
  }
  return min;
}

void FindConnectedDots (Dots* dots, int current, unordered_set <int> &uset,
  int fx, int fy)
{
  SDL_Color C = dots[current].clr;
  dots[current].visited = true;
  uset.insert(current);
  
  //up-right direction
  if (((current + 1) % fx != 0) && (current - fx >= 0)
    && (dots[current + 1 - fx].exist == true)
    && (dots[current + 1 - fx].clr == C)
    && (dots[current + 1 - fx].visited == false))
  {
    FindConnectedDots(dots, current + 1 - fx, uset, fx, fy);
  }

  // right direction
  if (((current + 1) % fx != 0)
    && (dots[current + 1].exist == true)
    && (dots[current + 1].clr == C)
    && (dots[current + 1].visited == false))
  {                  
    FindConnectedDots(dots, current + 1, uset, fx, fy);
  }

  //down-right direction  
  if (((current + 1) % fx != 0) && (current + fx < fx * fy)
    && (dots[current + 1 + fx].exist == true)
    && (dots[current + 1 + fx].clr == C)
    && (dots[current + 1 + fx].visited == false))   
  {
    FindConnectedDots(dots, current + 1 + fx, uset, fx, fy);  
  }

  // down direction
  if  ((current + fx < fx * fy)
    && (dots[current + fx].exist == true)
    && (dots[current + fx].clr == C)
    && (dots[current + fx].visited == false))
  { 
    FindConnectedDots(dots, current + fx, uset, fx, fy);
  }

  //down-left direction
  if  ((current % fx != 0) && (current + fx < fx * fy)
    && (dots[current - 1 + fx].exist == true)
    && (dots[current - 1 + fx].clr == C)
    && (dots[current - 1 + fx].visited == false))
  { 
    FindConnectedDots(dots, current - 1 + fx, uset, fx, fy); 
  }

  // left direction
  if  ((current % fx != 0)
    && (dots[current - 1].exist == true)
    && (dots[current - 1].clr == C)
    && (dots[current - 1].visited == false))
  {  
    FindConnectedDots(dots, current - 1, uset, fx, fy);  
  }

  //up-left direction 
  if  ((current % fx != 0) && (current - fx >= 0)
    && (dots[current - 1 - fx].exist == true)
    && (dots[current - 1 - fx].clr == C)
    && (dots[current - 1 - fx].visited == false))
  {
    FindConnectedDots(dots, current - 1 - fx, uset, fx, fy); 
  }

  // up diterction
  if  ((current - fx >= 0)
    && (dots[current - fx].exist == true)
    && (dots[current - fx].clr == C)
    && (dots[current - fx].visited == false))                                        
  { 
    FindConnectedDots(dots, current - fx, uset, fx, fy);
  }
}

void ExtractCycle (Dots* dots, int current, unordered_set <int> &uset, 
  int fx, int fy)
{
  SDL_Color C = dots[current].clr;
  dots[current].cycle += 1;

  cout << current << " " << endl;
  uset.insert(current);

  //up-right direction
  if (((current + 1) % fx != 0)
    && (current - fx >= 0 )
    && (dots[current + 1 - fx].exist == true)
    && (dots[current + 1 - fx].clr == C) 
    && (dots[current + 1 - fx].cycle == false))
  {
    ExtractCycle(dots, current + 1 - fx, uset, fx, fy);
  }
  // right direction
  else if (((current + 1) % fx != 0)
    && (dots[current + 1].exist == true)
    && (dots[current + 1].clr == C)
    && (dots[current + 1].cycle == false))
  {                  
    ExtractCycle(dots, current + 1, uset, fx, fy);
  }
  //down-right direction  
  else if (((current + 1) % fx != 0)
    && ( current + fx < fx * fy )
    && (dots[current + 1 + fx].exist == true)
    && (dots[current + 1 + fx].clr == C)
    && (dots[current + 1 + fx].cycle == false))   
  {
    ExtractCycle(dots, current + 1 + fx, uset, fx, fy);  
  }
  // down direction
  else if ((current + fx < fx * fy)
    && (dots[current + fx].exist == true)
    && (dots[current + fx].clr == C)
    && (dots[current + fx].cycle == false))
  { 
    ExtractCycle(dots, current + fx, uset, fx, fy);
  }
  //down-left direction
  else if ((current % fx != 0)
    && (current + fx < fx * fy)
    && (dots[current - 1 + fx].exist == true)
    && (dots[current - 1 + fx].clr == C)
    && (dots[current - 1 + fx].cycle == false))
  { 
    ExtractCycle(dots, current - 1 + fx, uset, fx, fy); 
  }
  // left direction
  else if (( current % fx != 0)
    && (dots[current - 1].exist == true)
    && (dots[current - 1].clr == C)
    && (dots[current - 1].cycle == false))
  {  
    ExtractCycle(dots, current - 1, uset, fx, fy);  
  }
  //up-left direction 
  else if ((current % fx != 0)
    && (current - fx >= 0)
    && (dots[current - 1 - fx].exist == true)
    && (dots[current - 1 - fx].clr == C)
    && (dots[current - 1 - fx].cycle == false))
  {
    ExtractCycle(dots, current - 1 - fx, uset, fx, fy); 
  }
  // up diterction
  else if ((current - fx >= 0)
    && (dots[current - fx].exist == true)
    && (dots[current - fx].clr == C)
    && (dots[current - fx].cycle == false))                                        
  { 
    ExtractCycle(dots, current - fx, uset, fx, fy);
  }
}

int DifferentWaysUset
  (Dots* dots, unordered_set <int> uset, int current, int fx , int fy)
{
  int retval = 0;

  if (uset.count(current + 1))
    retval++;

  if (uset.count(current + 1 + fx) )
    retval++;

  if (uset.count(current + fx))
    retval++;

  if (uset.count(current - 1 + fx))
    retval++;

  if ( uset.count(current - 1))
    retval++;

  if ( uset.count(current - 1 - fx) )
    retval++;

  if ( uset.count(current -  fx))  
    retval++;

  if ( uset.count(current + 1 - fx))
    retval++;

  return retval;
}

void DeleteBranchesUset
 (Dots* dots, unordered_set <int> &uset, int fx, int fy)
{
  bool branchdeleted = true;

  while (branchdeleted == true)
  {
    branchdeleted = false;
    for (auto i = uset.begin(); i != uset.end(); )
    {
      if (DifferentWaysUset(dots, uset, *i, fx, fy) <= 1)
      { 
        branchdeleted = true; 
        i = uset.erase(i);
      }
      else
        i++;
    }
  }
}

bool CheckForColor(unordered_set <int> CycleSet, Dots* dots, int fx)
{
  bool Check = false;
  vector <int> CycleVector;
  for (auto i = CycleSet.begin(); i != CycleSet.end(); i++)
    CycleVector.push_back(*i);

  sort(CycleVector.begin(), CycleVector.end());

  int i = 0;
  while (i < CycleVector.size())
  {
    vector <int> SameLine;
    SameLine.push_back(CycleVector[i]);

    i++;
    while ((i < CycleVector.size()) &&
          (CycleVector[i] / fx) == (SameLine[0] / fx))
    {
      SameLine.push_back(CycleVector[i]);
      i++;
    }
    
    for (int k = SameLine.front(); k < SameLine.back(); k++)
    {
      if ((dots[SameLine.back()].clr != dots[k].clr) && (dots[k].exist == true))
      {
        Check = true;
        return Check;
      }
    }
  }

  return Check;
}

ColorSpace CreateColorSpace(unordered_set <int> CycleSet, Dots* dots, int fx)
{
  ColorSpace retval;

  vector <int> CycleVector;
  for (auto i = CycleSet.begin(); i != CycleSet.end(); i++)
    CycleVector.push_back(*i);

  sort(CycleVector.begin(), CycleVector.end());

  int i = 0;
  while (i < CycleVector.size())
  {
    vector <int> SameLine;
    SameLine.push_back(CycleVector[i]);

    i++;
    while ((i < CycleVector.size()) &&
          (CycleVector[i] / fx) == (SameLine[0] / fx))
    {
      SameLine.push_back(CycleVector[i]);
      i++;
    }
    retval.BorderDots.insert(SameLine.back());
    retval.BorderDots.insert(SameLine.front());

    for (int k = SameLine.front() + 1; k < SameLine.back() - 1; k++)
    {
      retval.InnerDots.insert(k);
    }
  }

  return retval;
}

void ColorColorSpace (unordered_set <int> CycleSet, Dots* dots, int fx)
{
  vector <int> CycleVector;
  for (auto i = CycleSet.begin(); i != CycleSet.end(); i++)
    CycleVector.push_back(*i);
  sort(CycleVector.begin(), CycleVector.end());
  
  int start = CycleVector[0];
  int copystart = start;
  SDL_SetRenderDrawColor(renderer, dots[start].clr.r,
    (dots[start].clr.g), dots[start].clr.b, SPACE_OPACITY);

  unordered_set <int> contoured;

  while (contoured.size() < CycleSet.size())
  {  
    if (CycleSet.count(start + 1) && !contoured.count(start + 1))
    {  
      contoured.insert(start + 1);
      SDL_RenderDrawLine(renderer, (start % fx + 2) * intrv - X_offset - 1,
        (start / fx + 2) * intrv - Y_offset - 1, ((start + 1) % fx + 2)
        * intrv - X_offset - 1, ((start + 1) / fx + 2) * intrv - Y_offset - 1);

      start = start + 1;
    }
    else if (CycleSet.count(start + 1 + fx) && !contoured.count(start + 1 + fx))
    {  
      contoured.insert(start + 1 + fx);
      SDL_RenderDrawLine(renderer, (start % fx + 2) * intrv - X_offset - 1,
        (start / fx + 2) * intrv - Y_offset - 1, ((start + 1 + fx) % fx + 2)
        * intrv - X_offset - 1, ((start + 1 + fx) / fx + 2) * intrv - Y_offset - 1);
      start = start + 1 + fx;
    }
    else if (CycleSet.count(start + fx) && !contoured.count(start + fx))
    {  
      contoured.insert(start + fx);
      SDL_RenderDrawLine(renderer, (start % fx + 2) * intrv - X_offset - 1,
        (start / fx + 2) * intrv - Y_offset - 1, ((start + fx) % fx + 2) * intrv
        - X_offset - 1, ((start + fx) / fx + 2) * intrv - Y_offset - 1);
      start = start + fx;
    }
    else if (CycleSet.count(start + fx - 1) && !contoured.count(start + fx - 1))
    {  
      contoured.insert(start + fx - 1);
      SDL_RenderDrawLine(renderer, (start % fx + 2) * intrv - X_offset - 1,
        (start / fx + 2) * intrv - Y_offset - 1, ((start + fx - 1) % fx + 2) * intrv
         - X_offset - 1, ((start + fx - 1) / fx + 2) * intrv - Y_offset - 1);
      start = start + fx - 1;
    }
    else if (CycleSet.count(start - 1) && !contoured.count(start - 1))
    {  
      contoured.insert(start - 1);
      SDL_RenderDrawLine(renderer, (start % fx + 2) * intrv - X_offset - 1,
        (start / fx + 2) * intrv - Y_offset - 1, ((start - 1) % fx + 2) * intrv
         - X_offset - 1, ((start - 1) / fx + 2) * intrv - Y_offset - 1);
      start = start - 1;
    }
    else if (CycleSet.count(start - fx - 1) && !contoured.count(start - fx - 1))
    {  
      contoured.insert(start - fx - 1);
      SDL_RenderDrawLine(renderer, (start % fx + 2) * intrv - X_offset - 1,
        (start / fx + 2) * intrv - Y_offset - 1, ((start - fx - 1) % fx + 2) * intrv
         - X_offset - 1, ((start - fx - 1) / fx + 2) * intrv - Y_offset - 1);
      start = start - fx - 1;
    }
    else if (CycleSet.count(start - fx) && !contoured.count(start - fx))
    {  
      contoured.insert(start - fx);
      SDL_RenderDrawLine(renderer, (start % fx + 2) * intrv - X_offset - 1,
        (start / fx + 2) * intrv - Y_offset - 1, ((start - fx) % fx + 2) * intrv
         - X_offset - 1, ((start - fx) / fx + 2) * intrv - Y_offset - 1);
      start = start - fx;
    }
    else if (CycleSet.count(start + 1 - fx) && !contoured.count(start + 1 - fx))
    {  
      contoured.insert(start + 1 - fx);
      SDL_RenderDrawLine(renderer, (start % fx + 2) * intrv - X_offset - 1,
        (start / fx + 2) * intrv - Y_offset - 1, ((start + 1 - fx) % fx + 2) * intrv
         - X_offset - 1, ((start + 1 - fx) / fx + 2) * intrv - Y_offset);
      start = start + 1 - fx;
    }
  }
}

list <ColorSpace> ProcessDotInteraction (Dots* dots, int fx, int fy)
{
  list <ColorSpace> result;
  int current = 0;
  for (int i = 0; i < fx * fy; i++)
  {
    dots[i].visited = false;
    dots[i].deadend = false;
    dots[i].cycle = false;
  }

  while (1)
  {
    while ((current < fx * fy) &&
      (dots[current].exist == false) || (dots[current].visited == true))
      current++;
    
    if (current == fx * fy)
      return result;
  
    unordered_set <int> ConnectedCycles;
    FindConnectedDots(dots, current, ConnectedCycles, fx, fy);
    DeleteBranchesUset(dots, ConnectedCycles, fx, fy);
    //cout << "Connected cycles size = "  << ConnectedCycles.size()  << endl;

    while ((ConnectedCycles.size() != 0))
    { 
      for (auto i = ConnectedCycles.begin(); i != ConnectedCycles.end(); i++)
        dots[*i].cycle = false;

      unordered_set <int> Cycle;
      int start = Min(ConnectedCycles);
      ExtractCycle(dots, start, Cycle, fx, fy);
      cout << "Cycle size = " << Cycle.size() << endl;
      start = Min(Cycle);
      DeleteBranchesUset(dots, Cycle, fx, fy);
      cout << "Cycle size = " << Cycle.size() << endl;

      if (CheckForColor(Cycle, dots, fx))
      {
        ColorSpace CS = CreateColorSpace(Cycle, dots, fx);
        result.push_back(CS);  
      }
      
      DeleteBranchesUset(dots, ConnectedCycles, fx, fy);
      ConnectedCycles -= Cycle;
      cout << "Connected cycles size = " << ConnectedCycles.size() << endl;
    }
  }
  return result;
}

void MoveBoard(int fx, int fy , int speed)
{
  int CurScWidth;
  int CurScHeight;
  SDL_GetWindowSize(window, &CurScWidth, &CurScHeight);

  if (xMouse <= minintrv / 2.0)
    X_offset -= speed;
  if (xMouse >= CurScWidth - minintrv / 2.0)
    X_offset += speed;
  if (yMouse <= minintrv / 2.0)
    Y_offset -= speed; 
  if (yMouse >= CurScHeight - minintrv / 2.0)
    Y_offset += speed;

  if (event.key.keysym.sym == SDLK_s)
    Y_offset += speed;
  if (event.key.keysym.sym == SDLK_w)
    Y_offset -= speed;
  if (event.key.keysym.sym == SDLK_d)
    X_offset += speed;
  if (event.key.keysym.sym == SDLK_a)
    X_offset -= speed;
  if (event.key.keysym.sym == SDLK_DOWN)
    Y_offset += speed;
  if (event.key.keysym.sym == SDLK_UP)
    Y_offset -= speed;
  if (event.key.keysym.sym == SDLK_LEFT)
    X_offset -= speed;
  if (event.key.keysym.sym == SDLK_RIGHT)
    X_offset += speed;

  if (X_offset >= intrv * fx - CurScWidth + intrv * 4)
    X_offset = intrv * fx - CurScWidth + intrv * 4;
  if (Y_offset >= intrv * fy - CurScHeight + intrv * 4) 
    Y_offset = intrv * fy - CurScHeight + intrv * 4;

  if (X_offset <= 0)
    X_offset = 0;
  if (Y_offset <= 0)
    Y_offset = 0;
}

void DrawDot(SDL_Renderer *renderer, int x, int y, int radius, 
  SDL_Color clr)
{
  SDL_SetRenderDrawColor(renderer, clr.r, clr.g, clr.b, DOT_OPACITY);
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
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, LINE_OPACITY);
  for (int i = 2; i <= dot_rows + 1; i++)
  {  
    SDL_RenderDrawLine(renderer, intrv * i - 0 - X_offset, 2 * intrv - Y_offset,
      intrv * i - 0 - X_offset, intrv * (dot_clmns + 1) - Y_offset);
    SDL_RenderDrawLine(renderer, intrv * i - 1 - X_offset, 2 * intrv - Y_offset,
      intrv * i - 1 - X_offset, intrv * (dot_clmns + 1) - Y_offset);
    SDL_RenderDrawLine(renderer, intrv * i - 2 - X_offset, 2 * intrv - Y_offset,
      intrv * i - 2 - X_offset, intrv * (dot_clmns + 1) - Y_offset);
  }

  for (int i = 2; i <= dot_clmns + 1; i++)
  {  
    SDL_RenderDrawLine(renderer, 2 * intrv - X_offset, intrv * i - 0 - Y_offset,
      intrv * (dot_rows + 1) - X_offset, intrv * i - 0 - Y_offset);
    SDL_RenderDrawLine(renderer, 2 * intrv - X_offset, intrv * i - 1 - Y_offset,
      intrv * (dot_rows + 1) - X_offset, intrv * i - 1 - Y_offset);
    SDL_RenderDrawLine(renderer, 2 * intrv - X_offset, intrv * i - 2 - Y_offset,
      intrv * (dot_rows + 1) - X_offset, intrv * i - 2 - Y_offset);
  }
}

void EnumerateField (int dot_clmns, int dot_rows, TTF_Font *my_Font,
  Text_num* textureText)
{
  int index = 0;

  SDL_Surface* surfaceText;
  SDL_Rect rectangleE;

  int number = 0;
  string num_str;
  char* num;
  rectangleE.w = intrv * 1.5;
  rectangleE.h = intrv * 1.5;

  number = dot_rows - 1;
  num_str = to_string(number);
  num = (char*) num_str.c_str();

  surfaceText = TTF_RenderText_Solid(my_Font, num, TextColor);
  textureText[index].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
  SDL_FreeSurface(surfaceText);
  index++;

  surfaceText = TTF_RenderText_Solid(my_Font, num, TextColor);
  textureText[index].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
  SDL_FreeSurface(surfaceText);
  index++;

  number = dot_clmns - 1;
  num_str = to_string(number);
  num = (char*) num_str.c_str();

  surfaceText = TTF_RenderText_Solid(my_Font, num, TextColor);
  textureText[index].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
  SDL_FreeSurface(surfaceText);
  index++;

  surfaceText = TTF_RenderText_Solid(my_Font, num, TextColor);
  textureText[index].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
  SDL_FreeSurface(surfaceText);
  index++;

  for (int number = 0; number < dot_rows; number += 5)
  {
    SDL_Surface* surfaceText;
    num_str = to_string(number);
    num = (char*) num_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, TextColor);
    textureText[index].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_FreeSurface(surfaceText);
    index++;
  }
  
  for (int number = 0; number < dot_clmns; number += 5)
  {
    num_str = to_string(number);
    num = (char*) num_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, TextColor);
    textureText[index].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_FreeSurface(surfaceText);
    index++;
  }

  for (int number = 0; number < dot_rows; number += 5)
  {
    num_str = to_string(number);
    num = (char*) num_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, TextColor);
    textureText[index].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_FreeSurface(surfaceText);
    index++;
  }
  
  for (int number = 0; number < dot_clmns; number += 5)
  {
    num_str = to_string(number);
    num = (char*) num_str.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, TextColor);
    textureText[index].txtr = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_FreeSurface(surfaceText);
    index++;
  }
}

void DrawFieldNumbers (int dot_clmns, int dot_rows, TTF_Font *my_Font,
  Text_num* textureText)
{
  int index {};
  int number {};
  SDL_Rect rectangleE;
  rectangleE.w = intrv * 1.5;
  rectangleE.h = intrv * 1.5;

  number = dot_rows - 1;

  rectangleE.x = intrv * (number + 1.3) - X_offset;
  rectangleE.y = - Y_offset + 0.5;
  SDL_RenderCopy(renderer, textureText[index].txtr, NULL, &rectangleE);
  index++;

  rectangleE.x = intrv * ( number + 1.3) - X_offset;
  rectangleE.y = intrv * (dot_clmns + 1.5) - Y_offset;
  SDL_RenderCopy(renderer, textureText[index].txtr, NULL, &rectangleE);
  index++;

  number = dot_clmns - 1;
  rectangleE.x = - X_offset + 0.5;
  rectangleE.y = intrv * ( number + 1.3) - Y_offset;
  SDL_RenderCopy(renderer, textureText[index].txtr, NULL, &rectangleE);
  index++;

  rectangleE.x = intrv * (dot_rows + 1.5) - X_offset;
  rectangleE.y = intrv * ( number + 1.3) - Y_offset;
  SDL_RenderCopy(renderer, textureText[index].txtr, NULL, &rectangleE);
  index++;

  for (int number = 0; number < dot_rows; number += 5)
  {
    rectangleE.x = intrv * (number + 1.3) - X_offset;
    rectangleE.y = - Y_offset + 0.5;
    SDL_RenderCopy(renderer, textureText[index].txtr, NULL, &rectangleE);
    index++;
  }
  
  for (int number = 0; number < dot_clmns; number += 5)
  {
    rectangleE.x = - X_offset + 0.5;
    rectangleE.y = intrv * ( number + 1.3) - Y_offset;
    SDL_RenderCopy(renderer, textureText[index].txtr, NULL, &rectangleE);
    index++;
  }

  for (int number = 0; number < dot_rows; number += 5)
  {

    rectangleE.x = intrv * ( number + 1.3) - X_offset;
    rectangleE.y = intrv * (dot_clmns + 1.5) - Y_offset;
    SDL_RenderCopy(renderer, textureText[index].txtr, NULL, &rectangleE);
    index++;
  }
  
  for (int number = 0; number < dot_clmns; number += 5)
  {
    rectangleE.x = intrv * (dot_rows + 1.5) - X_offset;
    rectangleE.y = intrv * ( number + 1.3) - Y_offset;
    SDL_RenderCopy(renderer, textureText[index].txtr, NULL, &rectangleE);
    index++;
  }
}

void FieldColor() 
{
  if (event.type == SDL_KEYDOWN)
  {  
    switch(event.key.keysym.sym)
    {
      case SDLK_g:
        background_green += 1;
        TextColor.g++;
        break;
      case SDLK_r:
        background_red += 1;
        TextColor.r++;
        break;
      case SDLK_b:
        background_blue += 1;
        TextColor.b++;
        break;
      case SDLK_o:
      case SDLK_0:
      {
        background_red = red_default;
        background_green = green_default;
        background_blue = blue_default;
        TextColor.r = text_red_default;
        TextColor.g = text_green_default;
        TextColor.b = text_blue_default;
        break;
      }
      case SDLK_w:
      {
        background_red = 255;
        background_green = 255;
        background_blue = 255;
        TextColor.r = red_default;
        TextColor.g = green_default;
        TextColor.b = blue_default;
      }
    }
  }
}

void DrawDots(Dots* dots, int size, int fx)
{
  for (int i = 0; i < size; i++)
  { 
    if (dots[i].exist == true) 
      DrawDot(renderer, (i % fx + 2) * intrv - X_offset - 1,
        (i / fx + 2) * intrv - Y_offset - 1, dot_size, dots[i].clr);  
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
  if (event.type == SDL_MOUSEWHEEL)
  {
    if ((event.wheel.y > 0) && (intrv <= maxintrv)) // Zoom in
    {
      intrv++;
      X_offset = X_offset * double(intrv) / (intrv - 1) + 
        (2 * xMouse * (double(intrv) / (intrv - 1) - 1 )) / 2;
      
      Y_offset = Y_offset * double(intrv) / (intrv - 1) + 
        (2 * yMouse * (double(intrv) / (intrv - 1) -  1)) / 2 ;
    }
    else if ((event.wheel.y < 0) && (intrv >= minintrv)) // Zoom out
    {  
      intrv--;
      X_offset = X_offset * double(intrv) / (intrv + 1) + 
        (2 * xMouse * (double(intrv) / (intrv + 1) - 1 )) / 2;
      
      Y_offset = Y_offset * double(intrv) / (intrv + 1) + 
        (2 * yMouse * (double(intrv) / (intrv + 1) -  1)) / 2 ;
    }
    
    dot_size = round(intrv * dots_to_intrv);
    mouse.circle_size = dot_size + 2;
  }
}

bool GetDotInput(Dots *dots, SDL_Color clr, int fxs, int fys)
{
  if ( (event.button.button == SDL_BUTTON_LEFT) &&
     (event.type == SDL_MOUSEBUTTONUP) &&
     (  (((yMouse + Y_offset) % intrv) <= intrv / 3.0)
     || (((yMouse + Y_offset) % intrv) >= intrv * 2 / 3.0)) 
     && 
       ((((xMouse + X_offset) % intrv) <= intrv / 3.0)
     || (((xMouse + X_offset) % intrv) >= intrv * 2 / 3.0))
     && 
        (xMouse + X_offset >= 5 / 3.0 * intrv)
     && (yMouse + Y_offset >= 5 / 3.0 * intrv)
     && (xMouse + X_offset <= (fxs + 5 / 3.0) * intrv)
     && (yMouse + Y_offset <= (fys + 5 / 3.0) * intrv))
  {  
    int coeff = round((xMouse + X_offset) / float(intrv)) - 2 +
      + (round((yMouse + Y_offset) / float(intrv)) - 2) * fxs;
      
    if (dots[coeff].exist == false)
    {  
      dots[coeff].exist = true;
      dots[coeff].clr = clr;
      return true;
    }      
  }
  return false;
}

void GetDotErase(Dots *dots, int fxs, int fys)
{
  if ( (event.button.button == SDL_BUTTON_RIGHT) &&
     (event.type == SDL_MOUSEBUTTONUP) &&
     (  (((yMouse + Y_offset) % intrv) <= intrv / 3.0)
     || (((yMouse + Y_offset) % intrv) >= intrv * 2 / 3.0)) 
     && 
       ((((xMouse + X_offset) % intrv) <= intrv / 3.0)
     || (((xMouse + X_offset) % intrv) >= intrv * 2 / 3.0))
     && 
        (xMouse + X_offset >= 5 / 3.0 * intrv)
     && (yMouse + Y_offset >= 5 / 3.0 * intrv)
     && (xMouse + X_offset <= (fxs + 5 / 3.0) * intrv)
     && (yMouse + Y_offset <= (fys + 5 / 3.0) * intrv))
  {  
    int coeff = round((xMouse + X_offset) / float(intrv)) - 2 +
      + (round((yMouse + Y_offset) / float(intrv)) - 2) * fxs;
    
    if ((dots[coeff].spaceborder == false) &&
       (dots[coeff].spaceinside == false)) 
      dots[coeff].exist = false;      
  }
}

void CheckPositionInField(int fxs, int fys)
{
  if ( (  (((yMouse + Y_offset) % intrv) <= intrv / 3.0)
    || (((yMouse + Y_offset) % intrv) >= intrv * 2 / 3.0)) 
    && 
      (((xMouse + X_offset) % intrv) <= intrv / 3.0)
    || (((xMouse + X_offset) % intrv) >= intrv * 2 / 3.0)
    && 
      (xMouse + X_offset >= 5 / 3.0 * intrv)
    && (yMouse + Y_offset >= 5 / 3.0 * intrv)
    && (xMouse + X_offset <= (fxs + 5 / 3.0) * intrv)
    && (yMouse + Y_offset <= (fys + 5 / 3.0) * intrv))
  {  
    SDL_Surface* surfaceText;
    SDL_Texture* textureText;
    SDL_Rect rectangleE;
    int line = round((yMouse + Y_offset) / float(intrv)) - 2;
    int clmn = round((xMouse + X_offset) / float(intrv)) - 2;

    rectangleE.h = max (uint16_t(intrv / 2.0), uint16_t(30));
    rectangleE.w = rectangleE.h * 4;

    string line_num = "line" + to_string(line) + ", clmn" + to_string(clmn);
    char* num = (char*) line_num.c_str();
    surfaceText = TTF_RenderText_Solid(my_Font, num, mouse.clr);
    textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
    rectangleE.x = xMouse + mouse.circle_size;
    rectangleE.y = yMouse;
  
    SDL_RenderCopy(renderer, textureText, NULL, &rectangleE);
    SDL_DestroyTexture(textureText);
    SDL_FreeSurface(surfaceText);
  }
}

void ChangeDotColor()
{
  switch (player_amount)
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