#include "GameScene.h"

#include <FilesPaths.h>
#include <iomanip>

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
  timerHeaderItem = createHeaderItem(
    "00:00", icon_clock,
    Vec2(100, VISIBLE_SIZE.height - 60));

  // Flags counter
  flagsCounterHeaderItem = createHeaderItem(
    to_string(boardMap.qtyBombs),
    icon_flag,
    Vec2(VISIBLE_SIZE.width - 100, VISIBLE_SIZE.height - 60));
}

void GameScene::clockTick() {
  seconds++;

  const auto minutes = seconds / 60;
  const auto second = seconds % 60;

  const auto strMinutes = (minutes > 9 ? "" : "0") + to_string(minutes);
  const auto strSeconds = (second > 9 ? "" : "0") + to_string(second);

  timerHeaderItem->setString(strMinutes + ":" + strSeconds);
}


void GameScene::win() {
  const auto restartButton = createRestartButton();
  const auto labelText = Label::createWithTTF("VOCÊ GANHOU!", font_arial, 20);

  labelText->setColor(Color3B::GREEN);
  labelText->setPosition(Vec2(VISIBLE_SIZE.width / 2,
                              timerHeaderItem->getPosition().y));

  this->addChild(labelText, 2);
  this->addChild(restartButton, 3);

  this->unschedule(CLOCK_TICK_SCHEDULE_KEY);
}

void GameScene::gameOver() {
  const auto restartButton = createRestartButton();
  const auto labelText = Label::createWithTTF("VOCÊ PERDEU!", font_arial, 20);

  this->addChild(labelText, 2);
  this->addChild(restartButton, 3);

  labelText->setColor(Color3B::RED);
  labelText->setPosition(Vec2(VISIBLE_SIZE.width / 2,
                              VISIBLE_SIZE.height - 50));

  this->unschedule(CLOCK_TICK_SCHEDULE_KEY);
}

void GameScene::restart() {
  _eventDispatcher->removeAllEventListeners();
  Director::getInstance()->replaceScene(TransitionFade::create(1, createScene()));
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
  flagsCounterHeaderItem->setString(
  to_string(boardMap.qtyBombs - boardMap.getFlagsQty()));
}

Node* GameScene::createRestartButton() {
  const auto restartButton = ui::Button::create(labels_white);
  restartButton->addTouchEventListener(CC_CALLBACK_0(GameScene::restart, this));
  restartButton->setPosition(Vec2(VISIBLE_SIZE.width / 2,
                                  VISIBLE_SIZE.height - 100));

  const auto restartText = Label::createWithTTF("Reiniciar", font_arial, 16);
  restartText->setColor(Color3B::BLACK);
  restartText->setPosition(Vec2(restartButton->getContentSize().width / 2,
                                restartButton->getContentSize().height / 2));

  restartButton->addChild(restartText, 2);
  return restartButton;
}

Label* GameScene::createHeaderItem(const string initialText,
                                   const string iconPath,
                                   const Vec2 position) {
  const auto sprite = Sprite::create(labels_white);
  sprite->setPosition(position);
  sprite->setScale(1);

  const auto icon = Sprite::create(iconPath);
  icon->setContentSize(Size(sprite->getContentSize().height * 0.5,
                            sprite->getContentSize().height * 0.5));
  icon->setPosition(Vec2(icon->getContentSize().width / 2 + 10,
                         sprite->getContentSize().height / 2));

  const auto label = Label::createWithTTF(initialText, font_arial, 20);
  label->setColor(Color3B::BLACK);
  label->setPosition(Vec2(sprite->getContentSize().width / 2,
                          sprite->getContentSize().height / 2));

  sprite->addChild(icon, 1);
  sprite->addChild(label, 2);

  this->addChild(sprite);
  return label;
}
