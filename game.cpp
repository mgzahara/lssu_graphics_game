#include "game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "globals.h"
#include "tile.h"

Game::Game()
{

  //first activeTile will have unreal row and col
  activeTile.setRow(-10);
  activeTile.setCol(-10);
  activeTile.setType(-1);
}
Game::~Game()
{
  return;
}

bool Game::validLeftMouseClick(int mouse_x, int mouse_y)
{
  return (mouse_x > 0) and (mouse_y > 0) and
    ((mouse_x % GRID_SIZE) > BOX_SIZE) and
    ((mouse_x % GRID_SIZE) < (GRID_SIZE - BOX_SIZE)) and
    ((mouse_y % GRID_SIZE) > BOX_SIZE) and
    ((mouse_y % GRID_SIZE) < (GRID_SIZE - BOX_SIZE)) and
    (mouse_x < (BOARD_SIZE * GRID_SIZE)) and
    (mouse_y < (BOARD_SIZE * GRID_SIZE));
}

bool Game::shouldSwapTiles(int clickRow, int clickCol)
{
  return (activeTile.getRow() >= 0) and
    (activeTile.getCol() >= 0) and
    (board[activeTile.getRow()][activeTile.getCol()]
     .isAdjacent(board[clickRow][clickCol]));
}

void Game::drawHighlightBox()
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
  left = {box_x,
          box_y,
          BOX_SIZE,
          GRID_SIZE};

  //draw selection box - green
  SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
  SDL_RenderFillRect(renderer, &top);
  SDL_RenderFillRect(renderer, &right);
  SDL_RenderFillRect(renderer, &bot);
  SDL_RenderFillRect(renderer, &left);
}

void Game::startBoard()
{
  //board is [row][col]
  //col is inc inside inner loop, so the filling order is as follows
  //(0, 0) then the entire first row gets filled left to right
  //then the row is inc and it repeats
  //there will never be Tiles to the right or below
  //as tiles are filled, check to the left and above for matches
  //if match was created, chose a diffrent type so as to not create a match

  static int lower = 0;
  static int upper = 5;

  for (int i = 0; i < BOARD_SIZE; i++)
    { //row
      for (int j = 0; j < BOARD_SIZE; j++)
	{ //col

	  //set matchBoard[][] defaults while we're here
	  // matchBoard[i][j] = false;

	  //set up Tiles
	  board[i][j].setRow(i);
	  board[i][j].setCol(j);
	  board[i][j].setType((rand() % (upper - lower + 1)) + lower);
	  board[i][j].setDstX((float)(j * GRID_SIZE + BOX_SIZE + 6));
	  board[i][j].setDstY((float)(i * GRID_SIZE + BOX_SIZE + 6));

	  if (i >= 2)
	    { //on/past row 2, check 2 above for matching type
	      bool aboveMatch = false;
	      bool leftMatch = false;
	      if (board[i - 1][j].getType() == board[i - 2][j].getType())
		{
		  aboveMatch = true;
		}
	      if (j >= 2)
		{ //on/past col 2, check 2 to the left for matching type
		  if (board[i][j - 1].getType() == board[i][j - 2].getType())
		    {
		      leftMatch = true;
		    }
		}
	      //if above or left have matches AND current type matches either one
	      // randomize until it doesnt
	      while ((aboveMatch or leftMatch) and
		     (board[i][j].getType() == board[i - 1][j].getType() or
		      board[i][j].getType() == board[i][j - 1].getType()))
		{ //pick a new type while it makes a match with above or left
		  board[i][j].setType((rand() % (upper - lower + 1)) + lower);
		}
	    } //end if i >= 2
	  else
	    {
	      if (j >= 2)
		{ //on/past col 2, check 2 to the left for matching type
		  bool leftMatch = false;
		  if (board[i][j - 1].getType() == board[i][j - 2].getType())
		    {
		      leftMatch = true;
		    }

		  while (leftMatch and
			 board[i][j].getType() == board[i][j - 1].getType())
		    { //pick a new type while it makes a match with left
		      board[i][j].setType((rand() % (upper - lower + 1)) + lower);
		    }
		} //end if j >= 2
	    }   //end else
	}     //end inner for loop
    }       //end outer for loop

  //start above board Tiles
  for(int i = 0; i <= 7; i++)
    {
      aboveBoard[i].setRow(-1); //just above board
      aboveBoard[i].setCol(i); //but still in proper cols
      aboveBoard[i].setType((rand() % (upper - lower + 1)) + lower); //random type
      aboveBoard[i].setDstX((float)(i * GRID_SIZE + BOX_SIZE + 6)); //in line with cols
      aboveBoard[i].setDstY((float)(-1 * GRID_SIZE + BOX_SIZE + 6)); //just above board
    }
}

