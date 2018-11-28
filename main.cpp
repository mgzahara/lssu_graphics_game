/* TODO
 *
 * Swap doesn't preserve boosts
 * Points
 * Time Limit
 *
 */

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
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
void loadSounds();
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
	      else if(SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT))
		{
		  //for testing purposes only - remove Tile on right click
		  SDL_GetMouseState(&mouse_x, &mouse_y);
		  int clickRow = mouse_y / GRID_SIZE;
		  int clickCol = mouse_x / GRID_SIZE;

		  if (game.validLeftMouseClick(mouse_x, mouse_y)
		      and board[clickRow][clickCol].isNotBusy())
		    {
		      board[clickRow][clickCol].changeState("empty");
		    }
		}
	      break;




	    case SDL_KEYDOWN:
	      //testing purposes only - change gem type and boost
	      if(e.key.keysym.scancode == SDL_SCANCODE_1 or
		 e.key.keysym.scancode == SDL_SCANCODE_2 or
		 e.key.keysym.scancode == SDL_SCANCODE_3 or
		 e.key.keysym.scancode == SDL_SCANCODE_4 or
		 e.key.keysym.scancode == SDL_SCANCODE_5 or
		 e.key.keysym.scancode == SDL_SCANCODE_6 or
		 e.key.keysym.scancode <= SDL_SCANCODE_B or
		 e.key.keysym.scancode <= SDL_SCANCODE_H or
		 e.key.keysym.scancode <= SDL_SCANCODE_V or
		 e.key.keysym.scancode <= SDL_SCANCODE_N or
		 e.key.keysym.scancode <= SDL_SCANCODE_Z)
		{
		  SDL_GetMouseState(&mouse_x, &mouse_y);
		  int clickRow = mouse_y / GRID_SIZE;
		  int clickCol = mouse_x / GRID_SIZE;
		  switch(e.key.keysym.scancode)
		    {
		    case SDL_SCANCODE_1:board[clickRow][clickCol].setType(0);
		      break;
		    case SDL_SCANCODE_2:board[clickRow][clickCol].setType(1);
		      break;
		    case SDL_SCANCODE_3:board[clickRow][clickCol].setType(2);
		      break;
		    case SDL_SCANCODE_4:board[clickRow][clickCol].setType(3);
		      break;
		    case SDL_SCANCODE_5:board[clickRow][clickCol].setType(4);
		      break;
		    case SDL_SCANCODE_6:board[clickRow][clickCol].setType(5);
		      break;
		    case SDL_SCANCODE_B:board[clickRow][clickCol].changeBoostMode(1);
		      break;
		    case SDL_SCANCODE_H:board[clickRow][clickCol].changeBoostMode(2);
		      break;
		    case SDL_SCANCODE_V:board[clickRow][clickCol].changeBoostMode(3);
		      break;
		    case SDL_SCANCODE_Z:board[clickRow][clickCol].changeBoostMode(4);
		      break;
		    case SDL_SCANCODE_N:board[clickRow][clickCol].changeBoostMode(0);
		      break;
		    default:
		      //do nothing
		      printf("wrong key\n");
		    }

		  mouse_x = mouse_y = -1;
		}
	      break;




	    }
	}

      //draw board grid - is a grid necessary?
      game.drawBoard();

      game.handleLeftMouseClick(mouse_x, mouse_y);

      game.updateBoard();

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
  
  //Free the sound effects
  Mix_FreeChunk( zapSound );
  Mix_FreeChunk( bombSound );
  Mix_FreeChunk( matchSound );

  //free textures
  SDL_DestroyTexture(gems[0]);
  SDL_DestroyTexture(gems[1]);
  SDL_DestroyTexture(gems[2]);
  SDL_DestroyTexture(gems[3]);
  SDL_DestroyTexture(gems[4]);
  SDL_DestroyTexture(gems[5]);
  SDL_DestroyTexture(empty_gem);
  SDL_DestroyTexture(electric);
  SDL_DestroyTexture(explosion);
  SDL_DestroyTexture(fire);
  SDL_DestroyTexture(zap);
  SDL_DestroyTexture(background);
  
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

  //Initialize SDL_mixer
  if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
      return false;
    }

  loadTextures();
  
  loadSounds();
  
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
  electric = loadImage("img/electric.png");
  explosion = loadImage("img/explosion.png");
  fire = loadImage("img/fireball_75.png");
  zap = loadImage("img/zap_sheet.png");
  background = loadImage("img/background.png");
}
void loadSounds()
{
    //Load the sound effects
    zapSound = Mix_LoadWAV(
	     "sounds/136542__joelaudio__electric-zap-001.wav");
    bombSound = Mix_LoadWAV(
	     "sounds/366093__benjaminharveydesign__manhole-blows-metallic-boom.wav");
    matchSound = Mix_LoadWAV(
	     "sounds/346404__robinhood76__06698-gem-collect-ding.wav");
    shuffleSound = Mix_LoadWAV(
	     "sounds/153583__sheepfilms__8-bit-beeping-computer-sounds-edit.wav");
    
    //If there was a problem loading the sound effects
    if( zapSound == NULL or
	bombSound == NULL or
	matchSound == NULL or
	shuffleSound == NULL)
    {
      printf("error loading sounds\n");
      exit(-1);
    }
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

