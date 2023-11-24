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

#include "Classes.h"

using std::cout;
using std::endl;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::string;
using std::list;
using std::to_string;
using std::max;


void FindConnectedDots (Dots* dots, int current, unordered_set <int> &uset,
  int fx, int fy);

int Min (unordered_set <int> uset);

void ExtractCycle (Dots* dots, int current, unordered_set <int> &uset, 
  int fx, int fy);

int DifferentWaysUset
  (Dots* dots, unordered_set <int> uset, int current, int fx , int fy);

void DeleteBranchesUset
 (Dots* dots, unordered_set <int> &uset, int fx, int fy);

bool CheckForColor(unordered_set <int> CycleSet, Dots* dots, int fx);

ColorSpace CreateColorSpace(unordered_set <int> CycleSet, Dots* dots, int fx);

void ColorColorSpace (unordered_set <int> CycleSet, Dots* dots, int fx);

list <ColorSpace> ProcessDotInteraction (Dots* dots, int fx, int fy);

void MoveBoard(int fx, int fy , int speed);

void DrawDot(SDL_Renderer *renderer, int x, int y, int radius, 
  SDL_Color clr);

void DrawField(int dot_rows, int dot_clmns);

void FillCredits(TTF_Font *my_Font, Text_num* textureText);

void EnumerateField (int dot_clmns, int dot_rows, TTF_Font *my_Font,
  Text_num* textureText);

void DrawCredits( Text_num* textureText);

void DrawFieldNumbers (int dot_clmns, int dot_rows, TTF_Font *my_Font,
  Text_num* textureText);

void FieldColor(); 

void DrawDots(Dots* dots, int size, int fx);

void ScreenShot();

void Zoom();

bool GetDotInput(Dots *dots, SDL_Color clr, int fxs, int fys);

void GetDotErase(Dots *dots, int fxs, int fys);

void CheckPositionInField(int fxs, int fys);

void ChangeDotColor();