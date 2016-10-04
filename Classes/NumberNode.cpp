//
//  NumberNode.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/9.
//
//

#include "NumberNode.hpp"
#include "GameUtil.hpp"
#include "Constants.h"
#include "Numbers.hpp"
#include "SoundControl.hpp"
#include <string>
USING_NS_CC;
using std::string;

enum{
    TAG_NUMBER_NODE_BG,
    TAG_NUMBER_NODE_Label,
};

NumberNode::NumberNode():
_index(0),
_type(UNINITIAL),
_state(NORMAL),
_num(0),
_target(0),
_callBack(0)
{
    setContentSize(Size(50, 70));
}

NumberNode* NumberNode::create(int index, int type, int num){
    NumberNode* p = new (std::nothrow)NumberNode();
    if(p && p->init(index, type, num)){
        p->autorelease();
        return p;
    }
    else{
        delete p;
    }
    
    return nullptr;
}

bool NumberNode::init(int index, int type, int num){
    if(!Node::init()){
        return false;
    }
    
    _index = index;
    _type = type;
    _num = num;
    _state = PRE;
    
    updateView();
    updateStateView();
    return true;
}


void NumberNode::setState(int st){
    if(_state == st){
        return;
    }
    
    _state = st;
    updateStateView();
    playSound();
}

void NumberNode::playSound(){
//    int sound = INVALIDATE_NUMBER;
//    switch (_state) {
//        case CORRECT:
//            sound = 3;
//            break;
//        case WRONG:
//            sound = -1;
//            break;
//        case END:
////            sound = 0;
//        default:
//            break;
//    }
//    
//    if(sound != INVALIDATE_NUMBER){
//        SoundControl::getInstance()->playSound(sound);
//    }
    
    if(_state == WRONG){
        SoundControl::getInstance()->playSound(0);
    }
}

bool NumberNode::updateView(){
    
    //set num str
    string s = " ";
    Color4B bgColor(0xff, 0xff, 0xff, 30);
    Color3B lColor = Color3B::WHITE;
    if(_type == PLAY){
        ::uIntToStr(_num, s);
        bgColor.a = 100;
        lColor = Color3B::BLACK;
    }else if(_type == DISPLAY){
        ::uIntToStr(_num, s);
    }else if(_type == DELAY){
        s = "-";
    }
    
    this->removeAllChildrenWithCleanup(true);
    
    auto bg = LayerColor::create(bgColor, this->getContentSize().width, this->getContentSize().height);
    bg->setTag(TAG_NUMBER_NODE_BG);
    this->addChild(bg);
    
    auto l = Label::createWithTTF(s, "fonts/Marker Felt.ttf", 14);
    l->setColor(lColor);
    l->setAnchorPoint(Vec2(0.5f, 0.5f));
    l->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
//    l->setScale(Director::getInstance()->getOpenGLView()->getScaleX());
    l->setTag(TAG_NUMBER_NODE_Label);
    this->addChild(l);
    
    return true;
}

#define NUMBER_NODE_GUIDE_TAG 2
void NumberNode::setGuide(const string& guideStr, Type type, State state){
    
    if(_type != type){
        return;
    }
    
    _guideArr[state] = guideStr;
    if(_state == state){
        showGuide();
    }
    
    
}

void NumberNode::showGuide(){
    Node* guide = dynamic_cast<Node*>(this->getChildByTag(NUMBER_NODE_GUIDE_TAG));
    if(guide){
        guide->removeFromParentAndCleanup(true);
    }
    
    
    
}

void NumberNode::hideGuide(){
    Node* guide = dynamic_cast<Node*>(this->getChildByTag(NUMBER_NODE_GUIDE_TAG));
    if(guide){
        guide->removeFromParentAndCleanup(true);
    }
    
    for(int i = 0; i < NORMAL; ++i){
        _guideArr[i].clear();
    }
}


void NumberNode::updateStateView(){
    auto bg = dynamic_cast<LayerColor*>(this->getChildByTag(TAG_NUMBER_NODE_BG));
    if(!bg){
        return;
    }

    switch (_state) {
        case NOW:
            bg->updateDisplayedOpacity(255);
            break;
        case CORRECT:
            bg->setColor(Color3B(0xff, 0xba, 0x00));
            break;
        case WRONG:
            bg->setColor(Color3B::RED);
            break;
        case END:
            bg->updateDisplayedOpacity(100);
        default:
            break;
    }
    
}

void NumberNode::play(){
    if(_state != PRE){
        return;
    }
    
    setState(NOW);
    
    //play music
//    if(_type != DELAY){
//        SoundControl::getInstance()->playSound(_num);
//    }
    
    //call back
    int num = _num;
    if(_type != PLAY)
    {
        num = INVALIDATE_NUMBER;
    }
    if(_target && _callBack){
        (_target->*_callBack)(_index, num);
    }
    
}
