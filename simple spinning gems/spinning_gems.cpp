/*
  summary of program
  
display each of the six options for gems in the game
with their spinning animations

gem source: Code Inferno Games on Open Game Art
at https://opengameart.org/content/animated-spinning-gems

the last gem (blue tear drop) needs a little work
--it is not uniform in size for a few frames
*/

#include <SDL.h>
#include <stdio.h>
#include "SDL_image.h"
#include <iostream>

using namespace std;

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

struct realPos
{float x, y;};
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

}gem1, gem2, gem3, gem4, gem5, gem6;

bool init();
void quitSDL();
bool loop();
void setColors();
SDL_Texture* loadImage(const char*);
//SDL_Texture* loadKeyedImage(const char*);

//globals
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event e;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int RECT_SIZE = 10;

int main(int argc, char** args) {

  //if we cant init, end main
  if ( !init() ) return 1;
  
  while ( loop() ) {
    // wait before processing the next frame
    //prevents weird screen tearing
    SDL_Delay(10);
    }

  return 0;
}

bool loop() {
//the real main
//handles events, drawing
//returns false when end con is met

  static const int pause = 4;

  //inc for each frame
  static int frameCounter = 0;
  
  // Event loop
  while ( SDL_PollEvent( &e ) != 0 ) {
    switch ( e.type ) {
    case SDL_QUIT:
      //close button
      return false;
    }
  }
  //black background
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);
  
  frameCounter++;

  if( frameCounter % pause == 0 )
    {//only move every 4 calls to loop()

      //gem1
      gem1.spriteCounter++;

      gem1.src.x = (( gem1.spriteCounter % gem1.endSprite )
		    + gem1.startSprite ) * gem1.spriteWidth;

      //gem2
      gem2.spriteCounter++;

      gem2.src.x = (( gem2.spriteCounter % gem2.endSprite )
		    + gem2.startSprite ) * gem2.spriteWidth;

      //gem3
      gem3.spriteCounter++;

      gem3.src.x = (( gem3.spriteCounter % gem3.endSprite )
		    + gem3.startSprite ) * gem3.spriteWidth;

      //gem4
      gem4.spriteCounter++;

      gem4.src.x = (( gem4.spriteCounter % gem4.endSprite )
		    + gem4.startSprite ) * gem4.spriteWidth;

      //gem5
      gem5.spriteCounter++;

      gem5.src.x = (( gem5.spriteCounter % gem5.endSprite )
		    + gem5.startSprite ) * gem5.spriteWidth;

      //gem6
      gem6.spriteCounter++;

      gem6.src.x = (( gem6.spriteCounter % gem6.endSprite )
		    + gem6.startSprite ) * gem6.spriteWidth;


    }

  //gem1
  SDL_RenderCopy(renderer, gem1.tex, &gem1.src, &gem1.dst);
  
  //gem2
  SDL_RenderCopy(renderer, gem2.tex, &gem2.src, &gem2.dst);
  
  //gem3
  SDL_RenderCopy(renderer, gem3.tex, &gem3.src, &gem3.dst);
  
  //gem4
  SDL_RenderCopy(renderer, gem4.tex, &gem4.src, &gem4.dst);
  
  //gem5
  SDL_RenderCopy(renderer, gem5.tex, &gem5.src, &gem5.dst);
  
  //gem6
  SDL_RenderCopy(renderer, gem6.tex, &gem6.src, &gem6.dst);
  
  // Update window
  SDL_RenderPresent( renderer );

  return true;
}

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

/*SDL_Texture* loadKeyedImage(const char* filename)
{
  SDL_Surface* surface;
  SDL_Texture* texture;

  surface = IMG_Load(filename);
  SDL_SetColorKey(surface, SDL_TRUE, 0xff00ff);
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  return texture;
  }*/
  
