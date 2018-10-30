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
  TILE_FALLING_SPEED = 2.5;
  TILE_SWAPPING_SPEED = 2.5;

  state = STATE::IDLE;

  // falling = false;
  // swapping = false;
  // active = false;
}
Tile::~Tile()
{
  return;
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
  //set current sprite to 0
  this->setSpriteCounter(0); //maybe dont do this... allow them to finish their rotation
                             //max sprite to appropriate int
                             //play around with everything have a maxSprite of 30
  this->setMaxSprite(30);
  // if (this->getType() == 4 or this->getType() == 5)
  // {
  //   this->setMaxSprite(30);
  // }
  // else
  // {
  //   this->setMaxSprite(60);
  // }
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
float Tile::getFallingSpeed()
{
  return fallingSpeed;
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

bool Tile::isInMatchState()
{
  return this->state == STATE::MATCH;
}

//action functions

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
    matchBoard[this->getRow()][this->getCol()] = true;
  }
  else
  {
    this->setFallingSpeed(speed);
  }
  this->setDstX(this->getDefaultSpriteX());
  this->setDstY(this->getDefaultSpriteY());
}

void Tile::update()
{
  SDL_Rect src, dst;
  int deltaX, deltaY;

  switch (this->state)
  {
  case STATE::IDLE:
    printf("my state is idle\n");
    this->setDstY(this->getDefaultSpriteY());
    this->setDstY(this->getDefaultSpriteY());
    if (this->getSpriteCounter() != 0)
    {
      //sprite not done spinning, finish the animation
      this->setSpriteCounter((this->getSpriteCounter() + 1) % this->getMaxSprite());
    }
    break;

  case STATE::SWAP:
    printf("my state is swap\n");

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

    if (this->getDstX() == (this->getDefaultSpriteX() + (sign(deltaX) * 50)) and
        this->getDstY() == (this->getDefaultSpriteY() + (sign(deltaY) * 50)))
    {
      //tell game to check me for match
      this->state = STATE::MATCH;
      //swap my type right after texture is blitted to ensure the proper Tiles are drawn
    }

    break;

  case STATE::PAWS:
    printf("my state is paws\n");

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

    if (this->getDstX() == (this->getDefaultSpriteX() + (sign(deltaX) * 50)) and
        this->getDstY() == (this->getDefaultSpriteY() + (sign(deltaY) * 50)))
    {
      //back to normal
      this->state = STATE::IDLE;
      //swap my type right after texture is blitted to ensure the proper Tiles are drawn
    }

    break;

  case STATE::ACTIVE:
    printf("my state is active\n");
    this->setSpriteCounter(
        (this->getSpriteCounter() + 1) % this->getMaxSprite());

    break;

  case STATE::EMPTY:
    printf("my state is empty\n");
    //ensure the empty sprite - OR - dont put them on renderer
    this->setType(-1);
    break;

  case STATE::FALL:
    printf("my state is fall\n");

    //sprite is falling, inc visual Y pos
    this->setDstY(this->getDstY() + this->getFallingSpeed());
    //be sure that each falling sprite is at 0
    this->setSpriteCounter(0);

    if (this->getDstY() >= (this->getDefaultSpriteY() + 50))
    {
      //texture visual is in place to be taken by next tile
      board[this->getRow() + 1][this->getCol()]
          .changeState("fall",
                       this->getType(),
                       this->getFallsRemaining() - 1,
                       this->getFallingSpeed());
    }
    break;

  case STATE::MATCH: //shouldnt even get here - Game should change it before update gets called again
    printf("my state is match\n");
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
  if (this->state == STATE::MATCH or this->state == STATE::IDLE)
  {
    //done falling or swapping or swapping back
    //udpate type just to be safe - swaps needs it
    this->setType(this->getSwapType());
  }
}

int Tile::changeState(const char *newState, int arg2, int arg3, int arg4)
{ //fall
  if (newState == "fall")
  {
    /*
    arg2 - type
    arg3 - my fallsRemaining
    arg4 - fallingSpeed
    */
    this->setType(arg2);
    if (arg3 == 0)
    {
      //im done falling, go to MATCH state to wait for match checking
      this->state = STATE::MATCH;
      this->setFallsRemaining(0);
      this->setFallingSpeed(0);
    }
    else
    {
      //im still falling
      this->state = STATE::FALL;
      this->setFallsRemaining(arg3);
      this->setFallingSpeed(arg4);
    }
  }
}

int Tile::changeState(const char *newState, int arg2)
{ //swap
  if (newState == "swap")
  {
    /* arg2 is new direction
	     1 - go left
	     2 - go right
	     3 - go up
	     4 - go down */

    printf("state changed to swap\n");
    this->state = STATE::SWAP;
    this->swappingDirection = arg2;
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
}

int Tile::changeState(const char *newState)
{ //idle & active & empty & paws

  if (newState == "idle")
  {
    printf("state changed to idle\n");
    this->state = STATE::IDLE;
  }
  else if (newState == "active")
  {
    printf("state changed to active\n");
    this->state = STATE::ACTIVE;
  }
  else if (newState == "empty")
  {
    printf("state changed to empty\n");
    this->state = STATE::EMPTY;
  }
  else if (newState == "paws")
  {
    printf("state changed to paws\n");
    this->state = STATE::PAWS;

    switch (this->getSwappingDirection())
    { // reverse swapping direction and set new swapType
    case 1:
      this->setSwappingDirection(2);
      this->setSwapType(board[this->row][this->col + 1].getType());
      break;
    case 2:
      this->setSwappingDirection(1);
      this->setSwapType(board[this->row][this->col - 1].getType());
      break;
    case 3:
      this->setSwappingDirection(4);
      this->setSwapType(board[this->row + 1][this->col].getType());
      break;
    case 4:
      this->setSwappingDirection(3);
      this->setSwapType(board[this->row - 1][this->col].getType());
      break;
    default:
      printf("Paws direction of Tile at row %d col %d was %d\n\n", this->getRow(), this->getCol(), this->getSwappingDirection());
      exit(-1);
    } //end switch(swappingDirection)
  }   //end else if newState == "paws"
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
