//
//  BasicScene.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/3.
//
//

#ifndef BasicScene_hpp
#define BasicScene_hpp

#include "cocos2d.h"
#include "BasicLayer.hpp"

class BasicScene: public cocos2d::Scene{
public:
    enum BASIC_LAYER_INDEX{
        BG_LAYER_INDEX = 0,
        OPE_LAYER_INDEX,
        EFFECT_LAYER_INDEX,
        TIP_LAYER_INDEX,
        SETTING_LAYER_INDEX,
        GUIDE_LAYER_INDEX,
        LAYER_END,
    };
public:
    BasicScene();
    virtual ~BasicScene(){}
    virtual void stopAct(){};
    virtual void startAct(){};
    bool addToLayer(cocos2d::Node* child, int layerIndex, bool clear);
    Node* getLayer(int layerIndex){
        if(layerIndex < 0 || layerIndex >= LAYER_END)
            return nullptr;
        return _layerVec[layerIndex];
    }
    
    virtual void guideCallBack(cocos2d::Rect tag){}
    
protected:
    bool init() override;
    void initBg();
    void showSettingLayer(Ref* pSender);
    
    int _layerNum;
    std::string _bgName;
    cocos2d::MenuItemSprite* _settingBtn;
    
private:
    bool initLayer();
    
    std::vector<BasicLayer*> _layerVec;
};

#endif /* BasicScene_hpp */
