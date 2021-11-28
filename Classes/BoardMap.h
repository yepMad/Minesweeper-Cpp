#ifndef __BOARDMAP_SCENE_H__
#define __BOARDMAP_SCENE_H__

#include "Mine.h"
#include "cocos2d.h"

USING_NS_CC;

class BoardMap {
public:
  BoardMap(int width, int height, int qtyMines);
  void drawMap(Node *parent);
  void clickOnMap(Vec2 location);

public:
  float Width, Height, QtyMines;
  
  Mine **Mines;
};

#endif // __BOARDMAP_SCENE_H__