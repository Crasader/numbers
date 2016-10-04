//
//  OpeSlideNode.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/19.
//
//

#include "OpeSlideNode.hpp"
USING_NS_CC;

OpeSlideNode::OpeSlideNode(const cocos2d::Size& size, Orientation orien, OpeNode* slideNode)
:_slideNode(slideNode)
,_orien(orien){
    setContentSize(size);
    if(_slideNode){
        _slideNode->retain();
    }
}

OpeSlideNode::~OpeSlideNode(){
    _slideNode->release();
}


OpeSlideNode* OpeSlideNode::create(const cocos2d::Size& size, OpeNode *slideNode, Orientation orien){
    auto ret = new OpeSlideNode(size, orien, slideNode);
    if(ret && ret->init()){
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}

bool OpeSlideNode::init(){
    if(!Node::init()){
        return false;
    }
    
    if(!_slideNode){
        return false;
    }

    
    //add bar
    Size size = getContentSize();
//    LayerColor* layer = LayerColor::create(Color4B::RED, size.width, size.height);
//    this->addChild(layer);
    
    Size nodeSize = _slideNode->getContentSize();
    size.width = MAX(size.width, nodeSize.width);
    size.height = MAX(size.height, nodeSize.height);
    if(!size.equals(getContentSize())){
        setContentSize(size);
    }
    
    Vec2 orign = _orien == VERTICAL ? Vec2(size.width/2, nodeSize.height/2) : Vec2(nodeSize.width/2, size.height/2);
    Vec2 desti = _orien == VERTICAL ? Vec2(0, size.height-nodeSize.height) :Vec2(size.width-nodeSize.width, 0);
    DrawNode* node = DrawNode::create();
    node->drawLine(Vec2::ZERO, desti, Color4F::GRAY);
    node->setPosition(Vec2(size.width/2, nodeSize.height/2));
    this->addChild(node);
    
    _slideNode->setAnchorPoint(Vec2(0.5f, 0.5f));
    _slideNode->setPosition(orign);
    this->addChild(_slideNode);
    
    return true;
    
}

void OpeSlideNode::startSlide(){
    _slideNode->setState(OpeNode::STATE_SELECT, true);
}

void OpeSlideNode::stopSlide(){
    _slideNode->setState(OpeNode::STATE_SELECT, false);
    
    //adjust the position
    Size size = getContentSize();
    Size nodeSize = _slideNode->getContentSize();
    bool on = _slideNode->getState(OpeNode::STATE_ON);
    if(_orien == VERTICAL){
        float y = on ? size.height - nodeSize.height/2 : nodeSize.height/2;
        _slideNode->setPositionY(y);
    }else{
        float x = on ? size.width - nodeSize.width/2 : nodeSize.width/2;
        _slideNode->setPositionX(x);
    }
}

void OpeSlideNode::slide(const cocos2d::Vec2& delta){
    Vec2 point = _slideNode->getPosition();
    Size size = getContentSize();
    Size nodeSize = _slideNode->getContentSize();
    bool on = false;
    if(_orien == VERTICAL){
        float y = point.y + delta.y;
        y = MAX(nodeSize.height/2, MIN(y, size.height-nodeSize.height/2));
        _slideNode->setPositionY(y);
        on = y > size.height/2;
    }else{
        float x = point.x + delta.x;
        x = MAX(nodeSize.width/2, MIN(x, size.width-nodeSize.width/2));
        _slideNode->setPositionX(x);
        on = x > size.width/2;
    }
    _slideNode->setState(OpeNode::STATE_ON, on);
}

bool OpeSlideNode::pointInNode(const cocos2d::Vec2& worldPoint){
    Vec2 point = _slideNode->convertToNodeSpace(worldPoint);
    Rect rect = Rect(0,0, _slideNode->getContentSize().width, _slideNode->getContentSize().height);
    return rect.containsPoint(point);
}

void OpeSlideNode::resetSlide(){
    _slideNode->resetState();
    Size size = getContentSize();
    Size nodeSize = _slideNode->getContentSize();
    Vec2 orign = _orien == VERTICAL ? Vec2(size.width/2, nodeSize.height/2) : Vec2(nodeSize.width/2, size.height/2);
    _slideNode->setPosition(orign);
}

void OpeSlideNode::setGuideState(bool st){
    _slideNode->setGuideState(st);
}

bool OpeSlideNode::guide(){
    return _slideNode->guide();
}