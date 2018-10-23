#include "iostream"
#include "stdlib.h"
#include "time.h"
#include "tile.h"

using namespace std;

/*
char* matchCheck(int refRow, int refCol, int refType)
{//checks the Tile passed for a match
 //results is a string containing ints of the number of matches in a direction
 // "<left> <right> <above> <below>"
  
  int leftMatches, rightMatches, aboveMatches, belowMatches, varRow, varCol, type;
  static char matchString[10];
  char intString[15];
  
  printf("checking for a ");
  switch( board[refRow][refCol].getType())
    {
    case 0:
      printf("red ");
      break;
    case 1:
      printf("yellow ");
      break;
    case 2:
      printf("pink ");
      break;
    case 3:
      printf("green ");
      break;
    case 4:
      printf("clear ");
      break;
    case 5:
      printf("blue ");
      break;
    default:
      printf("\ntype is not 0-5!!\n");
    }
  printf("Tile\n");
  fflush(stdout);





  strcpy(matchString, "");//reset
  leftMatches = rightMatches = aboveMatches = belowMatches = 0;
  varRow = refRow;
  varCol = refCol;
  type = refType;

  //currTile is board[row][col]

  while(varCol > 0 and board[varRow][varCol - 1].getType() == type)
    {//while left matches
      leftMatches++;
      varCol--;
    }
  varCol = refCol;

  while(varCol < BOARD_SIZE - 1 and board[varRow][varCol + 1].getType() == type)
    {//while right matches
      rightMatches++;
      varCol++;
    }
  varCol = refCol;

  while(varRow > 0 and board[varRow - 1][varCol].getType() == type)
    {//while above matches
      aboveMatches++;
      varRow--;
    }
  varRow = refRow;

  while(varRow < BOARD_SIZE - 1 and board[varRow + 1][varCol].getType() == type)
    {//while below matches
      belowMatches++;
      varRow++;
    }
  varRow = refRow;

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

  cout << "match string for Tile at\n\trow: " << varRow << "\n\tcol: " << varCol;
  cout << endl << matchString << endl;

  return matchString;

}
*/


char* matchCheck(Tile currTile)
{//checks the Tile passed for a match
 //results is a string containing ints of the number of matches in a direction
 // "<left><right><above><below>"
  
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

  cout << "match string for Tile at\n\trow: " << row << "\n\tcol: " << col;
  cout << endl << matchString << endl;

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
	  tex = gems[gemType].tex;

	  if(board[i][j].getCol() == activeTile.getCol() and
	     board[i][j].getRow() == activeTile.getRow() and
	     board[i][j].getType() == activeTile.getType() )
	    {//current tile is 'active' - animate it
	      //printf("acttive tile is ");
          //printTileColor(i, j);
          //printf("\n");          

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
	    {//
	      src = {0,
		     0,
		     GEM_SIZE,
		     GEM_SIZE};
	    }
	  
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
  drawTiles();
  
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


  float aDstX = a.getDstX();
  float aDstY = a.getDstY();
  float bDstX = b.getDstX();
  float bDstY = b.getDstY();
  float aDeltaX, aDeltaY, bDeltaX, bDeltaY;
  float delta = 2.5;
  bool matchMade1 = false, matchMade2 = false;
  char* matchString;
  int left, right, above, below;
  
  printf("\nstart of swap()\n---------------\n");
  fflush(stdout);

//
//


  if(aDstY == bDstY)
    {
      if(aDstX > bDstX)
	    {//a left of b
	      printf("a right of b\n");
	      fflush(stdout);
	      aDeltaX = -delta;
	      aDeltaY = 0;
	      bDeltaX = delta;
	      bDeltaY = 0;
	    }
      else if(aDstX < bDstX)
	    {//a right of b
	      printf("a left of b\n");
	      fflush(stdout);
	      aDeltaX = delta;
	      aDeltaY = 0;
	      bDeltaX = -delta;
	      bDeltaY = 0;
	    }
      
      while(board[a.getRow()][a.getCol()].getDstX() != bDstX and
	    board[b.getRow()][b.getCol()].getDstX() != aDstX)
	    {//visual swap
	      printf("swap x loop\n");
	      fflush(stdout);

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
	      printf("a below b\n");
	      fflush(stdout);
	      aDeltaX = 0;
	      aDeltaY = -delta;
	      bDeltaX = 0;
	      bDeltaY = delta;
	    }
      else if(aDstY < bDstY)
	    {//a above b
	      printf("a above b\n");
	      fflush(stdout);
	      aDeltaX = 0;
	      aDeltaY = delta;
	      bDeltaX = 0;
	      bDeltaY = -delta;
	    }

      while(board[a.getRow()][a.getCol()].getDstY() != bDstY and
	    board[b.getRow()][b.getCol()].getDstY() != aDstY)
	    {//visual swap
	      printf("swap y loop\n");
	      fflush(stdout);

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

//
//


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
  
  //redraw  
  //drawBoard();
  //SDL_RenderPresent( renderer );

  //SDL_Delay(500);

  //check match for first Tile
  matchString = matchCheck(board[a.getRow()][a.getCol()]);
  sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);
  printf("matchCheck() on a\n-----------------\n");
  printf("\tleft: %d\n\tright: %d\n\tabove: %d\n\tbelow: %d\n\n",
            left, right, above, below);

  if( (left + right) >= 2 or (above + below) >= 2)
    { printf("matchMade1 is true\n"); matchMade1 = true; }

  //check match for second Tile
  matchString = matchCheck(board[b.getRow()][b.getCol()]);
  sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);
  printf("matchCheck() on b\n-----------------\n");
  printf("\tleft: %d\n\tright: %d\n\tabove: %d\n\tbelow: %d\n\n",
            left, right, above, below);

  if( (left + right) >= 2 or (above + below) >= 2)
    { printf("matchMade2 is true\n"); matchMade2 = true; }

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
           board[b.getRow()][b.getCol()]);

      return 0;

    }

