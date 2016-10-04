//
//  Numbers.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/9.
//
//

#ifndef Numbers_hpp
#define Numbers_hpp
//#include <vector>
#include "cocos2d.h"

#define INVALIDATE_NUMBER 999
#define DELAY_NUMBER 111

class Numbers:public cocos2d::Ref{
public:
    
    Numbers(): _nextIndex(0), _nextPlayIndex(0), _numId(-2), _playId(-2),_infinite(false), _preNum(0), _prePlayIndex(0){}
    bool init(int numId, int playId);
    int next(int& num, int& type);
    void reset(){ _nextIndex = 0; _nextPlayIndex = 0; _prePlayIndex = 0; _preNum = 0; }
    
    int getNumSize() { return _infinite ? 10000 : (int)_numVec.size(); }
    int getNextPlayNum(int index);
    
    static Numbers* create(int numId, int playId);
    
    
private:
    std::string getNumStrById(int id);
    void splitNum(std::string& s, std::vector<int>& vec);
    int getNextPlayIndex();
    int getNumByIndex(int index);
    
    void makeRandom(int& num, int& type);
    
private:
    int _numId;
    int _playId;
    bool _infinite;
    int _preNum;
    int _prePlayIndex;
    
    int _nextIndex;
    int _nextPlayIndex;
    std::vector<int> _numVec;
    std::vector<int> _playIndexVec;
};

#endif /* Numbers_hpp */
