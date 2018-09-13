#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "basics.h"
#include "tile.h"

using namespace std;

bool loop();
void startBoard();

const int BOARD_SIZE = 8;
Tile board[BOARD_SIZE][BOARD_SIZE];

int main(int argc, char** args) {

/*  Tile t;
  t.setCol(1);
  t.setRow(2);
  t.setType(3);
*/
  //R = (rand() % (upper - lower + 1)) + lower;


  if ( !initSDL() ) return 1;//if we cant init, end main
  
  startBoard();
  
  while ( loop() ) {
    // wait before processing the next frame
    //prevents weird screen tearing
    SDL_Delay(10);
    }

  return 0;
}

bool loop() {
  //the real main
  //handles events, drawing
  //returns false when end con is met
  
  //both are used to limit screen refreshes further
  //static const int pause = 8;
  //static int frameCounter = 0;
  SDL_PumpEvents();
  
  static int i, j, mouse_x, mouse_y;
  //box selection base x, y
  static int box_x = -BOX_SIZE * 2;
  static int box_y = -BOX_SIZE * 2;
  static SDL_Rect top, right, bot, left;//SDL_Rect's for box selection
  
  // Event loop
  while ( SDL_PollEvent( &e ) != 0 ) {
    switch ( e.type ) {//no events
    case SDL_QUIT:
      //close button
      return false;
    case SDL_MOUSEBUTTONDOWN:
      if( SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT) )
	{//grab mouse position on left click
	  SDL_GetMouseState(&mouse_x, &mouse_y);
	  //when this is all OOPed, the args to the above func
	  //will be data members of an instance of a class
	}
      break;
    }
  }
  //black background
  SDL_SetRenderDrawColor(renderer, c.bla.r, c.bla.g, c.bla.b, 255);
  SDL_RenderClear(renderer);
  
  //draw a grid in purple
  SDL_SetRenderDrawColor(renderer, c.whi.r, c.whi.g, c.whi.b, 255);
  for(i = 1; i <= (WINDOW_WIDTH / GRID_SIZE); i++){
    //draw vertical lines
    SDL_RenderDrawLine(renderer, i * GRID_SIZE, 0, i * GRID_SIZE, WINDOW_HEIGHT);
  }
  for(i = 1; i <= (WINDOW_HEIGHT / GRID_SIZE); i++){
    //draw horizontal lines
    SDL_RenderDrawLine(renderer, 0, i * GRID_SIZE, WINDOW_WIDTH, i * GRID_SIZE);
  }

//draw all tiles
  for(i = 0; i< BOARD_SIZE; i++)
  {
    for(j = 0; j < BOARD_SIZE; j++)
    {
      switch(board[i][j].getType())
      {
        case 1:
          SDL_SetRenderDrawColor(renderer, c.whi.r, c.whi.g, c.whi.b, 255);
        break;

        case 2:
          SDL_SetRenderDrawColor(renderer, c.red.r, c.red.g, c.red.b, 255);
        break;

        case 3:
          SDL_SetRenderDrawColor(renderer, c.pur.r, c.pur.g, c.pur.b, 255);
        break;

        case 4:
          SDL_SetRenderDrawColor(renderer, c.blu.r, c.blu.g, c.blu.b, 255);
        break;

        case 5:
          SDL_SetRenderDrawColor(renderer, c.ora.r, c.ora.g, c.ora.b, 255);
        break;

        default:
        printf("tile %d, %d has no type\n", i, j);
      }
      //draw each rect with proper coors and size
      SDL_Rect r = {board[i][j].getCol() * GRID_SIZE + BOX_SIZE, 
                    board[i][j].getRow() * GRID_SIZE + BOX_SIZE,
                    GRID_SIZE - (2 * BOX_SIZE) + 1,
                    GRID_SIZE - (2 * BOX_SIZE) + 1};
      SDL_RenderFillRect(renderer, &r);
    }
  }


  if( mouse_x % GRID_SIZE > BOX_SIZE and
      mouse_x % GRID_SIZE < GRID_SIZE - BOX_SIZE and
      mouse_y % GRID_SIZE > BOX_SIZE and
      mouse_y % GRID_SIZE < GRID_SIZE - BOX_SIZE and
      mouse_x > 0 and mouse_y > 0)
  {//ignore clicks if they are within 3 pixels of a grid line
    //snap click to an int of row num and col num
    box_x = (mouse_x / GRID_SIZE) * GRID_SIZE;
    box_y = (mouse_y / GRID_SIZE) * GRID_SIZE;
    //it may be advantageous to leave the above ints in terms of
    //which row and col was selected so we can easily determine
    //the object it affects
    
    //printf("mouse_x: %d\nmouse_y: %d\n---------\n", mouse_x, mouse_y);
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
    left ={box_x,
	     box_y,
	     BOX_SIZE,
	     GRID_SIZE};
  }
  //draw selection box
  SDL_SetRenderDrawColor(renderer, c.gre.r, c.gre.g, c.gre.b, 255);
  SDL_RenderFillRect(renderer, &top);
  SDL_RenderFillRect(renderer, &right);
  SDL_RenderFillRect(renderer, &bot);
  SDL_RenderFillRect(renderer, &left);

  
  // Update window
  SDL_RenderPresent( renderer );
  mouse_x = mouse_y = -1;
  return true;
}

void startBoard()
{
  int lower = 1;
  int upper = 5;

  for(int i = 0; i < BOARD_SIZE; i++)
  {
    for(int j = 0; j < BOARD_SIZE; j++)
    {
      board[i][j].setRow(i);
      board[i][j].setCol(j);
      board[i][j].setType((rand() % (upper - lower + 1)) + lower);
    }
  }

/*  //print board state  
  for(int i = 0; i < BOARD_SIZE; i++)
  {
    printf("row #%d\n", i);
    for(int j = 0; j < BOARD_SIZE; j++)
    {
      printf("\tcol#%d\n", j);
      printf("\t\trow %d, col %d, type %d\n\n", board[i][j].getRow(), board[i][j].getCol(), board[i][j].getType());
    }
  }
  */
}