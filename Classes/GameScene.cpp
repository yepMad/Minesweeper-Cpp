#include "GameScene.h"

#include <FilesPaths.h>

USING_NS_CC;

Scene* GameScene::createScene() {
  Scene* scene = Scene::create();

  const auto layerColor = LayerColor::create(Color4B(239, 239, 239, 255));
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

  this->enableListeners();

  return true;
}

void GameScene::enableListeners() {
  const auto listener = EventListenerTouchOneByOne::create();

  listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchesBegan, this);
  listener->setSwallowTouches(true);

  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

bool GameScene::onTouchesBegan(const Touch* touch, Event* event) const {
  if (boardMap->hasEndGame()) {
    return true;
  }
  
  boardMap->onClick(touch->getLocation());
  if (boardMap->getGameOver()) {
    CCLOG("GameOver!");
  }
  
  if (boardMap->getWin()) {
    CCLOG("Win!");
  }
  
  return true;
}
