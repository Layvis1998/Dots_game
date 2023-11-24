#pragma once
void FindConnectedDots (Dots*, int, unordered_set <int>&,
  int, int);

int Min (unordered_set <int>);

void ExtractCycle (Dots*, int, unordered_set <int>&, int, int);
int DifferentWaysUset (Dots*, unordered_set <int>, int, int, int);
void DeleteBranchesUset (Dots* , unordered_set <int>&, int, int);
bool CheckForColor(unordered_set <int>, Dots*, int);
ColorSpace CreateColorSpace(unordered_set <int>, Dots*, int);
void ColorColorSpace (unordered_set <int>, Dots*, int);
list <ColorSpace> ProcessDotInteraction (Dots*, int, int);
void MoveBoard(int , int ,int);
void DrawDot(SDL_Renderer*, int, int, int, SDL_Color);
void DrawField(int, int);
void FillCredits(TTF_Font *, Text_num*);
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