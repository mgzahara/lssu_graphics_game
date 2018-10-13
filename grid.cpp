#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "basics.h"
#include "tile.h"

using namespace std;

bool loop();
void startBoard();


int main(int argc, char** args) {

/*  Tile t;
  t.setCol(1);
  t.setRow(2);
  t.setType(3);
*/
  //R = (rand() % (upper - lower + 1)) + lower;


  if ( !initSDL() ) return 1;//if we cant init, end main
  
  startBoard();
  
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
  
  //both are used to limit screen refreshes further
  //static const int pause = 8;
  //static int frameCounter = 0;
  SDL_PumpEvents();
  
  static int mouse_x, mouse_y, clickRow = -1, clickCol = -1;
  //box selection base x, y
  static int box_x = -BOX_SIZE * 2;
  static int box_y = -BOX_SIZE * 2;
  static SDL_Rect top, right, bot, left;//SDL_Rect's for box selection
  
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
        //when this is all OOPed, the args to the above func
        //will be data members of an instance of a class

        // handleLeftMouseClick(mouse_x, mouse_y);

      }
      break;
    }
  }

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
    //wont be suitable for animating the swapping of tiles,
    //but i'll figure that out later - simple check against values of activeTile and swappingTile

  if( mouse_x > 0 and mouse_y > 0 and
      mouse_x % GRID_SIZE > BOX_SIZE and
      mouse_x % GRID_SIZE < GRID_SIZE - BOX_SIZE and
      mouse_y % GRID_SIZE > BOX_SIZE and
      mouse_y % GRID_SIZE < GRID_SIZE - BOX_SIZE )
  {
    printf("inside restrictive box click if()\n");
    //ignore clicks if they are within 3 pixels of a grid line
    //snap click to an int of row num and col num
    //indices into board array
    clickCol = mouse_x / GRID_SIZE;
    clickRow = mouse_y / GRID_SIZE;
    //pixel coordinates for activeTile highlighting
    box_x = clickCol * GRID_SIZE;
    box_y = clickRow * GRID_SIZE;

    Tile newlyClicked;
    newlyClicked.setRow(  board[clickRow][clickCol].getRow()  );
    newlyClicked.setCol(  board[clickRow][clickCol].getCol()  );
    newlyClicked.setType( board[clickRow][clickCol].getType() );

    printf("after newlyClicked assignment\n");

    if( activeTile.isAdjacent( newlyClicked ) )
    {//swap
      printf("\n\n-------------\n");
      printf("activeTile.isAdjacent()\n");
      printf("box_x: %d\n", box_x);
      printf("box_y: %d\n", box_y);
      printf("clickRow: %d\n", clickRow);
      printf("clickCol: %d\n", clickCol);
      printf("board[clickRow][clickCol]");
      printf("actual row:  %d\n", newlyClicked.getRow() );
      printf("actual col:  %d\n", newlyClicked.getCol() );
      printf("actual type: %d\n", newlyClicked.getType() );

      switch( newlyClicked.getType() )
      {//print color
        case 1: printf("color: white\n");//w
          break;
        case 2: printf("color: red\n");//r
          break;
        case 3: printf("color: purple\n");//p
          break;
        case 4: printf("color: blue\n");//b
          break;
        case 5: printf("color: orange\n");//o
          break;
        default: printf("no valid color type :(\n");
          break;
      }

      //right now the swap is as easy as switching the types of the Tiles
      //eventually it will need to be animated - multithreaded? child process?

      Tile temp;
      temp.setRow(newlyClicked.getRow() );
      temp.setCol(newlyClicked.getCol() );
      temp.setType(newlyClicked.getType() );

      board[clickRow][clickCol].setType( activeTile.getType() );//curr click Tile = activeTile

      board[activeTile.getRow()]
        [activeTile.getCol()]
        .setType( temp.getType() );//activeTIle = curr click Tile
      
      //reset activeTile stats
      activeTile.setRow(-2);
      activeTile.setCol(-2);
      activeTile.setType(0);

      // //reset activeTile highlight box - not working?
      top.x = -200;
      top.y = -200;
      bot.x = -200;
      bot.y = -200;
      left.x = -200;
      left.y = -200;
      right.x = -200;
      right.y = -200;
    }
    else
    { 
      printf("\nbefore activeTile reassignment\n");
      activeTile.setRow(newlyClicked.getRow() );
      activeTile.setCol(newlyClicked.getCol() );
      activeTile.setType(newlyClicked.getType() );

      printf("\n\n-------------\nNOT activeTile.isAdjacent()\n");
      
      printf("activeTile row:  %d\n", activeTile.getRow() );
      printf("activeTile col:  %d\n", activeTile.getCol() );
      printf("activeTile type: %d\n", activeTile.getType() );
    }
    
    //printf("mouse_x: %d\nmouse_y: %d\n---------\n", mouse_x, mouse_y);
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
  }


  //draw selection box
  SDL_SetRenderDrawColor(renderer, c.gre.r, c.gre.g, c.gre.b, 255);
  SDL_RenderFillRect(renderer, &top);
  SDL_RenderFillRect(renderer, &right);
  SDL_RenderFillRect(renderer, &bot);
  SDL_RenderFillRect(renderer, &left);

  // Update window
  SDL_RenderPresent( renderer );
  //reset 
  mouse_x = mouse_y = -1;
  return true;
}

