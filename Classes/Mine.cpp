#include "Mine.h"

#include "FilesPaths.h"
#include "GameScene.h"

bool Mine::init() {
  this->initWithFile(sprite_frame_mine_default);
  return true;
}

void Mine::initialize(const int x, const int y) {
  init();

  this->x = x;
  this->y = y;
}

void Mine::setNearBombCount(const int number) {
  if (mineType == bomb) {
    return;
  }

  nearBombCount = number;
  mineType = nearBombCount > 0 ? counter : none;
}

void Mine::setFlag(const bool isFlagged) {
  this->isFlagged = isFlagged;

  if (this->isFlagged) {
    flag = create(icon_flag);
    
    flag->setContentSize(Size(this->getContentSize().width * 0.5, this->getContentSize().width * 0.5));
    flag->setPosition(this->getContentSize().width/2, this->getContentSize().height/2);
    
    this->addChild(flag, 2);
  } else {
    this->removeChild(flag, false);
  }
}


void Mine::dig() {
  if (wasDug)
    return;

  wasDug = true;

  switch (mineType) {
    case none: {
      this->setSpriteFrame(create(sprite_frame_mine_dug)->getSpriteFrame());
      break;
    }
    case counter: {
      createLabel(Color3B::WHITE, std::to_string(nearBombCount));
      break;
    }
    case bomb: {
      createLabel(Color3B::RED, "B");
      break;
    }
    default:
      break;
  }
}

void Mine::createLabel(const Color3B color, const string text) {
  const auto mineSize = this->getSpriteFrame()->getRect().size;
  const auto label = Label::createWithTTF(text, font_arial, 24);
  
  label->setColor(color);
  label->setPosition(mineSize.width / 2, mineSize.height / 2);
  
  this->addChild(label, 100);
}
