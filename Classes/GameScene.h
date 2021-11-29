#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "BoardMap.h"
#include "HeaderItem.h"

#define VISIBLE_SIZE Director::getInstance()->getVisibleSize()
#define CLOCK_TICK_SCHEDULE_KEY "clock_tick"

USING_NS_CC;

class GameScene final : public Scene {
public:
  static Scene* createScene();

  bool init() override;
  void enableListeners();
  void enableHeaderItems();
  void clockTick();
  bool onTouchesBegan(const Touch* touch, Event* event);

  CREATE_FUNC(GameScene);

public:
  BoardMap* boardMap;
  int seconds;

private:
  HeaderItem* timerHeaderItem = new HeaderItem();
  HeaderItem* flagsCounterHeaderItem = new HeaderItem();
};

#endif // __GAMESCENE_H__
