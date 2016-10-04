//
//  GameUtil.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/16.
//
//

#ifndef GameUtil_hpp
#define GameUtil_hpp
#include <string>
#include "cocos2d.h"

void uIntToStr(int d, std::string&);
std::string uIntToStr(int d);
cocos2d::Sprite* createSpriteFrameWithScale(const std::string& name);
cocos2d::MenuItemSprite* createMenuItemWithScale(const std::string& normalName, const std::string& selectName, const cocos2d::ccMenuCallback& callback);

std::string getFileNameFromRes(const std::string& fileName);
#endif /* GameUtil_hpp */
