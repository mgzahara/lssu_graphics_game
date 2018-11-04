#ifndef GAME
#define GAME
#include "tile.h"
#include "SDL.h"

class Game
{
private:
  bool validLeftMouseClick(int, int);
  bool shouldSwapTiles(int, int);
  char *matchCheck(Tile);
  void match(Tile);
  void displayInfo(const char*, float, int, int, SDL_Color);
  void displayInfo(const char*, int, int, int, SDL_Color);
  void displayInfo(const char*, int, int, SDL_Color);
  bool shuffleClick(int, int);
  
public:
  Game();
  ~Game();
  void startBoard();
  void drawBoard();
  void handleLeftMouseClick(int, int);
  void drawHighlightBox();
  void updateBoard();
  void checkForMatches();
  void drawPanel();
};

#endif
