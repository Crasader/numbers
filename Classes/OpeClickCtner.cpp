//
//  OpeClickCtner.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/19.
//
//

#include "OpeClickCtner.hpp"
#include "Constants.h"
USING_NS_CC;
using namespace std;


OpeClickCtner::OpeClickCtner()
{
    initPosArr();
}

OpeClickCtner* OpeClickCtner::create(const Size &size, const vector<OpeNodeData*> &nodeTypes){
    OpeClickCtner* ret = new OpeClickCtner();
    if(ret && ret->init(size, nodeTypes)){
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}

bool OpeClickCtner::init(const Size &size, const vector<OpeNodeData*> &nodeTypes){
    if(!Node::init()){
        return false;
    }
    
    setContentSize(size);
    //    auto layer = LayerColor::create(Color4B::BLUE, size.width, size.height);
    //    this->addChild(layer);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event* event)->bool{
        auto target = static_cast<OpeNode*>(event->getCurrentTarget());
        auto pointInNode = target->convertToNodeSpace(touch->getLocation());
        Size size = target->getContentSize();
        Rect rect(0, 0, size.width, size.height);
        if(rect.containsPoint(pointInNode)){
            target->setState(OpeNode::STATE_SELECT, true);
            return true;
        }
        
        return false;
    };
    listener->onTouchMoved =[](Touch* touch, Event* event){
    };
    listener->onTouchEnded =[](Touch* touch, Event* event){
        auto target = static_cast<OpeNode*>(event->getCurrentTarget());
        target->setState(OpeNode::STATE_SELECT, false);
        target->switchState(OpeNode::STATE_ON);
        
    };
    
    size_t num = nodeTypes.size();
    int index = getStartIndex((int)num);
    auto iter = nodeTypes.begin();
    for(; iter != nodeTypes.end(); ++iter){
        auto node = OpeNode::create(*iter);
        node->setAnchorPoint(Vec2(0.5f, 0.5f));
        Size nodeSize = node->getContentSize();
        node->setPosition(getPosByIndex(index++, size, nodeSize));
        this->addChild(node);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), node);
    }
    
    return true;
    
}

void OpeClickCtner::setOpeNodeCB(cocos2d::Ref *target, NODE_STATE_CHANGE_CALL_BACK handler){
    auto children = this->getChildren();
    auto iter = children.begin();
    for(; iter != children.end(); ++iter){
        auto node = dynamic_cast<OpeNode*>(*iter);
        if(node){
            node->setStChangeCB(target, handler);
        }
    }
}

void OpeClickCtner::initPosArr(){
    //1
    _posArr.push_back(Rect(POSITION_C, POSITION_C));
    //2
    _posArr.push_back(Rect(POSITION_L, POSITION_C));
    _posArr.push_back(Rect(POSITION_R, POSITION_C));
    //3
    _posArr.push_back(Rect(POSITION_L, POSITION_B));
    _posArr.push_back(Rect(POSITION_R, POSITION_B));
    _posArr.push_back(Rect(POSITION_C, POSITION_T));
    //4
    _posArr.push_back(Rect(POSITION_L, POSITION_C));
    _posArr.push_back(Rect(POSITION_C, POSITION_T));
    _posArr.push_back(Rect(POSITION_R, POSITION_C));
    _posArr.push_back(Rect(POSITION_C, POSITION_B));
    //5
    _posArr.push_back(Rect(POSITION_L, POSITION_C));
    _posArr.push_back(Rect(POSITION_C, POSITION_T));
    _posArr.push_back(Rect(POSITION_R, POSITION_C));
    _posArr.push_back(Rect(POSITION_C, POSITION_B));
    _posArr.push_back(Rect(POSITION_C, POSITION_C));
    
    
}

int OpeClickCtner::getStartIndex(int num){
    int sum = 0;
    while(--num > 0){
        sum += num;
    }
    
    return sum;
}

Vec2 OpeClickCtner::getPosByIndex(int index, const Size& size, const Size& nodeSize){
    if(index < 0 || index >= _posArr.size()){
        index = 0;
    }
    
    const Rect& r = _posArr[index];
    float x = size.width*r.origin.x + nodeSize.width*r.origin.y;
    float y = size.height*r.size.width + nodeSize.height*r.size.height;
    
    return Vec2(x, y);
}

void OpeClickCtner::resetNodeState(){
    auto children = this->getChildren();
    auto iter = children.begin();
    for(; iter != children.end(); ++iter){
        auto node = dynamic_cast<OpeNode*>(*iter);
        if(node){
            node->resetState();
        }
    }
}