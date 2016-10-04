//
//  Numbers.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/9.
//
//

#include "Numbers.hpp"
#include "GameUtil.hpp"
#include "NumberNode.hpp"
#include "DataControl.hpp"
#include <vector>
#include <string>
#include <stdlib.h>
using namespace std;

Numbers* Numbers::create(int numId, int playId){
    Numbers* ret = new Numbers();
    if(ret && ret->init(numId, playId)){
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool Numbers::init(int numId, int playId){
    if(_numId == numId && _playId == playId){
        reset();
        return true;
    }
    
    _numId = numId;
    _playId = playId;
    
    _nextIndex = 0;
    _nextPlayIndex = 0;
    
    if(numId < 0 && playId < 0){
        _infinite = true;
        return true;
    }
    
    _infinite = false;
    
    string numStr = getNumStrById(_numId);
    string playStr = getNumStrById(_playId);
    
    splitNum(numStr, _numVec);
    splitNum(playStr, _playIndexVec);
    
    return true;
}

int Numbers::next(int &num, int &type){
    if(_infinite){
        makeRandom(num, type);
        return _nextIndex++;
    }
    
    if(_nextIndex >= _numVec.size()){
        return -1;
    }
    num = _numVec[_nextIndex];
    
    type = NumberNode::DISPLAY;
    int playIndex = getNextPlayIndex();
    if(_nextIndex == playIndex){
        _nextPlayIndex++;
        type = NumberNode::PLAY;
    }
    if(num == DELAY_NUMBER){
        type = NumberNode::DELAY;
    }
    
    return _nextIndex++;
}

string Numbers::getNumStrById(int id){
    
    return DataControl::getInstance().getNumById(id).s;
}

void Numbers::splitNum(std::string &s, std::vector<int> &vec){
    vec.clear();
    
    string::iterator iter = s.begin();
    int num = 0;
    int sign = 1;
    for(; iter != s.end(); ++iter){
        if(*iter == '|'){
            vec.push_back(sign * num);
            num = 0;
            sign = 1;
        }else if(*iter == '-'){
            sign = -1;
        }else{
            CCASSERT(*iter >= '0' && *iter <= '9', "Data Init Failed!");
            num = num * 10 + (*iter-'0');
        }
    }
}

int Numbers::getNextPlayNum(int index){
    if(_infinite){
        return DELAY_NUMBER;
    }
    
    
    auto iter = _playIndexVec.begin();
    for(; iter != _playIndexVec.end(); ++iter){
        if(*iter > index){
            break;
        }
    }
    
    if(iter == _playIndexVec.end()){
        return INVALIDATE_NUMBER;
    }
    
    int re = getNumByIndex(*iter);
    while( re == DELAY_NUMBER){
        if(++iter != _playIndexVec.end()){
            re = getNumByIndex(*iter);
        }else{
            re = INVALIDATE_NUMBER;
            break;
        }        
    }
    
    return re;
}

int Numbers::getNextPlayIndex(){
    return _nextPlayIndex < _playIndexVec.size() ? _playIndexVec[_nextPlayIndex] : INVALIDATE_NUMBER;
}

int Numbers::getNumByIndex(int index){
    if(index > _numVec.size()){
        return INVALIDATE_NUMBER;
    }
    
    return _numVec[index];
}

void Numbers::makeRandom(int &num, int &type){
    //range
    int min = MAX(_preNum -10, -19);
    int max = MIN(_preNum + 10, 20);
    int range = max - min;
    //num
    num = rand() * 1.0 / RAND_MAX * range + min;
    
    //type
    if(_nextIndex - _prePlayIndex > 4){
        type = NumberNode::PLAY;
        _prePlayIndex = _nextIndex;
    }else{
        int random = rand() * 1.0 / RAND_MAX * 10;
        if(random <= 5){
            type = NumberNode::PLAY;
            _prePlayIndex = _nextIndex;
        }else if(random <= 8){
            type = NumberNode::DISPLAY;
        }else{
            type = NumberNode::DELAY;
            num = DELAY_NUMBER;
        }
    }
    
//    _nextIndex++;
}