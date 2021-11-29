#include "GameScene.h"

#include <FilesPaths.h>
#include <iomanip>

#include "HeaderItem.h"

USING_NS_CC;

Scene* GameScene::createScene() {
  Scene* scene = Scene::create();

  const auto layerColor = LayerColor::create(Color4B(232, 207, 224, 255));
  const auto layer = create();

  scene->addChild(layerColor, 1);
  scene->addChild(layer, 2);

  return scene;
}

bool GameScene::init() {
  if (!Scene::init()) {
    return false;
  }

  boardMap = new BoardMap(14, 18, 40);
  boardMap->drawMap(this);

  this->enableHeaderItems();
  this->enableListeners();

  this->schedule(
    CC_CALLBACK_0(GameScene::clockTick, this), 1, 59 * 60, 0,
    CLOCK_TICK_SCHEDULE_KEY);

  return true;
}


void GameScene::enableListeners() {
  const auto listener = EventListenerTouchOneByOne::create();

  listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
  listener->setSwallowTouches(true);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::enableHeaderItems() {
  // Timer
  timerHeaderItem->init();
  timerHeaderItem->addIcon(icon_clock);
  timerHeaderItem->addLabel("00:00");
  timerHeaderItem->setPosition(Vec2(
    timerHeaderItem->getContentSize().width / 2 + 20,
    VISIBLE_SIZE.height - timerHeaderItem->getContentSize().height));

  // Flags counter
  flagsCounterHeaderItem->init();
  flagsCounterHeaderItem->addIcon(icon_flag);
  flagsCounterHeaderItem->addLabel(to_string(boardMap->qtyBombs));
  flagsCounterHeaderItem->setPosition(Vec2(
    VISIBLE_SIZE.width - flagsCounterHeaderItem->getContentSize().width / 2 -
    20,
    VISIBLE_SIZE.height - flagsCounterHeaderItem->getContentSize().height));

  // Add to game parent
  this->addChild(timerHeaderItem, 2);
  this->addChild(flagsCounterHeaderItem, 2);
}

void GameScene::clockTick() {
  seconds++;

  const auto minutes = seconds / 60;
  const auto second = seconds % 60;

  const auto strMinutes = minutes > 9 ? "" : "0" + to_string(minutes);
  const auto strSeconds = second > 9 ? "" : "0" + to_string(second);

  timerHeaderItem->setLabelText(strMinutes + ":" + strSeconds);
}


bool GameScene::onTouchesBegan(const Touch* touch, Event* event) {
  if (boardMap->hasEndGame()) {
    return true;
  }

  boardMap->onClick(touch->getLocation());
  if (boardMap->getGameOver()) {
    CCLOG("GameOver!");
    this->unschedule(CLOCK_TICK_SCHEDULE_KEY);
  }

  if (boardMap->getWin()) {
    CCLOG("Win!");
    this->unschedule(CLOCK_TICK_SCHEDULE_KEY);
  }

  return true;
}
