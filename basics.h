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
int activeTileX;
int activeTileY;

//Tile activeTile;//highlighted Tile - first click
Tile swappingTile;//never highlighted - second click

const int BOARD_SIZE = 8;//board is 8x8
const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int GRID_SIZE = 50;//size of one square on board
const int BOX_SIZE = 3;//thickness of highlight box
const int GEM_SIZE = 32;//size of gem sprites

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
bool validLeftMouseClick(int, int);
void startBoard();
char* matchCheck(int, int, int);
bool swap(Tile, Tile);
void drawBoard();
void swapTilePos(int, int, int, int);
void drawHighlightBox(int, int);



char* matchCheck(int refRow, int refCol, int refType)
{//checks the Tile passed for a match
 //results is a string containing ints of the number of matches in a direction
 // "<left><right><above><below>"
  
  int leftMatches, rightMatches, aboveMatches, belowMatches, row, col, type;
  static char matchString[10];
  char intString[15];
  
  strcpy(matchString, "");//reset
  leftMatches = rightMatches = aboveMatches = belowMatches = 0;
  row = refRow;
  col = refCol;
  type = refType;

  //currTile is board[row][col]

  while(col >= 0 and board[row][col - 1].getType() == type)
    {//while left matches
      leftMatches++;
      col--;
    }
  col = refCol;


  while(col <= BOARD_SIZE and board[row][col + 1].getType() == type)
    {//while right matches
      rightMatches++;
      col++;
    }
  col = refCol;

  while(row >= 0 and board[row - 1][col].getType() == type)
    {//while above matches
      aboveMatches++;
      row--;
    }
  row = refRow;

  while(row <= BOARD_SIZE and board[row + 1][col].getType() == type)
    {//while below matches
      belowMatches++;
      row++;
    }
  row = refRow;

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

  cout << "match string for Tile at\n\trow: " << row << "\n\tcol: " << col;
  cout << endl << matchString << endl;

  return matchString;

}


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
  activeTileX = -10;
  activeTileY = -10;
  //activeTile.setType(1);
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
	  
	  if(board[i][j].getCol() == activeTileX and
	     board[i][j].getRow() == activeTileY)
	    {//current tile is 'active' - animate it
	      gems[board[i][j].getType() - 1].spriteCounter++;
	      
	      src = {( ( gems[board[i][j].getType() - 1].spriteCounter % 
			 gems[board[i][j].getType() - 1].endSprite ) + 
		       gems[board[i][j].getType() - 1].startSprite ) * 
		     GEM_SIZE,
		     0,
		     GEM_SIZE,
		     GEM_SIZE};
	    }
	  //right here check if tile is special
	  //add effects for special tiles
	  else
	    {//
	      src = {0,
		     0,
		     GEM_SIZE,
		     GEM_SIZE};
	    }
	  
	  dst = {(int)board[i][j].getDstX(),
		 (int)board[i][j].getDstY(),
		 GEM_SIZE,
		 GEM_SIZE};
	  
	  SDL_RenderCopy(renderer, tex, &src, &dst);
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


