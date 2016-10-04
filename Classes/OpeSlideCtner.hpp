//
//  OpeSlideCtner.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/18.
//
//

#ifndef OpeSlideCtner_hpp
#define OpeSlideCtner_hpp

#include "cocos2d.h"
#include "OpeCtner.hpp"

class OpeSlideCtner:public OpeCtner{
public:
    OpeSlideCtner();
    bool init(const cocos2d::Size& size,const std::vector<OpeNodeData*>& nodeTypes);
    
    static OpeSlideCtner* create(const cocos2d::Size& size, const std::vector<OpeNodeData*>& nodeTypes);
    
    void setOpeNodeCB(Ref* target, NODE_STATE_CHANGE_CALL_BACK handler);
    
    void resetNodeState();
    
private:
    
};

#endif /* OpeSlideCtner_hpp */
