#include "tile.h"

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