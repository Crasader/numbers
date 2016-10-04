//
//  NumContainer.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/17.
//
//

#include "NumContainer.hpp"
#include "NumberNode.hpp"
#include "GameUtil.hpp"
#include <string>
using std::string;
USING_NS_CC;


NumContainer::NumContainer():
_data(nullptr)
,_curEnd(0)
,_curBegin(0)
,_isStart(false)
,_speed(100)
,_endTarget(nullptr)
,_playTarget(nullptr)
,_endCallBack(nullptr)
,_playCallBack(nullptr)
,_curPlayIndex(-1)
,_nextPlayIndex(0)
,_showGuide(false)
,_clipper(nullptr)
,_lock(false)
,_init(false)
{
    for(int i = 0; i < NUM_CONTAINER_MAX_NUM; ++i){
        _container[i] = nullptr;
    }
}

NumContainer::~NumContainer(){
    clearContainer();
    _data->release();
}

NumContainer* NumContainer::create(int numId, int playId, int speed){
    NumContainer* ret = new NumContainer();
    if(ret && ret->init(numId, playId, speed)){
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}

bool NumContainer::init(int numId, int playId, int speed){
    if(!_init){
        Node::init();
        float w = 480.0f;
        float h = 84.0f;
        this->setContentSize(Size(w, h));
        _data = Numbers::create(numId, playId);
        _data->retain();
        //bg1
        auto bg = ::createSpriteFrameWithScale("play_rect_bg.png");
        bg->setAnchorPoint(Vec2(0.5f, 0.5f));
        bg->setPosition(Vec2(w/2, h/2));
        this->addChild(bg);
        
        
        //clipper
        _clipper = ClippingRectangleNode::create(Rect(0, 0, 400, 70));
        _clipper->setAnchorPoint(Vec2(0, 0));
        _clipper->setPosition(Vec2(40.0f, 7.0f));
        this->addChild(_clipper);
        
        //bg2
        bg = ::createSpriteFrameWithScale("play_rect.png");
        bg->setAnchorPoint(Vec2(0.5f, 0.5f));
        bg->setPosition(Vec2(w/2, h/2));
        this->addChild(bg);
        //add line1
        auto line = ::createSpriteFrameWithScale("play_line.png");
        line->setPosition(Vec2(40.0f + DUR_OF_START_PLAY_NUM, h/2));
        this->addChild(line);
        
        _init = true;
    }else{
        _data->init(numId, playId);
    }
    
    _speed = speed;
    
    resetData();
    
    return true;
}


void NumContainer::start(){
    if(_isStart || _lock)
        return;
    this->schedule(CC_SCHEDULE_SELECTOR(NumContainer::moveNode), 0);
    _isStart = true;
}

void NumContainer::stop(){
    if(!_isStart || _lock)
        return;
    
    this->unschedule(CC_SCHEDULE_SELECTOR(NumContainer::moveNode));
    _isStart = false;
}

void NumContainer::reset(){
    if(_data == nullptr || _lock)
        return;
    
    stop();
    resetData();
    
}

void NumContainer::resetData(){
    _data->reset();
    
    _curBegin = 0;
    int numSize = _data->getNumSize();
    _curEnd = NUM_CONTAINER_MAX_NUM > numSize ? numSize-1 : NUM_CONTAINER_MAX_NUM-1;
    _curPlayIndex = -1;
    _nextPlayIndex = 0;
    
    int x = DUR_OF_START_PLAY_NUM;
    int type, num;
    for(int i = 0; i <= _curEnd; ++i){
        _data->next(num, type);
        
        NumberNode* node = _container[i];
        if(node == nullptr){
            node = NumberNode::create(i, type, num);
            _clipper->addChild(node);
            _container[i] = node;
            node->retain();
        }else{
            node->init(i, type, num);
        }
        node->setAnchorPoint(Vec2(0, 0));
        node->setPosition(x, 0);
        node->setPlayCallBack(_playTarget, _playCallBack);
        
        setNodeGuides(node);
        
        x += node->getContentSize().width;
    }
}

void NumContainer::addLine(){
    DrawNode* node = DrawNode::create();
    node->drawLine(Vec2::ZERO, Vec2(0, 50), Color4F::GREEN);
    node->setPosition(Vec2(DUR_OF_START_PLAY_NUM, 0));
    this->addChild(node);
}

void NumContainer::moveNode(float dt){
    int start = _curBegin;
    int end = _curBegin > _curEnd ? _curEnd + NUM_CONTAINER_MAX_NUM : _curEnd;
    
    for(int i = start; i <= end; ++i){
        int index = i % NUM_CONTAINER_MAX_NUM;
        auto node = _container[index];
        float x = node->getPositionX();
        float bordx = x + node->getContentSize().width * (1.0f-node->getAnchorPoint().x);
        //out of side
        if(bordx < 0){
            if(!setNextNum(index)){
                return;
            }
            x = node->getPositionX();
        }else if(_nextPlayIndex == index && bordx - node->getContentSize().width < DUR_OF_START_PLAY_NUM){
            if(_curPlayIndex >= 0){
                _container[_curPlayIndex]->setState(NumberNode::END);
            }
            _curPlayIndex = _nextPlayIndex;
            _container[_curPlayIndex]->play();
            _nextPlayIndex = (_nextPlayIndex + 1) % NUM_CONTAINER_MAX_NUM;
        }
        
        node->setPositionX(x - _speed*dt);
        
    }
}

bool NumContainer::setNextNum(int index){
    _curBegin = (index+1) % NUM_CONTAINER_MAX_NUM;
    int type, num;
    
    int i = _data->next(num, type);
    if( i >= 0){
        NumberNode* node = _container[index];
        node->init(i, type, num);
        
        int preIndex =(index + NUM_CONTAINER_MAX_NUM-1) % NUM_CONTAINER_MAX_NUM;
        NumberNode* preNode = _container[preIndex];
        int x = preNode->getPositionX() + preNode->getContentSize().width;
        node->setPositionX(x);
        setNodeGuides(node);
        
        _curEnd = index;
        return true;
    }
    
    int finalEnd = (_curEnd + 1) % NUM_CONTAINER_MAX_NUM;
    if(finalEnd != _curBegin){
        return true;
    }
    
    stop();
    if(_endTarget&&_endCallBack){
        (_endTarget->*_endCallBack)();
    }
    return false;
    
}

void NumContainer::setPlayCallBack(cocos2d::Ref *target, PLAY_CALLBACK handler){
    _playTarget = target;
    _playCallBack = handler;
    for(int i = 0; i < NUM_CONTAINER_MAX_NUM; ++i){
        auto p = _container[i];
        if(p != nullptr){
            p->setPlayCallBack(_playTarget, _playCallBack);
        }
    }
    
}

void NumContainer::setNumState(int index, int state){
    int pos = index % NUM_CONTAINER_MAX_NUM;
    auto node = _container[pos];
    if(node->getIndex() == index){
        node->setState(state);
    }
}

void NumContainer::clearContainer(){
    for(int i = 0; i < NUM_CONTAINER_MAX_NUM; ++i){
        auto p = _container[i];
        if(p != nullptr){
            _container[i]->release();
            _container[i] = nullptr;
        }
    }
}

void NumContainer::setShowGuide(bool flag){
    if(_showGuide == flag){
        return;
    }
    
    _showGuide = flag;
    for(int i = 0; i < NUM_CONTAINER_MAX_NUM; ++i){
        auto p = _container[i];
        setNodeGuides(p);
    }
}

void NumContainer::setNodeGuides(NumberNode *node){
//    if(!node){
//        return;
//    }
//    node->hideGuide();
//    
//    if(_showGuide){
//        node->setGuide("Will Play", NumberNode::PLAY, NumberNode::PRE);
//        node->setGuide("Now Play", NumberNode::PLAY, NumberNode::NOW);
//        node->setGuide("Play Success", NumberNode::PLAY, NumberNode::CORRECT);
//        node->setGuide("Play End", NumberNode::PLAY, NumberNode::WRONG);
//    }
    
    
}