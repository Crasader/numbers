//
//  PlayScene.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/9.
//
//

#include "PlayScene.hpp"
#include "NumContainer.hpp"
#include "GameControl.hpp"
#include "GameUtil.hpp"
#include "OpeSlideCtner.hpp"
#include "Constants.h"
#include "StrConstants.h"
#include "OpeClickCtner.hpp"
#include "DataControl.hpp"
#include "GuideControl.hpp"
#include "SceneControl.hpp"
#include "SoundControl.hpp"
#include <string>
#include <math.h>
USING_NS_CC;
using namespace std;

enum GUIDE_ACTION_TAG{
    GUIDE_ACTION_STOP,
    GUIDE_ACTION_START,
};


PlayScene::PlayScene(int chapterId):
_chapterId(chapterId)
,_numLabel(nullptr)
,_sumLabel(nullptr)
,_curPlayNum(INVALIDATE_NUMBER)
,_curPlayIndex(-1)
,_curOpeSum(0)
,_score(0)
,_miss(0)
,_scoreLabel(nullptr)
,_missLabel(nullptr)
,_isStop(false)
,_isNewPlayGuide(false)
,_preGuideIndex(0)
{
    
}

PlayScene::~PlayScene(){
    clearData();
}

PlayScene* PlayScene::create(int chapterId){
    PlayScene* ret = new PlayScene(chapterId);
    if(ret && ret->init(true)){
        ret->autorelease();
        return ret;
    }
    
    delete ret;
    return nullptr;
}

bool PlayScene::init(bool first){
    if(!BasicScene::init()){
        return false;
    }
    
    if(_chapterId < 0){
        return false;
    }
    
    initData();
    if(first){
        initView();
    }else{
        _numContainer->init(_chapterData.numId, _chapterData.playId, _chapterData.speed);
        _opeContainer->init(getVecOfData(_opeData));
    }
    
    resetData();
    
    _settingBtn->setVisible(false);
    
    
    addStopLayer(START);
    if(_isNewPlayGuide){
        startGuide();
    }
    
    
    
    return true;
}

void PlayScene::initView(){
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //stop btn
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addToLayer(menu, OPE_LAYER_INDEX, false);
    
    auto menuItem = ::createMenuItemWithScale("btn_stop.png", "btn_stop2.png", CC_CALLBACK_1(PlayScene::stop, this));
    menuItem->setPosition(visibleSize.width-menuItem->getContentSize().width*menuItem->getScaleX()/2-10,
                          visibleSize.height-menuItem->getContentSize().height*menuItem->getScaleY()/2-10);
    menu->addChild(menuItem);
    
    
    
    _numContainer = NumContainer::create(_chapterData.numId, _chapterData.playId, _chapterData.speed);
    _numContainer->setPlayCallBack(this, static_cast<PLAY_CALLBACK>(&PlayScene::setCurNum));
    _numContainer->setEndCallBack(this, static_cast<END_CALLBACK>(&PlayScene::end));
    _numContainer->setPosition(Vec2(0, 185.0f));
    this->addToLayer(_numContainer, OPE_LAYER_INDEX, false);
    
    _opeContainer = OpeClickCtner2::create(getVecOfData(_opeData));
    _opeContainer->setAnchorPoint(Vec2(0.5f, 0));
    _opeContainer->setPosition(visibleSize.width/2, 20.0f);
    _opeContainer->setOpeNodeCB(this, static_cast<NODE_STATE_CHANGE_CALL_BACK>(&PlayScene::opeNodeChange));
    this->addToLayer(_opeContainer, OPE_LAYER_INDEX, false);
    
}

