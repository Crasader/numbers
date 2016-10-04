//
//  NumberNode.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/9.
//
//

#ifndef NumberNode_hpp
#define NumberNode_hpp

#include "cocos2d.h"
typedef void(cocos2d::Ref::*PLAY_CALLBACK)(int index, int num);

class NumberNode: public cocos2d::Node{
public:
    enum Type{
        UNINITIAL,
        PLAY,
        DISPLAY,
        DELAY,
        
    };
    enum State{
        PRE,
        NOW,
        CORRECT,
        WRONG,
        END,
        NORMAL,
    };
    
    NumberNode();
    virtual ~NumberNode(){};
    
    static NumberNode* create(int index, int type, int num);
    
    bool init(int index, int type,int num);
    
    int getIndex(){ return _index; }
    void setState(int st);
    
    void play();
    
    void setPlayCallBack(cocos2d::Ref* target, PLAY_CALLBACK handler){
        _target = target;
        _callBack = handler;
    }
    
    void setGuide(const std::string& guideStr, Type type, State state);
    void showGuide();
    void hideGuide();
    
private:
    
    bool updateView();
    void updateStateView();
    void playSound();
    
    int _index;
    int _type;
    int _num;
    int _state;
    cocos2d::Ref* _target;
    PLAY_CALLBACK _callBack;
    
    std::string _guideArr[NORMAL];
    
};

#endif /* NumberNode_hpp */
