#include "math.h"
#include "iostream"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "globals.h"
#include "tile.h"
#include "Fixed_print.h"
#include "effect.h"

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

  state = STATE::IDLE;
  boost = BOOST::NORMAL;
  oldBoost = BOOST::NORMAL;

  this->setMaxSprite(30);
  //toggle to allow tiles to finish 'falling' after bottom place is filled
  this->keepFalling = false;
  this->shouldBeEmpty = false;
  this->matchStatus = 0;
  
  effectFrameCounter = 0;
  effectFrameMax = 0;
  effectSize = 0;
  effectFrameWait = 0; //only update effect every 5 frames
  effectFrameAngle = 0;
  //add info for the Game class to handle match effects
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
void Tile::swapBoosts()
{
  BOOST tmp = this->boost;
  this->boost = this->oldBoost;
  this->oldBoost = tmp;
}
void Tile::swapSpriteCounters()
{
  //change values of spriteCounter and oldSpriteCounter with each other
  int tmp = this->spriteCounter;
  this->spriteCounter = this->oldSpriteCounter;
  this->oldSpriteCounter = tmp;
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
  //printf("setSwapping()\n");
  //printf("d: %s\n", b ? "true" : "false");
  //printf("direction: %d\n swapType: %d\n\n", d, t);

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

  //printf("setFalling() \n");
  //printf("f: %s\n", f ? "true" : "false");
  //printf("dis: %d\n", dis);

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

  //printf("takeFallingSprite() \n");

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

bool Tile::isNotBusy()
{
  return this->isInIdleState() and !this->emptyBelowMe();
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
bool Tile::isFalling()
{
  return this->emptyBelowMe();
}

void Tile::setOldBoostType(BOOST b)
{
  this->oldBoost = b;
}
Tile::BOOST Tile::getBoostType()
{
  return this->boost;
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
  return this->TILE_FALLING_SPEED * (float) emptyTiles;
}

//action functions

void Tile::spriteHandOff(int newType, int newSpriteCounter, BOOST b)
{
  //accept new sprite

  this->setType(newType); // will reset visual pos
  this->setSpriteCounter(newSpriteCounter);

  switch(b)
    {
    case BOOST::NORMAL:
      this->changeBoostMode(0);
      break;

    case BOOST::BOMB:
      this->changeBoostMode(1);
      break;

    case BOOST::ZAP_H:
      this->changeBoostMode(2);
      break;

    case BOOST::ZAP_V:
      this->changeBoostMode(3);
      break;

    case BOOST::ZAP_B:
      this->changeBoostMode(4);
      break;            
    }
  this->boost = b;
  
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

      this->matchStatus = 0; //reset match status from a possible transition from STATE::MATCH
      if (this->getSpriteCounter() != 0)
	{
	  //sprite not done spinning, finish the animation
	  this->setSpriteCounter((this->getSpriteCounter() + 1) % this->getMaxSprite());
	}

      if(this->emptyBelowMe())
	{
	  //there is an empty Tile below me - fall

	  //inc visual Y pos of gem and effect
	  this->setDstY(this->getDstY() + this->getFallingSpeed());
	  
	  if (this->getDstY() >= (this->getDefaultSpriteY() + this->GRID_SIZE))
	    {
	      //my visual is in place for the next Tile to take it
	      //hand off my info to the tile below me
	      board[this->row + 1][this->col].spriteHandOff(this->type,
							    this->spriteCounter,
							    this->boost);

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
		  //reset my boost so it doesn't trigger when I get set to empty later
		  this->changeBoostMode(0); // set my boost mode to NORMAL
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
	  //swap values of type and swapType, also both spriteCounters
	  this->swapTypes();
	  //swap boost types
	  this->swapBoosts();
	  //reset visual coords
	  this->setDstX(getDefaultSpriteX());
	  this->setDstY(getDefaultSpriteY());
	  // this->setType(this->getSwapType());
	  //swap my type right after texture is blitted to ensure the proper Tiles are drawn
	}


      break;

    case STATE::PAWS:

      deltaX = deltaY = 0;
      this->matchStatus = 0;//reset match status from a possible transition from STATE::MATCH
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

	  //back to normal
	  this->state = STATE::IDLE; //resets visual coords
	  //sprites are visually back to normal, take my original type back
	  this->setType(this->getSwapType());
	  //swap boost types
	  this->swapBoosts();
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
      
      //printf("tile (row: %d col: %d) is in FALL\n", this->row, this->col);
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

    case STATE::MATCH:

      //shouldnt even get here - Game should change it before update gets called again
      printf("my state is match... it probably shouldn't be\n");
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

  //update frame counter of effect
  effectFrameWait = (effectFrameWait + 1) % 5;
  effectFrameAngle = (effectFrameAngle + 1) % 360;
  if(effectFrameWait == 0)
    {
      //delay tile effects to only every 5 frames
      if(effectFrameMax != 0)
	{
	  //only update effect frame counter every 5 frames to slow it down
	  effectFrameCounter = (effectFrameCounter + 1) % effectFrameMax;
	}
      else
	{
	  effectFrameCounter = 0;
	}
    }
  //set src for effect
  src={
    effectFrameCounter * effectSize,
    0,
    effectSize,
    effectSize };

      
  //place boost visual on top of Tile
  SDL_Color c; c.r = 255; c.g = 255; c.b = 255;
  SDL_Point p = {24, 24};//point of rotation
  SDL_RendererFlip flip = SDL_FLIP_NONE;//dont flip, just rotate

  switch(this->boost)
    {
      //0
    case BOOST::NORMAL: //no extra visual needed
      break;
      
      //1
    case BOOST::BOMB:
      //displayInfo("B", this->getCol() * 50, this->getRow() * 50, c);

      //set dst for effect
      dst = {
	(int) getDstX(),
	(int) getDstY(),
	32,
	32 };

      SDL_RenderCopy(renderer, fire, &src, &dst);
      break;

      //2
    case BOOST::ZAP_H:
      //displayInfo("ZH", this->getCol() * 50, this->getRow() * 50, c);
      //set src for effect

      //set dst for effect
      dst = {
	(int) getDstX() - 8,
	(int) getDstY() - 8,
	48,
	48 };
      
      SDL_RenderCopy(renderer, electric, &src, &dst);
      break;

      //3
    case BOOST::ZAP_V: // rotate this one 180 deg
      //displayInfo("ZV", this->getCol() * 50, this->getRow() * 50, c);

      //set dst for effect
      dst = {
	(int) getDstX() - 8,
	(int) getDstY() - 8,
	48,
	48 };

      
      //do rotate here - 180
       SDL_RenderCopyEx(renderer, electric, &src, &dst, (double) 180, &p, flip);     
      //SDL_RenderCopy(renderer, electric, &src, &dst);
      break;

      //4
    case BOOST::ZAP_B: // spin this one
      //displayInfo("ZB", this->getCol() * 50, this->getRow() * 50, c);

      //set dst for effect
      dst = {
	(int) getDstX() - 8,
	(int) getDstY() - 8,
	48,
	48 };

      //do rotate here - 5 deg / frame
      SDL_RenderCopyEx(renderer, electric, &src, &dst,
		       (double) effectFrameAngle, &p, flip);
       
      //SDL_RenderCopy(renderer, electric, &src, &dst);
      break;
    }
  
  if(this->shouldBeEmpty)
    {
      this->shouldBeEmpty = false;
      this->changeState("empty");
    }
  
}

int Tile::changeState(const char *newState, int arg2, int arg3, int arg4)
{ //fall
  //printf("Tile:: change state to fall on tile (row: %d col:%d)\n", this->row, this->col);
  if (newState == "fall")
    {
      //printf("\trow: %d and col: %d - changed to state FALL\n", this->getRow(), this->getCol());
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

int Tile::changeState(const char *newState, int arg2) //, int arg3)
{ //swap
  if (newState == "swap")
    {
      //printf("\trow: %d and col: %d - changed to state SWAP\n", this->getRow(), this->getCol());
      /* arg2 is new direction
	 1 - go left
	 2 - go right
	 3 - go up
	 4 - go down */
      int row = 0, col = 0;

      this->state = STATE::SWAP;
      this->swappingDirection = arg2;

      switch (arg2)
	{ //what is my new type once I swap?
	case 1:
	  row = this->row;
	  col = this->col - 1;
	  
	  break;

	case 2:
	  row = this->row;
	  col = this->col + 1;

	  break;

	case 3:
	  row = this->row - 1;
	  col = this->col;

	  break;

	case 4:
	  row = this->row + 1;
	  col = this->col;

	  break;

	default:
	  printf("swap direction invalid: %d\n", arg2);
	  exit(-1);
	}
      //do necessary ops w/ row col
      this->setSwapType(board[row][col].getType());
      this->setOldBoostType(board[row][col].getBoostType());

    }
    else if (newState == "empty")
    {
      //arg2 is boostMode
      //reset swap direction
      this->setSwappingDirection(0);
      this->triggerBoost();

      if(arg2 == 0)
	{
	  //no special Tile made
	  this->state = STATE::EMPTY;
	  this->setType(-1); //update type and swapType to -1 - empty
	}
      else
	{
	  //I am now a special Tile
	  this->changeBoostMode(arg2);
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
      //printf("\trow: %d and col: %d - changed to state IDLE\n", this->getRow(), this->getCol());
      this->state = STATE::IDLE;
      //reset my visual coords
      this->setDstX(this->getDefaultSpriteX());
      this->setDstY(this->getDefaultSpriteY());
      //reset swap direction
      this->setSwappingDirection(0);
      //consider setting swapType to type
      //and oldBoost to boost
    }
  else if (newState == "active")
    {
      //printf("\trow: %d and col: %d - changed to state ACTIVE\n", this->getRow(), this->getCol());
      this->state = STATE::ACTIVE;
    }
  else if (newState == "empty")
    {
      //printf("\trow: %d and col: %d - changed to state EMPTY\n", this->getRow(), this->getCol());
      this->state = STATE::EMPTY;
      this->setType(-1); //update type and swapType to -1 - empty
      //reset swap direction
      this->setSwappingDirection(0);
      this->triggerBoost();
    }
  else if (newState == "paws")
    {
      //printf("\trow: %d and col: %d - changed to state PAWS\n", this->getRow(), this->getCol());
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

void Tile::triggerBoost()
{
  bool above, below, left, right; //bools for bomb
  above = (this->row - 1) >= 0;
  below = (this->row + 1) <= 7;
  left  = (this->col - 1) >= 0;
  right = (this->col + 1) <= 7;
  
  switch(this->boost)
    {
    case BOOST::NORMAL: //do nothing
      break;
      
    case BOOST::BOMB: //the 8 surrounding
      //play sound
      if( Mix_PlayChannel( -1, bombSound, 0 ) == -1 )
	{
	  exit(-1);
	}
      
      this->boost = BOOST::NORMAL;//reset my boost to prevent an infinite loop

      effects[this->row][this->col].startEffect(1);
      
      if(above)
	{
	  //12:00
	  board[this->row - 1][this->col].changeState("empty");
	  score += 25;
	}
      if(above and right)
	{
	  //1:30
	  board[this->row - 1][this->col + 1].changeState("empty");
	  score += 25;
	}
      if(right)
	{
	  //3:00
	  board[this->row][this->col + 1].changeState("empty");
	  score += 25;
	}
      if(below and right)
	{
	  //4:30
	  board[this->row + 1][this->col + 1].changeState("empty");
	  score += 25;
	}
      if(below)
	{
	  //6:00
	  board[this->row + 1][this->col].changeState("empty");
	  score += 25;
	}
      if(below and left)
	{
	  //7:30
	  board[this->row + 1][this->col - 1].changeState("empty");
	  score += 25;
	}
      if(left)
	{
	  //9:00
	  board[this->row][this->col - 1].changeState("empty");
	  score += 25;
	}
      if(above and left)
	{
	  //10:30
	  board[this->row - 1][this->col - 1].changeState("empty");
	  score += 25;
	}
      break;
      
    case BOOST::ZAP_V: //entire col

      //play sound
      if( Mix_PlayChannel( -1, zapSound, 0 ) == -1 )
	{
	  exit(-1);
	}
      
      this->boost = BOOST::NORMAL;//reset my boost to prevent an infinite loop
      for(int i = 0; i < 8; i++)
	{
	  //clear all tiles in my col
	  board[i][this->col].changeState("empty");
	  effects[i][this->col].startEffect(3);
	  score += 25;
	}
      break;
      
    case BOOST::ZAP_H: //entire row

      //play sound
      if( Mix_PlayChannel( -1, zapSound, 0 ) == -1 )
	{
	  exit(-1);
	}
      
      this->boost = BOOST::NORMAL;//reset my boost to prevent an infinite loop
      for(int i = 0; i < 8; i++)
	{
	  //clear all tiles in my row
	  board[this->row][i].changeState("empty");
	  effects[this->row][i].startEffect(2);
	  score += 25;
	}
      break;
      
    case BOOST::ZAP_B: //entire col and row

      //play sound
      if( Mix_PlayChannel( -1, zapSound, 0 ) == -1 )
	{
	  exit(-1);
	}

      this->boost = BOOST::NORMAL;//reset my boost to prevent an infinite loop

      for(int i = 0; i < 8; i++)
	{
	  //clear all tiles in my col and row
	  board[i][this->col].changeState("empty");
	  effects[i][this->col].startEffect(3);
	  board[this->row][i].changeState("empty");
	  effects[this->row][i].startEffect(2);
	  score += 25;
	}
      effects[this->row][this->col].startEffect(4);
      break;
    }
}

void Tile::changeBoostMode(int b)
{
  switch(b)
    {
    case 0:
      this->boost = BOOST::NORMAL;
      this->effectFrameMax = 0;
      this->effectSize = 0;
      
      break;

    case 1:
      this->boost = BOOST::BOMB;
      this->effectFrameMax = 4;
      this->effectSize = 32;
      
      break;

    case 2:
      this->boost = BOOST::ZAP_H;
      this->effectFrameMax = 6;
      this->effectSize = 64;
	    
      break;

    case 3:
      this->boost = BOOST::ZAP_V;
      this->effectFrameMax = 6;
      this->effectSize = 64;
	    
      break;

    case 4:
      this->boost = BOOST::ZAP_B;
      this->effectFrameMax = 6;
      this->effectSize = 64;
	    
      break;
    }
}

void Tile::displayInfo(const char* str, int x, int y, SDL_Color c)
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


#endif
