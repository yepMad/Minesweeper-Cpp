#ifndef __MINE_H__
#define __MINE_H__

#include "cocos2d.h"

/// MineTypes
enum mine_type
{
  none = 0,
  counter = 1,
  bomb = 2
};

class Mine final : public cocos2d::Sprite  {
public:
  void initialize(int x, int y);
  void createLabel(cocos2d::Color3B color, std::string text);
  void dig();
  
  void setFlag(bool isFlagged);
  void setNearBombCount(int number);
  
public:
  int x, y;
  bool wasDug = false;
  bool isFlagged = false;
  mine_type mineType = none;
  
  int getNearBombCount() const {
    return nearBombCount;
  }

private:
  bool init() override;
  
private:
  int nearBombCount = 0;
  Sprite* flag = nullptr;
};

#endif