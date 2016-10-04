//
//  OpeClickCtner2.cpp
//  MyCppGame
//
//  Created by msempire on 16/9/28.
//
//

#include "OpeClickCtner2.hpp"
#include "OpeClickNode.hpp"
USING_NS_CC;
using namespace std;

OpeClickCtner2* OpeClickCtner2::create(const std::vector<OpeNodeData *> &nodeTypes){
    auto ret = new OpeClickCtner2();
    if(ret && ret->init(nodeTypes)){
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}

OpeClickCtner2::OpeClickCtner2():
_target(nullptr)
,_handler(nullptr){
    
}

bool OpeClickCtner2::init(const std::vector<OpeNodeData *> &nodeTypes){
    if(!OpeCtner::init()){
        return false;
    }
    
    this->removeAllChildren();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* event)->bool{
        auto target = static_cast<OpeClickNode*>(event->getCurrentTarget());
        if(target->pointInNode(touch->getLocation())){
            return true;
        }
        return false;
    };
    listener->onTouchEnded =[](Touch* touch, Event* event){
        auto target = static_cast<OpeClickNode*>(event->getCurrentTarget());
        target->switchState(OpeNode::STATE_ON);
        
    };
    
    float x = 0;
    float y = 0;
    for(auto iter = nodeTypes.begin(); iter != nodeTypes.end(); ++iter){
        auto child = OpeClickNode::create(*iter);
        child->setAnchorPoint(Vec2(0, 0));
        child->setPosition(x, 0);
        child->setStChangeCB(_target, _handler);
        this->addChild(child);
        getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener->clone(), child);
        x += child->getContentSize().width + 10.0f;
        y = y > child->getContentSize().height ? y : child->getContentSize().height;
    }
    
    this->setContentSize(Size(x, y));
    return true;
}

void OpeClickCtner2::setOpeNodeCB(cocos2d::Ref *target, NODE_STATE_CHANGE_CALL_BACK handler){
    _target = target;
    _handler = handler;
    
    auto children = this->getChildren();
    auto iter = children.begin();
    for(; iter != children.end(); ++iter){
        auto node = dynamic_cast<OpeClickNode*>(*iter);
        if(node){
            node->setStChangeCB(target, handler);
        }
    }
}

void OpeClickCtner2::resetNodeState(){
    auto children = this->getChildren();
    auto iter = children.begin();
    for(; iter != children.end(); ++iter){
        auto node = dynamic_cast<OpeClickNode*>(*iter);
        if(node){
            node->resetState();
        }
    }
}