void PlayScene::initData(){
    clearData();
    
    _chapterData = DataControl::getInstance().getChapterById(_chapterId);
    _isNewPlayGuide = _chapterData.type == ChapterData::TYPE_NEW_PLAYER;
    
    const string& s = _chapterData.opeSet;
    
    auto iter = s.begin();
    OpeNodeData* p = nullptr;
    vector<OpeDataFlag>* store = nullptr;
    int num = 0;
    for(; iter != s.end(); ++iter){
        char c = *iter;
        if(c == '|'){
            if(store){
                if(!p){
                    p = new OpeNegtiveData(0);
                }
                p->mNum = num;
                store->push_back(OpeDataFlag(p, false));
            }
            p = nullptr;
            store = nullptr;
            num = 0;
        }else if(c == '*'){
            p = new OpeMultiData(0);
            store = &_opeData;
        }else if(c == '-'){
            store = &_opeData;
        }else if(c >= '0' && c <= '9'){
            if(!store){
                store = &_opeData;
                p = new OpeNumData(0);
            }else if(!p){
                p = new OpeMinData(0);
            }
            num = num * 10 + (c - '0');
        }
    }
    _guide.setGuideContainer(this);
    _sumVec.clear();
}

void PlayScene::resetData(){
    _numContainer->reset();
    _opeContainer->resetNodeState();
    
    _curPlayNum = INVALIDATE_NUMBER;
    _curPlayIndex = -1;
    _score = 0;
    _miss = 0;
    _preGuideIndex = 0;
    
    
    updateCurNum();
    updateOpeNum();
    updateScore();
    
}

void PlayScene::clearData(){
    function<void(OpeDataFlag&)> func = [](OpeDataFlag& p){
        if(p.first){
            delete p.first;
            p.first = nullptr;
            p.second = false;
        }
    };
    for_each(_opeData.begin(), _opeData.end(), func);
    _opeData.clear();
}

void PlayScene::saveData(){
    if(_score < _chapterData.userScore){
        return;
    }
    if(_score == _chapterData.userScore && _miss < _chapterData.userMiss){
        return;
    }
    
    _chapterData.userScore = _score;
    _chapterData.userMiss = _miss;
    DataControl::getInstance().saveChapterScore(_chapterId, _score, _miss);
}

void PlayScene::close(cocos2d::Ref *pSender){
    GameControl::getInstance().endGame();
}

void PlayScene::restart(cocos2d::Ref *pSender){
    //reset data
    resetData();
    
    resume(pSender);
}

void PlayScene::resume(cocos2d::Ref *pSender){
    //hide Layer
    hideStopLayer();
    
    //resume or start
    _numContainer->start();
    
}

void PlayScene::stop(cocos2d::Ref *pSender){
    //stop
    _numContainer->stop();
    
    //add layer
    addStopLayer(STOP);
    
    
    
}

void PlayScene::end(){
    _numContainer->stop();
    addStopLayer(END);
}

void PlayScene::next(cocos2d::Ref *pSender){
    _chapterId = DataControl::getInstance().getNextChapterId(_chapterId);
    GameControl::getInstance().setCurChapterId(_chapterId);
    hideStopLayer();
    this->init(false);
}

void PlayScene::addStopLayer(int way){
    if(_isStop){
        return;
    }
    
    saveData();
    
    Size screenSize = Size(480, 320);
    LayerColor* bg = LayerColor::create(Color4B::BLACK, screenSize.width, screenSize.height);
    this->addToLayer(bg, TIP_LAYER_INDEX, false);
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [](Touch* touch, Event* evet){
        return true;
    };
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, bg);
    
    addMenuOnStop(way, screenSize);
    
    addLabelOnStop(way, screenSize);
    
    _settingBtn->setVisible(true);
    _isStop = true;
    
    //pause music
    SoundControl::getInstance()->pauseBgMusic();
    
    if(_isNewPlayGuide){
        _guide.setLayerShow(false);
    }
    
}

