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

class BoardMap {
public:
  BoardMap(int width, int height, int qtyBombs);

  void drawMap(Node* parent) const;
  void onClick(Vec2 location);

private:
  void initializeMines();

  void openAdjacentMinesAt(const Mine& mine) const;

  void createBombMines() const;
  void createCounterNearMines() const;

  /// <summary>
  /// <para>Here you can make sure that the player's first click will always be an empty mine</para>
  /// </summary>
  bool canPlaceBomb(int x, int y) const;

public:
  int width, height, qtyBombs;
  Mine** mines;

  Mine* getMine(Vec2 location) const;
  MinePosition getRandomMinePosition() const;

private:
  Mine* firstMineClicked;
  bool hasInitialized = false;
};

#endif // __BOARDMAP_SCENE_H__
