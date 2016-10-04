//
//  ChapterItem.hpp
//  MyCppGame
//
//  Created by msempire on 16/9/2.
//
//

#ifndef ChapterItem_hpp
#define ChapterItem_hpp

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class ChapterData;
class ChapterItem:public cocos2d::ui::Widget{
public:
    ChapterItem();
    
    static ChapterItem* create(const ChapterData& data);
    
    int getChapterId() { return _id; }
    
    void setSelected(bool selected);
protected:
    bool init(const ChapterData& data);
    
private:
    int _id;
};

#endif /* ChapterItem_hpp */
