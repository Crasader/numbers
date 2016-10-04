//
//  BasicLayer.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/9.
//
//

#ifndef BasicLayer_hpp
#define BasicLayer_hpp

#include "cocos2d.h"

class BasicLayer:public cocos2d::Layer{
public:
    BasicLayer(){};
    virtual ~BasicLayer(){}
    
    CREATE_FUNC(BasicLayer);
    
};

#endif /* BasicLayer_hpp */
