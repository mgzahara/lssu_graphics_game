/* TODO
 *
 * Tile class assignment operator
 * Tile class << operator
 */


#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "basics.h"
#include "tile.h"
#include "string"

using namespace std;

bool loop();
void drawBoard();
void swapTilePos(int, int, int, int);
void drawHighlightBox(int, int);

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
  //the real main
  //handles events, drawing
  //returns false when end con is met
  
  //both are used to limit screen refreshes further
  //static const int pause = 8;
  //static int frameCounter = 0;

  //update SDLs event array
  SDL_PumpEvents();
  
  static int mouse_x, mouse_y, clickRow = -1, clickCol = -1;
  //coords for Tile selection box
  static int box_x = -BOX_SIZE * 2;
  static int box_y = -BOX_SIZE * 2;
  //number of matches in the given direction
  static int left, right, above, below;

  static char* matchString;
  
  // Event loop
  while ( SDL_PollEvent( &e ) != 0 ) 
  {
    switch ( e.type ) {//no events
    case SDL_QUIT:
      //close button
      return false;

    case SDL_MOUSEBUTTONDOWN:
      if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) )
      {//grab mouse position on left click
        SDL_GetMouseState(&mouse_x, &mouse_y);
      }
      break;
    }
  }

  drawBoard();//draw grid & Tiles
  
  if( mouse_x > 0 and mouse_y > 0 and
      mouse_x % GRID_SIZE > BOX_SIZE and
      mouse_x % GRID_SIZE < GRID_SIZE - BOX_SIZE and
      mouse_y % GRID_SIZE > BOX_SIZE and
      mouse_y % GRID_SIZE < GRID_SIZE - BOX_SIZE and
      mouse_x < (BOARD_SIZE * GRID_SIZE) and
      mouse_y < (BOARD_SIZE * GRID_SIZE) )
  {

    clickCol = mouse_x / GRID_SIZE;
    clickRow = mouse_y / GRID_SIZE;

    if( activeTile.isAdjacent( board[clickRow][clickCol] ) )
      {//swap
	//right now the swap is as easy as switching the types of the Tiles
	//eventually it will need to be animated - multithreaded? child process?
	bool matchMade1 = false, matchMade2 = false;
	//swap Tiles, check for match
	swapTilePos(clickRow, clickCol, activeTile.getRow(), activeTile.getCol() );

	//ONLY DISPLAYING MATCHSTRING FOR SECOND TILE CLICKED
	matchString = matchCheck(board[clickRow][clickCol]);
	sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);

	//	printf("ints\n\tleft: %d\n\tright: %d\n\tabove: %d\n\tbelow: %d\n",
	//	       left, right, above, below);
	//	fflush(stdout);
	

	if(left + right >= 2 or above + below >= 2)
	  { matchMade1 = true; }

	matchString = matchCheck(board[activeTile.getRow()][activeTile.getCol()]);
	sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);

	if(left + right >= 2 or above + below >= 2)
	  { matchMade2 = true; }

	
	
	if(!matchMade1 and !matchMade2)
	  {//match was unsuccessful, swap back
	    printf("No Match...\n");
	    SDL_Delay(300);
	    swapTilePos(clickRow,
			clickCol,
			activeTile.getRow(),
			activeTile.getCol() );
	  }
	else
	  { printf("Match Made!!!\n"); }

	//reset activeTile stats
	activeTile.setRow(-2);
	activeTile.setCol(-2);
	activeTile.setType(0);
      }
    else
      { //click was not adjacent, reset activeTile to current click
	activeTile.setRow(board[clickRow][clickCol].getRow() );
	activeTile.setCol(board[clickRow][clickCol].getCol() );
	activeTile.setType(board[clickRow][clickCol].getType() );
	
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
  //reset activeTile highlight box - not working?
  if(activeTile.getRow() < 0 and activeTile.getCol() < 0)
    { box_x = box_y = -200; }
  
  return true;
}

void drawBoard()
{
  //black background
  SDL_SetRenderDrawColor(renderer, c.bla.r, c.bla.g, c.bla.b, 255);
  SDL_RenderClear(renderer);
  
  //draw a grid in maroon
  SDL_SetRenderDrawColor(renderer, c.mar.r, c.mar.g, c.mar.b, 255);
  for(int i = 1; i <= (WINDOW_WIDTH / GRID_SIZE); i++){
    //draw vertical lines
    SDL_RenderDrawLine(renderer, i * GRID_SIZE, 0, i * GRID_SIZE, WINDOW_HEIGHT);
  }
  for(int i = 1; i <= (WINDOW_HEIGHT / GRID_SIZE); i++){
    //draw horizontal lines
    SDL_RenderDrawLine(renderer, 0, i * GRID_SIZE, WINDOW_WIDTH, i * GRID_SIZE);
  }
  //draw all tiles
  drawTiles();
  
}

void swapTilePos(int row1, int col1, int row2, int col2)
{
  int temp = board[row1][col1].getType();
  board[row1][col1].setType(board[row2][col2].getType());
  board[row2][col2].setType(temp);
}

void drawHighlightBox(int box_x, int box_y)
{
  SDL_Rect top, right, bot, left;
  top = {box_x,
	 box_y,
	 GRID_SIZE,
	 BOX_SIZE};
  right = {box_x + GRID_SIZE - BOX_SIZE,
	   box_y + 1,
	   BOX_SIZE,
	   GRID_SIZE - 1};
  bot = {box_x + 1,
	 box_y + GRID_SIZE - BOX_SIZE,
	 GRID_SIZE - 1,
	 BOX_SIZE};
  left ={box_x,
	 box_y,
	 BOX_SIZE,
	 GRID_SIZE};

  //draw selection box
  SDL_SetRenderDrawColor(renderer, c.gre.r, c.gre.g, c.gre.b, 255);
  SDL_RenderFillRect(renderer, &top);
  SDL_RenderFillRect(renderer, &right);
  SDL_RenderFillRect(renderer, &bot);
  SDL_RenderFillRect(renderer, &left);
  
}
