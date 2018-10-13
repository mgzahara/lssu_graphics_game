#include "iostream"
#include "stdlib.h"
#include "time.h"
#include "tile.h"

using namespace std;

//globals
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Event e;
//tile setup
Tile activeTile;//highlighted Tile - first click
Tile swappingTile;//never highlighted - second click

const int BOARD_SIZE = 8;
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int GRID_SIZE = 50;
const int BOX_SIZE = 3;
const int GEM_SIZE = 32;

Tile board[BOARD_SIZE][BOARD_SIZE];//hold all board Tiles

struct colors
{
  struct color
  {int r, g, b;};
  color whi, bla, red, blu, gre, pur, yel, ora, pin, cya, mar;
}c;

struct sprite
{
  struct velocity{ int x, y; };
  struct bounds{ int left, right, top, bot; };

  SDL_Texture* tex;
  SDL_Rect src, dst;
  int spriteCounter, startSprite, endSprite, scaleFactor;
  velocity vel;
  bounds bound;
  int spriteWidth, spriteHeight;

}gems[6];

enum colorType
  {
    color_white,
    color_black,
    color_red,
    color_green,
    color_blue,
    color_purple,
    color_yellow,
    color_orange,
    color_pink,
    color_cyan,
    color_maroon
  };


bool initSDL();
void quitSDL();
void setColors();
SDL_Texture* loadImage(const char*);
SDL_Texture* loadKeyedImage(const char*);
void drawTiles();
void printBoard();
void handleLeftMouseClick(int, int);


SDL_Texture* loadImage(const char* filename)
{
  SDL_Texture *texture;

  texture = IMG_LoadTexture(renderer, filename);
  if (texture == NULL)
    {
      printf("IMG_LoadTexture Error: %s\n", IMG_GetError() );
      exit(1);
    }

  return texture;
}

SDL_Texture* loadKeyedImage(const char* filename)
{
  SDL_Surface* surface;
  SDL_Texture* texture;

  surface = IMG_Load(filename);
  SDL_SetColorKey(surface, SDL_TRUE, 0xff00ff);
  texture = SDL_CreateTextureFromSurface(renderer, surface);
  return texture;
}

bool initSDL() {

  int x = atexit(quitSDL);//set exit function
  if( x != 0 )
    {
      fprintf(stderr, "cannot set exit function\n");
      exit(EXIT_FAILURE);
    }
  //set up rgb values for colors
  setColors();
  //first activeTile will have unreal row and col
  activeTile.setRow(-10);
  activeTile.setCol(-10);
  activeTile.setType(1);
  //swappingTile is also impossible
  swappingTile.setRow(-15);
  swappingTile.setCol(-15);
  swappingTile.setType(1);


  //seed rand
  srand(time(NULL));
  
  // See last example for comments
  if ( SDL_Init( SDL_INIT_EVERYTHING ) != 0 ) {
    cout << "Error initializing SDL: " << SDL_GetError() << endl;
    //system("pause");
    return false;
  }
  //create window
  window = SDL_CreateWindow( "Grid",
			     SDL_WINDOWPOS_UNDEFINED,
			     SDL_WINDOWPOS_UNDEFINED,
			     WINDOW_WIDTH,
			     WINDOW_HEIGHT,
			     SDL_WINDOW_SHOWN );
  
  if ( !window ) {
    cout << "Error creating window: " << SDL_GetError()  << endl;
    //system("pause");
    return false;
  }
  //create renderer
  renderer = SDL_CreateRenderer( window,
				 -1,
				 SDL_RENDERER_ACCELERATED );
  
  if ( !renderer ) {
    cout << "Error creating renderer: " << SDL_GetError() << endl;
    return false;
  }



  //set up gem1 sprite
  gems[0].tex = loadImage("img/gem1.png");
  gems[0].startSprite = 0;
  gems[0].spriteCounter = 0;
  gems[0].endSprite = 60;

  //set up gem2 sprite
  gems[1].tex = loadImage("img/gem2.png");
  gems[1].startSprite = 0;
  gems[1].spriteCounter = 0;
  gems[1].endSprite = 60;

  //set up gem3 sprite
  gems[2].tex = loadImage("img/gem3.png");
  gems[2].startSprite = 0;
  gems[2].spriteCounter = 0;
  gems[2].endSprite = 60;

    //set up gem4 sprite
  gems[3].tex = loadImage("img/gem4.png");
  gems[3].startSprite = 0;
  gems[3].spriteCounter = 0;
  gems[3].endSprite = 60;

  //set up gem5 sprite
  gems[4].tex = loadImage("img/gem5.png");
  gems[4].startSprite = 0;
  gems[4].spriteCounter = 0;
  gems[4].endSprite = 30;

  //set up gem6 sprite
  gems[5].tex = loadImage("img/gem6.png");
  gems[5].startSprite = 0;
  gems[5].spriteCounter = 0;
  gems[5].endSprite = 60;

  //white out the screen
  SDL_SetRenderDrawColor( renderer, c.whi.r, c.whi.g, c.whi.b, 255 );
  SDL_RenderClear( renderer );

  return true;
}