void startBoard()
{
  //board is [row][col]
  //col is inc inside inner loop, so the filling order is as follows
    //(0, 0) then the entire first row gets filled left to right
    //then the row is inc and it repeats
//this means that there will never be Tiles to the right or below
//as tiles are filled, check to the left and above for matches
//if match was created, chose a diffrent type so as to not create a match


  int lower = 1;
  int upper = 5;

  for(int i = 0; i < BOARD_SIZE; i++)
    {//row
      for(int j = 0; j < BOARD_SIZE; j++)
	{//col
	  board[i][j].setRow(i);
	  board[i][j].setCol(j);
	  board[i][j].setType((rand() % (upper - lower + 1)) + lower);

	  if(i >= 2)
	    {//on/past row 2, check 2 above for matching type
	      bool aboveMatch = false;
	      bool leftMatch = false;
	      if(board[i - 1][j].getType() == board[i - 2][j].getType() )
		{
		  aboveMatch = true;
		}
	      if(j >= 2)
		{//on/past col 2, check 2 to the left for matching type
		  if(board[i][j - 1].getType() == board[i][j - 2].getType() )
		    {
		      leftMatch = true;
		    }
		}
	      //if above or left have matches AND current type matches either
	      ///randomize until it doesnt
	      if(aboveMatch and leftMatch)
		{//both are a match
		  while(board[i][j].getType() == board[i - 1][j].getType() or
			board[i][j].getType() == board[i][j - 1].getType() )
		    {//pick a new type while it makes a match with above or left
		      board[i][j].setType((rand() % (upper - lower + 1)) + lower);
		    }
		}
	      else if(aboveMatch)
		{//only above is a match
		  while(board[i][j].getType() == board[i - 1][j].getType() )
		    {//pick a new type while it makes a match with above
		      board[i][j].setType((rand() % (upper - lower + 1)) + lower);
		    }
		}
	      else if(leftMatch)
		{//only left is a match
		  while(board[i][j].getType() == board[i][j - 1].getType() )
		    {//pick a new type while it makes a match with left
		      board[i][j].setType((rand() % (upper - lower + 1)) + lower);
		    }
		}
	    }//end if i >= 2
	  else
	    {
	      if(j >= 2)
		{//on/past col 2, check 2 to the left for matching type
		  bool leftMatch = false;
		  if(board[i][j - 1].getType() == board[i][j - 2].getType() )
		    {
		      leftMatch = true;
		    }
		  if(leftMatch)
		    {
		      while(board[i][j].getType() == board[i][j - 1].getType() )
			{//pick a new type while it makes a match with left
			  board[i][j].setType((rand() % (upper - lower + 1)) + lower);
			} 
		    }
		}//end if j >= 2
	    }//end else
	}//end inner for loop
    }//end outer for loop
  //printBoard();
}
