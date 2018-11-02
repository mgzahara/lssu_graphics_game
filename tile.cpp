//include something here to init SDL renderer immediately
#include "math.h"
#include "iostream"
#include "SDL.h"
#include "SDL_image.h"
#include "globals.h"
#include "tile.h"

#ifndef TILE_CPP
#define TILE_CPP

Tile::Tile()
{
  //no designated row or col
  row = -1;
  col = -1;
  type = -1;
  GRID_SIZE = 50;
  GEM_SIZE = 32;
  BOX_SIZE = 3;
  //TILE_FALLING_SPEED = 10.0;
  //TILE_SWAPPING_SPEED = 10.0;
  TILE_SWAPPING_SPEED = 2.5;
  TILE_FALLING_SPEED = 2.5;
  //  TILE_SWAPPING_SPEED = 2.5;

  state = STATE::IDLE;

  this->setMaxSprite(30);
  //toggle to allow tiles to finish 'falling' after bottom place is filled
  this->keepFalling = false;
  this->shouldBeEmpty = false;
  this->matchStatus = 0;

  // falling = false;
  // swapping = false;
  // active = false;
}
Tile::~Tile()
{
  return;
}
void Tile::swapTypes()
{
  //change the values of type and swapType with each other
  int tmp = this->type;
  this->type = this->swapType;
  this->swapType = tmp;
}
void Tile::setMatchStatus(int s)
{
  this->matchStatus = s;
}
int Tile::getMatchStatus()
{
  return this->matchStatus;
}
int Tile::getRandType()
{
  //get random type for aboveBoard[]
  int lower = 0;
  int upper = 5;
  return ((rand() % (upper - lower + 1)) + lower);
}


//setters
void Tile::setRow(int r)
{
  row = r;
}
void Tile::setCol(int c)
{
  col = c;
}
void Tile::setType(int t)
{
  this->type = t;
  //same as swapType
  this->setSwapType(t);

  //visual loc to default
  this->setDstX(this->getDefaultSpriteX());
  this->setDstY(this->getDefaultSpriteY());
}
void Tile::setDstX(float x)
{
  spriteDstX = x;
}
void Tile::setDstY(float y)
{
  spriteDstY = y;
}
void Tile::setSpriteCounter(int s)
{
  spriteCounter = s;
}
void Tile::setMaxSprite(int m)
{
  maxSprite = m;
}
void Tile::setSwappingDirection(int s)
{
  swappingDirection = s;
}
void Tile::setFallsRemaining(int f)
{
  fallsRemaining = f;
}
void Tile::setSwapType(int s)
{
  swapType = s;
}
void Tile::setSwapping(bool b)
{
  swapping = b;
}
void Tile::setSwapping(bool b, int d, int t)
{
  printf("setSwapping()\n");
  printf("d: %s\n", b ? "true" : "false");
  printf("direction: %d\n swapType: %d\n\n", d, t);

  swapping = b;
  this->setSwappingDirection(d);
  this->setSwapType(t);
  //change state to swap
  this->state = STATE::SWAP;
}
void Tile::setFallingSpeed(float s)
{
  fallingSpeed = s;
}
void Tile::setFalling(bool f, int dis)
{

  printf("setFalling() \n");
  printf("f: %s\n", f ? "true" : "false");
  printf("dis: %d\n", dis);

  //public to be called by match to start falling process
  falling = f;
  this->setFallsRemaining(dis);
  this->setFallingSpeed((float)(dis * TILE_FALLING_SPEED));
}
void Tile::setActive(bool a)
{
  active = a;
}

//getters
int Tile::getRow()
{
  return row;
}
int Tile::getCol()
{
  return col;
}
int Tile::getType()
{
  return type;
}
float Tile::getDstX()
{
  return spriteDstX;
}
float Tile::getDstY()
{
  return spriteDstY;
}
int Tile::getSpriteCounter()
{
  return spriteCounter;
}
int Tile::getMaxSprite()
{
  return maxSprite;
}
int Tile::getSwappingDirection()
{
  return swappingDirection;
}
int Tile::getFallsRemaining()
{
  return fallsRemaining;
}
int Tile::getSwapType()
{
  return swapType;
}
bool Tile::getSwapping()
{
  return swapping;
}
bool Tile::getActive()
{
  return active;
}
bool Tile::getFalling()
{
  return falling;
}
float Tile::getDefaultSpriteX()
{
  return ((this->getCol() * GRID_SIZE) + BOX_SIZE + 6);
}
float Tile::getDefaultSpriteY()
{
  return ((this->getRow() * GRID_SIZE) + BOX_SIZE + 6);
}
float Tile::getFallingConst()
{
  return this->TILE_FALLING_SPEED;
}

