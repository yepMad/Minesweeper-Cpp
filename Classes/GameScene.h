#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "BoardMap.h"
#include "ui/UIButton.h"

#define VISIBLE_SIZE Director::getInstance()->getVisibleSize()
#define CLOCK_TICK_SCHEDULE_KEY "clock_tick"

USING_NS_CC;

struct GameData {
  int width;
  int height;
  int qtyMines;
};

class GameScene final : public Scene {
public:
  static Scene* createScene();

  bool init() override;
  void gameOver();
  void win();
  
  void enableListeners();
  void enableHeaderItems();
  
  void clockTick();
  void updateFlagsCounter() const;
  
  bool onTouchesBegan(const Touch* touch, Event* event);
  void onMouseDown(Event* event);

  CREATE_FUNC(GameScene);

public:
  BoardMap boardMap;
  int seconds;

private:
  Label* timerHeaderItem = nullptr;
  Label* flagsCounterHeaderItem = nullptr;
  
  Node* createRestartButton();
  Label* createHeaderItem(string initialText, string iconPath, Vec2 position);
  ui::Button* createDifficultyButton(const string label) const;
  GameData getGameData() const;
  static void setGameData(int width, int height, int qtyMines);

  void restart() const;
  void setEasyDifficulty();
  void setMediumDifficulty();
};

#endif // __GAMESCENE_H__