void PlayScene::addLabelOnStop(int way, const cocos2d::Size & screenSize){
    
    auto nameLabel = Label::createWithTTF(_chapterData.name, "fonts/Time Roman Italic.ttf", 24);
//    nameLabel->setScale(x);
    nameLabel->setPosition(screenSize.width/2, 245.0f);
    this->addToLayer(nameLabel, TIP_LAYER_INDEX, false);
    
    nameLabel = Label::createWithTTF(string(STR_PASS_SCORE) + ::uIntToStr(_chapterData.passScore), "fonts/Time Roman Italic.ttf", 10);
//    nameLabel->setScale(x);
    nameLabel->setAnchorPoint(Vec2(0, 0.5f));
    nameLabel->setPosition(60.0f, 200.0f);
    this->addToLayer(nameLabel, TIP_LAYER_INDEX, false);
    
    
    string scoreStr;
    switch (way) {
        case START:
            scoreStr = STR_STORE_TIP
            +  string(STR_STORE_SCORE) + ::uIntToStr(_chapterData.userScore)
            +  string(STR_STORE_MISS) + ::uIntToStr(_chapterData.userMiss);
            break;
        case STOP:
            scoreStr = STR_CUR_TIP
            +  string(STR_STORE_SCORE) + ::uIntToStr(_score)
            +  string(STR_STORE_MISS) + ::uIntToStr(_miss);
            break;
        case END:
            scoreStr = STR_STORE_SCORE + ::uIntToStr(_score) + STR_STORE_MISS + ::uIntToStr(_miss);
            break;
    }
    
    auto scoreLabel = Label::createWithTTF(scoreStr, "fonts/Time Roman Italic.ttf", 10);
//    scoreLabel->setScale(x);
    scoreLabel->setAnchorPoint(Vec2(0, 0.5f));
    scoreLabel->setPosition(60.0f, 170.0f);
    this->addToLayer(scoreLabel, TIP_LAYER_INDEX, false);
    
}

void PlayScene::addMenuOnStop(int way, const Size& screenSize){
    
    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addToLayer(menu, TIP_LAYER_INDEX, false);
    
    float h = 77.0f;
    
    if(way == START){
        auto menuItem = ::createMenuItemWithScale("btn_start.png", "btn_start2.png", CC_CALLBACK_1(PlayScene::resume, this));
        menuItem->setPosition(Vec2(240.0f, h));
        menu->addChild(menuItem);
    }
    
    else if(way == STOP){
        auto menuItem = ::createMenuItemWithScale("btn_start.png", "btn_start2.png", CC_CALLBACK_1(PlayScene::resume, this));
        menuItem->setPosition(Vec2(100.0f, h));
        menu->addChild(menuItem);
        
        menuItem = ::createMenuItemWithScale("btn_restart.png", "btn_restart2.png", CC_CALLBACK_1(PlayScene::restart, this));
        menuItem->setPosition(Vec2(380.0f, h));
        menu->addChild(menuItem);
    }
    
    else if(way == END){
        auto menuItem = ::createMenuItemWithScale("btn_restart.png", "btn_restart2.png", CC_CALLBACK_1(PlayScene::restart, this));
        menu->addChild(menuItem);
        
        if(_score >= _chapterData.passScore){
            menuItem->setPosition(Vec2(100.0f, h));
            
            menuItem = ::createMenuItemWithScale("btn_next.png", "btn_next2.png", CC_CALLBACK_1(PlayScene::next, this));
            menuItem->setPosition(Vec2(380.0f, h));
            menu->addChild(menuItem);
        }else{
            menuItem->setPosition(Vec2(240.0f, h));
        }
    }
    
    
}

void PlayScene::hideStopLayer(){
    this->addToLayer(nullptr, TIP_LAYER_INDEX, true);
    _isStop = false;
    _settingBtn->setVisible(false);
    
    //resume music
    SoundControl::getInstance()->resumeBgMusic();
    
    if(_isNewPlayGuide){
        _guide.setLayerShow(true);
    }
}


