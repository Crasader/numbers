//
//  GuideControl.hpp
//  MyCppGame
//
//  Created by msempire on 16/9/29.
//
//

#ifndef GuideControl_hpp
#define GuideControl_hpp

#include "cocos2d.h"
#include "BasicScene.hpp"

typedef std::pair<std::string, cocos2d::Rect> GuideStep;
#define GUIDE_COMMANDS_ADD_RECT "ADD_RECT"
#define GUIDE_COMMANDS_ADD_STR ""
#define GUIDE_COMMANDS_WAIT_TOUCH "WAIT_FOR_TOUCH"
#define GUIDE_COMMANDS_CLEAR_RECT "CLEAR_RECT"
#define GUIDE_COMMANDS_REMOVE_GUIDE "REMOVE_GUIDE"
#define GUIDE_COMMANDS_CALL_BACK "CALL_BACK"

class GuideControl{
public:
    GuideControl();
    virtual ~GuideControl();
    void setGuideContainer(BasicScene* p){
        _container = p;
    }
    void addGuideStep(const std::string& command, const cocos2d::Rect& r);
    void startGuide();
    
    void reset(){
        _curStep = -1;
        _stepVec.clear();
    }
    
    void setStep(int step){
        _curStep = step;
    }
    
    void setLayerShow(bool flag);
protected:
    void onTouchCallBack(cocos2d::EventCustom* evt);
    bool guideNext();
    
private:
    std::vector<GuideStep> _stepVec;
    int _curStep;
    BasicScene* _container;
    cocos2d::EventListenerCustom* _listener;
};

#endif /* GuideControl_hpp */
