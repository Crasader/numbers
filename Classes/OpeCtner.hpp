//
//  OpeCtner.hpp
//  MyCppGame
//
//  Created by msempire on 16/9/18.
//
//

#ifndef OpeCtner_hpp
#define OpeCtner_hpp

#include <stdio.h>
#include "cocos2d.h"
#include "OpeNode.hpp"

class OpeCtner:public cocos2d::Node{
public:
    OpeCtner():guideIndex(-1){}
    virtual void setOpeNodeCB(Ref* target, NODE_STATE_CHANGE_CALL_BACK handle)=0;
    
    virtual void resetNodeState()=0;
    
    void guideData(const std::vector<bool>& data);
    
    bool guideNext();
private:
    int guideIndex;
};

#endif /* OpeCtner_hpp */
