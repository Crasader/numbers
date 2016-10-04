//
//  ChapterItem.cpp
//  MyCppGame
//
//  Created by msempire on 16/9/2.
//
//

#include "ChapterItem.hpp"
#include "DataControl.hpp"
#include "GameUtil.hpp"
#include "StrConstants.h"
#include <string>
USING_NS_CC;
using std::string;

#define TAG_CHAPTER_ITEM_SELECT 1
ChapterItem* ChapterItem::create(const ChapterData &data){
    auto ret = new ChapterItem();
    if(ret && ret->init(data)){
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}

ChapterItem::ChapterItem(): _id(0){
}

bool ChapterItem::init(const ChapterData& data){
    if(!ui::Widget::init()){
        return false;
    }
    
    _id = data.id;
    float x = Director::getInstance()->getOpenGLView()->getScaleX();
    float w = 380.0f;
    float h = 36.0f;
    this->setContentSize(Size(w, h));

    
    //bg
//    auto normal = Sprite::createWithSpriteFrameName("chapter_item_bg.png");
    auto normal = ::createSpriteFrameWithScale("chapter_item_bg.png");
    this->addChild(normal);
    
//    auto select = Sprite::createWithSpriteFrameName("chapter_item_bg2.png");
    auto select = ::createSpriteFrameWithScale("chapter_item_bg2.png");
    select->setTag(TAG_CHAPTER_ITEM_SELECT);
    select->setVisible(false);
    this->addChild(select);
    

    normal->setPosition(Vec2(w/2, h/2));
    select->setPosition(Vec2(w/2, h/2));
    
    
    //label
    w = (w - select->getContentSize().width*select->getScale()) / 2 + 10.0f;
    w = 10.0f;
    string s = data.name;
    auto l = Label::createWithTTF(s, "fonts/Time Roman Italic.ttf", 12);
    l->setAnchorPoint(Vec2(0, 0.5f));
    l->setPosition(w, h/2);
    this->addChild(l);
    w += l->getContentSize().width + 10.0f;
    
    s = string(STR_PASS_SCORE) + ::uIntToStr(data.passScore) + "  " + string(STR_STORE_SCORE) + ::uIntToStr(data.userScore) + STR_STORE_MISS + ::uIntToStr(data.userMiss);
    l = Label::createWithTTF(s, "fonts/Time Roman Italic.ttf", 8);
    l->setAnchorPoint(Vec2(0, 0.5f));
    l->setPosition(w, h/2);
    this->addChild(l);
    
    
    
    return true;
}

void ChapterItem::setSelected(bool selected){
    Node* selectImg = this->getChildByTag(TAG_CHAPTER_ITEM_SELECT);
    if(!selectImg){
        return;
    }
    
    selectImg->setVisible(selected);
}