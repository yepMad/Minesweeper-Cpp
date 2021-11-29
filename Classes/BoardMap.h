#ifndef __BOARDMAP_SCENE_H__
#define __BOARDMAP_SCENE_H__

#include "Mine.h"
#include "cocos2d.h"

using namespace std;
USING_NS_CC;

struct MinePosition {
  int x;
  int y;
};

enum click_type {
  left = 0,
  right = 1,
};

class BoardMap {
public:
  BoardMap(int width, int height, int qtyBombs);

  void drawMap(Node* parent) const;
  void onClick(Vec2 location, click_type clickType);

private:
  void initializeMines();
  void onMineInteracts(Mine* mine);
  void setMineFlag(Mine* mine, bool isFlagged);

  void openAdjacentMinesAt(const Mine& mine) const;

  void createBombMines() const;
  void createCounterNearMines() const;

  /// <summary>
  /// <para>Here you can make sure that the player's first click will always be an empty mine</para>
  /// </summary>
  bool canPlaceBomb(int x, int y) const;
  bool allRemainingClosedMinesAreBombs() const;

public:
  int width, height, qtyBombs;
  Mine** mines;

  MinePosition getRandomMinePosition() const;
  Mine* getMine(Vec2 location) const;
  
  bool hasEndGame() const { return hasGameOver || hasWin; };
  int getFlagsQty() const { return qtyMinesFlagged; }
  bool getGameOver() const { return hasGameOver; }
  bool getWin() const { return hasWin; }

private:
  Mine* firstMineClicked;
  int qtyMinesFlagged = 0;

  bool hasInitialized = false;
  bool hasGameOver = false;
  bool hasWin = false;
};

#endif // __BOARDMAP_SCENE_H__
