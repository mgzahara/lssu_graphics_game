#ifndef EFFECT_H
#define EFFECT_H


//when a bomb effect starts in Tile.cpp:
//be sure to tell all surrounding Effects that they are busy

class Effect
{
 public:
  Effect();
  int currFrame;
  int maxFrame;
  int startFrame;
  int type;
  //0 - none
  //1 - bomb
  //2 - horizontal lightning
  //3 - vertical lightning
  //4 - bidirectional lightning

  //dimensions of single frame
  int texWidth;
  int texHeight;
  //x, y to start source texture
  int srcX;
  int srcY;
  //screen coords for texure blitting
  int dstX;
  int dstY;
  //corresponding to indecies of Tiles
  int row;
  int col;

  void update();
  void startEffect(int); //pass in type of animation to perform
  //Effect will know the rest based on the type

  void setRow(int);
  void setCol(int);
  void setBusy(); //for when a bomb animation is adjacent to me
  void setIdle(); //for when a bomb animation is done
};

#endif
