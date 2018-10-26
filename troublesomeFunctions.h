#include "iostream"
#include "stdlib.h"
#include "time.h"
#include "tile.h"

using namespace std;


char* matchCheck(Tile currTile)
{//checks the Tile passed for a match
 //results is a string containing ints of the number of matches in a direction
 // "<left> <right> <above> <below>"
  
  int leftMatches, rightMatches, aboveMatches, belowMatches, row, col, type;
  static char matchString[10];
  char intString[15];
  
  strcpy(matchString, "");//reset
  leftMatches = rightMatches = aboveMatches = belowMatches = 0;
  row = currTile.getRow();
  col = currTile.getCol();
  type = currTile.getType();

  //currTile is board[row][col]

  while(col > 0 and board[row][col - 1].getType() == type)
    {//while left matches
      leftMatches++;
      col--;
    }
  col = currTile.getCol();


  while(col < BOARD_SIZE - 1 and board[row][col + 1].getType() == type)
    {//while right matches
      rightMatches++;
      col++;
    }
  col = currTile.getCol();

  while(row > 0 and board[row - 1][col].getType() == type)
    {//while above matches
      aboveMatches++;
      row--;
    }
  row = currTile.getRow();


  while(row < BOARD_SIZE - 1 and board[row + 1][col].getType() == type)
    {//while below matches
      belowMatches++;
      row++;
    }
  row = currTile.getRow();

  //put together return string
  sprintf(intString, "%d", leftMatches);
  strcat(matchString, intString);
  strcat(matchString, " ");
  
  sprintf(intString, "%d", rightMatches);
  strcat(matchString, intString);
  strcat(matchString, " ");
  
  sprintf(intString, "%d", aboveMatches);
  strcat(matchString, intString);
  strcat(matchString, " ");
  
  sprintf(intString, "%d", belowMatches);
  strcat(matchString, intString);

  //  cout << "match string for Tile at\n\trow: " << row << "\n\tcol: " << col;
  //  cout << endl << matchString << endl;

  return matchString;

}



void drawTiles()
{//draw all tiles

  //

  SDL_Texture* tex = NULL;
  SDL_Rect src;
  SDL_Rect dst;
  
  for(int i = 0; i< BOARD_SIZE; i++)
    {
      for(int j = 0; j < BOARD_SIZE; j++)
	{
	  int gemType = board[i][j].getType();
	  if(gemType == -1)
	    {//empty gem
	      tex = empty_gem.tex;
	      src = {0, 0, GEM_SIZE, GEM_SIZE};
	    }
	  else
	    {
	      tex = gems[gemType].tex;
	      
	      if(board[i][j].getCol() == activeTile.getCol() and
		 board[i][j].getRow() == activeTile.getRow() and
		 board[i][j].getType() == activeTile.getType() )
		{//current tile is 'active' - animate it
		  
		  gems[gemType].spriteCounter++;
		  
		  src = {( ( gems[gemType].spriteCounter % 
			     gems[gemType].endSprite ) + 
			   gems[gemType].startSprite ) * 
			 GEM_SIZE,
			 0,
			 GEM_SIZE,
			 GEM_SIZE};
		}
	      //right here check if tile is special
	      //add effects for special tiles
	      else
		{//current tile is static
		  src = {0,
			 0,
			 GEM_SIZE,
			 GEM_SIZE};
		}
	      
	    }//end if type == -1
	  dst = {(int)board[i][j].getDstX(),
		 (int)board[i][j].getDstY(),
		 GEM_SIZE,
		 GEM_SIZE};
	  
	  SDL_RenderCopy(renderer, tex, &src, &dst);
	}
    }  
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
  //drawTiles();
  
}

void swapTilePos(int row1, int col1, int row2, int col2)
{
/*
  printf("color of tile at row %d and col %d is ", row1, col1);
  printTileColor(row1, col1);
  printf("\n");

  printf("color of tile at row %d and col %d is ", row2, col2);
  printTileColor(row2, col2);
  printf("\n");

  printf("\nswapping\n");
*/
  int temp = board[row1][col1].getType();
  board[row1][col1].setType(board[row2][col2].getType());
  board[row2][col2].setType(temp);

/*
  printf("color of tile at row %d and col %d is ", row1, col1);
  printTileColor(row1, col1);
  printf("\n");

  printf("color of tile at row %d and col %d is ", row2, col2);
  printTileColor(row2, col2);
  printf("\n");
*/
}


