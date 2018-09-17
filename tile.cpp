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

bool Tile::isAdjacent(Tile t)
{//retruns true is this Tile and arg Tile are vertically or horizontally adjacent

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