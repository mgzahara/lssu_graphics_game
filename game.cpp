#include "game.h"
#include "SDL.h"
#include "SDL_image.h"
#include "iostream"
#include "globals.h"
#include "tile.h"

Game::Game()
{
    //first activeTile will have unreal row and col
    activeTile.setRow(-10);
    activeTile.setCol(-10);
    activeTile.setType(-1);
}
Game::~Game()
{
    return;
}

bool Game::validLeftMouseClick(int mouse_x, int mouse_y)
{
    return mouse_x > 0 and mouse_y > 0 and
           mouse_x % GRID_SIZE > BOX_SIZE and
           mouse_x % GRID_SIZE < GRID_SIZE - BOX_SIZE and
           mouse_y % GRID_SIZE > BOX_SIZE and
           mouse_y % GRID_SIZE < GRID_SIZE - BOX_SIZE and
           mouse_x < (BOARD_SIZE * GRID_SIZE) and
           mouse_y < (BOARD_SIZE * GRID_SIZE);
}

bool Game::shouldSwapTiles(int clickRow, int clickCol)
{
    return ((activeTile.getRow() >= 0) and
            (activeTile.getCol() >= 0) and
            (board[activeTile.getRow()][activeTile.getCol()]
                 .isAdjacent(board[clickRow][clickCol])));
}

void Game::drawHighlightBox()
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
    left = {box_x,
            box_y,
            BOX_SIZE,
            GRID_SIZE};

    //draw selection box - green
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &top);
    SDL_RenderFillRect(renderer, &right);
    SDL_RenderFillRect(renderer, &bot);
    SDL_RenderFillRect(renderer, &left);
}

void Game::startBoard()
{
    //board is [row][col]
    //col is inc inside inner loop, so the filling order is as follows
    //(0, 0) then the entire first row gets filled left to right
    //then the row is inc and it repeats
    //there will never be Tiles to the right or below
    //as tiles are filled, check to the left and above for matches
    //if match was created, chose a diffrent type so as to not create a match

    int lower = 0;
    int upper = 5;

    for (int i = 0; i < BOARD_SIZE; i++)
    { //row
        for (int j = 0; j < BOARD_SIZE; j++)
        { //col

            //set matchBoard[][] defaults while we're here
            matchBoard[i][j] = false;

            //set up Tiles
            board[i][j].setRow(i);
            board[i][j].setCol(j);
            board[i][j].setType((rand() % (upper - lower + 1)) + lower);
            board[i][j].setDstX((float)(j * GRID_SIZE + BOX_SIZE + 6));
            board[i][j].setDstY((float)(i * GRID_SIZE + BOX_SIZE + 6));

            if (i >= 2)
            { //on/past row 2, check 2 above for matching type
                bool aboveMatch = false;
                bool leftMatch = false;
                if (board[i - 1][j].getType() == board[i - 2][j].getType())
                {
                    aboveMatch = true;
                }
                if (j >= 2)
                { //on/past col 2, check 2 to the left for matching type
                    if (board[i][j - 1].getType() == board[i][j - 2].getType())
                    {
                        leftMatch = true;
                    }
                }
                //if above or left have matches AND current type matches either one
                // randomize until it doesnt
                while ((aboveMatch or leftMatch) and
                       (board[i][j].getType() == board[i - 1][j].getType() or
                        board[i][j].getType() == board[i][j - 1].getType()))
                { //pick a new type while it makes a match with above or left
                    board[i][j].setType((rand() % (upper - lower + 1)) + lower);
                }
            } //end if i >= 2
            else
            {
                if (j >= 2)
                { //on/past col 2, check 2 to the left for matching type
                    bool leftMatch = false;
                    if (board[i][j - 1].getType() == board[i][j - 2].getType())
                    {
                        leftMatch = true;
                    }

                    while (leftMatch and
                           board[i][j].getType() == board[i][j - 1].getType())
                    { //pick a new type while it makes a match with left
                        board[i][j].setType((rand() % (upper - lower + 1)) + lower);
                    }
                } //end if j >= 2
            }     //end else
        }         //end inner for loop
    }             //end outer for loop
                  //printBoard();
}

//new

void Game::updateBoard()
{
    //update all Tiles - go backwards to facilitate falling Tile sprite hand off
    for (int i = 7; i >= 0; i--)
    {
        for (int j = 7; j >= 0; j--)
        {
            board[i][j].update();
        }
    }
}

void Game::checkForMatches()
{
    //handle any matches
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (matchBoard[i][j])
            {
                match(board[i][j]); //not actually implemented yet
            }
        }
    }
}

//troublesome

char *Game::matchCheck(Tile currTile)
{ //checks the Tile passed for a match
    //results is a string containing ints of the number of matches in a direction
    // "<left> <right> <above> <below>"

    int leftMatches, rightMatches, aboveMatches, belowMatches, row, col, type;
    char *matchString;
    matchString = new char[10];
    char intString[15];

    strcpy(matchString, ""); //reset
    leftMatches = rightMatches = aboveMatches = belowMatches = 0;
    row = currTile.getRow();
    col = currTile.getCol();
    type = currTile.getType();

    //currTile is board[row][col]

    while (col > 0 and board[row][col - 1].getType() == type)
    { //while left matches
        leftMatches++;
        col--;
    }
    col = currTile.getCol();

    while (col < BOARD_SIZE - 1 and board[row][col + 1].getType() == type)
    { //while right matches
        rightMatches++;
        col++;
    }
    col = currTile.getCol();

    while (row > 0 and board[row - 1][col].getType() == type)
    { //while above matches
        aboveMatches++;
        row--;
    }
    row = currTile.getRow();

    while (row < BOARD_SIZE - 1 and board[row + 1][col].getType() == type)
    { //while below matches
        belowMatches++;
        row++;
    }
    row = currTile.getRow();

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

    //  cout << "match string for Tile at\n\trow: " << row << "\n\tcol: " << col;
    //  cout << endl << matchString << endl;

    return matchString;
}

