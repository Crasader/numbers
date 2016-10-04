//
//  GuideLayer.hpp
//  MyCppGame
//
//  Created by msempire on 16/9/28.
//
//

#ifndef GuideLayer_hpp
#define GuideLayer_hpp

#include "cocos2d.h"

#define GUIDE_TOUCH_GUSTOM_EVENT_NAME "GUIDE_TOUCH_GUSTOM_EVENT_NAME"

class GuideLayer:public cocos2d::Layer{
public:
    GuideLayer();
    CREATE_FUNC(GuideLayer);
    
    void addVisibleRect(const cocos2d::Rect& rect);
    void addGuideStr(const std::string& text, cocos2d::Vec2 pos);
    void removeAllRect();
    
protected:
    bool init();
private:
    Node* _stencil;
    cocos2d::ClippingNode* _clipper;
    
};

#endif /* GuideLayer_hpp */