int Tile::sign(int x)
{
  if (x > 0)
    {
      return 1;
    }
  if (x < 0)
    {
      return -1;
    }
  return 0;
}
int Tile::sign(float x)
{
  if (x > (float)0.0)
    {
      return 1;
    }
  if (x < (float)0.0)
    {
      return -1;
    }
  return 0;
}

void Tile::takeFallingSprite(int type, int falls, float speed) //old
{

  printf("takeFallingSprite() \n");

  //how a Tile hands off its falling sprite to another Tile
  this->setType(type);
  this->setFallsRemaining(falls - 1);
  if (this->getFallsRemaining() == 0)
    {
      //done falling
      this->setFalling(false, 0);
      this->setFallingSpeed((float)0);
      //check this Tile for a match
      // matchBoard[this->getRow()][this->getCol()] = true;
      this->state = STATE::MATCH;
    }
  else
    {
      this->setFallingSpeed(speed);
    }
  this->setDstX(this->getDefaultSpriteX());
  this->setDstY(this->getDefaultSpriteY());
}

bool Tile::isAdjacent(Tile t)
{
  //true -> this Tile and arg Tile are vertically or horizontally adjacent

  if ((abs(row - t.getRow()) == 1 and
       abs(col - t.getCol()) == 0) //directly above/below
      or
      (abs(col - t.getCol()) == 1 and
       abs(row - t.getRow()) == 0)) //directly left/right

    { //only true if directly left, right, up, or down
      return true;
    }

  return false;
}


bool Tile::isInMatchState()
{
  return (this->state == STATE::MATCH);
}
bool Tile::isInFallingState()
{
  return (this->state == STATE::FALL);
}
bool Tile::isInEmptyState()
{
  return (this->state == STATE::EMPTY);
}
bool Tile::isInIdleState()
{
  return (this->state == STATE::IDLE);
}
void Tile::printColor(int newType)
{
  //  printf("top of printColor, this->type: %d\n", this->type);
  switch(newType)
    {
    case 0: 
	printf("red");
      break;
    case 1: 
	printf("yellow");
      break;
    case 2: 
	printf("pink");
      break;
    case 3: 
	printf("green");
      break;
    case 4: 
	printf("white");
      break;
    case 5: 
	printf("blue");
      break;
    default:
      printf("printColor() type not found\n");
      exit(-1);
    }
}
bool Tile::emptyBelowMe()
{
  bool flag = false;
  for(int i = this->row; i <= 6; i++)
    {
      if(board[i + 1][this->col].isInEmptyState())
	{
	  //found an empty Tile below my position
	  flag = true;
	  //this->keepFalling = true;
	}
    }
  /*
  if(flag and this->keepFalling)
    {
      //empty found - real true
      return true;
    }
  else if(this->keepFalling)
    {
      //no empty found - fake true
      this->keepFalling = false;
      return true;
    }
  else
    {
      //no empty found and toggle is off - real false
      return false;
    }
  */
  return flag;

}
float Tile::getFallingSpeed()
{
  int emptyTiles = 0;
  for(int i = this->row; i <= 6; i++)
    {
      if(board[i + 1][this->col].isInEmptyState())
	{
	  //found an empty Tile below my position
	  emptyTiles = emptyTiles + 1;
	}
    }
  /*
  if(this->emptyBelowMe() and this->keepFalling)
    {
      //emptyTiles is accurate
      return this->TILE_FALLING_SPEED * (float) emptyTiles;    
    }
  else if (this->keepFalling)
    {
      //emptyTiles is inaccurate
      return this->TILE_FALLING_SPEED * ((float) emptyTiles + 1.0);
    }
  else
    {
      return (float) 0.0;
    }
  */
  return this->TILE_FALLING_SPEED * (float) emptyTiles;
}

//action functions

