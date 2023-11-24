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
#include <vector>
#include <stdint.h>

#include "Classes.h"

using std::vector;

//gloabal variables
extern uint16_t intrv; // interval
extern float move_game_speed;
extern uint16_t maxintrv;
extern uint16_t minintrv; 
extern int vo;  //vertical offset
extern int ho;  //horizontal offset
extern int X_offset;
extern int Y_offset;
extern int maxdotamount;
extern int maxeatendots;
extern bool classic;
extern vector <Button> ButtonVector;
extern vector <Label> LabelVector;
extern vector <ColorButton> ClrButtonVector;
extern Dots dots[menu_x_field * menu_y_field];
extern Dots dots_credits[credits_y_field * credits_x_field];
extern Dots* dots_game;
extern int dot_size;
extern char* field_x;
extern char* field_y;  

extern int ScWidth;
extern int ScHeight;
extern int game_x_field;
extern int game_y_field;

extern uint16_t rectangle_w;
extern uint16_t rectangle_h;

extern TTF_Font* my_Font; 
extern Mouse mouse;
extern int xMouse;
extern int yMouse;

extern uint8_t player_amount;
extern SDL_Color white;
extern SDL_Color TextColor;
extern SDL_Color cur_color;
extern uint8_t background_red;
extern uint8_t background_green;
extern uint8_t background_blue;

extern GameStateType GameState;