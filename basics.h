#include "iostream"
#include "stdlib.h"
#include "time.h"

using namespace std;

//globals
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event e;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int GRID_SIZE = 50;
const int BOX_SIZE = 3;

struct colors
{
  struct color
  {int r, g, b;};
  color whi, bla, red, blu, gre, pur, yel, ora, pin, cya, mar;
}c;

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


bool initSDL();
void quitSDL();
void setColors();
SDL_Texture* loadImage(const char*);

SDL_Texture* loadImage(const char* filename)
{
  SDL_Texture *texture;

  texture = IMG_LoadTexture(renderer, filename);
  if (texture == NULL)
    {
      printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
      exit(1);
    }

  return texture;
}

SDL_Texture* loadKeyedImage(const char* filename)
{
  SDL_Surface* surface;
  SDL_Texture* texture;

  surface = IMG_Load(filename);
  SDL_SetColorKey(surface, SDL_TRUE, 0xff00ff);
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  return texture;
}

bool initSDL() {

  int x = atexit(quitSDL);//set exit function
  if( x != 0 )
    {
      fprintf(stderr, "cannot set exit function\n");
      exit(EXIT_FAILURE);
    }

  setColors();
  
  //seed rand
  srand(time(NULL));
  
  // See last example for comments
  if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) {
    cout << "Error initializing SDL: " << SDL_GetError() << endl;
    //system("pause");
    return false;
  }
  //create window
  window = SDL_CreateWindow( "Grid",
			     SDL_WINDOWPOS_UNDEFINED,
			     SDL_WINDOWPOS_UNDEFINED,
			     WINDOW_WIDTH,
			     WINDOW_HEIGHT,
			     SDL_WINDOW_SHOWN );
  
  if ( !window ) {
    cout << "Error creating window: " << SDL_GetError()  << endl;
    //system("pause");
    return false;
  }
  //create renderer
  renderer = SDL_CreateRenderer( window,
				 -1,
				 SDL_RENDERER_ACCELERATED );
  
  if ( !renderer ) {
    cout << "Error creating renderer: " << SDL_GetError() << endl;
    return false;
  }

  //white out the screen
  SDL_SetRenderDrawColor( renderer, c.whi.r, c.whi.g, c.whi.b, 255 );
  SDL_RenderClear( renderer );

    
  return true;
}

void quitSDL() {
  // Quit - memory clean up
  SDL_DestroyRenderer( renderer );
  SDL_DestroyWindow( window );
  SDL_Quit();
}

void setColors()
{
  c.pur.r = c.pur.b  = c.mar.r = 128;
  c.ora.g = 69;
  c.pin.g = 20;
  c.pin.b = 147;
  c.whi.r =
    c.whi.g =
    c.whi.b =
    c.red.r =
    c.blu.b =
    c.gre.g =
    c.ora.r =
    c.yel.r =
    c.yel.g =
    c.cya.g =
    c.cya.b =
    c.pin.r = 255;

  c.bla.r =
    c.bla.g =
    c.bla.b =
    c.red.g =
    c.red.b =
    c.blu.r =
    c.blu.g =
    c.gre.r =
    c.gre.b =
    c.pur.g =
    c.ora.b =
    c.yel.b =
    c.mar.g =
    c.mar.b =
    c.cya.r = 0;
}
