#ifndef TILE_H
#define TILE_H

class Tile{
    private:
        int row;
        int col;
        int type;

    public:
        //constructor
        Tile();
        //destructor
        ~Tile();
        //setters
        void setRow(int r);
        void setCol(int c);
        void setType(int t);
        //getters
        int getRow();
        int getCol();
        int getType();
        
        bool isAdjacent(Tile t);

        //Tile operator =  (Tile t);
        //bool operator == (Tile t);

};
#endif