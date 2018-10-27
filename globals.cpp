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

Tile board[8][8]; //hold all board Tiles
//parallel with above 2d array
//indicates which Tiles need to be checked for matches
bool matchBoard[8][8];
Tile activeTile; //used as reference point for which Tile is 'active'

int box_x, box_y; //location for activeTile highlighting

SDL_Texture *loadImage(const char *filename)
{
    SDL_Texture *texture;

    texture = IMG_LoadTexture(renderer, filename);
    if (texture == NULL)
    {
        printf("Could not load: %s\n", filename);
        printf("IMG_LoadTexture Error: %s\n", IMG_GetError());
        exit(1);
    }

    return texture;
}
