#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "BoardMap.h"

#define VISIBLE_SIZE Director::getInstance()->getVisibleSize()

USING_NS_CC;

class GameScene : public Scene
{
public:
  static Scene* createScene();
  
  virtual bool init();
  void enableListeners();
  
  CREATE_FUNC(GameScene);
  
public:
  BoardMap* boardMap;
  bool onTouchesBegan(cocos2d::Touch* touch, cocos2d::Event* event);
};

#endif // __GAMESCENE_H__
