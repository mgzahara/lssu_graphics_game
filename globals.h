#ifndef GLOBALS1
#define GLOBALS1

#include "iostream"
#include "stdlib.h"
#include "time.h"
#include "tile.h"
#include "game.h"
#include "effect.h"
#include "SDL_mixer.h"

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

extern int box_x;
extern int box_y; //location for activeTile highlighting

extern Tile board[8][8]; //hold all board Tiles
extern Tile aboveBoard[8]; //hold tiles to fill in board
extern Tile activeTile;  //used as reference point for which Tile is 'active'

extern SDL_Texture *gems[6];
extern SDL_Texture *empty_gem;
extern SDL_Texture *electric, *explosion, *fire, *zap;
extern SDL_Texture *background;

extern Effect effects[8][8];

extern int shockSize;
extern int explosionSize;
extern int electricSize;
extern int fireSize;

extern int shockFrameMax;
extern int explosionFrameMax;
extern int electricFrameMax;
extern int fireFrameMax;

extern int shockFrameCounter;
extern int explosionFrameCounter;
extern int electricFrameCounter;
extern int fireFrameCounter;

extern int score;

//The music that will be played
//extern Mix_Music *music = NULL;

//The sound effects that will be used
extern Mix_Chunk *zapSound;
extern Mix_Chunk *bombSound;
extern Mix_Chunk *matchSound;
extern Mix_Chunk *shuffleSound;

#endif
