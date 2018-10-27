/* TODO
 *
 * Tile class assignment operator
 * Tile class << operator
 *
 */

#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "string"
#include "globals.h"
#include "tile.h"
#include "game.h"

using namespace std;

// bool loop();
bool initSDL();
void quitSDL();

int main(int argc, char **args)
{

	if (!initSDL()) //if we cant init, end main
	{
		return 1;
	}
	bool done = false;
	int mouse_x = -1, mouse_y = -1;
	/* dies */ Game game; //start game stuff
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

		game.checkForMatches();

		game.drawHighlightBox();

		// Update window
		SDL_RenderPresent(renderer);

		//reset mouse position
		mouse_x = mouse_y = -1;

		SDL_Delay(20);
	}

	return 0;
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
	window = SDL_CreateWindow("Grid",
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

	//black out the screen
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	return true;
}

void quitSDL()
{
	// Quit - memory clean up
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

/*
bool loop() {
  //update SDLs event array
  //SDL_PumpEvents();
  
  static int mouse_x, mouse_y, clickRow = -1, clickCol = -1;
  //coords for Tile selection box
  static int box_x = -200;
  static int box_y = -200;
  //number of matches in the given direction
  static int left, right, above, below;
  
  // Event loop
  while ( SDL_PollEvent( &e ) != 0 ) 
    {
      switch ( e.type )
	{
	case SDL_QUIT:
	  //close button
	  return false;
	  
	case SDL_MOUSEBUTTONDOWN:
	  if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) )
	    {//grab mouse position on left click
	      SDL_GetMouseState(&mouse_x, &mouse_y);
	    }
	  break;
	  
	case SDL_KEYDOWN:
	  if(e.key.keysym.sym == SDLK_SPACE)
	    {//display board info
	      printBoard();
	    }
	}
    }
  
  drawBoard();//draw grid & Tiles
  
  
  if (validLeftMouseClick(mouse_x, mouse_y) )
    {
      //bool matchMade1 = false, matchMade2 = false;
      
      clickCol = mouse_x / GRID_SIZE;
      clickRow = mouse_y / GRID_SIZE;    
      
      if(activeTile.getRow() >= 0 and 
         activeTile.getCol() >= 0 and
         board[activeTile.getRow()][activeTile.getCol()]
	 .isAdjacent( board[clickRow][clickCol] ) )
	
	{//swap
	  //printf("just before swap()\n");
	  int matches = swapAndCheck(board[activeTile.getRow()][activeTile.getCol()],
				     board[clickRow][clickCol]);
	  
	  switch(matches)
	    {
            case 0: //no match - handled by swapAndCheck
	      //printf("loop(): match == 0\n");
	      break;
            case 1: //match occured at coords of first arg
	      //printf("loop(): match == 1\n");
	      match(board[activeTile.getRow()][activeTile.getCol()]);
	      break;
            case 2: //match occured at coords of second arg
	      //printf("loop(): match == 2\n");
	      match(board[clickRow][clickCol]);
	      break;
            case 3: //match occured at coords of both args
	      //printf("loop(): match == 3\n");
	      match(board[activeTile.getRow()][activeTile.getCol()]);
	      match(board[clickRow][clickCol]);
	      break;
            default:
	      printf("Error: switch(matches) not 0, 1, 2, 3\n");
	      exit(-1);
	    }
	  //do not draw highlight box after second click
	  box_x = box_y = -200;
	  
	  //reset activeTile stats
	  activeTile.setRow(-5);
	  activeTile.setCol(-5);
	}
      else
	{ //click was not adjacent, reset activeTile to current click
	  activeTile.setRow(clickRow);
	  activeTile.setCol(clickCol);
	  activeTile.setType(board[clickRow][clickCol].getType());
	  
	  //pixel coordinates for activeTile highlighting
	  box_x = clickCol * GRID_SIZE;
	  box_y = clickRow * GRID_SIZE;
	}    
    }
  
  drawHighlightBox(box_x, box_y);
  // Update window
  SDL_RenderPresent( renderer );
  
  //reset 
  mouse_x = mouse_y = -1;
  
  return true;
}
*/
