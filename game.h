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
    void startBoard();
    void drawBoard();
    void handleLeftMouseClick(int, int);
    void drawHighlightBox();

    //new
    void updateBoard();
    void checkForMatches();
};

#endif