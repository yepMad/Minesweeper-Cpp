#include "GameScene.h"

#include <FilesPaths.h>
#include <iomanip>

#include "HeaderItem.h"
#include "ui/UIButton.h"

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

  boardMap.initData(18, 14, 40);
  boardMap.drawMap(this);

  this->enableHeaderItems();
  this->enableListeners();

  this->schedule(
    CC_CALLBACK_0(GameScene::clockTick, this), 1, 59 * 60, 0,
    CLOCK_TICK_SCHEDULE_KEY);

  return true;
}


void GameScene::enableListeners() {
  const auto touchListener = EventListenerTouchOneByOne::create();
  const auto mouseListener = EventListenerMouse::create();

  mouseListener->onMouseDown = CC_CALLBACK_1(GameScene::onMouseDown, this);
  touchListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
  touchListener->setSwallowTouches(true);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
  _eventDispatcher->addEventListenerWithFixedPriority(mouseListener, 1);
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
  flagsCounterHeaderItem->addLabel(to_string(boardMap.qtyBombs));
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

  const auto strMinutes = (minutes > 9 ? "" : "0") + to_string(minutes);
  const auto strSeconds = (second > 9 ? "" : "0") + to_string(second);

  timerHeaderItem->setLabelText(strMinutes + ":" + strSeconds);
}


void GameScene::win() {
  const auto restartButton = createRestartButton();
  const auto labelText = Label::createWithTTF("VOCÊ GANHOU!", font_arial, 20);
  
  labelText->setColor(Color3B::GREEN);
  labelText->setPosition(Vec2(VISIBLE_SIZE.width / 2, timerHeaderItem->getPosition().y));

  this->addChild(labelText, 2);
  this->addChild(restartButton, 3);
  
  this->unschedule(CLOCK_TICK_SCHEDULE_KEY);
}

void GameScene::gameOver() {
  const auto restartButton = createRestartButton();
  const auto labelText = Label::createWithTTF("VOCÊ PERDEU!", font_arial, 20);
  
  labelText->setColor(Color3B::RED);
  labelText->setPosition(Vec2(VISIBLE_SIZE.width / 2, timerHeaderItem->getPosition().y));

  this->addChild(labelText, 2);
  this->addChild(restartButton, 3);
  
  this->unschedule(CLOCK_TICK_SCHEDULE_KEY);
}

void GameScene::restart() {
  Director::getInstance()->replaceScene(createScene());
}


bool GameScene::onTouchesBegan(const Touch* touch, Event* event) {
  if (boardMap.hasEndGame()) {
    return true;
  }

  boardMap.onClick(touch->getLocation(), click_type::left);
  updateFlagsCounter();
  
  if (boardMap.getGameOver()) {
    gameOver();
    return true;
  }

  if (boardMap.getWin()) {
    win();
    return true;
  }

  return true;
}

void GameScene::onMouseDown(Event* event) {
  if (boardMap.hasEndGame()) {
    return;
  }

  const auto eventMouse = static_cast<EventMouse*>(event);
  if (eventMouse->getMouseButton() != EventMouse::MouseButton::BUTTON_RIGHT) {
    return;
  }

  boardMap.onClick(eventMouse->getLocationInView(), click_type::right);
  updateFlagsCounter();
}


void GameScene::updateFlagsCounter() const {
  flagsCounterHeaderItem->setLabelText(
    to_string(boardMap.qtyBombs - boardMap.getFlagsQty()));
}

Node* GameScene::createRestartButton() {
  const auto restartButton = ui::Button::create(labels_white);
  restartButton->setPosition(Vec2(VISIBLE_SIZE.width / 2, timerHeaderItem->getPosition().y - 60));
  restartButton->addTouchEventListener(CC_CALLBACK_0(GameScene::restart, this));
  
  const auto restartText = Label::createWithTTF("Reiniciar", font_arial, 16);
  restartText->setColor(Color3B::BLACK);
  restartText->setPosition(Vec2(restartButton->getContentSize().width / 2, restartButton->getContentSize().height/2));
  
  restartButton->addChild(restartText, 2);
  return restartButton;
}