void Tile::spriteHandOff(int newType, int newSpriteCounter)
{
  //accept new sprite
  printf("spriteHandOff() to Tile (row: %d col: %d)\n", this->row, this->col);
  printf("\treceived a Tile of type %d and color: ", newType);
  //  fflush(stdout);
  printColor(newType);
  printf("\n\n");

  //when this function is called, I am empty
  //if there is no other empty Tile below me, call for a match
  
  
  this->setType(newType); // will reset visual pos
  this->setSpriteCounter(newSpriteCounter);

  if(!this->emptyBelowMe())
    {
      //no empty Tiles below me, check me for a match
      this->state = STATE::MATCH;
    }
  else
    {
      //there are still empty Tiles below me, keep on
      this->changeState("idle");
    }
}


void Tile::update()
{
  SDL_Rect src, dst;
  int deltaX, deltaY;

  switch (this->state)
    {
    case STATE::IDLE:
      //      this->setDstY(this->getDefaultSpriteY());
      //      this->setDstY(this->getDefaultSpriteY());
      this->matchStatus = 0; //reset match status from a possible transition from STATE::MATCH
      if (this->getSpriteCounter() != 0)
	{
	  //sprite not done spinning, finish the animation
	  this->setSpriteCounter((this->getSpriteCounter() + 1) % this->getMaxSprite());
	}

      if(this->emptyBelowMe())
	{
	  //there is an empty Tile below me - fall

	  //inc visual Y pos
	  this->setDstY(this->getDstY() + this->getFallingSpeed());

	  if (this->getDstY() >= (this->getDefaultSpriteY() + this->GRID_SIZE))
	    {
	      //my visual is in place for the next Tile to take it

	      //hand off my info
	      printf("before calling psriteHandOff()\n\tthis->type: %d\n\tthis->spriteCounter: %d\n\n",
		     this->type, this->spriteCounter);
	      board[this->row + 1][this->col].spriteHandOff(this->type, this->spriteCounter);

	      
	      if(this->row == -1)
		{
		  //this Tile is aboveBoard

		  //get new type - will reset visuals
		  this->setType(this->getRandType());
		}
	      else
		{
		  //I am a board Tile
		  //set flag to make me empty right after rendering
		  //I am the new bottom of the col
		  this->shouldBeEmpty = true;
		}
	    }
	}//end of if emptyBelowMe
      

      break;

    case STATE::SWAP:
      deltaX = deltaY = 0;
      this->setSpriteCounter((this->getSpriteCounter() + 1) % this->getMaxSprite());
      switch (this->getSwappingDirection())
	{ //decide which direction to move
	case 1:
	  deltaX = -TILE_SWAPPING_SPEED;
	  break;
	case 2:
	  deltaX = TILE_SWAPPING_SPEED;
	  break;
	case 3:
	  deltaY = -TILE_SWAPPING_SPEED;
	  break;
	case 4:
	  deltaY = TILE_SWAPPING_SPEED;
	  break;
	default:
	  printf("Swap direction of Tile at row %d col %d was %d\n\n", this->getRow(), this->getCol(), this->getSwappingDirection());
	  exit(-1);
	}
      this->setDstX(this->getDstX() + deltaX);
      this->setDstY(this->getDstY() + deltaY);

      if (this->getDstX() == (this->getDefaultSpriteX() + (sign(deltaX) * this->GRID_SIZE)) and
	  this->getDstY() == (this->getDefaultSpriteY() + (sign(deltaY) * this->GRID_SIZE)))
	{
	  //tell game to check me for match
	  this->state = STATE::MATCH;
	  this->swapTypes(); //change type to swapType and swapType to type
	  //reset visual coords
	  this->setDstX(getDefaultSpriteX());
	  this->setDstY(getDefaultSpriteY());
	  // this->setType(this->getSwapType());
	  //swap my type right after texture is blitted to ensure the proper Tiles are drawn
	}


      break;

    case STATE::PAWS:

      deltaX = deltaY = 0;
      this->matchStatus = 0;                                                         //reset match status from a possible transition from STATE::MATCH
      this->setSpriteCounter((this->getSpriteCounter() + 1) % this->getMaxSprite()); //spin

      switch (this->getSwappingDirection())
	{ //decide which direction to move
	case 1:
	  deltaX = TILE_SWAPPING_SPEED;
	  break;
	case 2:
	  deltaX = -TILE_SWAPPING_SPEED;
	  break;
	case 3:
	  deltaY = TILE_SWAPPING_SPEED;
	  break;
	case 4:
	  deltaY = -TILE_SWAPPING_SPEED;
	  break;
	default:
	  printf("Swap direction of Tile at row %d col %d was %d\n\n", this->getRow(), this->getCol(), this->getSwappingDirection());
	  exit(-1);
	}
      this->setDstX(this->getDstX() - deltaX);
      this->setDstY(this->getDstY() - deltaY);

      if (this->getDstX() == (this->getDefaultSpriteX() - (sign(deltaX) * this->GRID_SIZE)) and
	  this->getDstY() == (this->getDefaultSpriteY() - (sign(deltaY) * this->GRID_SIZE)))
	{
	  /*
	    if (this->getDstX() == this->getDefaultSpriteX() and
	    this->getDstY() == this->getDefaultSpriteY())
	    { */

	  //back to normal
	  this->state = STATE::IDLE; //resets visual coords
	  //sprites are visually back to normal, take my original type back
	  this->setType(this->getSwapType());
	}


      break;

    case STATE::ACTIVE:
      //ensure default coords
      this->setDstY(this->getDefaultSpriteY());
      this->setDstY(this->getDefaultSpriteY());
      //spin
      this->setSpriteCounter(
			     (this->getSpriteCounter() + 1) % this->getMaxSprite());
      break;

    case STATE::EMPTY:
      //ensure the empty sprite - OR - dont put them on renderer
      this->setType(-1);
      this->matchStatus = 0; //reset match status from a possible transition from STATE::MATCH
      break;

    case STATE::FALL:
      printf("tile (row: %d col: %d) is in FALL\n", this->row, this->col);
      //be sure that each falling sprite is at 0
      // this->setSpriteCounter(0);
      if (this->getSpriteCounter() != 0)
	{
	  //sprite not done spinning, finish the animation
	  this->setSpriteCounter((this->getSpriteCounter() + 1) % this->getMaxSprite());
	}

      if (this->getDstY() >= (this->getDefaultSpriteY() + this->GRID_SIZE))
	{
	  //my visual is in place for the next Tile to take it

	  //hand off my texture to the Tile below me
	  board[this->getRow() + 1][this->getCol()]
	    .changeState("fall",
			 this->getType(),
			 this->getFallsRemaining() - 1,
			 this->getFallingSpeed());
	  if(this->getRow() == -1)
	    {
	      //only for aboveBoard Tiles - reset myself
	      //drop another, or reset to idle
	      aboveBoard[this->getCol()].changeState("fall",
						     this->getRandType(),
						     this->getFallsRemaining() - 1,
						     this->getFallingSpeed());
	    }//end of if row == -1
	}//end of if dstY == defaultDstY + GRID_SIZE
      break;

    case STATE::MATCH: //shouldnt even get here - Game should change it before update gets called again
      // printf("my state is match\n");
      break;

    default:
      printf("not a state\n");
      exit(-1);
    }

  src = {
    this->getSpriteCounter() * GEM_SIZE,
    0,
    this->GEM_SIZE,
    this->GEM_SIZE};
  dst = {
    (int)this->getDstX(),
    (int)this->getDstY(),
    this->GEM_SIZE,
    this->GEM_SIZE};

  //put this tile on renderer
  if (this->getType() != -1)
    {
      //tile is visible
      SDL_RenderCopy(renderer, gems[this->getType()], &src, &dst);
    }

  if(this->shouldBeEmpty)
    {
      this->shouldBeEmpty = false;
      this->changeState("empty");
    }
}

