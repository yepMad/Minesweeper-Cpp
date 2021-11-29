#ifndef __HEADERITEM_H__
#define __HEADERITEM_H__

#include "cocos2d.h"

using namespace std;
USING_NS_CC;

class HeaderItem final : public Sprite {
public:
  bool init() override;

  void addIcon(string iconPath);
  void addLabel(string initialText);
  void setLabelText(string newText) const;

private:
  Vec2 getDefaultIconPosition() const;
  Size getDefaultIconSize() const;
  
  Label* labelText = nullptr;
};

#endif