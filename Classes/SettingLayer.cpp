//
//  SettingLayer.cpp
//  MyCppGame
//
//  Created by msempire on 16/9/1.
//
//

#include "SettingLayer.hpp"
#include "StrConstants.h"
#include "GameControl.hpp"
#include "SceneControl.hpp"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC::ui;

enum{
    TAG_MENU_HOME,
    TAG_MENU_SOUND,
    TAG_MENU_QUIT,
    TAG_MENU_CLOSE,
};

bool SettingLayer::init(){
    if(!Layer::init())
        return false;
    
    
    float width = 480;
    float height = 320;
    this->setContentSize(Size(480, 320));
    
    //big bg
    Color4B color(255, 255, 255, 125);
    auto bg = LayerColor::create(color, width, height);
    this->addChild(bg);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* evt){
        return true;
    };
    listener->setSwallowTouches(true);
    
    auto bg2 = ::createSpriteFrameWithScale("setting_bg.png");
//    auto bg2 = Scale9Sprite::createWithSpriteFrameName("setting_bg.png");
//    bg2->setContentSize(Size(280, 200));
    bg2->setPosition(240.0f, 160.0f);
    this->addChild(bg2);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, bg2);
    
    auto menu = Menu::create();
    menu->setAnchorPoint(Vec2::ZERO);
    menu->setPosition(100.0f, 60.0f);
    this->addChild(menu);
    float w = 240.0f;
    float h = 200.0f;
    
    //home
    auto menuItem = ::createMenuItemWithScale("btn_home.png", "btn_home2.png", CC_CALLBACK_1(SettingLayer::btnClick, this));
    menuItem->setAnchorPoint(Vec2(0.5f, 0.5f));
    menuItem->setTag(TAG_MENU_HOME);
    menuItem->setPosition(w/6 + 20.0f , h/3*2);
    menu->addChild(menuItem);
    
    //sound
    if(GameControl::getInstance().getSound()){
        menuItem = ::createMenuItemWithScale("btn_voice_on.png", "btn_voice2.png", CC_CALLBACK_1(SettingLayer::btnClick, this));
    }else{
        menuItem = ::createMenuItemWithScale("btn_voice_off.png", "btn_voice2.png", CC_CALLBACK_1(SettingLayer::btnClick, this));
    }
    menuItem->setAnchorPoint(Vec2(0.5f, 0.5f));
    menuItem->setPosition(w/2 + 20.0f, h/3*2);
    menuItem->setTag(TAG_MENU_SOUND);
    menu->addChild(menuItem);
    
    
    //quit
    menuItem = ::createMenuItemWithScale("btn_quit.png", "btn_quit2.png", CC_CALLBACK_1(SettingLayer::btnClick, this));
    menuItem->setAnchorPoint(Vec2(0.5f, 0.5f));
    menuItem->setPosition(w/6*5 + 20.0f, h/3*2);
    menuItem->setTag(TAG_MENU_QUIT);
    menu->addChild(menuItem);
    
    //close
    menuItem = ::createMenuItemWithScale("btn_close.png", "btn_close2.png", CC_CALLBACK_1(SettingLayer::btnClick, this));
    menuItem->setIgnoreAnchorPointForPosition(false);
    menuItem->setAnchorPoint(Vec2(1.0f, 0));
    menuItem->setPosition(280.0f-20.0f, 20.0f);
    menuItem->setTag(TAG_MENU_CLOSE);
    menu->addChild(menuItem);
    
    //author
    auto l = Label::createWithTTF("music: Robert Schumann - The Merry Farmer\nauthor:mgp\nhave fun!", "fonts/Time Roman Italic.ttf", 10);
//    l->setScale(Director::getInstance()->getOpenGLView()->getScaleX());
//    l->setAnchorPoint(Vec2(0.5f, 0.5f));
    l->setPosition(240.0f, 60.0f + h/3);
    this->addChild(l);
    
    
    return true;
}

void SettingLayer::initSoundNode(){
    _soundNode = OpeNode::create(nullptr, 20.0f);
    
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
        bool state = !target->getState(OpeNode::STATE_ON);
        GameControl::getInstance().setSound(state);
        target->setState(OpeNode::STATE_ON, state);
        
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _soundNode);
    
    bool state = GameControl::getInstance().getSound();
    _soundNode->setState(OpeNode::STATE_ON, state);
}

void SettingLayer::btnClick(Ref* pSender){
    auto node = dynamic_cast<MenuItemSprite*>(pSender);
    if(!node){
        return;
    }
    int tag = node->getTag();
    switch (tag) {
        case TAG_MENU_CLOSE:
            this->removeFromParentAndCleanup(true);
            break;
        case TAG_MENU_QUIT:
            GameControl::getInstance().endGame();
            break;
        case TAG_MENU_HOME:
            this->removeFromParentAndCleanup(true);
            SceneControl::getInstance().enterScene(SceneControl::SCENE_ID_MAIN, SceneControl::ENTER_TYPE_POP);
            break;
        case TAG_MENU_SOUND:
        {
            auto flag = GameControl::getInstance().getSound();
            GameControl::getInstance().setSound(!flag);
            std::string name = flag ? "btn_voice_off.png" : "btn_voice_on.png";
            auto normal = Sprite::createWithSpriteFrameName(name);
            node->setNormalImage(normal);
        }
            break;
            
        default:
            break;
    }
}

//bool SettingLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event){
//    return true;
//}