#include "iostream"
#include "stdlib.h"
#include "time.h"
#include "tile.h"

using namespace std;

//globals
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event e;


Tile activeTile;//highlighted Tile - first click
Tile swappingTile;//never highlighted - second click

const int BOARD_SIZE = 8;//board is 8x8
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int GRID_SIZE = 50;//size of one square on board
const int BOX_SIZE = 3;//thickness of highlight box
const int GEM_SIZE = 32;//size of gem sprites

Tile board[BOARD_SIZE][BOARD_SIZE];//hold all board Tiles

struct colors
{
  struct color
  {int r, g, b;};
  color whi, bla, red, blu, gre, pur, yel, ora, pin, cya, mar;
}c;

struct sprite
{
  struct velocity{ int x, y; };
  struct bounds{ int left, right, top, bot; };

  SDL_Texture* tex;
  SDL_Rect src, dst;
  int spriteCounter, startSprite, endSprite, scaleFactor;
  velocity vel;
  bounds bound;
  int spriteWidth, spriteHeight;

}gems[6], empty_gem;

enum colorType
  {
    color_white,
    color_black,
    color_red,
    color_green,
    color_blue,
    color_purple,
    color_yellow,
    color_orange,
    color_pink,
    color_cyan,
    color_maroon
  };


