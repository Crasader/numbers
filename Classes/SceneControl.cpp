//
//  SceneControl.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/3.
//
//

#include "SceneControl.hpp"
#include "GameControl.hpp"
#include "PlayScene.hpp"
#include "ChapterScene.hpp"
#include "cocos2d.h"
#include "MainScene.hpp"
#include <vector>
#include <string>
USING_NS_CC;
using std::string;


void SceneControl::enterScene(int sid, int enterType){
    Director* director = Director::getInstance();
    
    //TODO: replace this place with the director and add the game end listener
    
    //check if the same sid
    if(enterType != ENTER_TYPE_POP && mCurSid == sid)
        return;
    
    //save sid
    opeSidVec(sid, enterType);
    
    //check if exit
    if(mSidVec.size() == 0){
        mPCurSce = nullptr;
        GameControl::getInstance().endGame();
    }
    
    sid = mSidVec.back();
    //for the pop
    if(mCurSid == sid){
        return;
    }
    
    BasicScene* scene = getSceneById(sid);
    CCASSERT(scene != nullptr, "scene id invalidate");
    mCurSid = sid;
    mPCurSce = scene;
    director->replaceScene(scene);
    
    
}

void SceneControl::opeSidVec(int sid, int enterType){
    switch (enterType) {
        case ENTER_TYPE_POP:
            while (mSidVec.size() > 0 && mSidVec.back() != sid) {
                mSidVec.pop_back();
            }
            break;
        case ENTER_TYPE_PUSH:
            mSidVec.push_back(sid);
            break;
            
        case ENTER_TYPE_REPLACE:
            if(mSidVec.size() > 0){
                mSidVec.back() = sid;
            }else{
                mSidVec.push_back(sid);
            }
            break;
            
            
        default:
            break;
    }
    
}

BasicScene* SceneControl::getSceneById(int sid){
    BasicScene* scene = nullptr;
    switch (sid) {
        case SCENE_ID_PLAY:
            return PlayScene::create(GameControl::getInstance().getCurChapterId());
            
        case SCENE_ID_MAIN:
            return MainScene::create();
            
        case SCENE_ID_CHAPTER:
            return ChapterScene::create();
            
        default:
            break;
    }
    
    return scene;
}

void SceneControl::stopSceneAct(){
    if(mPCurSce){
        mPCurSce->stopAct();
    }
}

void SceneControl::startSceneAct(){
    if(mPCurSce){
        mPCurSce->startAct();
    }
}

