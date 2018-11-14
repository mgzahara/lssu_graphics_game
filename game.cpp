#include "game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "globals.h"
#include "tile.h"
#include "Fixed_print.h"
#include "effect.h"


Game::Game()
{

  //first activeTile will have unreal row and col
  activeTile.setRow(-10);
  activeTile.setCol(-10);
  activeTile.setType(-1);

  scoreMultiplier = 0; //number of matches in current 'run'
}
Game::~Game()
{
  return;
}

bool Game::validLeftMouseClick(int mouse_x, int mouse_y)
{
  //include in here a check for all Tiles to be idle
  //dont allow swaps while other things are going on
  
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
  return (activeTile.getRow() >= 0)
    and (activeTile.getCol() >= 0)
    and (board[activeTile.getRow()][activeTile.getCol()]
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

	  effects[i][j].setRow(i);
	  effects[i][j].setCol(j);
	  
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
      aboveBoard[i].update();
    }

  //if no tiles are busy, reset match multiplier
  bool flag = true;
  for(int i = 0; i < 8; i++)
    {
      for(int j = 0; j < 8; j++)
	{
	  if(!board[i][j].isNotBusy())
	    {
	      //this tile IS busy
	      flag = false;
	    }
	}
    }
  if(flag)
    {
      //no tiles are busy - no possible matches to multiply
      scoreMultiplier = 0;
    }

  //update all visuals for effects
  for(int i = 0; i < 8; i++)
    {
      for(int j = 0; j < 8; j++)
	{
	  effects[i][j].update();
	}
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

  while (col > 0 and board[row][col - 1].getType() == type and !board[row][col - 1].isFalling())
    { //while left matches
      leftMatches++;
      col--;
    }
  col = currTile.getCol();

  while (col < BOARD_SIZE - 1 and board[row][col + 1].getType() == type and !board[row][col + 1].isFalling())
    { //while right matches
      rightMatches++;
      col++;
    }
  col = currTile.getCol();

  while (row > 0 and board[row - 1][col].getType() == type and !board[row - 1][col].isFalling())
    { //while above matches
      aboveMatches++;
      row--;
    }
  row = currTile.getRow();

  while (row < BOARD_SIZE - 1 and board[row + 1][col].getType() == type and !board[row + 1][col].isFalling())
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
  static SDL_Rect src = {
    0, 0, 640, 480 };
  static SDL_Rect dst = {
    0, 0, 640, 480 };
  SDL_RenderCopy(renderer, background, &src, &dst);

  return;
}

void Game::handleLeftMouseClick(int mouse_x, int mouse_y)
{
  // printf("handleLeftMouseClick() \n");
  int clickRow = mouse_y / GRID_SIZE;
  int clickCol = mouse_x / GRID_SIZE;

  if (validLeftMouseClick(mouse_x, mouse_y)
      and board[clickRow][clickCol].isNotBusy())
    {
      //click was made within constraints
      //and second Tile clicked was not busy

      if (shouldSwapTiles(clickRow, clickCol))
	{
	  //printf("a swap has occurred\n");
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

	  //printf("activeTile was updated to out of bounds\n");
	  //reset active tile location
	  activeTile.setRow(-2);
	  activeTile.setCol(-2);

	} //end if should swap tiles
      else
	{
	  //click was not adjacent, reset activeTile to current click
	  //printf("activeTile was updated to this click\n");
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
  else if (shuffleClick(mouse_x, mouse_y))
    {
      bool flag = true;
      for(int i = 0; i < 7; i++)
	{
	  for(int j = 0; j < 7; j++)
	    {
	      if(!board[i][j].isNotBusy())
		{
		  //this tile IS busy
		  flag = false;
		}
	    }
	}
      if(flag)
	{
	  //safe to shuffle
	  for(int i = 0; i < 5; i++)
	    {
	      //shuffle, drawboard, update, drawpanel, wait
	      startBoard();
	      drawBoard();	    
	      updateBoard();
	      drawPanel();
	      SDL_RenderPresent(renderer);
	      SDL_Delay(200);
	    }
	}
    }
}

void Game::match(Tile curr)
{
  int left, right, above, below, refLeft, refRight, refAbove, refBelow;
  char *matchString;
  bool shouldPaws, tileIsSwapping;
  int swapeeRow, swapeeCol, boostMode = 0;
  
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

      //special check      
      if(((left + right) >= 2) and ((above + below) >= 2))
	{
	  //ZAP_B
	  boostMode = 4;
	}
      else if(((left + right) == 3) or ((above + below) == 3))
	{
	  //BOMB
	  boostMode = 1;
	}
      else if((left + right) > 3)
	{
	  //ZAP_H
	  boostMode = 2;
	}
      else if((above + below) > 3)
	{
	  //ZAP_V
	  boostMode = 3;
	}

      //add to score based on boostMode
      switch(boostMode)
	{
	case 0: //regular 3 in a row
	  score += (scoreConst * scoreMultiplier) + baseScore3;
	  break;

	case 1: //4 in a row
	  score += (scoreConst * scoreMultiplier) + baseScore4;
	  break;

	case 2: //5 in a match of some sort
	case 3:
	case 4:
	  score += (scoreConst * scoreMultiplier) + baseScore5;
	  break;
	}

      //now that the sore is updated, inc score multiplier
      scoreMultiplier++;
      
      if (tileIsSwapping)
	{
	  board[curr.getRow()][curr.getCol()].setMatchStatus(1); //mark that I have made a match
	  switch (board[swapeeRow][swapeeCol].getMatchStatus())
	    {
	    case 0: //swapee not checked yet
	      board[curr.getRow()][curr.getCol()].changeState("empty", boostMode);
	      break;

	    case 1: //swapee made a match
	      board[curr.getRow()][curr.getCol()].changeState("empty", boostMode);

	      break;

	    case 2: //swapee did NOT make a match - me empty, them idle
	      //update swapee swapType to their type - they are staying where they are
	      board[swapeeRow][swapeeCol].setSwapType(board[swapeeRow][swapeeCol].getType());
	      //tell them they are idle
	      board[swapeeRow][swapeeCol].changeState("idle");          //make sure this doesnt do anything with types
	      board[curr.getRow()][curr.getCol()].changeState("empty", boostMode); // change my state to empty
	      break;

	    default:
	      printf("match() swapee matchStatus Tile at row %d col %d was %d\n\n", curr.getRow(), curr.getCol(), curr.getSwappingDirection());
	      exit(-1);
	    }
	}
      else
	{                                                           // this match check is being made on a falling tile - not a swapping one
	  board[curr.getRow()][curr.getCol()].changeState("empty", boostMode); // change my state to empty
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

bool Game::shuffleClick(int x, int y)
{
  return (x >= 465 and x <= 585) and (y >= 275 and y <= 325);
}

void Game::drawPanel()
{
  //draw entire side panel - start at (420, 20)
  static SDL_Rect panel = {450, 40, 150, 300};
  static SDL_Rect shuffleButton = {465, 275, 120, 50};
  static SDL_Rect scoreBox = {465, 55, 120, 75};
  static SDL_Color textColor;
  
  //background is dark blue (23, 77, 214)
  SDL_SetRenderDrawColor(renderer, 23, 77, 214, 255);
  SDL_RenderFillRect(renderer, &panel);

  //shuffle button & scoreBox is teal-ish (64, 192, 206)
  SDL_SetRenderDrawColor(renderer, 64, 192, 206, 255);
  SDL_RenderFillRect(renderer, &shuffleButton);
  SDL_RenderFillRect(renderer, &scoreBox);

  //text is black (0, 0, 0)
  textColor.r = textColor.g = textColor.b = 0;
  this->displayInfo("Shuffle", 490, 292, textColor);
  this->displayInfo("Score:", 485, 68, textColor);
  this->displayInfo("", score, 485, 95, textColor);
  
}

//print functions

void Game::displayInfo(const char* str, int val, int x, int y, SDL_Color c)
{//use schemms Fixed_print.c to print string and int
  
  char displayString[50];
  char intString[20];

  strcpy(displayString, str);//player 2 wins
  
  sprintf(intString, "%d", val);
  strcat(displayString, intString );
  FixedPrint(renderer, //sdl_renderer
             x, //x coord
	     y, //y coord
             displayString, //text
             &c, //address of color
             2); //size: 0, 1, or 2
}

void Game::displayInfo(const char* str, float val, int x, int y, SDL_Color c)
{//use schemms Fixed_print.c to print string and float
  
  char displayString[50];
  char floatString[20];

  strcpy(displayString, str);//player 2 wins
  
  sprintf(floatString, "%.2f", val);
  strcat(displayString, floatString );
  FixedPrint(renderer, //sdl_renderer
             x, //x coord
	     y, //y coord
             displayString, //text
             &c, //address of color
             2); //size: 0, 1, or 2
}

void Game::displayInfo(const char* str, int x, int y, SDL_Color c)
{//use schemms Fixed_print.c to print string only
  
  char displayString[50];

  strcpy(displayString, str);//player 2 wins
  FixedPrint(renderer, //sdl_renderer
             x, //x coord
             y, //y coord
             displayString, //text
             &c, //address of color
             2); //size: 0, 1, or 2
}
