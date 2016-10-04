//
//  GameControl.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/3.
//
//

#include "GameControl.hpp"
#include "SceneControl.hpp"
#include "DataControl.hpp"
#include "SoundControl.hpp"
#include "cocos2d.h"
#include <stdlib.h>
USING_NS_CC;

#define CURRENT_GAME_DATA_VERSION 1
void GameControl::startGame(){
    srand(time(0));
//    SoundControl::getInstance()->stopSound();
    SceneControl::getInstance().enterScene(SceneControl::SCENE_ID_MAIN, SceneControl::ENTER_TYPE_PUSH);
}

void GameControl::endGame(){
    //show exit window
    
    //save data
    DataControl::getInstance().close();
    
    //release res
//    SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("res/numbers.plist");
    
    //exit
    Director::getInstance()->end();    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
    
}

void GameControl::stopGame(){
    //play stop...
    SoundControl::getInstance()->stopSound();
    SceneControl::getInstance().stopSceneAct();
    
}

void GameControl::resumeGame(){
    SoundControl::getInstance()->resumeSound();
    SceneControl::getInstance().startSceneAct();
}


enum{
    SETTING_VERSION_KEY = 1,
    SETTING_SOUND_KEY = 2,
};

bool GameControl::init(){
    _updateVer = DataControl::getInstance().getSetting(SETTING_VERSION_KEY, _updateVer);
    _sound = DataControl::getInstance().getSetting(SETTING_SOUND_KEY, _sound);
    if(CURRENT_GAME_DATA_VERSION != _updateVer){
        DataControl::getInstance().updateDB();
        _updateVer = CURRENT_GAME_DATA_VERSION;
        DataControl::getInstance().setSetting(SETTING_VERSION_KEY, _updateVer);
    }
    _curChapterId = DataControl::getInstance().getCurChapterId();
    
    //
    SoundControl::getInstance()->initBgMusic();
    
    //init res
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/numbers.plist");

    
    return true;
}

void GameControl::setSound(bool on){
    if(_sound == on){
        return;
    }
    
    _sound = on;
    DataControl::getInstance().setSetting(SETTING_SOUND_KEY, on ? 1 : 0);
    
    if(!on){
        SoundControl::getInstance()->stopSound();
    }
}

