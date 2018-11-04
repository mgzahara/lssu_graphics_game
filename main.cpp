/* TODO
 *
 * Swap doesn't preserve boosts
 * Points
 * Time Limit
 *
 */

#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "string"
#include "globals.h"
#include "tile.h"
#include "game.h"
#include "Fixed_print.h"

using namespace std;

void quitSDL();
bool initSDL();

void displayInfo(const char*, int, int, int, SDL_Color);//string + int
void displayInfo(const char*, float, int, int, SDL_Color);//string + float
void displayInfo(const char*, int, int, SDL_Color);//string

void loadTextures();
SDL_Texture *loadImage(const char *);

int main(int argc, char **args)
{
  if (!initSDL())
  {
    //SDL did not init properly
    return 1;
  }

  bool done = false;
  int mouse_x = -1, mouse_y = -1;

  Game game; //start game stuff
  game.startBoard();

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

      case SDL_MOUSEBUTTONDOWN:
        if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
        { //grab mouse position on left click
          SDL_GetMouseState(&mouse_x, &mouse_y);
        }
        break;
      }
    }

    //draw board grid - is a grid necessary?
    game.drawBoard();

    game.handleLeftMouseClick(mouse_x, mouse_y);

    game.updateBoard();

    //TS
    //SDL_Delay(500);
    
    game.checkForMatches();

    game.drawHighlightBox();

    game.drawPanel();
    
    // Update window
    SDL_RenderPresent(renderer);

    //reset mouse position
    mouse_x = mouse_y = -1;

    SDL_Delay(20);
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

  //seed rand
  srand(time(NULL));

  // See last example for comments
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

  //init text to screen
  FixedPrint_Setup();

  return true;
}

void loadTextures()
{
  //load textures - just make them global
  gems[0] = loadImage("img/gem1.png");
  gems[1] = loadImage("img/gem2.png");
  gems[2] = loadImage("img/gem3.png");
  gems[3] = loadImage("img/gem4.png");
  gems[4] = loadImage("img/gem5.png");
  gems[5] = loadImage("img/gem6.png");
  empty_gem = loadImage("img/empty_gem.png");
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

