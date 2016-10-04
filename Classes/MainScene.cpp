//
//  MainScene.cpp
//  MyCppGame
//
//  Created by msempire on 16/9/1.
//
//

#include "MainScene.hpp"
#include "SceneControl.hpp"
#include "SettingLayer.hpp"
#include "SoundControl.hpp"
USING_NS_CC;
enum{
    TAG_MENU_START,
    TAG_MENU_SELECT,
    TAG_MENU_SETTING,
};


bool MainScene::init(){
    if(!BasicScene::init()){
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    float x = Director::getInstance()->getOpenGLView()->getScaleX();
//    float y = Director::getInstance()->getOpenGLView()->getScaleY();
    
    //add bg
//    auto bg = LayerColor::create(Color4B::GRAY, visibleSize.width, visibleSize.height);
//    this->addToLayer(bg, BG_LAYER_INDEX, false);
    
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addToLayer(menu, OPE_LAYER_INDEX, false);
    
    //add title
//    auto title = Sprite::createWithSpriteFrameName("number_title.png");
//    title->setScale(x);
    auto title = Label::createWithTTF("Numbers", "fonts/Time Roman Italic.ttf", 50.0f);
    title->setPosition(Vec2(240.0f, 185.0f));
    this->addToLayer(title, OPE_LAYER_INDEX, false);
    
    
    //add menu: start
    auto normal = Sprite::createWithSpriteFrameName("btn_start.png");
    auto select = Sprite::createWithSpriteFrameName("btn_start2.png");
    auto menuItem = MenuItemSprite::create(normal, select, CC_CALLBACK_1(MainScene::menuCallBack, this));
    menuItem->setTag(TAG_MENU_START);
    menuItem->setPosition(visibleSize.width/4, 70.0f);
    menuItem->setScale(x);
    menu->addChild(menuItem);
    
    
    //add menu: select
    normal = Sprite::createWithSpriteFrameName("btn_select.png");
    select = Sprite::createWithSpriteFrameName("btn_select2.png");
    menuItem = MenuItemSprite::create(normal, select, CC_CALLBACK_1(MainScene::menuCallBack, this));
    menuItem->setTag(TAG_MENU_SELECT);
    menuItem->setPosition(visibleSize.width/4*3, 70.0f);
    menuItem->setScale(x);
    menu->addChild(menuItem);
    

    return true;
}

void MainScene::onEnter(){
    BasicScene::onEnter();
    
    SoundControl::getInstance()->resumeBgMusic();
}

void MainScene::menuCallBack(cocos2d::Ref *pSender){
    Node* menuItem = dynamic_cast<Node*>(pSender);
    if(!menuItem){
        return;
    }
    
    int tag = menuItem->getTag();
    switch (tag) {
        case TAG_MENU_START:
            startPlay();
            break;
        case TAG_MENU_SELECT:
            openSelectLayer();
            break;
            
        default:
            break;
    }
}


void MainScene::startPlay(){
    SceneControl::getInstance().enterScene(SceneControl::SCENE_ID_PLAY, SceneControl::ENTER_TYPE_PUSH);
}

void MainScene::openSelectLayer(){
    SceneControl::getInstance().enterScene(SceneControl::SCENE_ID_CHAPTER, SceneControl::ENTER_TYPE_PUSH);
}