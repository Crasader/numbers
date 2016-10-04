//
//  OpeNode.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/18.
//
//

#ifndef OpeNode_hpp
#define OpeNode_hpp
#include "cocos2d.h"
#include "OpeNodeData.hpp"
#include "GuideNode.hpp"



typedef void(cocos2d::Ref::*NODE_STATE_CHANGE_CALL_BACK)(const OpeNodeData* data,int st, bool flag);

class OpeNode:public cocos2d::Node, public GuideNode{
public:
    
    enum STATE{
        STATE_ON,
        STATE_SELECT,
        STATE_GUIDE,
    };
    OpeNode(OpeNodeData* data, float r):_data(data), _st(0), _r(r), _target(nullptr), _handler(nullptr), _guideSt(NO_GUIDE){}
    bool init();
    void setState(STATE state, bool flag);
    bool getState(STATE state);
    void resetState(){
        setState(STATE_ON, false);
        setState(STATE_SELECT, false);
        setState(STATE_GUIDE, false);
    }
    void switchState(STATE state){
        setState(state, !getState(state));
    }
    
    void setStChangeCB(Ref* target, NODE_STATE_CHANGE_CALL_BACK handler){
        _target = target;
        _handler = handler;
    }
    
    void setLabelStr(const std::string& str);
    
    static OpeNode* create(OpeNodeData* data, float r = 20.0f);
    
    bool guide();
    void setGuideState(bool st);
    
    
private:
    std::string getDisplayStr();
private:
    OpeNodeData* _data;
    int _st;
    float _r;
    
    
    Ref* _target;
    NODE_STATE_CHANGE_CALL_BACK _handler;
    
    enum{
        NO_GUIDE = -1,
        GUIDE_NOT_CHANGE,
        GUIDE_CHANGE_NOT_NOW,
        GUIDE_CHANGE_NOW,
    };
    int _guideSt; //-1:no guide; 0:guide --not change; 1:guide --change but not now;2:guide --change now
};

#endif /* OpeNode_hpp */