int swapAndCheck(Tile a, Tile b)
{
//return values
//0 - no matches
//1 - first arg match
//2 - second arg match
//3 - both arg matches

  bool matchMade1 = false, matchMade2 = false;
  char* matchString;
  int left, right, above, below;
  
  swap(a, b, false);

  //check match for first Tile
  matchString = matchCheck(board[a.getRow()][a.getCol()]);
  sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);
  //printf("matchCheck() on a\n-----------------\n");
  //printf("\tleft: %d\n\tright: %d\n\tabove: %d\n\tbelow: %d\n\n",
  //left, right, above, below);

  if( (left + right) >= 2 or (above + below) >= 2)
    { //printf("matchMade1 is true\n");
      matchMade1 = true; }

  //check match for second Tile
  matchString = matchCheck(board[b.getRow()][b.getCol()]);
  sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);
  //printf("matchCheck() on b\n-----------------\n");
  //printf("\tleft: %d\n\tright: %d\n\tabove: %d\n\tbelow: %d\n\n",
  //left, right, above, below);

  if( (left + right) >= 2 or (above + below) >= 2)
    { //printf("matchMade2 is true\n");
      matchMade2 = true; }

  if (matchMade1 && matchMade2)
    {//both matched
      return 3;
    }
  else if(matchMade2)
    {//only second arg matched
      return 2;
    }
  else if(matchMade1)
    {//only first arg matched
      return 1;
    }
  else
    {//no matches made
      SDL_Delay(500);
      swap(board[a.getRow()][a.getCol()], 
           board[b.getRow()][b.getCol()],
	   true);

      return 0;
    }
}


void swap(Tile a, Tile b, bool redraw)
{
  float aDstX = a.getDstX();
  float aDstY = a.getDstY();
  float bDstX = b.getDstX();
  float bDstY = b.getDstY();
  float aDeltaX, aDeltaY, bDeltaX, bDeltaY;
  float delta = 2.5;

  if(aDstY == bDstY)
    {
      if(aDstX > bDstX)
	    {//a left of b
	      //printf("a right of b\n");
	      //fflush(stdout);
	      aDeltaX = -delta;
	      aDeltaY = 0;
	      bDeltaX = delta;
	      bDeltaY = 0;
	    }
      else if(aDstX < bDstX)
	    {//a right of b
	      //printf("a left of b\n");
	      //fflush(stdout);
	      aDeltaX = delta;
	      aDeltaY = 0;
	      bDeltaX = -delta;
	      bDeltaY = 0;
	    }
      
      while(board[a.getRow()][a.getCol()].getDstX() != bDstX and
	    board[b.getRow()][b.getCol()].getDstX() != aDstX)
	    {//visual swap
	      //printf("swap x loop\n");
	      //fflush(stdout);

	      while ( SDL_PollEvent( &e ) != 0 ) 
	        {
	          switch ( e.type )
		        {
		          case SDL_QUIT:
		            exit(-1);
		            break;
		        }
	        }

	      board[a.getRow()][a.getCol()]
	        .setDstX( board[a.getRow()][a.getCol()]
		          .getDstX() + aDeltaX );
	      
	      board[b.getRow()][b.getCol()]
	        .setDstX( board[b.getRow()][b.getCol()]
		          .getDstX() + bDeltaX );
	      
	      drawBoard();
	      SDL_RenderPresent( renderer );
	      SDL_Delay(30);
	    }
    }
  else if(aDstX == bDstX)
    {    
      if(aDstY > bDstY)
	    {//a below b
	      //printf("a below b\n");
	      //fflush(stdout);
	      aDeltaX = 0;
	      aDeltaY = -delta;
	      bDeltaX = 0;
	      bDeltaY = delta;
	    }
      else if(aDstY < bDstY)
	    {//a above b
	      //printf("a above b\n");
	      //fflush(stdout);
	      aDeltaX = 0;
	      aDeltaY = delta;
	      bDeltaX = 0;
	      bDeltaY = -delta;
	    }

      while(board[a.getRow()][a.getCol()].getDstY() != bDstY and
	    board[b.getRow()][b.getCol()].getDstY() != aDstY)
	    {//visual swap
	      //printf("swap y loop\n");
	      //fflush(stdout);

	      while ( SDL_PollEvent( &e ) != 0 ) 
	        {
	          switch ( e.type )
		        {
		          case SDL_QUIT:
		            exit(-1);
		            break;
		        }
	        }

	      board[a.getRow()][a.getCol()]
	        .setDstY( board[a.getRow()][a.getCol()]
		          .getDstY() + aDeltaY );
	      
	      board[b.getRow()][b.getCol()]
	        .setDstY( board[b.getRow()][b.getCol()]
		          .getDstY() + bDeltaY );
	      
	      drawBoard();
	      SDL_RenderPresent( renderer );
	      SDL_Delay(30);
	    }
    }

  //swap logical positions
  swapTilePos(a.getRow(), a.getCol(), b.getRow(), b.getCol() );

  //reassign 'a' texture destinations
  board[a.getRow()][a.getCol()]
    .setDstX((float)(a.getCol() * GRID_SIZE + BOX_SIZE + 6) );

  board[a.getRow()][a.getCol()]
    .setDstY((float)(a.getRow() * GRID_SIZE + BOX_SIZE + 6) );

  //reassign 'b' texture destinations
  board[b.getRow()][b.getCol()]
    .setDstX((float)(b.getCol() * GRID_SIZE + BOX_SIZE + 6) );

  board[b.getRow()][b.getCol()]
    .setDstY((float)(b.getRow() * GRID_SIZE + BOX_SIZE + 6) );

  if(redraw)
    {
      //redraw  
      drawBoard();
      SDL_RenderPresent( renderer );
    }
}

