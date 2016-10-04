//
//  OpeClickCtner.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/19.
//
//

#ifndef OpeClickCtner_hpp
#define OpeClickCtner_hpp
#include "cocos2d.h"
#include "OpeCtner.hpp"

class OpeClickCtner:public OpeCtner{
public:
    OpeClickCtner();
    bool init(const cocos2d::Size& size,const std::vector<OpeNodeData*>& nodeTypes);
    
    static OpeClickCtner* create(const cocos2d::Size& size, const std::vector<OpeNodeData*>& nodeTypes);
    
    void setOpeNodeCB(Ref* target, NODE_STATE_CHANGE_CALL_BACK handler);
    
    void resetNodeState();
    
private:
    void initPosArr();
    int getStartIndex(int num);
    cocos2d::Vec2 getPosByIndex(int index, const cocos2d::Size& size, const cocos2d::Size& nodeSize);
    std::vector<cocos2d::Rect> _posArr;
    
};

#endif /* OpeClickCtner_hpp */
