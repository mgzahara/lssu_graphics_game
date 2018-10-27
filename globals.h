#ifndef GLOBALS1
#define GLOBALS1

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

//parallel with board 2d array
//indicates which Tiles need to be checked for matches
extern bool matchBoard[8][8];

extern int box_x;
extern int box_y; //location for activeTile highlighting

extern Tile board[8][8]; //hold all board Tiles
extern Tile activeTile;  //used as reference point for which Tile is 'active'

extern SDL_Texture *gems[6];
extern SDL_Texture *empty_gem;

#endif