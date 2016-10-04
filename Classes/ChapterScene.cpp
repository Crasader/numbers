//
//  ChapterScene.cpp
//  MyCppGame
//
//  Created by msempire on 16/9/1.
//
//

#include "ChapterScene.hpp"
#include "DataControl.hpp"
#include "ChapterItem.hpp"
#include "SceneControl.hpp"
#include "GameControl.hpp"
#include "GameUtil.hpp"
#include "StrConstants.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"
USING_NS_CC;
USING_NS_CC::ui;
using namespace std;

ChapterScene::ChapterScene():
_list(nullptr),
_curSelId(GameControl::getInstance().getCurChapterId()){
    
}

bool ChapterScene::init(){
    if(!BasicScene::init()){
        return false;
    }
    
    
    //start btn
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addToLayer(menu, OPE_LAYER_INDEX, false);
    auto menuItem = ::createMenuItemWithScale("btn_start.png", "btn_start2.png", CC_CALLBACK_1(ChapterScene::startChapter, this));
    menuItem->setPosition(Vec2(240.0f, 40.0f));
    menu->addChild(menuItem);
    
    //add bg
    auto bg = ::createSpriteFrameWithScale("chapter_list_bg.png");
//    auto bg = Scale9Sprite::createWithSpriteFrameName("chapter_list_bg.png");
//    bg->setContentSize(Size(400.0f, 180.0f));
    bg->setPosition(Vec2(240.0f, 175.0f));
    this->addToLayer(bg, OPE_LAYER_INDEX, false);
    
    //add list
    _list = ui::ListView::create();
    _list->setDirection(ui::ScrollView::Direction::VERTICAL);
    _list->setTouchEnabled(true);
    _list->setBounceEnabled(true);
    _list->setContentSize(Size(380.0f, 160.0f));
//    container->setBackGroundColorType(cocos2d::ui::Layout::BackGroundColorType::SOLID);
//    container->setBackGroundColor(Color3B::RED);
    _list->setItemsMargin(5.0f);
    _list->setIgnoreAnchorPointForPosition(false);
    _list->setAnchorPoint(Vec2(0.5f, 0.5f));
    _list->setPosition(Vec2(240.0f, 175.0f));
    this->addToLayer(_list, OPE_LAYER_INDEX, false);
    
    //get data
    std::vector<ChapterData> data;
    DataControl::getInstance().getPassedChapterId(data);
    DataControl::getInstance().addCurChapterData(data);
    
    //add item to list
    auto iter = data.begin();
    for(; iter != data.end(); ++iter){
        ChapterItem* item = ChapterItem::create(*iter);
        item->setIgnoreAnchorPointForPosition(false);
        item->setAnchorPoint(Vec2::ZERO);
        item->setPosition(Vec2::ZERO);
        item->setTag(1000 + iter->id);
        item->setTouchEnabled(true);
        item->addClickEventListener(CC_CALLBACK_1(ChapterScene::onItemClick, this));
    
        _list->pushBackCustomItem(item);
    }
    
    
    //set sel
    ChapterItem* selItem = dynamic_cast<ChapterItem*>(_list->getChildByTag(1000 + _curSelId));
    if(selItem){
        selItem->setSelected(true);
    }
    
    
    return true;
}

void ChapterScene::startChapter(cocos2d::Ref *pSender){
    GameControl::getInstance().setCurChapterId(_curSelId);
    SceneControl::getInstance().enterScene(SceneControl::SCENE_ID_PLAY, SceneControl::ENTER_TYPE_PUSH);
}

void ChapterScene::onItemClick(cocos2d::Ref *target){
    //pre
    auto selItem = dynamic_cast<ChapterItem*>(_list->getChildByTag(1000+_curSelId));
    if(selItem){
        selItem->setSelected(false);
    }
    //now
    selItem = dynamic_cast<ChapterItem*>(target);
    selItem->setSelected(true);
    
    _curSelId = selItem->getChapterId();
}