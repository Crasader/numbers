//
//  GuideNode.hpp
//  MyCppGame
//
//  Created by msempire on 16/9/19.
//
//

#ifndef GuideNode_hpp
#define GuideNode_hpp

#include <stdio.h>
struct GuideNode{
    virtual bool guide() = 0;
    virtual void setGuideState(bool st) = 0;
};

#endif /* GuideNode_hpp */
