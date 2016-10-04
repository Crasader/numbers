//
//  OpeClickNode.hpp
//  MyCppGame
//
//  Created by msempire on 16/9/27.
//
//

#ifndef OpeClickNode_hpp
#define OpeClickNode_hpp

#include "cocos2d.h"
#include "OpeNode.hpp"

class OpeClickNode:public cocos2d::Node, public GuideNode{
public:
    OpeClickNode(OpeNodeData* data);
    virtual ~OpeClickNode();
    static OpeClickNode* create(OpeNodeData* data);
    
    void setState(OpeNode::STATE state, bool flag);
    void resetState();
    void switchState(OpeNode::STATE st);
    
    bool pointInNode(const cocos2d::Vec2& worldPoint);
    void setStChangeCB(Ref* target, NODE_STATE_CHANGE_CALL_BACK handler){
        _clickNode->setStChangeCB(target, handler);
    }
    
    virtual bool guide();
    virtual void setGuideState(bool st);
    
protected:
    bool init();
    
private:
    OpeNode* _clickNode;
};

#endif /* OpeClickNode_hpp */
