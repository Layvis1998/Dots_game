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
#include "headers/MenuFunc.h"

#if __linux__
  #include <X11/Xlib.h>
#endif

/*
if _WIN32
  #include "headers/wtypes.h"
#endif     
*/

using std::cout;
using std::endl;
using std::vector;
using std::unordered_set;
using std::unordered_map;
using std::string;
using std::list;
using std::to_string;
using std::max;

void MainMenu();
void CreditsMenu();
void SelectAmountOfPlayersMenu();
void OnePlayerMenu();
void TwoPlayerMenu();
void ThreePlayerMenu();
void FourPlayerMenu();
void GameSizeMenu();
void GameRuleMenu();
void GameItself();

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Surface *surface;
SDL_Event event;
SDL_Event event2;

//gloabal variables
uint16_t intrv = 20; // interval
float move_game_speed = 2.7;
uint16_t maxintrv = 120;
uint16_t minintrv = default_minintrv; 
int vo;  //vertical offset
int ho;  //horizontal offset
int X_offset = 0;
int Y_offset = 0;
int maxdotamount = 0;
int maxeatendots = 0;
bool classic = true;
vector <Button> ButtonVector;
vector <Label> LabelVector;
vector <ColorButton> ClrButtonVector;
Dots dots[menu_x_field * menu_y_field];
Dots dots_credits[credits_y_field * credits_x_field];
Dots* dots_game;
int dot_size = 8;
char* field_x;
char* field_y;  

int ScWidth;
int ScHeight;
int game_x_field = 30;
int game_y_field = 30;

uint16_t rectangle_w;
uint16_t rectangle_h;

TTF_Font* my_Font; 
Mouse mouse;
int xMouse = 0;
int yMouse = 0;

uint8_t player_amount = 2;
SDL_Color white = {255, 255, 255, DOT_OPACITY};
SDL_Color TextColor = 
  {text_red_default, text_green_default, text_blue_default, 240};
SDL_Color cur_color;
uint8_t background_red = red_default;
uint8_t background_green = green_default;
uint8_t background_blue = blue_default;

GameStateType GameState;


int main(int argc, char *argv[])
{
  ////////////////////Finding out display parameters///////////////////////////
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

  ///////////////////////////////////Some variables////////////////////////////
  cur_color = white;
  mouse.GiveColor(white);
  mouse.circle_size = 9;
  vo = ScHeight / 8;  //vertical offset
  ho = ScWidth / 5;  //horizontal offset

  for (int i = 0; i < menu_x_field * menu_y_field; i++)
  {
    dots[i].exist = false;
    dots[i].visited = false;
    dots[i].deadend = false;
    dots[i].spaceinside = false;
    dots[i].spaceborder = false;
  }
  for (int i = 0; i < credits_x_field * credits_y_field; i++)
  {
    dots_credits[i].exist = false;
    dots_credits[i].visited = false;
    dots_credits[i].deadend = false;
    dots[i].spaceinside = false;
    dots[i].spaceborder = false;
  }

  ///////////////////////////////////Initializing SDL//////////////////////////
  window = SDL_CreateWindow("Dots", 0, 0, ScWidth, ScHeight,
    SDL_WINDOW_FULLSCREEN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND); 

  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s",
      SDL_GetError());
    return 3;
  }

  ////////////////////////////Initializing SDL_TTF/////////////////////////////
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

  ///////////////All buttons and labels are here///////////////////////////////  
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
  LabelVector.push_back(PaLabel);  //0 - index
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
  LabelVector.push_back(Udlabel);  //2 - index
  Udlabel.~Label();

  const char* sgL = "images/sg.png";
  Label SgLabel(sgL, ho * 1.5, vo, 100, 600);
  SgLabel.srect.y = 0;
  SgLabel.drect.x = ScWidth / 2 - SgLabel.drect.w / 2; 
  SgLabel.drect.y = vo;
  LabelVector.push_back(SgLabel);  //3 - index
  SgLabel.~Label();

  const char* colorsL = "images/colors.png";
  Label ColorsLabel(colorsL, ho * 1.5, vo, 100, 600);
  ColorsLabel.srect.y = 0;
  ColorsLabel.drect.x = ScWidth / 2 - ColorsLabel.drect.w / 2; 
  ColorsLabel.drect.y = vo * 2;
  LabelVector.push_back(ColorsLabel);  //4 - index
  ColorsLabel.~Label();

  const char* rgbL = "images/rgb.png";
  Label RgbLabel(rgbL, ho * 1.5, vo, 100, 600);
  RgbLabel.srect.y = 0;
  RgbLabel.drect.x = ScWidth / 2 - RgbLabel.drect.w / 2; 
  RgbLabel.drect.y = vo * 3;
  LabelVector.push_back(RgbLabel);  //5 - index
  RgbLabel.~Label(); 

  const char* sgsL = "images/sgs.png";
  Label SgsLabel(sgsL, ho * 1.5, vo, 100, 600);
  SgsLabel.srect.y = 0;
  SgsLabel.drect.x = ScWidth / 2 - SgsLabel.drect.w / 2; 
  SgsLabel.drect.y = 0; 
  LabelVector.push_back(SgsLabel);  //6 - index
  SgsLabel.~Label();

  const char* useL = "images/use.png";
  Label UseLabel(useL, ho * 1.5, vo, 100, 600);
  UseLabel.srect.y = 0;
  UseLabel.drect.x = ScWidth / 2 - UseLabel.drect.w / 2; 
  UseLabel.drect.y = vo;
  LabelVector.push_back(UseLabel);  //7 - index
  UseLabel.~Label();

  const char* sgrL = "images/sgr.png";
  Label SgrLabel(sgrL, ho * 1.5, vo, 100, 600);
  SgrLabel.srect.y = 0;
  SgrLabel.drect.x = ScWidth / 2 - SgrLabel.drect.w / 2; 
  SgrLabel.drect.y = 0;
  LabelVector.push_back(SgrLabel);  //8 - index
  SgrLabel.~Label();

  const char* mdaL = "images/mda.png";
  Label MaxdotamountLabel(mdaL, ho * 1.5, vo, 100, 800);
  MaxdotamountLabel.srect.y = 0;
  MaxdotamountLabel.drect.x = ScWidth * 4 / 9 - MaxdotamountLabel.drect.w / 2; 
  MaxdotamountLabel.drect.y = vo * 4;
  LabelVector.push_back(MaxdotamountLabel);  //9 - index
  MaxdotamountLabel.~Label();

  const char* medL = "images/med.png";
  Label MaxeatendotsLabel(medL, ho * 1.5, vo, 100, 800);
  MaxeatendotsLabel.srect.y = 0;
  MaxeatendotsLabel.drect.x = ScWidth * 4 / 9 - MaxeatendotsLabel.drect.w / 2; 
  MaxeatendotsLabel.drect.y = vo * 5;
  LabelVector.push_back(MaxeatendotsLabel);  //10 - index
  MaxeatendotsLabel.~Label();

  ColorButton P21(45, 170, 0);
  P21.rect.x = ScWidth / 3 - P21.rect.w / 2; 
  P21.rect.y = vo * 4;
  ClrButtonVector.push_back(P21); //0 - index
  P21.~ColorButton();

  ColorButton P22(255, 220, 30);
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

  ///////////////////////////////Main cycle////////////////////////////////////
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

  ////////////////////////Exiting program//////////////////////////////////////
  TTF_CloseFont(my_Font);
  TTF_Quit();
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 666;
}