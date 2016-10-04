//
//  DataControl.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/3.
//
//

#ifndef DataControl_hpp
#define DataControl_hpp

#include <string>
#include <vector>
class sqlite3;

struct NumberData{
    int id;
    std::string s;
};

struct ChapterData{
    enum{
        TYPE_NEW_PLAYER = 1,
        TYPE_SIMPLE,
        TYPE_HARD,
        TYPE_INFINITE,
    };
    
    int id;
    int type;
    int numId;
    int playId;
    std::string name;
    int userScore;
    int userMiss;
    int passScore;
    int speed;
    std::string opeSet;
    
    bool isPass(){
        return userScore >= passScore;
    };
};

class DataControl{
public:
    
    void close();
    void updateDB();
    
    static DataControl& getInstance(){
        static DataControl instance;
        
        return instance;
    }
    
    ChapterData getChapterById(int id);
    NumberData getNumById(int id);
    int getCurChapterId();
    void getPassedChapterId(std::vector<ChapterData>&);
    void addCurChapterData(std::vector<ChapterData>&);
    int getNextChapterId(int id);
    bool saveChapterScore(int id, int score, int miss);
    int getSetting(int id, int defValue);
    void setSetting(int id, int value);
    
private:
    DataControl();
    virtual ~DataControl();
    DataControl(const DataControl&);
    const DataControl& operator=(const DataControl&);
    
    void initData();
    void initByJson();
    void initBySql();
    
    void readDataFromResult(char** re, int c, int row, ChapterData& data);
   
    sqlite3* _pdb;
    
};

#endif /* DataControl_hpp */