bool init() {

  int x = atexit(quitSDL);//set exit function
  if( x != 0 )
    {
      fprintf(stderr, "cannot set exit function\n");
      exit(EXIT_FAILURE);
    }

  setColors();
  
  //seed rand
  srand(0);
  
  // See last example for comments
  if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) {
    cout << "Error initializing SDL: " << SDL_GetError() << endl;
    //system("pause");
    return false;
  }
  //create window
  window = SDL_CreateWindow( "Assignment #2",
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

  //set up gem1 sprite
  gem1.tex = loadImage("img/gem1.png");
  gem1.spriteWidth = 32;
  gem1.spriteHeight = 32;
  gem1.scaleFactor = 1;
  gem1.vel.x = 0;
  gem1.vel.y = 0;

  gem1.bound.top = 0;
  gem1.bound.bot = WINDOW_HEIGHT - (gem1.spriteHeight * gem1.scaleFactor);
  gem1.bound.left = 0;
  gem1.bound.right = WINDOW_WIDTH - (gem1.spriteWidth * gem1.scaleFactor);
  
  gem1.startSprite = 0;
  gem1.spriteCounter = 0;
  gem1.endSprite = 60;
  gem1.src = {0, 0, gem1.spriteWidth, gem1.spriteHeight};
  gem1.dst = {50, 50,
	      gem1.spriteWidth  * gem1.scaleFactor,
	      gem1.spriteHeight * gem1.scaleFactor};

  //set up gem2 sprite
  gem2.tex = loadImage("img/gem2.png");
  gem2.spriteWidth = 32;
  gem2.spriteHeight = 32;
  gem2.scaleFactor = 1;
  gem2.vel.x = 0;
  gem2.vel.y = 0;

  gem2.bound.top = 0;
  gem2.bound.bot = WINDOW_HEIGHT - (gem2.spriteHeight * gem2.scaleFactor);
  gem2.bound.left = 0;
  gem2.bound.right = WINDOW_WIDTH - (gem2.spriteWidth * gem2.scaleFactor);
  
  gem2.startSprite = 0;
  gem2.spriteCounter = 0;
  gem2.endSprite = 60;
  gem2.src = {0, 0, gem2.spriteWidth, gem2.spriteHeight};
  gem2.dst = {100, 50,
	      gem2.spriteWidth  * gem2.scaleFactor,
	      gem2.spriteHeight * gem2.scaleFactor};

  //set up gem3 sprite
  gem3.tex = loadImage("img/gem3.png");
  gem3.spriteWidth = 32;
  gem3.spriteHeight = 32;
  gem3.scaleFactor = 1;
  gem3.vel.x = 0;
  gem3.vel.y = 0;

  gem3.bound.top = 0;
  gem3.bound.bot = WINDOW_HEIGHT - (gem3.spriteHeight * gem3.scaleFactor);
  gem3.bound.left = 0;
  gem3.bound.right = WINDOW_WIDTH - (gem3.spriteWidth * gem3.scaleFactor);
  
  gem3.startSprite = 0;
  gem3.spriteCounter = 0;
  gem3.endSprite = 60;
  gem3.src = {0, 0, gem3.spriteWidth, gem3.spriteHeight};
  gem3.dst = {150, 50,
	      gem3.spriteWidth  * gem3.scaleFactor,
	      gem3.spriteHeight * gem3.scaleFactor};

    //set up gem4 sprite
  gem4.tex = loadImage("img/gem4.png");
  gem4.spriteWidth = 32;
  gem4.spriteHeight = 32;
  gem4.scaleFactor = 1;
  gem4.vel.x = 0;
  gem4.vel.y = 0;

  gem4.bound.top = 0;
  gem4.bound.bot = WINDOW_HEIGHT - (gem4.spriteHeight * gem4.scaleFactor);
  gem4.bound.left = 0;
  gem4.bound.right = WINDOW_WIDTH - (gem4.spriteWidth * gem4.scaleFactor);
  
  gem4.startSprite = 0;
  gem4.spriteCounter = 0;
  gem4.endSprite = 60;
  gem4.src = {0, 0, gem4.spriteWidth, gem4.spriteHeight};
  gem4.dst = {200, 50,
	      gem4.spriteWidth  * gem4.scaleFactor,
	      gem4.spriteHeight * gem4.scaleFactor};

  //set up gem5 sprite
  gem5.tex = loadImage("img/gem5.png");
  gem5.spriteWidth = 32;
  gem5.spriteHeight = 32;
  gem5.scaleFactor = 1;
  gem5.vel.x = 0;
  gem5.vel.y = 0;

  gem5.bound.top = 0;
  gem5.bound.bot = WINDOW_HEIGHT - (gem5.spriteHeight * gem5.scaleFactor);
  gem5.bound.left = 0;
  gem5.bound.right = WINDOW_WIDTH - (gem5.spriteWidth * gem5.scaleFactor);
  
  gem5.startSprite = 0;
  gem5.spriteCounter = 0;
  gem5.endSprite = 30;
  gem5.src = {0, 0, gem5.spriteWidth, gem5.spriteHeight};
  gem5.dst = {250, 50,
	      gem5.spriteWidth  * gem5.scaleFactor,
	      gem5.spriteHeight * gem5.scaleFactor};

  //set up gem6 sprite
  gem6.tex = loadImage("img/gem6.png");
  gem6.spriteWidth = 32;
  gem6.spriteHeight = 32;
  gem6.scaleFactor = 1;
  gem6.vel.x = 0;
  gem6.vel.y = 0;

  gem6.bound.top = 0;
  gem6.bound.bot = WINDOW_HEIGHT - (gem6.spriteHeight * gem6.scaleFactor);
  gem6.bound.left = 0;
  gem6.bound.right = WINDOW_WIDTH - (gem6.spriteWidth * gem6.scaleFactor);
  
  gem6.startSprite = 0;
  gem6.spriteCounter = 0;
  gem6.endSprite = 60;
  gem6.src = {0, 0, gem6.spriteWidth, gem6.spriteHeight};
  gem6.dst = {300, 50,
	      gem6.spriteWidth  * gem6.scaleFactor,
	      gem6.spriteHeight * gem6.scaleFactor};


  
    
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
