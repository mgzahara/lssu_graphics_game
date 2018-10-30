#ifndef GAME
#define GAME
#include "tile.h"

class Game
{
private:
  bool validLeftMouseClick(int, int);
  bool shouldSwapTiles(int, int);
  char *matchCheck(Tile);
  void match(Tile);

public:
  Game();
  ~Game();
  //init matchBoard
  //start board without matches
  void startBoard();
  //blank out screen, draw grid lines
  void drawBoard();
  //two possibilities
  //click is adjacent to activeTile - tell both to swap
  //click is NOT adjacent to activeTile - update activeTile
  void handleLeftMouseClick(int, int);
  //draw box around activeTile, if there is one
  void drawHighlightBox();

  //new
  //call update method for each Tile in board[][]
  void updateBoard();
  //consults matchBoard[][] to determine if match() should be called on the given Tile
  void checkForMatches();
};

#endif