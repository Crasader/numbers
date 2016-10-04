//
//  SettingLayer.hpp
//  MyCppGame
//
//  Created by msempire on 16/9/1.
//
//

#ifndef SettingLayer_hpp
#define SettingLayer_hpp

#include "cocos2d.h"
#include "OpeNode.hpp"
class SettingLayer:public cocos2d::Layer{
public:
    CREATE_FUNC(SettingLayer);
    SettingLayer(){}
    
//    virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) override;
    
protected:
    bool init() override;
    void btnClick(Ref* pSend);
    
private:
    void initSoundNode();
    OpeNode* _soundNode;
};

#endif /* SettingLayer_hpp */
