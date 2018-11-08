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
  int scoreMultiplier; // inc w/each match
  const int scoreConst = 25; // how much the score increases by for each match
  const int baseScore3 = 100; // each match is worth <= baseScore
  const int baseScore4 = 150;
  const int baseScore5 = 200;
  
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
