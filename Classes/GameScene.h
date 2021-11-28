#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "BoardMap.h"

#define VISIBLE_SIZE Director::getInstance()->getVisibleSize()

USING_NS_CC;

class GameScene final : public Scene {
public:
  static Scene* createScene();

  bool init() override;
  void enableListeners();

  CREATE_FUNC(GameScene);

public:
  BoardMap* boardMap;
  bool onTouchesBegan(const Touch* touch, Event* event) const;
};

#endif // __GAMESCENE_H__
