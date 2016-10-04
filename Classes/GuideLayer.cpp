//
//  GuideLayer.cpp
//  MyCppGame
//
//  Created by msempire on 16/9/28.
//
//

#include "GuideLayer.hpp"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC::ui;

#define TAG_GUIDE_LAYER_RECT 1
GuideLayer::GuideLayer():
_stencil(nullptr)
,_clipper(nullptr)
{
    
}


bool GuideLayer::init(){
    if(!Layer::init()){
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //clip
    _clipper = ClippingNode::create();
    _clipper->setAnchorPoint(Vec2(0.5f, 0.5f));
    _clipper->setContentSize(visibleSize);
    _clipper->setPosition(visibleSize/2);
    _clipper->setInverted(true);
    this->addChild(_clipper);
    
    //bg
    auto bg = LayerColor::create(Color4B(0x0, 0x0, 0x0, 100), visibleSize.width, visibleSize.height);
//    bg->setPosition(visibleSize/2);
    _clipper->addChild(bg);
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* evt){
        //send event
        auto e = EventCustom(GUIDE_TOUCH_GUSTOM_EVENT_NAME);
        auto pos = new Vec2(touch->getLocation());
        e.setUserData(pos);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&e);
        delete pos;
        return true;
    };
    listener->onTouchEnded = [](Touch* touch, Event* evt){
        
    };
    listener->setSwallowTouches(true);
    
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    //stencil --for add rect
    _stencil = Node::create();
    _clipper->setStencil(_stencil);
    
    //rect node
    auto node = Node::create();
    node->setTag(TAG_GUIDE_LAYER_RECT);
    _clipper->addChild(node);
    
    return true;
}

void GuideLayer::addVisibleRect(const cocos2d::Rect &rect){
//    auto layer = LayerColor::create(Color4B(0xff, 0xff, 0xff, 255), rect.size.width, rect.size.height);
    auto hole = LayerColor::create(Color4B(0xff, 0xff, 0xff, 255), rect.size.width, rect.size.height);
    hole->setPosition(rect.origin);
    _stencil->addChild(hole);
    
    auto parent = _clipper->getChildByTag(TAG_GUIDE_LAYER_RECT);
    if(!parent){
        return;
    }
    
    auto layer = Scale9Sprite::createWithSpriteFrameName("guide_rect_bg2.png");
    layer->setContentSize(rect.size+Size(10, 10));
    layer->setPosition(rect.origin.x+ rect.size.width/2, rect.origin.y + rect.size.height/2);
    parent->addChild(layer);
    
    auto action = Blink::create(2, 2);
    layer->runAction(action);
}

void GuideLayer::addGuideStr(const std::string &text, cocos2d::Vec2 pos){
    auto parent = _clipper->getChildByTag(TAG_GUIDE_LAYER_RECT);
    if(!parent){
        return;
    }
    
    auto bg = Scale9Sprite::createWithSpriteFrameName("guide_text_bg.png");
    bg->setContentSize(Size(460, 45));
    bg->setPosition(pos);
    parent->addChild(bg);
    
    auto l = Label::createWithTTF(text, "fonts/Time Roman Italic.ttf", 12);
    l->setColor(Color3B(0xff, 0xba, 0x00));
    l->setPosition(pos);
    parent->addChild(l);
    
//    auto action = Blink::create(2, 2);
//    l->runAction(action);
}

void GuideLayer::removeAllRect(){
    _stencil->removeAllChildren();
    auto parent = _clipper->getChildByTag(TAG_GUIDE_LAYER_RECT);
    if(parent){
        parent->removeAllChildren();
    }
}