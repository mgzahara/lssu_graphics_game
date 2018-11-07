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
// extern bool matchBoard[8][8];

extern int box_x;
extern int box_y; //location for activeTile highlighting

extern Tile board[8][8]; //hold all board Tiles
extern Tile aboveBoard[8]; //hold tiles to fill in board
extern Tile activeTile;  //used as reference point for which Tile is 'active'

extern SDL_Texture *gems[6];
extern SDL_Texture *empty_gem;
extern SDL_Texture *electric, *explosion, *fire, *zap;

extern int shockSize = 64;
extern int explosionSize = 96;
extern int electricSize = 64;
extern int fireSize = 32;

extern int shockFrameMax = 8;
extern int explosionFrameMax = 12;
extern int electricFrameMax = 6;
extern int fireFrameMax = 4;

extern int shockFrameCounter = 0;
extern int explosionFrameCounter = 0;
extern int electricFrameCounter = 0;
extern int fireFrameCounter = 0;


#endif