void PlayScene::setCurNum(int index, int num){
    
    if(_curPlayIndex == index){
        return;
    }
    
    if(_curPlayNum != INVALIDATE_NUMBER){
        _numContainer->setNumState(_curPlayIndex, NumberNode::WRONG);
        _miss++;
        updateScore();
        if(_chapterData.type == ChapterData::TYPE_INFINITE){
            end();
            return;
        }
        
    }
    
    _curPlayIndex = index;
    _curPlayNum = num;
    
    checkPlayState();
    
    updateCurNum();
    
    //guide
    if(_curPlayNum != INVALIDATE_NUMBER){
        guideOpe(num);
    }
}



void PlayScene::updateOpeNum(){

    updateLabel(&_sumLabel, STR_CUR_OPE_NUM + _curOpeNumStr, 10, Vec2(30, 140));
    if(_isNewPlayGuide){
        guideLabel(_sumLabel);
    }

    if(checkPlayState()){
        updateCurNum();
    }
}

bool PlayScene::checkPlayState(){
    if(_curOpeSum == _curPlayNum){
        _numContainer->setNumState(_curPlayIndex, NumberNode::CORRECT);
        _curPlayNum = INVALIDATE_NUMBER;
        _score++;
        updateScore();
        return true;
    }
    
    return false;
}

void PlayScene::updateCurNum(){
    string s;
    if(_curPlayNum == INVALIDATE_NUMBER){
        s = STR_NEXT_PLAY_NUM;
        int next = _numContainer->getNextPlayNum(_curPlayIndex);
        if(next == INVALIDATE_NUMBER){
            s += "END";
        }else if( next == DELAY_NUMBER){
            s += "wait";
        }else{
            s += ::uIntToStr(next);
        }
        
    }else{
        s = STR_CUR_PLAY_NUM + ::uIntToStr(_curPlayNum);
    }
    
    updateLabel(&_numLabel, s, 10, Vec2(30, 165));
    
    if(_isNewPlayGuide){
        guideLabel(_numLabel);
    }
}

void PlayScene::updateScore(){
    
    updateLabel(&_scoreLabel, STR_CUR_SCORE_NUM + ::uIntToStr(_score), 8, Vec2(30, 290));
    
    updateLabel(&_missLabel, STR_CUR_MISS_NUM + ::uIntToStr(_miss), 8, Vec2(160, 290));
    
}

void PlayScene::updateLabel(cocos2d::Label** plabel, const std::string& text, int fontSize, const cocos2d::Vec2& pos){
    auto label = *plabel;
    if(label == nullptr){
        *plabel = Label::createWithTTF(text, "fonts/Time Roman Italic.ttf", fontSize);
        (*plabel)->setAnchorPoint(Vec2(0, 0.5f));
        (*plabel)->setPosition(pos);
//        (*plabel)->setScale(Director::getInstance()->getOpenGLView()->getScaleX());
        this->addToLayer(*plabel, OPE_LAYER_INDEX, false);
        return;
    }
    
    if(label->getString().compare(text) != 0){
        label->stopAllActions();
        label->setColor(Color3B::WHITE);
        label->setString(text);
    }

}



void PlayScene::opeNodeChange(const OpeNodeData* data,int st, bool flag){
    
    if(st != OpeNode::STATE_ON){
        return;
    }
    _curOpeSum = 0;
    _curOpeNumStr.clear();
    
    for(auto iter = _opeData.begin(); iter != _opeData.end(); ++iter){
        auto p = iter->first;
        if(p == data){
            iter->second = flag;
        }
        if(iter->second){
            _curOpeSum = p->ope(_curOpeSum);
            p->addOpeStr(_curOpeNumStr);
        }
    }
    
    _curOpeNumStr += "=" + ::uIntToStr(_curOpeSum);
    updateOpeNum();
    
    
}

vector<OpeNodeData*> PlayScene::getVecOfData(const vector<OpeDataFlag> &data){
    vector<OpeNodeData*> result;
    auto iter = data.begin();
    for(; iter != data.end(); ++iter){
        result.push_back(iter->first);
    }
    
    return result;
}

