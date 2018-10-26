#include "tile.h"
#include "math.h"
#include "iostream"

#ifndef TILE_CPP
#define TILE_CPP

Tile::Tile()
{
  //no designated row or col
  row = -1;
  col = -1;
  //undef type
  type = 0;
  GRID_SIZE = 50;
  BOX_SIZE = 3;
  TILE_FALLING_SPEED = 2.5;
  TILE_SWAPPING_SPEED = 2.5;
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
  type = t;
  //set current sprite to 0 
  this.setSpriteCounter(0);
  //max sprite to appropriate int
  if(this.getType() == 4)
    { this.setMaxSprite(30); }
  else { this.setMaxSprite(60); }
  //visual loc to default
  this.setDstX(this.getDefaultSpriteX() );
  this.setDstY(this.getDefaultSpriteY() );
  //same as swapType
  this->setSwapType(t);
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
  //1 - left
  //2 - right
  //3 - up
  //4 - down
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
voif Tile::setSwapping(bool b)
{
  swapping = b;
}
void Tile::setSwapping(bool b, int d, int t)
{
  swapping = b;
  this->setSwappingDirection(d);
  this->setSwapType(t);
}
void Tile::setFallingSpeed(float s)
{
  fallingSpeed = s;
}
void Tile::setFalling(bool f, int dis)
{//public to be called by match to start falling process
  falling = f;
  this->fallsRemaining(dis);
  this->setFallingSpeed( (float) (dis * TILE_FALLING_SPEED ) );
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
  return ( ( this.getCol() * GRID_SIZE ) + BOX_SIZE + 6);
}
float Tile::getDefaultSpriteY()
{
  return ( ( this.getRow() * GRID_SIZE ) + BOX_SIZE + 6);
}
float Tile::getFallingSpeed()
{
  return fallingSpeed;
}


int Tile::sign(int x)
{
  if( x > 0 )
    { return 1; }
  if( x < 0 )
    { return -1; }
  return 0;
}
int Tile::sign(float x)
{
  if( x > (float) 0.0 )
    { return 1; }
  if( x < (float) 0.0 )
    { return -1; }
  return 0;
}


//action functions

bool Tile::isAdjacent(Tile t)
{
  //true -> this Tile and arg Tile are vertically or horizontally adjacent
  
  if( (abs(row - t.getRow() ) == 1 and
       abs(col - t.getCol() ) == 0 )     //directly above/below
      or 
      (abs(col - t.getCol() ) == 1 and
       abs(row - t.getRow() ) == 0 ) )   //directly left/right
    
    {//only true if directly left, right, up, or down
      return true;
    }
  
  return false;
}

void Tile::takeFallingSprite(int type, int falls, float speed)
{
  //how a Tile hands off its falling sprite to another Tile
  this->setType(type);
  this->setFallsRemaining(falls - 1);
  if(this->getFallsRemaining() == 0)
    {
      //done falling
      this->setFalling(false, 0);
      this->setFallingSpeed( (float) 0 );
      //check this Tile for a match
      matchBoard[ this->getRow() ][ this->getCol() ] = true;
    }
  else
    {
      this->setFallingSpeed(speed);
    }
  this->setDstX( this->getDefaultSpriteX() );
  this->setDstY( this->getDefaultSpriteY() );
}

void Tile::update()
{
//this is all psuedo code


    SDL_Rect src, dst;
    
    if( !this->getFalling() and
	!this->getSwapping() and
	!this->getActive() )
      {
	//no special states, just waiting on the board
	this->setDstY( this->getDefaultSpriteY() );
        this->setDstY( this->getDefaultSpriteY() );
      }
    
    if(this->getFalling() )
      {
	//sprite is falling, inc visual Y pos
	this->setDstY( this->getDstY() + this.getFallingSpeed() );
	//be sure that each falling sprite is at 0
        this->setSpriteCounter(0);
	
        if( this->getDstY() >= ( this->getDefaultSpriteY() + 50 ) )
	  {
            //texture visual is in place to be taken by next tile
	    board[ this->getRow() + 1 ][ this->getCol() ].takeFallingSprite( this->getType(),
									     this->getFallsLeft(),
									     this->getFallingSpeed() );
	  }
      }
    if( this->getSwapping() or this->getActive() )
      {
        //this tile is swapping or active, animate
        this->setSpriteCounter( ( this->getSpriteCounter() + 1 ) % this->getMaxSprite() );
    }
    if( this.getSwapping() )
    {
        int deltaX = 0, deltaY = 0;
        switch( this.getSwappingDirection() )
        {
            case 1:
                deltaX = -TILE_SWAPPING_SPEED; break;
            case 2:
                deltaX =  TILE_SWAPPING_SPEED; break;
            case 3:
                deltaY = -TILE_SWAPPING_SPEED; break;
            case 4:
                deltaY =  TILE_SWAPPING_SPEED; break;
            default:
                printf("Swap direction of Tile at row %d col %d was %d\n\n", this->getRow(), this->getCol(), this->getSwappingDirection());
                exit( -1 );
        }
        this->setDstX( this->getDstX() + deltaX );
        this->setDstY( this->getDstY() + deltaY );

        if( this->getDstX == ( this->getDefaultSpriteX() + ( sign( deltaX ) * 50 ) ) and
            this->getDstY == ( this->getDefaultSpriteY() + ( sign( deltaY ) * 50 ) )   )
        {
            //swapped all the way to final destination
	  this->setSwapping(false);
	  //tell game to check me for match
	  matchBoard[ this->getRow() ][ this->getCol() ] = true;
            //swapTilePos - switch types with other tile - NEXT TIME - or right after texture is blitted
            //reset swapping variable - done by match()
            //add this tile to data structure of tiles to check for matches
                //vector?
                //parallel 2d array of bools?
        }


    }//end if this.getSwapping()
    src = {
        this->getSpriteCounter() * GEM_SIZE,
        0,
        GEM_SIZE,
        GEM_SIZE
    };
    dst = {
        this->getDstX(),
        this->getDstY(),
        GEM_SIZE,
        GEM_SIZE
    };

    //put this tile on renderer
    if( this->getType() < 0 )
    {
        //if this tile type is -1, empty tile
        SDL_RenderCopy( renderer, empty_gem, &src, &dst);
    }
    else
    {
        //this tile is visible
        SDL_RenderCopy( renderer, gem[ this->getType() ], &src, &dst);
    }    

    if(!this->getSwapping() )
      {
	//no swapping, either wasn't before -> no change
	//or was before -> change logical positions with swapee
	this->setType( this->getSwapType() );
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
