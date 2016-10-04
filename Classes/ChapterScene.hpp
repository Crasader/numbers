//
//  ChapterScene.hpp
//  MyCppGame
//
//  Created by msempire on 16/9/1.
//
//

#ifndef ChapterScene_hpp
#define ChapterScene_hpp

#include "BasicScene.hpp"
#include "ui/CocosGUI.h"
class ChapterScene:public BasicScene{
public:
    ChapterScene();
    CREATE_FUNC(ChapterScene);
    
protected:
    bool init() override;
    
private:
    void onItemClick(Ref* target);
    void startChapter(Ref* pSender);
    
    int _curSelId;
    cocos2d::ui::ListView* _list;
};

#endif /* ChapterScene_hpp */