int Tile::changeState(const char *newState, int arg2, int arg3, int arg4)
{ //fall
  printf("Tile:: change state to fall on tile (row: %d col:%d)\n", this->row, this->col);
  if (newState == "fall")
    {
      printf("\trow: %d and col: %d - changed to state FALL\n", this->getRow(), this->getCol());
      /*
	arg2 - type
	arg3 - my fallsRemaining
	arg4 - fallingSpeed
      */
      this->setType(arg2);
      if (arg3 == 0)
	{
	  //im done falling, go to MATCH state to wait for match checking
	  //this was just a hand off from tile above to me, the final loc for the falling tile
	  this->state = STATE::MATCH;
	  this->setFallsRemaining(0);
	  this->setFallingSpeed(0);
	}
      else
	{
	  //im still falling
	  this->state = STATE::FALL;
	  this->setFallsRemaining(this->getFallsRemaining() + arg3);
	  this->setFallingSpeed(arg4);
	}
    }
}

int Tile::changeState(const char *newState, int arg2)
{ //swap
  if (newState == "swap")
    {
      printf("\trow: %d and col: %d - changed to state SWAP\n", this->getRow(), this->getCol());
      /* arg2 is new direction
	 1 - go left
	 2 - go right
	 3 - go up
	 4 - go down */

      printf("state changed to swap\n");
      this->state = STATE::SWAP;
      this->swappingDirection = arg2;

      // return 0;

      switch (arg2)
	{ //what is my new type once I swap?
	case 1:
	  this->setSwapType(board[this->row][this->col - 1].getType());
	  break;

	case 2:
	  this->setSwapType(board[this->row][this->col + 1].getType());
	  break;

	case 3:
	  this->setSwapType(board[this->row - 1][this->col].getType());
	  break;

	case 4:
	  this->setSwapType(board[this->row + 1][this->col].getType());
	  break;

	default:
	  printf("swap direction invalid: %d\n", arg2);
	  exit(-1);
	}
    }
  else
    {
      printf("\nTile::changeState('', int) did not receive expected arg\n\tnewState: %s\n\targ2: %d\n\n", newState, arg2);
      exit(-1);
    }
}