void PlayScene::startGuide(){
    Rect r(202.0f, 39.0f, 76.0f, 76.0f);
    _guide.addGuideStep(GUIDE_COMMANDS_ADD_RECT, r);
    _guide.addGuideStep(GUIDE_LABEL_1, Rect(240.0f, 170.0f, 0, 0));
    _guide.addGuideStep(GUIDE_COMMANDS_WAIT_TOUCH, r);
    _guide.addGuideStep(GUIDE_COMMANDS_REMOVE_GUIDE, Rect::ZERO);
    
    _guide.addGuideStep(GUIDE_COMMANDS_CALL_BACK, Rect(GUIDE_ACTION_STOP, 0, 0, 0));
    
    _guide.addGuideStep(GUIDE_COMMANDS_ADD_RECT, Rect(30.0f, 125.0f, 160.0f, 55.0f));
    _guide.addGuideStep(GUIDE_LABEL_2, Rect(240.0f, 240.0f, 0, 0));
    _guide.addGuideStep(GUIDE_COMMANDS_WAIT_TOUCH, Rect::ZERO);
    _guide.addGuideStep(GUIDE_COMMANDS_CLEAR_RECT, Rect::ZERO);
    
    _guide.addGuideStep(GUIDE_COMMANDS_ADD_RECT, Rect(30.0f, 150.0f, 160.0f, 30.0f));
    _guide.addGuideStep(GUIDE_COMMANDS_ADD_RECT, Rect(190.0f, 185.0f, 50.0f, 80.0f));
    _guide.addGuideStep(GUIDE_LABEL_3, Rect(240.0f, 90.0f, 0, 0));
    _guide.addGuideStep(GUIDE_COMMANDS_WAIT_TOUCH, Rect::ZERO);
    _guide.addGuideStep(GUIDE_COMMANDS_CLEAR_RECT, Rect::ZERO);
    
    _guide.addGuideStep(GUIDE_COMMANDS_ADD_RECT, Rect(30.0f, 125.0, 160.0f, 30.0f));
    _guide.addGuideStep(GUIDE_COMMANDS_ADD_RECT, Rect(50.0f, 20.0f, 380.0f, 105.0f));
    _guide.addGuideStep(GUIDE_LABEL_4, Rect(240.0f, 240.0f, 0, 0));
    _guide.addGuideStep(GUIDE_COMMANDS_WAIT_TOUCH, Rect::ZERO);
    _guide.addGuideStep(GUIDE_COMMANDS_REMOVE_GUIDE, Rect::ZERO);
    
    _guide.addGuideStep(GUIDE_COMMANDS_CALL_BACK, Rect(GUIDE_ACTION_START, 0, 0, 0));
    
//    r = Rect(70.0f, 20.0f, 40.0f, 105.f);
//    _guide.addGuideStep(GUIDE_COMMANDS_ADD_RECT, r);
//    _guide.addGuideStep("click the circle to change the 'ope_num'", Rect(240.0f, 240.0f, 0, 0));
//    _guide.addGuideStep(GUIDE_COMMANDS_WAIT_TOUCH, r);
//    _guide.addGuideStep(GUIDE_COMMANDS_REMOVE_GUIDE, Rect::ZERO);
    
    _guide.startGuide();
    
    
}

void PlayScene::guideLabel(cocos2d::Label *label){
    if(!label){
        return;
    }
    
    Color3B color = Color3B::GREEN;
    auto animate1 = TintTo::create(0.5f, color.r, color.g, color.b);
//    auto animate2 = Blink::create(1, 3);
    color = Color3B::WHITE;
    auto animate3 = TintTo::create(0.5f, color.r, color.g, color.b);
    auto action1 = Sequence::create(animate1, animate3, NULL);
//    auto action = Spawn::create(action1, animate2, NULL);
    label->runAction(action1);
}


