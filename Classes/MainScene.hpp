//
//  MainScene.hpp
//  MyCppGame
//
//  Created by msempire on 16/9/1.
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include "cocos2d.h"
#include "BasicScene.hpp"

class MainScene: public BasicScene{
public:
    MainScene(){}
    
    CREATE_FUNC(MainScene);
    
    virtual void onEnter() override;
    
protected:
    bool init() override;
    
private:
    inline void startPlay();
    inline void openSelectLayer();
    
    void menuCallBack(cocos2d::Ref* pSender);
    
};
#endif /* MainScene_hpp */
