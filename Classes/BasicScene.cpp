//
//  BasicScene.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/3.
//
//

#include "BasicScene.hpp"
#include "BasicLayer.hpp"
#include "GameUtil.hpp"
#include "SettingLayer.hpp"
#include <vector>
#include <string>

USING_NS_CC;


BasicScene::BasicScene():_layerNum(LAYER_END), _settingBtn(nullptr){
    
}

bool BasicScene::init(){
    bool flag = Scene::init();
    
    return flag && initLayer();
}


bool BasicScene::initLayer(){
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    for(int i = 0; i < _layerNum; ++i){
        BasicLayer* layer = BasicLayer::create();
        layer->setContentSize(visibleSize);
        _layerVec.push_back(layer);
        this->addChild(layer);
    }
    
    //add menu: setting
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    addToLayer(menu, SETTING_LAYER_INDEX, false);
    _settingBtn = ::createMenuItemWithScale("btn_setting.png", "btn_setting2.png", CC_CALLBACK_1(BasicScene::showSettingLayer, this));
    _settingBtn->setPosition(visibleSize.width-_settingBtn->getContentSize().width*_settingBtn->getScaleX()/2-10,
                          visibleSize.height-_settingBtn->getContentSize().height*_settingBtn->getScaleY()/2-10);
    menu->addChild(_settingBtn);
    
    return true;
}

bool BasicScene::addToLayer(cocos2d::Node *child, int layerIndex, bool clear){
    if(layerIndex < 0 || layerIndex >= _layerVec.size()){
        return false;
    }
    
    BasicLayer* layer = _layerVec[layerIndex];
    if(clear){
        layer->removeAllChildrenWithCleanup(true);
    }
    
    if(child != nullptr){
        layer->addChild(child);
    }
    
    return true;
}

void BasicScene::initBg(){
    if(_bgName.empty()){
        return;
    }
    
    Sprite* bg = Sprite::createWithSpriteFrameName(_bgName);
    addToLayer(bg, BG_LAYER_INDEX, true);
}

void BasicScene::showSettingLayer(cocos2d::Ref *pSender){
    auto layer = SettingLayer::create();
    this->addToLayer(layer, SETTING_LAYER_INDEX, false);
}