//new

void Game::updateBoard()
{

  static int lower = 0;
  static int upper = 5;  
  
  //update all Tiles - go backwards to facilitate falling Tile sprite hand off
  for (int i = 7; i >= 0; i--)
    {
      for (int j = 7; j >= 0; j--)
	{
	  board[i][j].update();
	}
    }

  //update all Tile above board
  for(int i = 7; i >= 0; i--)
    {
      if(aboveBoard[i].isFalling() and aboveBoard[i].getType() == -1)
	{
	  //above board Tile is 'falling' but type is -1 (done falling)
	  aboveBoard[i].changeState("idle");
	  aboveBoard[i].setType((rand() % (upper - lower + 1)) + lower);
	}
      aboveBoard[i].update();
    }
}

void Game::checkForMatches()
{
  //handle any matches
  for (int i = 0; i < 8; i++)
    {
      for (int j = 0; j < 8; j++)
	{
	  if (board[i][j].isInMatchState())
	    {
	      match(board[i][j]);
	    }
	}
    }
}

//troublesome

char *Game::matchCheck(Tile currTile)
{ //checks the Tile passed for a match
  //results is a string containing ints of the number of matches in a direction
  // "<left> <right> <above> <below>"

  // printf("matchCheck() \n");

  int leftMatches, rightMatches, aboveMatches, belowMatches, row, col, type;
  char *matchString;
  matchString = new char[10];
  char intString[15];

  strcpy(matchString, ""); //reset
  leftMatches = rightMatches = aboveMatches = belowMatches = 0;
  row = currTile.getRow();
  col = currTile.getCol();
  type = currTile.getType();

  //currTile is board[row][col]

  while (col > 0 and board[row][col - 1].getType() == type)
    { //while left matches
      leftMatches++;
      col--;
    }
  col = currTile.getCol();

  while (col < BOARD_SIZE - 1 and board[row][col + 1].getType() == type)
    { //while right matches
      rightMatches++;
      col++;
    }
  col = currTile.getCol();

  while (row > 0 and board[row - 1][col].getType() == type)
    { //while above matches
      aboveMatches++;
      row--;
    }
  row = currTile.getRow();

  while (row < BOARD_SIZE - 1 and board[row + 1][col].getType() == type)
    { //while below matches
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

void Game::drawBoard()
{
  //black background
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  //draw a grid in maroon
  SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
  for (int i = 1; i <= (WINDOW_WIDTH / GRID_SIZE); i++)
    {
      //draw vertical lines
      SDL_RenderDrawLine(renderer, i * GRID_SIZE, 0, i * GRID_SIZE, WINDOW_HEIGHT);
    }
  for (int i = 1; i <= (WINDOW_HEIGHT / GRID_SIZE); i++)
    {
      //draw horizontal lines
      SDL_RenderDrawLine(renderer, 0, i * GRID_SIZE, WINDOW_WIDTH, i * GRID_SIZE);
    }
}

void Game::handleLeftMouseClick(int mouse_x, int mouse_y)
{
  // printf("handleLeftMouseClick() \n");

  if (validLeftMouseClick(mouse_x, mouse_y))
    {
      //click was made within constraints
      int clickRow = mouse_y / GRID_SIZE;
      int clickCol = mouse_x / GRID_SIZE;

      if (shouldSwapTiles(clickRow, clickCol))
	{
	  printf("a swap has occurred\n");
	  //swap - tell tiles they are to move
	  int activeTileDir = 0, newClickDir = 0;

	  //get highlight box out of the way
	  box_x = box_y = -200;

	  //  directions
	  //  0 - ignore it
	  //  1 - go left
	  //  2 - go right
	  //  3 - go up
	  //  4 - go down

	  //decide which direction each Tile goes
	  if (activeTile.getRow() == clickRow)
	    {
	      //swap in same row, left/right
	      if (activeTile.getCol() > clickCol)
		{
		  //activeTile col is larger - right of new click
		  activeTileDir = 1; //go left
		  newClickDir = 2;   //go right
		}
	      else
		{
		  //activeTile col is smaller - left of new click
		  activeTileDir = 2; //go right
		  newClickDir = 1;   //go left
		}
	    } //end if activeTile row == clickRow
	  else
	    {
	      //swap in same col, up/down
	      if (activeTile.getRow() > clickRow)
		{
		  //activeTile row is larger - below new click
		  activeTileDir = 3; //go up
		  newClickDir = 4;   //go down
		}
	      else
		{
		  //activeTile row is smaller - above new click
		  activeTileDir = 4; //go down
		  newClickDir = 3;   //go up
		}
	    } //end if activeTile row == clickRow else

	  //tell each Tile it is to swap, the direction, its new type after swapping
	  board[activeTile.getRow()][activeTile.getCol()]
	    .changeState("swap", activeTileDir);
	  board[clickRow][clickCol]
	    .changeState("swap", newClickDir);

	  printf("activeTile was updated to out of bounds\n");
	  //reset active tile location
	  activeTile.setRow(-2);
	  activeTile.setCol(-2);

	} //end if should swap tiles
      else
	{
	  //click was not adjacent, reset activeTile to current click
	  printf("activeTile was updated to this click\n");
	  //tell activeTile it is not active
	  if (activeTile.getRow() >= 0 and activeTile.getCol() >= 0)
	    {
	      board[activeTile.getRow()][activeTile.getCol()]
		.changeState("idle");
	    }

	  //tell new active Tile it is active
	  board[clickRow][clickCol]
	    .changeState("active");

	  //update reference Tile
	  activeTile.setRow(clickRow);
	  activeTile.setCol(clickCol);
	  activeTile.setType(board[clickRow][clickCol].getType());

	  //set pixel coords for activeTile highlighting
	  box_x = clickCol * GRID_SIZE;
	  box_y = clickRow * GRID_SIZE;

	} //end if should swap tiles else
    }   //end if valid mouse click
}

void Game::match(Tile curr)
{
  // printf("\nmatch()\n-------\n\trow: %d and col: %d\n", curr.getRow(), curr.getCol());

  int left, right, above, below, refLeft, refRight, refAbove, refBelow;
  char *matchString;
  bool shouldPaws, tileIsSwapping;
  int swapeeRow, swapeeCol;

  // decide up front if this tile is a swapping check or a falling check
  tileIsSwapping = (board[curr.getRow()][curr.getCol()].getSwappingDirection() != 0) ? true : false;

  if (tileIsSwapping)
    { // tile is swapping, determine swapee coords up front
      switch (board[curr.getRow()][curr.getCol()].getSwappingDirection())
	{
	case 1:
	  swapeeRow = curr.getRow();
	  swapeeCol = curr.getCol() - 1;
	  break;
	case 2:
	  swapeeRow = curr.getRow();
	  swapeeCol = curr.getCol() + 1;
	  break;
	case 3:
	  swapeeRow = curr.getRow() - 1;
	  swapeeCol = curr.getCol();
	  break;
	case 4:
	  swapeeRow = curr.getRow() + 1;
	  swapeeCol = curr.getCol();
	  break;
	default:
	  printf("match() direction of Tile at row %d col %d was %d\n\n", curr.getRow(), curr.getCol(), curr.getSwappingDirection());
	  exit(-1);
	}
    }

  matchString = matchCheck(board[curr.getRow()][curr.getCol()]);
  sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);

  refLeft = left;
  refRight = right;
  refAbove = above;
  refBelow = below;

  if (((left + right) >= 2) or ((above + below) >= 2))
    { //a match was made - update my match status and check my swapee's status

      if (tileIsSwapping)
	{
	  board[curr.getRow()][curr.getCol()].setMatchStatus(1); //mark that I have made a match
	  switch (board[swapeeRow][swapeeCol].getMatchStatus())
	    {
	    case 0: //swapee not checked yet
	      //do nothing
	      break;

	    case 1: //swapee made a match - set both of us to empty
	      board[curr.getRow()][curr.getCol()].changeState("empty");
	      board[swapeeRow][swapeeCol].changeState("empty");
	      break;

	    case 2: //swapee did NOT make a match - me empty, them idle
	      //update swapee swapType to their type - they are staying where they are
	      board[swapeeRow][swapeeCol].setSwapType(board[swapeeRow][swapeeCol].getType());
	      //tell them they are idle
	      board[swapeeRow][swapeeCol].changeState("idle");          //make sure this doesnt do anything with types
	      board[curr.getRow()][curr.getCol()].changeState("empty"); // change my state to empty
	      break;

	    default:
	      printf("match() swapee matchStatus Tile at row %d col %d was %d\n\n", curr.getRow(), curr.getCol(), curr.getSwappingDirection());
	      exit(-1);
	    }
	}
      else
	{                                                           // this match check is being made on a falling tile - not a swapping one
	  board[curr.getRow()][curr.getCol()].changeState("empty"); // change my state to empty
	}
    } // end of if match made

  if ((refAbove + refBelow) < 2 and (refLeft + refRight) < 2)
    { // I was NOT matched

      if (tileIsSwapping)
	{
	  board[curr.getRow()][curr.getCol()].setMatchStatus(2); // I have NOT made a match
	  switch (board[swapeeRow][swapeeCol].getMatchStatus())
	    {
	      //decide states based on the matchStatus of my swapee
	    case 0: // swapee not checked yet
	      // do nothing
	      break;

	    case 1: // swapee made a match - me idle, them empty
	      //update my swapType to my type - I am staying where I am
	      board[curr.getRow()][curr.getCol()].setSwapType(board[swapeeRow][swapeeCol].getType());
	      board[curr.getRow()][curr.getCol()].changeState("idle"); // I am idle
	      board[swapeeRow][swapeeCol].changeState("empty");        // swapee is empty
	      break;

	    case 2: // swapee did NOT make a match - both to paws
	      board[curr.getRow()][curr.getCol()].changeState("paws");
	      board[swapeeRow][swapeeCol].changeState("paws");
	      break;

	    default:
	      printf("match() no match direction of Tile at row %d col %d was %d\n\n", curr.getRow(), curr.getCol(), curr.getSwappingDirection());
	      exit(-1);

	    } //end of switch(curr.getSwappingDir())
	}   // end of if tileIsSwapping
      else
	{ // this match check is being made on a falling tile - not a swapping one
	  // this tile is falling - no match - go to idle
	  board[curr.getRow()][curr.getCol()].changeState("idle");
	}
    } //if no match made on this tile

  //handle other tiles involved in the match

  if ((left + right) >= 2)
    { //is left-right the match?

      //tell every Tile above match that it should fall
      for (int row = (curr.getRow() - 1); row >= 0; row--)
	{ //start right above this match, go up
	  for (int col = (curr.getCol() - refLeft); col <= (curr.getCol() + refRight); col++)
	    {
	      printf("\nGame:: telling tile (row: %d col: %d) to fall\n", row, col);
	      board[row][col].changeState("fall",                             // new state
					  board[row][col].getType(),          // keep type
					  1,                                  // fall 1
					  board[row][col].getFallingConst()); // regular speed
	    }
	}
      //tell all aboveBoard Tiles to fall
      for(int col = (curr.getCol() - refLeft); col <= (curr.getCol() + refRight); col++)
	{
	  aboveBoard[col].changeState("fall",                             // new state
				      aboveBoard[col].getType(),          // keep type
				      1,                                  // fall 1
				      aboveBoard[col].getFallingConst()); // regular speed
	}
      

      //set all matching Tiles to invisible
      while (left > 0)
	{ //remove all left
	  board[curr.getRow()][curr.getCol() - left].changeState("empty");
	  left--;
	}
      while (right > 0)
	{ //remove all right
	  board[curr.getRow()][curr.getCol() + right].changeState("empty");
	  right--;
	}
    } //end if left + right >= 2

  if ((above + below) >= 2)
    { // is above-below the match?

      //tell every Tile above match that it should fall
      for (int row = (curr.getRow() - refAbove - 1); row >= 0; row--)
	{
	  printf("\nGame:: telling tile (row: %d col: %d) to fall\n", row, curr.getCol());
	  board[row][curr.getCol()].changeState("fall",                                                             // new state
						board[row][curr.getCol()].getType(),                                // keep type
						refAbove + refBelow + 1,                                                  // fall above + below + 1
						board[row][curr.getCol()].getFallingConst() * (refAbove + refBelow + 1)); // appropriate speed
	}


      //tell all aboveBoard Tiles to fall
      aboveBoard[curr.getCol()].changeState("fall",
					    aboveBoard[curr.getCol()].getType(),
					    refAbove + refBelow + 1,
					    aboveBoard[curr.getCol()].getFallingConst() * (refAbove + refBelow + 1));
      
      //set all matching Tiles to invisible
      while (above > 0)
	{ //remove all above
	  board[curr.getRow() - above][curr.getCol()].changeState("empty");
	  above--;
	}
      while (below > 0)
	{ //remove all below
	  board[curr.getRow() + below][curr.getCol()].changeState("empty");
	  below--;
	}
    } // end if above + below >= 2
}
