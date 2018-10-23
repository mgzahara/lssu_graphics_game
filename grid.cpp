/* TODO
 *
 * Tile class assignment operator
 * Tile class << operator
 *
 * ACTIVE TILE IS WONKY
 * BECAUSE OF NEW SWAP()
 */


#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "tile.h"
#include "string"
#include "constants.h"
#include "goodFunctions.h"
#include "troublesomeFunctions.h"


using namespace std;

bool loop();

int main(int argc, char** args) {

  if ( !initSDL() ) return 1;//if we cant init, end main
  
  startBoard();
  
  while ( loop() ) {
    // wait before processing the next frame
    //prevents weird screen tearing
    SDL_Delay(20);
    }

  return 0;
}

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
      bool matchMade1 = false, matchMade2 = false;

      clickCol = mouse_x / GRID_SIZE;
      clickRow = mouse_y / GRID_SIZE;    
      
      if(activeTile.getRow() >= 0 and 
         activeTile.getCol() >= 0 and
         board[activeTile.getRow()][activeTile.getCol()]
	      .isAdjacent( board[clickRow][clickCol] ) )

	{//swap
	  printf("just before swap()\n");
	  int matches = swapAndCheck(board[activeTile.getRow()][activeTile.getCol()],
	  		board[clickRow][clickCol]);

      switch(matches)
        {
            case 0: //no match
                printf("loop(): match == 0\n");
                break;
            case 1: //match occured at coords of first arg
                printf("loop(): match == 1\n");
                break;
            case 2: //match occured at coords of second arg
                printf("loop(): match == 2\n");
                break;
            case 3: //match occured at coords of both args
                printf("loop(): match == 3\n");
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