void startBoard()
{
  //board is [row][col]
  //col is inc inside inner loop, so the filling order is as follows
    //(0, 0) then the entire first row gets filled left to right
    //then the row is inc and it repeats
  //this means that there will never be Tiles to the right or below
  //as tiles are filled, check to the left and above for matches
  //if match was created, chose a diffrent type so as to not create a match
  
  int lower = 1;
  int upper = 6;
  
  for(int i = 0; i < BOARD_SIZE; i++)
    {//row
      for(int j = 0; j < BOARD_SIZE; j++)
	{//col
	  board[i][j].setRow(i);
	  board[i][j].setCol(j);
	  board[i][j].setType((rand() % (upper - lower + 1)) + lower);
	  board[i][j].setDstX((float)(j * GRID_SIZE + BOX_SIZE + 6) );
	  board[i][j].setDstY((float)(i * GRID_SIZE + BOX_SIZE + 6) );
	  
	  
	  if(i >= 2)
	    {//on/past row 2, check 2 above for matching type
	      bool aboveMatch = false;
	      bool leftMatch = false;
	      if(board[i - 1][j].getType() == board[i - 2][j].getType() )
		{ aboveMatch = true; }
	      if(j >= 2)
		{//on/past col 2, check 2 to the left for matching type
		  if(board[i][j - 1].getType() == board[i][j - 2].getType() )
		    { leftMatch = true; }
		}
	      //if above or left have matches AND current type matches either one
	      // randomize until it doesnt
	      while( (aboveMatch or leftMatch) and
		     (board[i][j].getType() == board[i - 1][j].getType() or
		      board[i][j].getType() == board[i][j - 1].getType() ) )
		{//pick a new type while it makes a match with above or left
		  board[i][j].setType((rand() % (upper - lower + 1)) + lower);
		}
	    }//end if i >= 2
	  else
	    {
	      if(j >= 2)
		{//on/past col 2, check 2 to the left for matching type
		  bool leftMatch = false;
		  if(board[i][j - 1].getType() == board[i][j - 2].getType() )
		    { leftMatch = true; }
		  
		  while(leftMatch and board[i][j].getType() == board[i][j - 1].getType() )
		    {//pick a new type while it makes a match with left
		      board[i][j].setType((rand() % (upper - lower + 1)) + lower);
		    } 
		}//end if j >= 2
	    }//end else
	}//end inner for loop
    }//end outer for loop
  //printBoard();
}

void drawBoard()
{
  //black background
  SDL_SetRenderDrawColor(renderer, c.bla.r, c.bla.g, c.bla.b, 255);
  SDL_RenderClear(renderer);
  
  //draw a grid in maroon
  SDL_SetRenderDrawColor(renderer, c.mar.r, c.mar.g, c.mar.b, 255);
  for(int i = 1; i <= (WINDOW_WIDTH / GRID_SIZE); i++){
    //draw vertical lines
    SDL_RenderDrawLine(renderer, i * GRID_SIZE, 0, i * GRID_SIZE, WINDOW_HEIGHT);
  }
  for(int i = 1; i <= (WINDOW_HEIGHT / GRID_SIZE); i++){
    //draw horizontal lines
    SDL_RenderDrawLine(renderer, 0, i * GRID_SIZE, WINDOW_WIDTH, i * GRID_SIZE);
  }
  //draw all tiles
  drawTiles();
  
}

void swapTilePos(int row1, int col1, int row2, int col2)
{
  int temp = board[row1][col1].getType();
  board[row1][col1].setType(board[row2][col2].getType());
  board[row2][col2].setType(temp);
}

void drawHighlightBox(int box_x, int box_y)
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
  left ={box_x,
	 box_y,
	 BOX_SIZE,
	 GRID_SIZE};

  //draw selection box
  SDL_SetRenderDrawColor(renderer, c.gre.r, c.gre.g, c.gre.b, 255);
  SDL_RenderFillRect(renderer, &top);
  SDL_RenderFillRect(renderer, &right);
  SDL_RenderFillRect(renderer, &bot);
  SDL_RenderFillRect(renderer, &left);
  
}


bool validLeftMouseClick(int mouse_x, int mouse_y)
{
  return mouse_x > 0 and mouse_y > 0 and           //filter out loops w/o click
    mouse_x % GRID_SIZE > BOX_SIZE and             //
    mouse_x % GRID_SIZE < GRID_SIZE - BOX_SIZE and
    mouse_y % GRID_SIZE > BOX_SIZE and
    mouse_y % GRID_SIZE < GRID_SIZE - BOX_SIZE and
    mouse_x < (BOARD_SIZE * GRID_SIZE) and
    mouse_y < (BOARD_SIZE * GRID_SIZE);
}

