#include "globals.h"
#include "SDL.h"
#include "tile.h"
#include "iostream"

using namespace std;

//globals
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event e;

const int BOARD_SIZE = 8; //board is 8x8
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int GRID_SIZE = 50; //size of one square on board
const int BOX_SIZE = 3;   //thickness of highlight box
const int GEM_SIZE = 32;  //size of gem sprites

//parallel with board 2d array
//indicates which Tiles need to be checked for matches
bool matchBoard[8][8];

int box_x = -200; //location for activeTile highlighting
int box_y = -200; //location for activeTile highlighting

Tile board[8][8]; //hold all board Tiles
Tile activeTile;  //used as reference point for which Tile is 'active'

SDL_Texture *gems[6];
SDL_Texture *empty_gem;