void PlayScene::guideCallBack(cocos2d::Rect tag){
    int action = static_cast<int>(tag.origin.x);
    switch (action) {
        case GUIDE_ACTION_STOP:
            _numContainer->stop();
            _numContainer->lock();
            break;
        case GUIDE_ACTION_START:
            _numContainer->unlock();
            _numContainer->start();
            break;
            
        default:
            break;
    }
}

void PlayScene::guideOpe(int num){
    if(!_isNewPlayGuide){
        return;
    }
    
//    _numContainer->stop();
    _guide.addGuideStep(GUIDE_COMMANDS_CALL_BACK, Rect(GUIDE_ACTION_STOP, 0, 0, 0));
    
    
    //if not init, then init
    if(_sumVec.size() == 0){
        //show the first number to play
        
        _guide.addGuideStep(GUIDE_COMMANDS_ADD_RECT, Rect(190.0f, 185.0f, 50.0f, 80.0f));
        _guide.addGuideStep(GUIDE_LABEL_5, Rect(240.0f, 90.0f, 0, 0));
        _guide.addGuideStep(GUIDE_COMMANDS_WAIT_TOUCH, Rect::ZERO);
        _guide.addGuideStep(GUIDE_COMMANDS_CLEAR_RECT, Rect::ZERO);
        
        for(auto iter = _opeData.begin(); iter != _opeData.end(); ++iter){
            auto len = _sumVec.size();
            auto ope = iter->first;
            _sumVec.push_back(ope->ope(0));
            for(auto i = 0; i < len; ++i){
                _sumVec.push_back(ope->ope(_sumVec[i]));
            }
        }
    }
    
    
    //find the index in the sumVec
    auto len = _sumVec.size();
    auto findIndex = -1;
    auto i = _preGuideIndex;
    auto j = (i+1)%len;
    for(; i != j;  --i, ++j){
        if(i < 0){
            i += len;
        }
        if(j >= len){
            j -= len;
        }
        if(_sumVec[i] == num){
            findIndex = i;
            break;
        }
        if(_sumVec[j] == num){
            findIndex = static_cast<int>(j);
            break;
        }
    }
    
    if(num == 0){
        findIndex = -1;
    }
    
    //split the index of opeNode
    vector<bool> opeVec(_opeData.size(), false);
    if(findIndex >= 0){
        int index = findIndex + 1;
        int opeIndex = 0;
        do {
            int ji = 1;
            int temp = index;
            while (temp / 2 > 0) {
                opeIndex++;
                temp /= 2;
                ji *= 2;
            }
            opeVec[opeIndex] = true;
            index = index % ji;
            opeIndex= 0;
        }while (index > 0);
        
        _preGuideIndex = findIndex;
    }
    
    //get the opeNodes have to change
    vector<size_t> guideVec;
    for(auto i = 0; i < _opeData.size(); ++i){
        if(_opeData[i].second != opeVec[i]){
            guideVec.push_back(i);
        }
    }
    
    //guide
    float y = 20.0f;
    float width = 50.0f;
    float height = 105.0f;
    float x = (480.0f - 50.0f * _opeData.size()-10.0f) / 2;
    Rect r(x, y, width, height);
    for(auto i = 0; i < guideVec.size(); ++i){
        r.origin.x = x + width * (guideVec[i]);
        r.size.height = height;
        r.origin.y = y;
        _guide.addGuideStep(GUIDE_COMMANDS_ADD_RECT, r);
        _guide.addGuideStep(GUIDE_LABEL_6, Rect(240.0f, 240.0f, 0, 0));
        r.size.height = 40.0f;
        r.origin.y = _opeData[guideVec[i]].second ? y + height - r.size.height : y;
        _guide.addGuideStep(GUIDE_COMMANDS_WAIT_TOUCH, r);
        _guide.addGuideStep(GUIDE_COMMANDS_REMOVE_GUIDE, Rect::ZERO);
    }
    
    _guide.addGuideStep(GUIDE_COMMANDS_CALL_BACK, Rect(GUIDE_ACTION_START, 0, 0, 0));
    
    _guide.startGuide();
    
    
}