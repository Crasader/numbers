//
//  GameUtil.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/16.
//
//

#include "GameUtil.hpp"
#include <stdio.h>
using namespace std;
USING_NS_CC;

void uIntToStr(int d, std::string& str){
    //method 1
    char charArr[12];
    sprintf(charArr, "%d", d);
    str = charArr;
    
    //method 2
//    stringstream ss;
//    ss << d;
//    ss >> str;
    
}

string uIntToStr(int d){
    char charArr[12];
    sprintf(charArr, "%d", d);
    return charArr;
}

cocos2d::Sprite* createSpriteFrameWithScale(const std::string& name){
//    float x = cocos2d::Director::getInstance()->getOpenGLView()->getScaleX();
    auto s = cocos2d::Sprite::createWithSpriteFrameName(name);
//    auto s = SpriteFrameCache::sharedSpriteFrameCache()->getSpriteFrameByName(name);
    if(s){
        auto pixelSize = s->getSpriteFrame()->getOriginalSizeInPixels();
        auto actualSize = s->getSpriteFrame()->getOriginalSize();
        
        s->setScale(pixelSize.width/actualSize.width, pixelSize.height/actualSize.height);
    }
    return s;
}

cocos2d::MenuItemSprite* createMenuItemWithScale(const std::string& normalName, const std::string& selectName, const cocos2d::ccMenuCallback& callback){
    auto normal = cocos2d::Sprite::createWithSpriteFrameName(normalName);
    auto select = cocos2d::Sprite::createWithSpriteFrameName(selectName);
    auto item = cocos2d::MenuItemSprite::create(normal, select, callback);
    if(item){
        item->setScale(cocos2d::Director::getInstance()->getOpenGLView()->getScaleX());
    }
    return item;
}

std::string getFileNameFromRes(const std::string& fileName){
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        //writeable path
        auto pos = fileName.find_last_of('/');
        if(pos == string::npos ){
            pos = 0;
        }
        auto relName = fileName.substr(pos);
        auto wp = FileUtils::getInstance()->getWritablePath() + relName;
        FILE* file = fopen(wp.c_str(), "r");
        if(file != NULL){
            fclose(file);
            return wp;
        }

        auto data = FileUtils::getInstance()->getDataFromFile(FileUtils::getInstance()->fullPathForFilename(fileName));
        if(data.isNull()){
            CCLOG("MyCppGame getFileNameFromRes read file data failed: %s", relName.c_str());
            return relName;
        }
        file = fopen(wp.c_str(), "wb");
        if(file == NULL){
            CCLOG("MyCppGame getFileNameFromRes create file failed: %s", wp.c_str());
            return relName;
        }

        fwrite(data.getBytes(), sizeof(char), data.getSize(), file);
        fflush(file);
        fclose(file);
        return wp;
    
    #endif
    
    return FileUtils::getInstance()->fullPathForFilename(fileName);
}


