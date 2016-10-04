//
//  SceneControl.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/3.
//
//

#ifndef SceneControl_hpp
#define SceneControl_hpp

#include <vector>
#include "cocos2d.h"
#include "BasicScene.hpp"

class SceneControl{
public:
    enum ENTER_TYPE{
        ENTER_TYPE_PUSH,
        ENTER_TYPE_REPLACE,
        ENTER_TYPE_POP,
    };
    
    enum SCENE_ID{
        SCENE_ID_MAIN,
        SCENE_ID_CHAPTER,
        SCENE_ID_PLAY,
        SCENE_ID_SETTING,
    };
    
public:
    static SceneControl& getInstance(){
        static SceneControl instance;
        
        return instance;
    }
    
    void enterScene(int sid, int enterType);
    void stopSceneAct();
    void startSceneAct();
private:
    SceneControl():mCurSid(-1),mPCurSce(nullptr){}
    virtual ~SceneControl(){};
    SceneControl(const SceneControl&);
    const SceneControl& operator=(const SceneControl&);
    
    void opeSidVec(int sid, int enterType);
    BasicScene* getSceneById(int sid);


private:
    std::vector<int> mSidVec;
    int mCurSid;
    BasicScene* mPCurSce;
};

#endif /* SceneControl_hpp */
