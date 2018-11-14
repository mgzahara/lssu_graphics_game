#include "effect.h"
#include "globals.h"
#include "SDL.h"

#ifndef EFFECT_CPP
#define EFFECT_CPP

Effect::Effect()
{
  currFrame = 0;
  maxFrame = 0;
  startFrame = 0;
  type = 0;
  //0 - none
  //1 - bomb
  //2 - horizontal lightning
  //3 - vertical lightning
  //4 - bidirectional lightning

  //dimensions of single frame on texture
  texWidth = 0;
  texHeight = 0;
  //x, y to start source texture
  srcX = 0;
  srcY = 0;
  //screen coords for texure blitting
  dstX = -200;
  dstY = -200;
  //corresponding to indecies of Tiles
  row = -1;
  col = -1;  
}

void Effect::update()
{
  static SDL_Rect src, dst;
  static SDL_Texture *texture;

  bool above, below, left, right; //bools for bomb
  above = (row - 1) >= 0;
  below = (row + 1) <= 7;
  left  = (col - 1) >= 0;
  right = (col + 1) <= 7;
  
  if(currFrame > maxFrame)
    {
      //I have finished my animation, reset
      if(type == 1)
	{
	  //tell surrounding Effects they are no longer busy
	  if(above)
	    {
	      //12:00
	      effects[row - 1][col].setIdle();
	      score += 25;
	    }
	  if(above and right)
	    {
	      //1:30
	      effects[row - 1][col + 1].setIdle();
	    }
	  if(right)
	    {
	      //3:00
	      effects[row][col + 1].setIdle();
	    }
	  if(below and right)
	    {
	      //4:30
	      effects[row + 1][col + 1].setIdle();
	    }
	  if(below)
	    {
	      //6:00
	      effects[row + 1][col].setIdle();
	    }
	  if(below and left)
	    {
	      //7:30
	      effects[row + 1][col - 1].setIdle();
	    }
	  if(left)
	    {
	      //9:00
	      effects[row][col - 1].setIdle();
	    }
	  if(above and left)
	    {
	      //10:30
	      effects[row - 1][col - 1].setIdle();
	    }
	}
      type = 0;
      currFrame = 0;
      maxFrame = 0;
    }
  
  switch(type)
    {
    case -1: // busy
    case 0:  // idle
      return;
      
    case 1: // bomb

      src = {
	currFrame * texWidth,
	0,
	texWidth,
	texHeight };
      
      dst = {
	dstX,
	dstY,
	132,
	132 };
      
      SDL_RenderCopy(renderer, explosion, &src, &dst);
      break;
      
    case 2: // horizontal lightning

      src = {
	currFrame * texWidth,
	0,
	texWidth,
	texHeight };
      
      dst = {
	dstX,
	dstY,
	42,
	32 };

      SDL_RenderCopy(renderer, zap, &src, &dst);
      break;
      
    case 3: // vertical lightning

      src = {
	currFrame * texWidth,
	srcY,
	texWidth,
	texHeight };
	
      dst = {
	dstX,
	dstY - 10,
	32,
	32 + 20 };

      SDL_RenderCopy(renderer, zap, &src, &dst);
      break;
      
    case 4: // bidirectional lightning

      src = {
	currFrame * texWidth,
	0,
	texWidth,
	texHeight };
      
      dst = {
	dstX,
	dstY,
	32,
	32 };
      
      SDL_RenderCopy(renderer, zap, &src, &dst);
      //reassign src here
      src = {
	currFrame * texWidth,
	128,
	texWidth,
	texHeight };
      
      
      SDL_RenderCopy(renderer, zap, &src, &dst);
      break;

    default:
      //not of a recognized type
      printf("Effect row: %d col: %d had type not -1 - 4 (%d)", row, col, type);
      exit(-1);
    }
  currFrame++;
}

void Effect::setRow(int r)
{
  row = r;
}
void Effect::setCol(int c)
{
  col = c;
}

void Effect::startEffect(int e)
{
  if(type != 0)
    {
      //cannot do an effect right now, I am busy
      return;
    }
  type = e;
  currFrame = 0;
  startFrame = 0;
  
  switch(type)
    {
    case 0:
      //0 - none
      maxFrame = 0;
    case 1:

      //when the bomb animation is done, tell the surrounding Effects
      //they are not busy any more
      
      //1 - bomb
      maxFrame = 12;
      texWidth = 96;
      texHeight = 96;
      srcX = 0;
      srcY = 0;
      
      dstX = (col * GRID_SIZE) - 50;
      dstY = (row * GRID_SIZE) - 50;
      break;
      
    case 2:
      //2 - horizontal lightning
      maxFrame = 8;
      texWidth = 64;
      texHeight = 64;
      srcX = 0;
      srcY = 0;

      dstX = ((col * GRID_SIZE) + BOX_SIZE);
      dstY = ((row * GRID_SIZE) + BOX_SIZE);
      break;
      
    case 3:
      //3 - vertical lightning
      maxFrame = 8;
      texWidth = 64;
      texHeight = 64;
      srcX = 0;
      srcY = 128;

      dstX = ((col * GRID_SIZE) + BOX_SIZE + 6);
      dstY = ((row * GRID_SIZE) + BOX_SIZE + 6);
      break;
      
    case 4:
      //4 - bidirectional lightning
      maxFrame = 8;
      texWidth = 64;
      texHeight = 64;
      //be sure to blit both horizontal AND vertical textures
      //horizontal starts at 0,0 vertical starts at 0,128
      srcX = 0;
      srcY = 0;

      dstX = ((col * GRID_SIZE) + BOX_SIZE + 6);
      dstY = ((row * GRID_SIZE) + BOX_SIZE + 6);
      break;

    default:
      //not of a recognized type
      printf("Effect row: %d col: %d had type not 0-4 (%d)", row, col, type);
      exit(-1);
    }
}

void Effect::setBusy()
{
  type = -1;
}

void Effect::setIdle()
{
  type = 0;
}


#endif
