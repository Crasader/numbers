//
//  NumContainer.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/17.
//
//

#ifndef NumContainer_hpp
#define NumContainer_hpp

#include "cocos2d.h"
#include "math/Mat4.h"
#include "NumberNode.hpp"
#include "Numbers.hpp"


#define NUM_CONTAINER_MAX_NUM 11
#define DUR_OF_START_PLAY_NUM 150
typedef void(cocos2d::Ref::*END_CALLBACK)();

class NumberNode;
class NumContainer:public cocos2d::Node{
public:
    NumContainer();
    virtual ~NumContainer();
    
    static NumContainer* create(int numId, int playId, int speed);
    
    void start();
    void stop();
    void reset();
    
    
    bool init(int numId, int playId, int speed);
    
    bool isStart() const { return _isStart; }
    
    void setEndCallBack(cocos2d::Ref* target, END_CALLBACK handler){
        _endTarget = target;
        _endCallBack = handler;
    }
    void setPlayCallBack(cocos2d::Ref* target, PLAY_CALLBACK handler);
    
    void setNumState(int index, int state);
    
    int getNextPlayNum(int index){
        return _data->getNextPlayNum(index);
    }
    
    void setShowGuide(bool flag);
    
    void lock(){ _lock = true; }
    void unlock(){ _lock = false; }
    
//    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags);
    
private:
    
    void moveNode(float dt);
    
    bool setNextNum(int index);
    
    void clearContainer();
    void resetData();
    void addLine();
    
    void setNodeGuides(NumberNode* node);
    
    void initView();
    
private:
    bool _isStart;
    NumberNode* _container[NUM_CONTAINER_MAX_NUM];
    Numbers* _data;
    cocos2d::ClippingRectangleNode* _clipper;
    int _curEnd;
    int _curBegin;
    int _curPlayIndex;
    int _nextPlayIndex;
    bool _showGuide;
    bool _init;
    
    bool _lock;
    
    
    cocos2d::Ref* _endTarget;
    END_CALLBACK _endCallBack;
    cocos2d::Ref* _playTarget;
    PLAY_CALLBACK _playCallBack;
    
    float _speed;
    
    
};

#endif /* NumContainer_hpp */
