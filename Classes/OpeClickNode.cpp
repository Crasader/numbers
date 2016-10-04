//
//  OpeClickNode.cpp
//  MyCppGame
//
//  Created by msempire on 16/9/27.
//
//

#include "OpeClickNode.hpp"
USING_NS_CC;

OpeClickNode* OpeClickNode::create(OpeNodeData *data){
    auto ret = new OpeClickNode(data);
    if(ret && ret->init()){
        ret->autorelease();
        return ret;
    }
    
    delete  ret;
    return nullptr;
}

OpeClickNode::OpeClickNode(OpeNodeData* data):
_clickNode(OpeNode::create(data)){
    this->setContentSize(Size(40.0f, 105.0f));
    if(_clickNode){
        _clickNode->retain();
    }
}

OpeClickNode::~OpeClickNode(){
    if(_clickNode){
        _clickNode->release();
    }
}

bool OpeClickNode::init(){
    if(!Node::init() || !_clickNode){
        return false;
    }
    
    const Size& size = this->getContentSize();
    
    //line
    auto line = ::createSpriteFrameWithScale("play_circle_line.png");
    line->setPosition(Vec2(size.width/2, size.height/2));
    this->addChild(line);
    //node
    _clickNode->setAnchorPoint(Vec2(0.5f, 0.5f));
    _clickNode->setPosition(size.width/2, _clickNode->getContentSize().height/2);
    this->addChild(_clickNode);
    return true;
}

void OpeClickNode::switchState(OpeNode::STATE st){
    if(st == OpeNode::STATE_ON){
        bool flag = _clickNode->getState(st);
        setState(st, !flag);
    }else{
        _clickNode->switchState(st);
    }
    
}

void OpeClickNode::setState(OpeNode::STATE state, bool flag){
    if(state == OpeNode::STATE_ON){
        float y = flag ? this->getContentSize().height - _clickNode->getContentSize().height/2 : _clickNode->getContentSize().height/2;
        _clickNode->setPositionY(y);
    }
    
    _clickNode->setState(state, flag);
}

void OpeClickNode::resetState(){
    _clickNode->resetState();
    _clickNode->setPositionY(_clickNode->getContentSize().height/2);
}

bool OpeClickNode::pointInNode(const cocos2d::Vec2& worldPoint){
    Vec2 point = _clickNode->convertToNodeSpace(worldPoint);
    Rect rect = Rect(0,0, _clickNode->getContentSize().width, _clickNode->getContentSize().height);
    return rect.containsPoint(point);
}


void OpeClickNode::setGuideState(bool st){
    _clickNode->setGuideState(st);
}

bool OpeClickNode::guide(){
    return _clickNode->guide();
}