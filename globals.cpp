#include "globals.h"
#include "SDL.h"
#include "SDL_mixer.h"
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
// bool matchBoard[8][8];

int box_x = -200; //location for activeTile highlighting
int box_y = -200; //location for activeTile highlighting

Tile board[8][8]; //hold all board Tiles
Tile aboveBoard[8]; //hold tiles to fill in board
Tile activeTile;  //used as reference point for which Tile is 'active'

SDL_Texture *gems[6];
SDL_Texture *empty_gem;
SDL_Texture *electric, *explosion, *fire, *zap;
SDL_Texture *background;

Effect effects[8][8];

int shockSize = 64;
int explosionSize = 96;
int electricSize = 64;
int fireSize = 32;

int shockFrameMax = 8;
int explosionFrameMax = 12;
int electricFrameMax = 6;
int fireFrameMax = 4;

int shockFrameCounter = 0;
int explosionFrameCounter = 0;
int electricFrameCounter = 0;
int fireFrameCounter = 0;

int score = 0;

//The music that will be played
//Mix_Music *music = NULL;

//The sound effects that will be used
Mix_Chunk *zapSound = NULL;
Mix_Chunk *bombSound = NULL;
Mix_Chunk *matchSound = NULL;
