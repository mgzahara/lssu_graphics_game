#include "SDL.h"
#include "SDL_image.h"
#include "iostream"

using namespace std;

void quitSDL();
bool initSDL();

void displayInfo(const char*, int, int, int, SDL_Color);//string + int
void displayInfo(const char*, float, int, int, SDL_Color);//string + float
void displayInfo(const char*, int, int, SDL_Color);//string
void loadTextures();
SDL_Texture *loadImage(const char *);

SDL_Texture *fire, *explosion, *electric, *zap;
int WINDOW_HEIGHT = 300;
int WINDOW_WIDTH = 600;
SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event e;

int main(int argc, char **args)
{
  if (!initSDL())
  {
    //SDL did not init properly
    return 1;
  }

  bool done = false;
  //shock - lightning board effect - 64 x 64
  //explosion - bomb board effect - 96 x 96
  //electric - lightning tile effect - 64 x 64
  //fire ball - bomb tile effect - 32 x 32
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

  SDL_Rect shockSrc, shockDst, explosionSrc, explosionDst,
    electricSrc, electricDst, fireSrc, fireDst, shockDst2;
  
  while (!done)
  {
    //SDL event loop
    while (SDL_PollEvent(&e) != 0)
    {
      switch (e.type)
      {
      case SDL_QUIT:
        //close button
        done = true;
        break;
      }
    }

    //black out the screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //inc all counters
    shockFrameCounter = (shockFrameCounter + 1) % shockFrameMax;
    explosionFrameCounter = (explosionFrameCounter + 1) % explosionFrameMax;
    electricFrameCounter = (electricFrameCounter + 1) % electricFrameMax;
    fireFrameCounter = (fireFrameCounter + 1) % fireFrameMax;

    //set all src rects
    shockSrc = {
      shockFrameCounter * shockSize,
      0,
      shockSize,
      shockSize};

    explosionSrc = {
      explosionFrameCounter * explosionSize,
      0,
      explosionSize,
      explosionSize};
    
    electricSrc = {
      electricFrameCounter * electricSize,
      0,
      electricSize,
      electricSize};

    fireSrc = {
      fireFrameCounter * fireSize,
      0,
      fireSize,
      fireSize};
    
    //set all dst rects
    shockDst = {10, 10, 55, 50};
    shockDst2 = {10 + 50, 10, 55, 50};
    explosionDst = {280, 10, 132, 132};
    electricDst = {10, 200, 32, 32};
    fireDst = {90, 200, 32, 32};

    //do effects example here
    SDL_RenderCopy(renderer, zap, &shockSrc, &shockDst);
    SDL_RenderCopy(renderer, zap, &shockSrc, &shockDst2);
    SDL_RenderCopy(renderer, explosion, &explosionSrc, &explosionDst);
    SDL_RenderCopy(renderer, electric, &electricSrc, &electricDst);
    SDL_RenderCopy(renderer, fire, &fireSrc, &fireDst);


    
    // Update window
    SDL_RenderPresent(renderer);

    SDL_Delay(100);
  }

  return 0;
}

void quitSDL()
{
  // Quit - memory clean up
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

bool initSDL()
{
  //set up everything for SDL
  int x = atexit(quitSDL); //set exit function
  if (x != 0)
  {
    fprintf(stderr, "cannot set exit function\n");
    exit(EXIT_FAILURE);
  }

  if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
  {
    cout << "Error initializing SDL: " << SDL_GetError() << endl;
    //system("pause");
    return false;
  }
  //create window
  window = SDL_CreateWindow("Don't Be Jeweled!",
                            SDL_WINDOWPOS_UNDEFINED,
                            SDL_WINDOWPOS_UNDEFINED,
                            WINDOW_WIDTH,
                            WINDOW_HEIGHT,
                            SDL_WINDOW_SHOWN);

  if (!window)
  {
    cout << "Error creating window: " << SDL_GetError() << endl;
    //system("pause");
    return false;
  }
  //create renderer
  renderer = SDL_CreateRenderer(window,
                                -1,
                                SDL_RENDERER_ACCELERATED);

  if (!renderer)
  {
    cout << "Error creating renderer: " << SDL_GetError() << endl;
    return false;
  }

  loadTextures();

  //black out the screen
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  return true;
}

void loadTextures()
{
  //load textures - just make them global
  //shock - lightning board effect - 64 x 64
  //explosion - bomb board effect - 96 x 96
  //electric - lightning tile effect - 64 x 64
  //fire ball - bomb tile effect - 32 x 32

  //fire, explosion, electric, zap;  
  fire = loadImage("img/fireball_50.png");
  explosion = loadImage("img/explosion.png");
  electric = loadImage("img/electric_50.png");
  zap = loadImage("img/zap_horizontal.png");

}

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

