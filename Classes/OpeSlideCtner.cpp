//
//  OpeSlideCtner.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/18.
//
//

#include "OpeSlideCtner.hpp"
#include "OpeSlideNode.hpp"
USING_NS_CC;
using namespace std;

OpeSlideCtner::OpeSlideCtner()
{
}

OpeSlideCtner* OpeSlideCtner::create(const Size &size, const vector<OpeNodeData*> &nodeTypes){
    OpeSlideCtner* ret = new OpeSlideCtner();
    if(ret && ret->init(size, nodeTypes)){
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}

bool OpeSlideCtner::init(const Size &size, const vector<OpeNodeData*> &nodeTypes){
    if(!Node::init()){
        return false;
    }
    
    setContentSize(size);
//    auto layer = LayerColor::create(Color4B::BLUE, size.width, size.height);
//    this->addChild(layer);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event* event)->bool{
        auto target = static_cast<OpeSlideNode*>(event->getCurrentTarget());
        if(target->pointInNode(touch->getLocation())){
            target->startSlide();
            return true;
        }
        return false;
    };
    listener->onTouchMoved =[](Touch* touch, Event* event){
        auto target = static_cast<OpeSlideNode*>(event->getCurrentTarget());
        target->slide(touch->getDelta());
    };
    listener->onTouchEnded =[](Touch* touch, Event* event){
        auto target = static_cast<OpeSlideNode*>(event->getCurrentTarget());
        target->stopSlide();

    };
    
    size_t num = nodeTypes.size();
    float durW = size.width / num;
    float y = size.height / 2;
    float x = durW / 2;
    Size nodeSize(durW, size.height);
    auto iter = nodeTypes.begin();
    for(; iter != nodeTypes.end(); ++iter){
        OpeSlideNode* node = OpeSlideNode::create(nodeSize, *iter);
        this->addChild(node);
        node->setAnchorPoint(Vec2(0.5f, 0.5f));
        node->setPosition(x,  y);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), node);
        x += durW;
    }
    
    return true;
    
}

void OpeSlideCtner::setOpeNodeCB(cocos2d::Ref *target, NODE_STATE_CHANGE_CALL_BACK handler){
    auto children = this->getChildren();
    auto iter = children.begin();
    for(; iter != children.end(); ++iter){
        auto node = dynamic_cast<OpeSlideNode*>(*iter);
        if(node){
            node->setOpeNodeCB(target, handler);
        }
    }
}

void OpeSlideCtner::resetNodeState(){
    auto children = this->getChildren();
    auto iter = children.begin();
    for(; iter != children.end(); ++iter){
        auto node = dynamic_cast<OpeSlideNode*>(*iter);
        if(node){
            node->resetSlide();
        }
    }
}