void Game::drawBoard()
{
    //black background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //draw a grid in maroon
    SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
    for (int i = 1; i <= (WINDOW_WIDTH / GRID_SIZE); i++)
    {
        //draw vertical lines
        SDL_RenderDrawLine(renderer, i * GRID_SIZE, 0, i * GRID_SIZE, WINDOW_HEIGHT);
    }
    for (int i = 1; i <= (WINDOW_HEIGHT / GRID_SIZE); i++)
    {
        //draw horizontal lines
        SDL_RenderDrawLine(renderer, 0, i * GRID_SIZE, WINDOW_WIDTH, i * GRID_SIZE);
    }
}

void Game::match(Tile curr)
{
    int left, right, above, below;
    char *matchString;

    matchString = matchCheck(board[curr.getRow()][curr.getCol()]);
    sscanf(matchString, "%d %d %d %d", &left, &right, &above, &below);

    //take care of the easy one first
    board[curr.getRow()][curr.getCol()].setType(-1);

    if ((left + right) >= 2)
    { //is left-right the match?

        //tell every Tile above match that it should fall
        for (int i = curr.getRow() - 1; i <= 0; i--)
        { //start right above this match, go up
            for (int j = curr.getCol() - left; i <= curr.getCol() + right; j++)
            {
                //only tell cols of the match
                board[i][j].setFalling(true, 1); //fall 1
            }
        }

        //set all matching Tiles to invisible
        while (left > 0)
        { //remove all left
            board[curr.getRow()][curr.getCol() - left].setType(-1);
            left--;
        }
        while (right > 0)
        { //remove all right
            board[curr.getRow()][curr.getCol() + right].setType(-1);
            right--;
        }
    }
    if ((above + below) >= 2)
    { //is above-below the match?

        //tell every Tile above match that it should fall
        for (int i = curr.getRow() - above - 1; i <= 0; i--)
        {
            //start at Tile directly above the vertical match
            //tell Tile it should fall above + below + 1 times
            board[i][curr.getCol()].setFalling(true, above + below + 1);
        }

        //set all matching Tiles to invisible
        while (above > 0)
        { //remove all above
            board[curr.getRow() - above][curr.getCol()].setType(-1);
            above--;
        }
        while (below > 0)
        { //remove all below
            board[curr.getRow() + below][curr.getCol()].setType(-1);
            below--;
        }
    }
}

void Game::handleLeftMouseClick(int mouse_x, int mouse_y)
{
    if (validLeftMouseClick(mouse_x, mouse_y))
    {
        //click was made within constraints
        int clickRow = mouse_y / GRID_SIZE;
        int clickCol = mouse_x / GRID_SIZE;

        if (shouldSwapTiles(clickRow, clickCol))
        {
            //swap - tell tiles they are to move
            int activeTileDir = 0, newClickDir = 0;

            //get highlight box out of the way
            box_x = box_y = -200;

            /* directions
	     0 - ignore it
	     1 - go left
	     2 - go right
	     3 - go up
	     4 - go down
	   */

            if (activeTile.getRow() == clickRow)
            {
                //swap in same row, left/right
                if (activeTile.getCol() > clickCol)
                {
                    //activeTile col is larger - right of new click
                    activeTileDir = 1; //go left
                    newClickDir = 2;   //go right
                }
                else
                {
                    //activeTile col is smaller - left of new click
                    activeTileDir = 2; //go right
                    newClickDir = 1;   //go left
                }
            } //end if activeTile row == clickRow
            else
            {
                //swap in same col, up/down
                if (activeTile.getRow() > clickRow)
                {
                    //activeTile row is larger - below new click
                    activeTileDir = 3; //go up
                    newClickDir = 4;   //go down
                }
                else
                {
                    //activeTile row is smaller - above new click
                    activeTileDir = 4; //go down
                    newClickDir = 3;   //go up
                }
            } //end if activeTile row == clickRow else

            //tell each Tile it is to swap, the direction, its new type after swapping
            board[activeTile.getRow()][activeTile.getCol()]
                .setSwapping(true,
                             activeTileDir,
                             board[clickRow][clickCol].getType());

            board[clickRow][clickCol]
                .setSwapping(true,
                             newClickDir,
                             board[activeTile.getRow()][activeTile.getCol()].getType());

        } //end if should swap tiles
        else
        {
            //click was not adjacent, reset activeTile to current click

            //tell all Tiles they are not active
            for (int i = 0; i < 8; i++)
            {
                for (int j = 0; j < 8; j++)
                {
                    board[i][j].setActive(false);
                }
            }

            //tell new active Tile it is active
            board[clickRow][clickCol].setActive(true);

            //update rederence Tile
            activeTile.setRow(clickRow);
            activeTile.setCol(clickCol);
            activeTile.setType(board[clickRow][clickCol].getType());

            //set pixel coords for activeTile highlighting
            box_x = clickCol * GRID_SIZE;
            box_y = clickRow * GRID_SIZE;

        } //end if should swap tiles else
    }     //end if valid mouse click
}
