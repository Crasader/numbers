//
//  OpeClickCtner2.hpp
//  MyCppGame
//
//  Created by msempire on 16/9/28.
//
//

#ifndef OpeClickCtner2_hpp
#define OpeClickCtner2_hpp

#include "cocos2d.h"
#include "OpeCtner.hpp"

class OpeClickCtner2:public OpeCtner{
public:
    OpeClickCtner2();
    bool init(const std::vector<OpeNodeData*>& nodeTypes);
    
    static OpeClickCtner2* create(const std::vector<OpeNodeData*>& nodeTypes);
    
    void setOpeNodeCB(Ref* target, NODE_STATE_CHANGE_CALL_BACK handler);
    
    void resetNodeState();
private:
    Ref* _target;
    NODE_STATE_CHANGE_CALL_BACK _handler;
};

#endif /* OpeClickCtner2_hpp */