bool swap(Tile a, Tile b)
{
  bool loop = true;
  float aDstX = a.getDstX();
  float aDstY = a.getDstY();
  float bDstX = b.getDstX();
  float bDstY = b.getDstY();
  float aDeltaX, aDeltaY, bDeltaX, bDeltaY;
  float delta = 2.5;
  char* matchString;
  bool matchMade1, matchMade2;
  int left, right, above, below;
  
  printf("\nstart of swap()\n---------------\n");
  fflush(stdout);
  //printf("a dst:\n\tx: %f\n\ty: %f\n", aDstX, aDstY);
  //printf("b dst:\n\tx: %f\n\ty: %f\n", bDstX, bDstY);

  if(aDstY == bDstY)
    {
      if(aDstX > bDstX)
	{//a left of b
	  printf("a right of b\n");
	  fflush(stdout);
	  aDeltaX = -delta;
	  aDeltaY = 0;
	  bDeltaX = delta;
	  bDeltaY = 0;
	}
      else if(aDstX < bDstX)
	{//a right of b
	  printf("a left of b\n");
	  fflush(stdout);
	  aDeltaX = delta;
	  aDeltaY = 0;
	  bDeltaX = -delta;
	  bDeltaY = 0;
	}
      
      while(board[a.getRow()][a.getCol()].getDstX() != bDstX and
	    board[b.getRow()][b.getCol()].getDstX() != aDstX)
	{//visual swap
	  printf("swap x loop\n");
	  fflush(stdout);

	  while ( SDL_PollEvent( &e ) != 0 ) 
	    {
	      switch ( e.type )
		{
		case SDL_QUIT:
		  exit(-1);
		  break;
		}
	    }

	  board[a.getRow()][a.getCol()]
	    .setDstX( board[a.getRow()][a.getCol()]
		      .getDstX() + aDeltaX );
	  
	  board[b.getRow()][b.getCol()]
	    .setDstX( board[b.getRow()][b.getCol()]
		      .getDstX() + bDeltaX );
	  
	  drawBoard();
	  drawTiles();
	  SDL_RenderPresent( renderer );
	  SDL_Delay(30);
	}
    }
  else if(aDstX == bDstX)
    {    
      if(aDstY > bDstY)
	{//a below b
	  printf("a below b\n");
	  fflush(stdout);
	  aDeltaX = 0;
	  aDeltaY = -delta;
	  bDeltaX = 0;
	  bDeltaY = delta;
	}
      else if(aDstY < bDstY)
	{//a above b
	  printf("a above b\n");
	  fflush(stdout);
	  aDeltaX = 0;
	  aDeltaY = delta;
	  bDeltaX = 0;
	  bDeltaY = -delta;
	}

      while(board[a.getRow()][a.getCol()].getDstY() != bDstY and
	    board[b.getRow()][b.getCol()].getDstY() != aDstY)
	{//visual swap
	  printf("swap y loop\n");
	  fflush(stdout);

	  while ( SDL_PollEvent( &e ) != 0 ) 
	    {
	      switch ( e.type )
		{
		case SDL_QUIT:
		  exit(-1);
		  break;
		}
	    }

	  board[a.getRow()][a.getCol()]
	    .setDstY( board[a.getRow()][a.getCol()]
		      .getDstY() + aDeltaY );
	  
	  board[b.getRow()][b.getCol()]
	    .setDstY( board[b.getRow()][b.getCol()]
		      .getDstY() + bDeltaY );
	  
	  drawBoard();
	  drawTiles();
	  SDL_RenderPresent( renderer );
	  SDL_Delay(30);
	}
    }


  //DOUBLE CHECK THAT THE MODIFIED MATCHCHECK() CALLS ARE WORKING
  
  //any matches made by putting b where a is?
  matchString = matchCheck(a.getRow(), a.getCol(), b.getType() );
  sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);
  if(left + right >= 2 or above + below >= 2)
    { matchMade1 = true; }

  //any matches made by putting a where b is?
  matchString = matchCheck(b.getRow(), b.getCol(), a.getType() );
  sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);
  if(left + right >= 2 or above + below >= 2)
    { matchMade2 = true; }  
	    
  if(matchMade1 or matchMade2)
    {//match was made!
      return true;
    }
  else
    {//no match made... swap back, return false;
      //swap back
      return false;
    }
}
