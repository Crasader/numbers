//
//  SoundControl.cpp
//  MyCppGame
//
//  Created by msempire on 16/9/2.
//
//

#include "SoundControl.hpp"
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameUtil.hpp"
#include "GameControl.hpp"
#include "audio/include/AudioEngine.h"
USING_NS_CC;
using std::string;
using CocosDenshion::SimpleAudioEngine;
using experimental::AudioEngine;

SoundControl* SoundControl::_instance = nullptr;

SoundControl::SoundControl():_playSound(true), SOUND_MIN_INDEX(-24), SOUND_MAX_INDEX(24), _bgPlayed(false){
    
}

bool SoundControl::init(){
    
    return true;
}

void SoundControl::playSound(int index){
    if(!GameControl::getInstance().getSound()){
        return;
    }
    
    if(index < SOUND_MIN_INDEX || index > SOUND_MAX_INDEX){
        return;
    }
    
    
    string fileName = "music/fail_1.wav";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    fileName = "music/fail_3.m4a";
#endif
    
//    SimpleAudioEngine::getInstance()->stopAllEffects();
    SimpleAudioEngine::getInstance()->playEffect(fileName.c_str());
}

void SoundControl::initBgMusic(){
    SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/schumann.mp3");
//    AudioEngine::preload("music/schumann.mp3", CC_CALLBACK_1(SoundControl::loadCallBack, this));
    
    
    string fileName = "music/fail_1.wav";
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    fileName = "music/fail_1.m4a";
#endif
    SimpleAudioEngine::getInstance()->preloadEffect(fileName.c_str());
    
}

void SoundControl::pauseBgMusic(){
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    _playSound = false;
//    AudioEngine::pauseAll();
}

void SoundControl::resumeBgMusic(){
    if(!GameControl::getInstance().getSound()){
        return;
    }
    if(_bgPlayed){
//        AudioEngine::resumeAll();
        SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    }else{
        SimpleAudioEngine::getInstance()->playBackgroundMusic("music/schumann.mp3", true);
//        AudioEngine::play2d("music/schumann.mp3", true);
        _bgPlayed = true;
    }
    
    _playSound = true;
    
}

void SoundControl::startSound(){
    if(_playSound)
        resumeBgMusic();
}

void SoundControl::stopSound(){
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    SimpleAudioEngine::getInstance()->stopAllEffects();
    _bgPlayed = false;
}

void SoundControl::pauseSound(){
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
}

void SoundControl::resumeSound(){
    if(!GameControl::getInstance().getSound()){
        return;
    }
    if(_playSound){
        resumeBgMusic();
        SimpleAudioEngine::getInstance()->resumeAllEffects();
    }
}

void SoundControl::end(){
    SimpleAudioEngine::getInstance()->end();
}

void SoundControl::loadCallBack(bool load){
    AudioEngine::play2d("music/schumann.mp3", true);
}