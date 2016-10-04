//
//  PlayScene.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/9.
//
//

#ifndef PlayScene_hpp
#define PlayScene_hpp

#include "BasicScene.hpp"
#include "OpeNode.hpp"
#include "OpeClickCtner2.hpp"
#include "DataControl.hpp"
#include "GuideControl.hpp"
#include "NumContainer.hpp"
#include "cocos2d.h"

typedef std::pair<OpeNodeData*, bool> OpeDataFlag;

class PlayScene:public BasicScene{
public:
    PlayScene(int chapterId);
    virtual ~PlayScene();
    
    static PlayScene* create(int chapterId);
    
    void setCurNum(int index, int num);
    void updateOpeNum();
    void updateCurNum();
    void updateScore();
    bool checkPlayState();
    
    void opeNodeChange(const OpeNodeData* data, int st, bool flag);
    
    void stopAct() override { stop(nullptr); }
    
//    void startAct() override { stop(nullptr); }
    
    virtual void guideCallBack(cocos2d::Rect tag) override;
private:
    enum{
        START,
        STOP,
        END,
    };
    
    bool init(bool first);
    void initView();
    
    void close(cocos2d::Ref* pSender);
    void stop(cocos2d::Ref* pSender);
    void resume(cocos2d::Ref* pSender);
    void restart(cocos2d::Ref* pSender);
    void end();
    void next(cocos2d::Ref* pSender);
    
    void addStopLayer(int);
    void hideStopLayer();
    
    void startGuide();
    
    void initData();
    void resetData();
    void clearData();
    void saveData();
    
    
    void updateLabel(cocos2d::Label** plabel, const std::string& text, int fontSize, const cocos2d::Vec2& pos);
    void addMenuOnStop(int, const cocos2d::Size&);
    void addLabelOnStop(int, const cocos2d::Size&);
    
    void guideLabel(cocos2d::Label* label);
    void guideOpe(int num);
    
    
    std::vector<OpeNodeData*> getVecOfData(const std::vector<OpeDataFlag>& data);
   
private:
    int _chapterId;
    int _curPlayNum;
    int _curPlayIndex;
    int _curOpeSum;
    int _score;
    int _miss;
    ChapterData _chapterData;
    
    bool _isStop;
    bool _isNewPlayGuide;
    
    std::string _curOpeNumStr;
    
    NumContainer* _numContainer;
    OpeClickCtner2* _opeContainer;
    
    cocos2d::Label* _numLabel;
    cocos2d::Label* _sumLabel;
    cocos2d::Label* _scoreLabel;
    cocos2d::Label* _missLabel;
    
    std::vector<OpeDataFlag> _opeData;
    std::stack<int> _guideStack;
    
    std::vector<int> _sumVec;
    int _preGuideIndex;
    
    GuideControl _guide;
};

#endif /* PlayScene_hpp */
