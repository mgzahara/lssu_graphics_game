#ifndef GLOBALS
#define GLOBALS

#include "iostream"
#include "stdlib.h"
#include "time.h"
#include "tile.h"
#include "game.h"

using namespace std;

//globals
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Event e;

extern const int BOARD_SIZE; //board is 8x8
extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;
extern const int GRID_SIZE; //size of one square on board
extern const int BOX_SIZE;  //thickness of highlight box
extern const int GEM_SIZE;  //size of gem sprites

extern Tile board[8][8]; //hold all board Tiles
//parallel with above 2d array
//indicates which Tiles need to be checked for matches
extern bool matchBoard[8][8];
extern Tile activeTile; //used as reference point for which Tile is 'active'

extern int box_x;
extern int box_y; //location for activeTile highlighting

SDL_Texture *loadImage(const char *filename);

// extern color c;

// struct colors
// {
//   struct color
//   {
//     int r, g, b;
//   };
//   color whi, bla, red, blu, gre, pur, yel, ora, pin, cya, mar;
// } c;

//is colorType used anymore? maybe later?
// enum colorType
// {
//   color_white,
//   color_black,
//   color_red,
//   color_green,
//   color_blue,
//   color_purple,
//   color_yellow,
//   color_orange,
//   color_pink,
//   color_cyan,
//   color_maroon
// };

#endif