void match(Tile curr)
{
  int left, right, above, below;
  char* matchString;

  matchString = matchCheck(board[curr.getRow()][curr.getCol()]);
  sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);

  //take care of the easy one first
  board[curr.getRow()][curr.getCol()].setType(-1);

  if((left + right) >= 2)
    {//is left-right the match?
      while(left > 0)
	{//remove all left
	  board[curr.getRow()][curr.getCol() - left].setType(-1);
	  left--;
	}
      while(right > 0)
	{//remove all right
	  board[curr.getRow()][curr.getCol() + right].setType(-1);
	  right--;
	}
    }
  if((above + below) >= 2)
    {//is above-below the match?
      while(above > 0)
	{//remove all above
	  board[curr.getRow() - above][curr.getCol()].setType(-1);
	  above--;
	}
      while(below > 0)
	{//remove all below
	  board[curr.getRow() + below][curr.getCol()].setType(-1);
	  below--;
	}
    }
}


void handleLeftMouseClick(int mouse_x, int mouse_y)
{
  if( validLeftMouseClick(mouse_x, mouse_y) )
    {
      //click was made within constraints
      int clickRow = mouse_y / GRID_SIZE;
      int clickCol = mouse_x / GRID_SIZE;

      if( shouldSwapTiles( clickRow, clickCol ) )
	{
	  //swap - tell tiles they are to move
	  int activeTileDir = 0, newClickDir = 0;

	  //get highlight box out of the way
	  box_x = box_y = -200;

	  /* directions
	     0 - ignore it
	     1 - go left
	     2 - go right
	     3 - go up
	     4 - go down
	   */

	  if( activeTile.getRow() == clickRow )
	    {
	      //swap in same row, left/right
	      if( activeTile.getCol() > clickCol )
		{
		  //activeTile col is larger - right of new click
		  activeTileDir = 1; //go left
		  newClickDir = 2; //go right
		}
	      else
		{
		  //activeTile col is smaller - left of new click
		  activeTileDir = 2; //go right
		  newClickCol = 1; //go left
		}
	    }//end if activeTile row == clickRow
	  else
	    {
	      //swap in same col, up/down
	      if( activeTile.getRow() > clickRow )
		{
		  //activeTile row is larger - below new click
		  activeTileDir = 3; //go up
		  newClickDir = 4; //go down
		}
	      else
		{
		  //activeTile row is smaller - above new click
		  activeTileDir = 4; //go down
		  newClickDir = 3; //go up
		}
	    }//end if activeTile row == clickRow else

	  //tell each Tile it is to swap, the direction, its new type after swapping
	  board[ activeTile.getRow() ][ activeTile.getCol() ]
	    .setSwapping(true,
			 activeTileDir,
			 board[ clickRow ][ clickCol ].getType() );

	  board[ clickRow ][ clickCol ]
	    .setSwapping(true,
			 newClickDir,
			 board[ activeTile.getRow() ][ activeTile.getCol() ].getType() );

	}//end if should swap tiles
      else
	{
	  //click was not adjacent, reset activeTile to current click

	  //tell all Tiles they are not active
	  for(int i = o; i < 8; i++)
	    {
	      for(int j = 0; j < 8; j++)
		{
		  board[i][j].setActive(false);
		}
	    }

	  //tell new active Tile it is active
	  board[ clickRow ][ clickCol ].setActive(true);

	  //update rederence Tile
	  activeTile.setRow( clickRow );
	  activeTile.setCol( clickCol );
	  activeTile.setType( board[ clickRow ][ clickCol ].getType() );
	  
	  //set pixel coords for activeTile highlighting
	  box_x = clickCol * GRID_SIZE;
	  box_y = clickRow * GRID_SIZE;
	  
	}//end if should swap tiles else
    }//end if valid mouse click
}
