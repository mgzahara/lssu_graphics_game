#ifndef TILE
#define TILE

#include "SDL.h"
#include "SDL_image.h"
// #include "globals.h"

class Tile
{
  public:
	//states for Tile state machine
	enum STATE
	{
		IDLE,
		ACTIVE,
		SWAP,
		PAWS,
		EMPTY,
		FALL,
		MATCH
	};

	int row;
	int col;
	int type;
	//visual coords of sprite
	float spriteDstX;
	float spriteDstY;

	//consts
	int GRID_SIZE;
	int GEM_SIZE;
	int BOX_SIZE;
	int TILE_FALLING_SPEED;
	int TILE_SWAPPING_SPEED;

	STATE state;

	//new
	int spriteCounter;
	int maxSprite;
	bool active;
	bool swapping;
	bool falling;
	int swappingDirection;
	int fallsRemaining;
	int swapType;
	float fallingSpeed;

	void setSpriteCounter(int);
	void setMaxSprite(int);
	void setSwappingDirection(int);
	void setSwapType(int);
	void setFallingSpeed(float);
	void setFallsRemaining(int);

	int sign(int);
	int sign(float);
	int getSpriteCounter();
	int getMaxSprite();
	int getSwappingDirection();
	int getFallsRemaining();
	int getSwapType();

	bool getFalling();
	float getDefaultSpriteX();
	float getDefaultSpriteY();
	float getFallingSpeed(); // public

	//used to be public
	//void setHiddenType(int t);//unnecessary
	void setDstX(float x); //make private?
	void setDstY(float y); //make private?
	//new
	void setActive(bool);
	void setSwapping(bool);
	void setSwapping(bool, int, int);
	void setFalling(bool);
	//type, falls left, speed, must be public to be called by other Tiles
	void takeFallingSprite(int, int, float);
	//needs to be called during match()
	void setFalling(bool, int); //falling or not, distance to fall

	//getters
	int getRow();
	int getCol();
	int getType();
	//int getHiddenType();
	float getDstX();
	float getDstY();
	bool getActive();
	bool isAdjacent(Tile t);
	bool getSwapping();
	//new
	//the main function for a Tile

	//Tile operator =  (Tile t);
	//bool operator == (Tile t);

	//constructor
	Tile();
	//destructor
	~Tile();
	//setters
	void setRow(int r);
	void setCol(int c);
	void setType(int t);
	void update();
	//overload this function to your hearts content
	int changeState(const char *, int, int, int);
	int changeState(const char *, int);
	int changeState(const char *);
	//
	bool isInMatchState();
};
#endif
