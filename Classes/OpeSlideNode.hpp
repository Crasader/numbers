//
//  OpeSlideNode.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/19.
//
//

#ifndef OpeSlideNode_hpp
#define OpeSlideNode_hpp

#include "cocos2d.h"
#include "OpeNode.hpp"

class OpeSlideNode:public cocos2d::Node, public GuideNode{
public:
    enum Orientation{
        HORIZONTAL,
        VERTICAL,
    };
    OpeSlideNode(const cocos2d::Size& size, Orientation orien, OpeNode* slideNode);
    virtual ~OpeSlideNode();
    
    static OpeSlideNode* create(const cocos2d::Size& size, OpeNode* slideNode, Orientation orien = VERTICAL);
    static OpeSlideNode* create(const cocos2d:: Size& size, OpeNodeData* data, Orientation orien = VERTICAL) {
        return create(size, OpeNode::create(data), orien);
    }
    
    bool init();
    
    void startSlide();
    void slide(const cocos2d::Vec2& delta);
    void stopSlide();
    void resetSlide();
    
    bool pointInNode(const cocos2d::Vec2& worldPoint);
    void setOpeNodeCB(Ref* target, NODE_STATE_CHANGE_CALL_BACK handler){
        _slideNode->setStChangeCB(target, handler);
    }
    
    virtual bool guide();
    virtual void setGuideState(bool st);
    
private:
    Orientation _orien;
    OpeNode* _slideNode;
    
};

#endif /* OpeSlideNode_hpp */
