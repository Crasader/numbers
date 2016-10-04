//
//  OpeNode.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/18.
//
//

#include "OpeNode.hpp"
#include "GameUtil.hpp"
#include "Constants.h"
#include <string>
using namespace std;
USING_NS_CC;

#define OPE_NODE_LABEL_TAG 1
#define OPE_NODE_BG_TAG 2
#define OPE_NODE_GUIDE_TAG 3

OpeNode* OpeNode::create(OpeNodeData* data, float r){
    OpeNode* ret = new OpeNode(data, r);
    if(ret && ret->init()){
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}

bool OpeNode::init(){
    if(!Node::init()){
        return false;
    }

    DrawNode* bg = DrawNode::create();
    bg->drawSolidCircle(Vec2(_r, _r), _r, 360, 360, Color4F::WHITE);
//    auto bg = ::createSpriteFrameWithScale("play_circle.png");
    bg->setTag(OPE_NODE_BG_TAG);
    this->addChild(bg);
    
    Label* label = Label::createWithTTF(getDisplayStr(), "fonts/Marker Felt.ttf", 15);
    label->setPosition(Vec2(_r, _r));
    label->setColor(Color3B::BLACK);
    label->setTag(OPE_NODE_LABEL_TAG);
    this->addChild(label, 1);
    
    setContentSize(Size(2*_r, 2*_r));
    
    return true;
}

string OpeNode::getDisplayStr(){
    if(_data){
        return _data->toString();
    }
    
    return getState(STATE_ON) ? "On" : "Off";
}

void OpeNode::setState(OpeNode::STATE state, bool flag){
    
    if(getState(state) == flag){
        return;
    }
    
    if(state != STATE_GUIDE && (_guideSt == GUIDE_NOT_CHANGE || _guideSt == GUIDE_CHANGE_NOT_NOW)){
        return;
    }
    
    _st = flag ? _st | (1 << state) : _st & (~(1 << state));
    
    if(state == STATE_ON){
        auto color = flag ? Color4F(0xff/255.0f, 0xba/255.0f, 0x00/255.0f, 1) : Color4F::WHITE;
        auto bg = this->getChildByTag(OPE_NODE_BG_TAG);
        if(bg){
            bg->removeFromParentAndCleanup(true);
            auto bg2 = DrawNode::create();
            bg2->drawSolidCircle(Vec2(_r, _r), _r, 360, 360, color);
            bg2->setTag(OPE_NODE_BG_TAG);
            this->addChild(bg2);
        }
//        Color3B color = flag ? Color3B::GREEN : Color3B::BLACK;
//        Label* label = dynamic_cast<Label*>( this->getChildByTag(OPE_NODE_LABEL_TAG) );
//        if(!_data){
//            label->setString(getDisplayStr());
//        }
//        if(label){
//            label->setColor(color);
//        }
        if(_guideSt == GUIDE_CHANGE_NOW){
            _guideSt = GUIDE_NOT_CHANGE;
            setState(STATE_GUIDE, false);
        }
        
    }else if(state == STATE_GUIDE){
        //guide
        auto guide = dynamic_cast<Action*>(this->getActionByTag(OPE_NODE_GUIDE_TAG));
        if(guide){
            this->stopAction(guide);
            this->setVisible(true);
        }
        if(flag){
            auto action = Blink::create(1, 1);
            guide = RepeatForever::create(action);
            guide->setTag(OPE_NODE_GUIDE_TAG);
            this->runAction(guide);
        }
        
    }
    
//    EventCustom event(EVENT_CUSTOM_STATE_CHANGE);
//    pair<int, bool> data(state, flag);
//    event.setUserData(&data);
//    _eventDispatcher->dispatchEvent(&event);
    
    if(_target && _handler && _data){
        (_target->*_handler)(_data, state, flag);
    }
}

bool OpeNode::getState(OpeNode::STATE state){
    return _st & 1 << state;
}

void OpeNode::setLabelStr(const std::string &str){
    Label* label = dynamic_cast<Label*>( this->getChildByTag(OPE_NODE_LABEL_TAG) );
    if(label){
        label->setString(str);
    }
}

void OpeNode::setGuideState(bool st){
    _guideSt = st != getState(STATE_ON);
}

bool OpeNode::guide(){
    if(_guideSt != GUIDE_CHANGE_NOT_NOW){
        return false;
    }
    
    _guideSt = GUIDE_CHANGE_NOW;
    setState(STATE_GUIDE, true);
    return true;
}