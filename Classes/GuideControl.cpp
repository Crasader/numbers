//
//  GuideControl.cpp
//  MyCppGame
//
//  Created by msempire on 16/9/29.
//
//

#include "GuideControl.hpp"
#include "GuideLayer.hpp"
USING_NS_CC;
using namespace std;

#define TAG_GUIDE_LAYER 2

GuideControl::GuideControl():_curStep(-1){
    _listener = Director::getInstance()->getEventDispatcher()->addCustomEventListener(GUIDE_TOUCH_GUSTOM_EVENT_NAME, CC_CALLBACK_1(GuideControl::onTouchCallBack, this));
}

GuideControl::~GuideControl(){
//    Director::getInstance()->getEventDispatcher()->removeEventListener(_listener);
    _listener = nullptr;
    Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(GUIDE_TOUCH_GUSTOM_EVENT_NAME);
}

void GuideControl::addGuideStep(const string& commands, const Rect& r){
    _stepVec.push_back(pair<string, Rect>(commands, r));
}

void GuideControl::startGuide(){
    while (guideNext());
    
    //dispath
}

bool GuideControl::guideNext(){
    //at the last step
    int lastIndex = static_cast<int>(_stepVec.size()) -1;
    if(_curStep >= lastIndex){
        return false;
    }
    
    _curStep++;
    auto curStep = _stepVec[_curStep];
    auto commands = curStep.first;
    if(commands == GUIDE_COMMANDS_REMOVE_GUIDE){
        _container->addToLayer(nullptr, BasicScene::GUIDE_LAYER_INDEX, true);
        return true;
    }else if(commands == GUIDE_COMMANDS_WAIT_TOUCH){
        return false;
    }else if(commands == GUIDE_COMMANDS_CALL_BACK){
        _container->guideCallBack(curStep.second);
        return true;
    }
    
    auto p = _container->getLayer(BasicScene::GUIDE_LAYER_INDEX);
    auto guide = dynamic_cast<GuideLayer*>( p->getChildByTag(TAG_GUIDE_LAYER) );
    if(!guide){
        guide = GuideLayer::create();
        guide->setTag(TAG_GUIDE_LAYER);
        _container->addToLayer(guide, BasicScene::GUIDE_LAYER_INDEX, true);
    }else if(commands == GUIDE_COMMANDS_CLEAR_RECT){
        guide->removeAllRect();
        return true;
    }
    
    if(commands == GUIDE_COMMANDS_ADD_RECT){
        guide->addVisibleRect(curStep.second);
        return true;
    }else{
        guide->addGuideStr(commands, curStep.second.origin);
        return true;
    }
    
}

void GuideControl::setLayerShow(bool flag){
    auto p = _container->getLayer(BasicScene::GUIDE_LAYER_INDEX);
    auto guide = dynamic_cast<GuideLayer*>( p->getChildByTag(TAG_GUIDE_LAYER) );
    if(guide){
        guide->setVisible(flag);
        if(flag){
            Director::getInstance()->getEventDispatcher()->resumeEventListenersForTarget(guide);
        }else{
            Director::getInstance()->getEventDispatcher()->pauseEventListenersForTarget(guide);
        }
    }
}

void GuideControl::onTouchCallBack(cocos2d::EventCustom *evt){
    if(_curStep >= _stepVec.size()){
        return;
    }
    
    auto curStep = _stepVec[_curStep];
    if(curStep.first != GUIDE_COMMANDS_WAIT_TOUCH){
        return;
    }
    
    auto rect = curStep.second;
    Vec2* pos = static_cast<Vec2*>(evt->getUserData());
    if(rect.size.width == 0 && rect.size.height == 0){
        startGuide();
    }else if(rect.containsPoint(*pos)){
        startGuide();
    }
    
}