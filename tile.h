#ifndef TILE_H
#define TILE_H

class Tile{
    private:
        int row;
        int col;
        int type;
	int hiddenType;
	float spriteDstX;
	float spriteDstY;

    public:
        //constructor
        Tile();
        //destructor
        ~Tile();
        //setters
        void setRow(int r);
        void setCol(int c);
        void setType(int t);
	void setHiddenType(int t);
	void setDstX(float x);
	void setDstY(float y);


	//getters
        int getRow();
        int getCol();
        int getType();
	int getHiddenType();
	float getDstX();
	float getDstY();
        
        bool isAdjacent(Tile t);

        //Tile operator =  (Tile t);
        //bool operator == (Tile t);

};
#endif
