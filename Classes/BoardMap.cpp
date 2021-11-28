#include "BoardMap.h"

BoardMap::BoardMap(const int width, const int height, const int qtyMines) {
  Width = width;
  Height = height;
  QtyMines = qtyMines;
  
  Mines = new Mine*[Height];
  
  for (auto i = 0; i < Height; i++) {
    Mines[i] = new Mine[Width];
  }
}

void BoardMap::drawMap(Node* parent) {
  for (auto i = 0; i < Height; ++i) {
    for (auto j = 0; j < Width; ++j) {
      Mines[i][j].init();
      
      const auto posX = (j + 1) * Mines[i][j].getContentSize().width / 2;
      const auto posY = (i + 1) * Mines[i][j].getContentSize().height / 2;
      
      Mines[i][j].setPosition(posX, posY);
      Mines[i][j].setScale(0.5);
      
      parent->addChild(&Mines[i][j]);
    }
  }
}

void BoardMap::clickOnMap(const Vec2 location) {
  for (auto i = 0; i < Height; ++i) {
    for (auto j = 0; j < Width; ++j) {
      const auto mine = &Mines[i][j];
      const auto isMinePoint = mine->getBoundingBox().containsPoint(location);
      
      if (isMinePoint) {
        CCLOG("Yep, is a mine site.");
        break;
      }
    }
  }
}