int Tile::changeState(const char *newState)
{ //idle & active & empty & paws

  if (newState == "idle")
    {
      printf("\trow: %d and col: %d - changed to state IDLE\n", this->getRow(), this->getCol());
      this->state = STATE::IDLE;
      //reset my visual coords
      this->setDstX(this->getDefaultSpriteX());
      this->setDstY(this->getDefaultSpriteY());
      //reset swap direction
      this->setSwappingDirection(0);
    }
  else if (newState == "active")
    {
      printf("\trow: %d and col: %d - changed to state ACTIVE\n", this->getRow(), this->getCol());
      this->state = STATE::ACTIVE;
    }
  else if (newState == "empty")
    {
      //TELL EVERY TILE ABOVE TO FALL
      printf("\trow: %d and col: %d - changed to state EMPTY\n", this->getRow(), this->getCol());
      this->state = STATE::EMPTY;
      this->setType(-1); //update type and swapType to -1 - empty
      //reset swap direction
      this->setSwappingDirection(0);
    }
  else if (newState == "paws")
    {
      printf("\trow: %d and col: %d - changed to state PAWS\n", this->getRow(), this->getCol());
      this->state = STATE::PAWS;

      return 0;

      switch (this->getSwappingDirection())
	{ // reverse swapping direction and set new swapType
	case 1:
	  // this->setSwappingDirection(2);
	  this->setSwapType(board[this->row][this->col + 1].getType());
	  break;
	case 2:
	  // this->setSwappingDirection(1);
	  this->setSwapType(board[this->row][this->col - 1].getType());
	  break;
	case 3:
	  // this->setSwappingDirection(4);
	  this->setSwapType(board[this->row + 1][this->col].getType());
	  break;
	case 4:
	  // this->setSwappingDirection(3);
	  this->setSwapType(board[this->row - 1][this->col].getType());
	  break;
	default:
	  printf("Paws direction of Tile at row %d col %d was %d\n\n", this->getRow(), this->getCol(), this->getSwappingDirection());
	  exit(-1);
	} //end switch(swappingDirection)
    }   //end else if newState == "paws"
  else
    {
      printf("\nTile::changeState('') did not receive expected arg\n\tnewState: %s\n\n", newState);
      exit(-1);
    }
}

/* research more on overloading 
   
   Tile Tile::operator = (Tile t)
   {//overloading assignment operator
   Tile temp;
   temp.setRow(row);
   temp.setCol(col);
   temp.setType(type);
   
   printf("\n\n\t = operator returning:\n");
   printf("\ttemp row:  %d\n", temp.getRow()  );
   printf("\ttemp col:  %d\n", temp.getCol()  );
   printf("\ttemp type: %d\n", temp.getType() );
   printf("\n");
   
   return temp;
   }
*/
/*
  bool Tile::operator == (Tile t)
  {//overloading comparison operator
  
  if(row ==  t.getRow() and 
  col ==  t.getCol() and 
  type == t.getType() )
  {
  return true;
  }
  
  return false;
  }
*/

#endif