void quitSDL() {
  // Quit - memory clean up
  SDL_DestroyRenderer( renderer );
  SDL_DestroyWindow( window );
  SDL_Quit();
}

void setColors()
{
  c.pur.r = c.pur.b  = c.mar.r = 128;
  c.ora.g = 69;
  c.pin.g = 20;
  c.pin.b = 147;
  c.whi.r =
    c.whi.g =
    c.whi.b =
    c.red.r =
    c.blu.b =
    c.gre.g =
    c.ora.r =
    c.yel.r =
    c.yel.g =
    c.cya.g =
    c.cya.b =
    c.pin.r = 255;

  c.bla.r =
    c.bla.g =
    c.bla.b =
    c.red.g =
    c.red.b =
    c.blu.r =
    c.blu.g =
    c.gre.r =
    c.gre.b =
    c.pur.g =
    c.ora.b =
    c.yel.b =
    c.mar.g =
    c.mar.b =
    c.cya.r = 0;
}

void drawTiles()
{//draw all tiles

  SDL_Texture* tex = NULL;
  SDL_Rect src;
  SDL_Rect dst;

  for(int i = 0; i< BOARD_SIZE; i++)
  {
    for(int j = 0; j < BOARD_SIZE; j++)
    {
      tex = gems[board[i][j].getType() - 1].tex;

      if(board[i][j].getCol() == activeTile.getCol() and
        board[i][j].getRow() == activeTile.getRow() and
        board[i][j].getType() == activeTile.getType())
      {//current tile is 'active' - animate it
        gems[board[i][j].getType() - 1].spriteCounter++;

        src = {(( gems[board[i][j].getType() - 1].spriteCounter % 
                    gems[board[i][j].getType() - 1].endSprite ) + 
                  gems[board[i][j].getType() - 1].startSprite ) * 
                GEM_SIZE,
                0,
                GEM_SIZE,
                GEM_SIZE};
      }
      else
      {
        src = {0,
               0,
               GEM_SIZE,
               GEM_SIZE};
      }
      
      dst = {board[i][j].getCol() * GRID_SIZE + BOX_SIZE + 6,
              board[i][j].getRow() * GRID_SIZE + BOX_SIZE + 6,
              GEM_SIZE,
              GEM_SIZE};

      SDL_RenderCopy(renderer, tex, &src, &dst);

      //position of each image should be as follows:
      //x: board[i][j].getCol() * GRID_SIZE + BOX_SIZE + 9
      //y: board[i][j].getRow() * GRID_SIZE + BOX_SIZE + 9
      
    }
  }

}

void printBoard()
{//print tile info for board - for debugging
  printf("\nprinting board\n");

  for(int i = 0; i < BOARD_SIZE; i++)
  {
    printf("row #%d\n", i);
    for(int j = 0; j < BOARD_SIZE; j++)
    {
      printf("\tcol#%d\n", j);
      printf("\t\trow:  %d\n", board[i][j].getRow()  );
      printf("\t\tcol:  %d\n", board[i][j].getCol()  );
      printf("\t\ttype: %d\n", board[i][j].getType() );
    }
  }
}

/*
void handleLeftMouseCLick(int mouse_x, int mouse_y)
{
  int clickCol, clickRow, box_x, box_y;

  if( mouse_x > 0 and mouse_y > 0 and
      mouse_x % GRID_SIZE > BOX_SIZE and
      mouse_x % GRID_SIZE < GRID_SIZE - BOX_SIZE and
      mouse_y % GRID_SIZE > BOX_SIZE and
      mouse_y % GRID_SIZE < GRID_SIZE - BOX_SIZE )
  {//ignore clicks if they are within 3 pixels of a grid line
    //snap click to an int of row num and col num
    //indices into board array
    clickCol = mouse_x / GRID_SIZE;
    clickRow = mouse_y / GRID_SIZE;
    //pixel coordinates for activeTile highlighting
    box_x = clickCol * GRID_SIZE;
    box_y = clickRow * GRID_SIZE;

    if(activeTile.isAdjacent(board[clickRow][clickCol]) )
    {//swap

      //right now the swap is as easy as switching the types of the Tiles
      //eventually it will need to be animated - multithreaded? child process?

      board[clickRow][clickCol].setType( activeTile.getType() );//curr click Tile = activeTile

      board[activeTile.getRow()]
        [activeTile.getCol()]
        .setType( board[clickRow][clickCol].getType() );//activeTIle = curr click Tile
      
      //reset activeTile stats
      activeTile.setRow(-2);
      activeTile.setCol(-2);
      activeTile.setType(0);

      // //reset activeTile highlight box
      // top.x = -200;
      // top.y = -200;
      // bot.x = -200;
      // bot.y = -200;
      // left.x = -200;
      // left.y = -200;
      // right.x = -200;
      // right.y = -200;
    }
    else{ activeTile = board[clickRow][clickCol]; }
    
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
}
*/