/*
  //any matches made by putting b where a is?
  matchString = matchCheck(a.getRow(), a.getCol(), a.getType() );
  sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);

    printf("first check for swap\n");
    printf("\tleft: %d\n\tright: %d\n\tabove: %d\n\tbelow: %d\n\n",
            left, right, above, below);


    printf("left (%d) + right (%d) = %d\n", left, right, left + right);
    printf("above (%d) + below (%d) = %d\n", above, below, above + below);
  if(left + right >= 2 or above + below >= 2)
    { printf("matchMade1 is true\n"); matchMade1 = true; }

  //any matches made by putting a where b is?
  matchString = matchCheck(b.getRow(), b.getCol(), a.getType() );
  sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);

    printf("second check for swap\n");
    printf("\tleft: %d\n\tright: %d\n\tabove: %d\n\tbelow: %d\n\n",
            left, right, above, below);

    printf("left (%d) + right (%d) = %d\n", left, right, left + right);
    printf("above (%d) + below (%d) = %d\n", above, below, above + below);
  if(left + right >= 2 or above + below >= 2)
    { printf("matchMade2 is true\n"); matchMade2 = true; }  
	    
  if(matchMade1)
    {//match was made!
      printf("matchMade1 is true\n");
      return true;
    }
    else if(matchMade2)
    {
      printf("matchMade1 is true\n");
      return true;
    }
  else
    {//no match made... swap back, return false;
      printf("no match made");
      //swap back
      swap(a, b);
      return false;
    }

*/
}





void swap(Tile a, Tile b)
{
  float aDstX = a.getDstX();
  float aDstY = a.getDstY();
  float bDstX = b.getDstX();
  float bDstY = b.getDstY();
  float aDeltaX, aDeltaY, bDeltaX, bDeltaY;
  float delta = 2.5;
  char* matchString;
  bool matchMade1, matchMade2;
  int left, right, above, below;
  
  printf("\nstart of swap()\n---------------\n");
  fflush(stdout);
  //printf("a dst:\n\tx: %f\n\ty: %f\n", aDstX, aDstY);
  //printf("b dst:\n\tx: %f\n\ty: %f\n", bDstX, bDstY);

  if(aDstY == bDstY)
    {
      if(aDstX > bDstX)
	    {//a left of b
	      printf("a right of b\n");
	      fflush(stdout);
	      aDeltaX = -delta;
	      aDeltaY = 0;
	      bDeltaX = delta;
	      bDeltaY = 0;
	    }
      else if(aDstX < bDstX)
	    {//a right of b
	      printf("a left of b\n");
	      fflush(stdout);
	      aDeltaX = delta;
	      aDeltaY = 0;
	      bDeltaX = -delta;
	      bDeltaY = 0;
	    }
      
      while(board[a.getRow()][a.getCol()].getDstX() != bDstX and
	    board[b.getRow()][b.getCol()].getDstX() != aDstX)
	    {//visual swap
	      printf("swap x loop\n");
	      fflush(stdout);

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
	      printf("a below b\n");
	      fflush(stdout);
	      aDeltaX = 0;
	      aDeltaY = -delta;
	      bDeltaX = 0;
	      bDeltaY = delta;
	    }
      else if(aDstY < bDstY)
	    {//a above b
	      printf("a above b\n");
	      fflush(stdout);
	      aDeltaX = 0;
	      aDeltaY = delta;
	      bDeltaX = 0;
	      bDeltaY = -delta;
	    }

      while(board[a.getRow()][a.getCol()].getDstY() != bDstY and
	    board[b.getRow()][b.getCol()].getDstY() != aDstY)
	    {//visual swap
	      printf("swap y loop\n");
	      fflush(stdout);

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

  //redraw  
  drawBoard();
  SDL_RenderPresent( renderer );

}




/* animate swapping of positions



  if(aDstY == bDstY)
    {
      if(aDstX > bDstX)
	{//a left of b
	  printf("a right of b\n");
	  fflush(stdout);
	  aDeltaX = -delta;
	  aDeltaY = 0;
	  bDeltaX = delta;
	  bDeltaY = 0;
	}
      else if(aDstX < bDstX)
	{//a right of b
	  printf("a left of b\n");
	  fflush(stdout);
	  aDeltaX = delta;
	  aDeltaY = 0;
	  bDeltaX = -delta;
	  bDeltaY = 0;
	}
      
      while(board[a.getRow()][a.getCol()].getDstX() != bDstX and
	    board[b.getRow()][b.getCol()].getDstX() != aDstX)
	{//visual swap
	  printf("swap x loop\n");
	  fflush(stdout);

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
	  printf("a below b\n");
	  fflush(stdout);
	  aDeltaX = 0;
	  aDeltaY = -delta;
	  bDeltaX = 0;
	  bDeltaY = delta;
	}
      else if(aDstY < bDstY)
	{//a above b
	  printf("a above b\n");
	  fflush(stdout);
	  aDeltaX = 0;
	  aDeltaY = delta;
	  bDeltaX = 0;
	  bDeltaY = -delta;
	}

      while(board[a.getRow()][a.getCol()].getDstY() != bDstY and
	    board[b.getRow()][b.getCol()].getDstY() != aDstY)
	{//visual swap
	  printf("swap y loop\n");
	  fflush(stdout);

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

*/
