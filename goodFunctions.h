#include "iostream"
#include "stdlib.h"
#include "time.h"
#include "tile.h"

using namespace std;




bool initSDL();//keep
void quitSDL();//keep
void setColors();//keep
SDL_Texture* loadImage(const char*);//keep
SDL_Texture* loadKeyedImage(const char*);//keep
bool validLeftMouseClick(int, int);//keep
bool shouldSwapTiles(int, int);//keep
void drawHighlightBox(int, int);//keep
void startBoard();//keep
void printBoard();//maybe
void printTileColor(int, int);//maybe
//1, 0, or -1
int sign(int);
int sign(float);

//might have problems

void drawTiles();
char* matchCheck(int, int, int);//keep
int swapAndCheck(Tile, Tile);
void swap(Tile, Tile, bool);
void drawBoard();
void swapTilePos(int, int, int, int);
//Tile which matched, Tiles to remove left, right, up, down
void match(Tile, int, int, int);
//new way
void handleLeftMouseClick(int, int);


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
  activeTile.setType(-1);
  //swappingTile is also impossible - remove?
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

    //MAKE THESE STATIC TO THE TILE CLASS
  //set up gem1 sprite
  gems[0] = loadImage("img/gem1.png");

  //set up gem2 sprite
  gems[1] = loadImage("img/gem2.png");

  //set up gem3 sprite
  gems[2] = loadImage("img/gem3.png");

    //set up gem4 sprite
  gems[3] = loadImage("img/gem4.png");

  //set up gem5 sprite
  gems[4] = loadImage("img/gem5.png");

  //set up gem6 sprite
  gems[5] = loadImage("img/gem6.png");

  //set up empty sprite
  empty_gem = loadImage("img/empty_gem.png");

  
  //black out the screen
  SDL_SetRenderDrawColor( renderer, c.bla.r, c.bla.g, c.bla.b, 255 );
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


bool validLeftMouseClick(int mouse_x, int mouse_y)
{
  return mouse_x > 0 and mouse_y > 0 and
    mouse_x % GRID_SIZE > BOX_SIZE and
    mouse_x % GRID_SIZE < GRID_SIZE - BOX_SIZE and
    mouse_y % GRID_SIZE > BOX_SIZE and
    mouse_y % GRID_SIZE < GRID_SIZE - BOX_SIZE and
    mouse_x < (BOARD_SIZE * GRID_SIZE) and
    mouse_y < (BOARD_SIZE * GRID_SIZE);
}

bool shouldSwapTiles(int clickRow, int clickCol)
{
  return ( (activeTile.getRow() >= 0) and
	   (activeTile.getCol() >= 0) and
	   (board[ activeTile.getRow() ][ activeTile.getCol() ]
	    .isAdjacent(board[ clickRow ][ clickCol ]) ) ); 
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


void startBoard()
{
  //board is [row][col]
  //col is inc inside inner loop, so the filling order is as follows
    //(0, 0) then the entire first row gets filled left to right
    //then the row is inc and it repeats
  //this means that there will never be Tiles to the right or below
  //as tiles are filled, check to the left and above for matches
  //if match was created, chose a diffrent type so as to not create a match
  
  int lower = 0;
  int upper = 5;
  
  for(int i = 0; i < BOARD_SIZE; i++)
    {//row
      for(int j = 0; j < BOARD_SIZE; j++)
	{//col

	  //set matchBoard[][] feaults while I'm here
	  matchBoard[i][j] = false;
	  
	  //set up Tiles
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
		  
		      while(leftMatch and 
                    board[i][j].getType() == board[i][j - 1].getType() )
    		    {//pick a new type while it makes a match with left
    		      board[i][j].setType((rand() % (upper - lower + 1)) + lower);
    		    } 
    		}//end if j >= 2
	    }//end else
	}//end inner for loop
    }//end outer for loop
  //printBoard();
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
  printf("active tile\n");
  printf("row: %d\ncol: %d\ntpye: %d\n\n", 
    activeTile.getRow(), activeTile.getCol(), activeTile.getType() );

}

void printTileColor(int row, int col)
{
    switch( board[row][col].getType())
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
          printf("clear");
          break;
        case 5:
          printf("blue");
          break;
        default:
          printf("\ntype is not 0-5!!\n");
        }
}

int sign(int x)
{
  if(x > 0)
    { return 1; }
  if(x < 0)
    { return -1; }
  return 0;
}

int sign(float x)
{
  if(x > 0)
    { return 1; }
  if(x < 0)
    { return -1; }
  return 0;
}
