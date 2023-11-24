#pragma once

struct Dots
{
  SDL_Color clr;
  bool exist;
  bool visited;
  bool deadend;
  bool cycle;
  bool spaceborder;
  bool spaceinside;
};

struct ColorSpace
{
  unordered_set <int> InnerDots;
  unordered_set <int> BorderDots;   
};

struct Text_num
{
  SDL_Texture* txtr;
};

//global constants
const uint16_t default_minintrv = 10;
const uint8_t red_default = 5;
const uint8_t green_default = 35;
const uint8_t blue_default = 90;
const uint8_t text_red_default = 50;
const uint8_t text_green_default = 70;
const uint8_t text_blue_default = 230;
const uint16_t menu_x_field = 200;
const uint16_t menu_y_field = 240;
const uint16_t credits_x_field = 50;
const uint16_t credits_y_field = 250;
const uint8_t DOT_OPACITY = 210;
const uint8_t LINE_OPACITY = 220;
const uint8_t BACKGROUND_OPACITY = 20;
const uint8_t SPACE_OPACITY = 160;
const double dots_to_intrv = 20 / 61.0;

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

enum GameStateType {Menu, Credits, SAP, ONEPM, TWOPM,
                    THREEPM, FOURPM, GSM, GRM, Game};
GameStateType GameState;
