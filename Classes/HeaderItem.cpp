#include "HeaderItem.h"
#include "FilesPaths.h"

bool HeaderItem::init() {
  this->initWithFile("Labels/White.png");
  this->setScale(1);

  return true;
}

void HeaderItem::addIcon(const string iconPath) {
  const auto icon = create(iconPath);
  const auto iconSize = Size(getDefaultIconSize());

  icon->setContentSize(iconSize);
  icon->setPosition(getDefaultIconPosition());

  this->addChild(icon, 1);
}

void HeaderItem::addLabel(const string initialText = "") {
  labelText = Label::createWithTTF(initialText, font_arial, 20);
  labelText->setColor(Color3B::BLACK);
  labelText->setPosition(
    labelText->getContentSize().width / 2 + getDefaultIconSize().width +
    getDefaultIconPosition().x, this->getContentSize().height / 2);

  this->addChild(labelText, 2);
}


void HeaderItem::setLabelText(const string newText) const {
  labelText->setString(newText);
}


Vec2 HeaderItem::getDefaultIconPosition() const {
  return Vec2(20, this->getContentSize().height / 2);
}

Size HeaderItem::getDefaultIconSize() const {
  return Size(this->getContentSize().height * 0.6,
              this->getContentSize().height * 0.6);
}
