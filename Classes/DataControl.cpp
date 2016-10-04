//
//  DataControl.cpp
//  MyCppGame
//
//  Created by msempire on 16/8/3.
//
//

#include "DataControl.hpp"
#include "sqlite3.h"
#include "cocos2d.h"
#include "GameUtil.hpp"

#include <string>
#include <fstream>
#include <vector>
using namespace std;
USING_NS_CC;


DataControl::DataControl():_pdb(nullptr){
    initData();
}

DataControl::~DataControl(){
    
}

void DataControl::updateDB(){
    //open db
    initBySql();
    
    //read command
    string path = "update/commands.sql";
    string filePath = ::getFileNameFromRes(path);
    
    ifstream fs(filePath);
    string command;
    CCLOG("MyCppGame: start read Commands");
    //exec command
    while (getline(fs, command)) {
        CCLOG("MyCppGame: read Commands: %s", command.c_str());
        if(command.empty()){
            continue;
        }
        sqlite3_exec(_pdb, command.c_str(), nullptr, nullptr, nullptr);
    }
    CCLOG("MyCppGame: end read Commands");
    fs.close();
    
}

void DataControl::initData(){
    initBySql();
}

void DataControl::initBySql(){
    if(_pdb == nullptr){
        string path = FileUtils::getInstance()->getWritablePath() + "data.db";
        int result = sqlite3_open(path.c_str(), &_pdb);
        if(result != SQLITE_OK){
            return;
        }
    }
    
}

void DataControl::initByJson(){
    
}

void DataControl::close(){
    if(_pdb != nullptr){
        sqlite3_close(_pdb);
        _pdb = nullptr;
    }
}

enum{
    CHAPTER_ROW_ID,
    CHAPTER_ROW_TYPE,
    CHAPTER_ROW_NAME,
    CHAPTER_ROW_NUM_ID,
    CHAPTER_ROW_PLAY_ID,
    CHAPTER_ROW_SCORE,
    CHAPTER_ROW_MISS,
    CHAPTER_ROW_PASS_SCORE,
    CHAPTER_ROW_SPEED,
    CHAPTER_ROW_OPE_SET,
};
ChapterData DataControl::getChapterById(int id){
    char** re;
    int r, c;
    string sql = "select * from Chapter where Id = " + ::uIntToStr(id) + ";";
    
    sqlite3_get_table(_pdb, sql.c_str(), &re, &r, &c, nullptr);
    ChapterData data;
    if(r > 0 && c > 0){
        readDataFromResult(re, c, 1, data);
    }
    
    sqlite3_free_table(re);
    
    return data;
}

NumberData DataControl::getNumById(int id){
    char** re;
    int r, c;
    string sql = "select * from Numbers where Id = " + ::uIntToStr(id);
    sqlite3_get_table(_pdb, sql.c_str(), &re, &r, &c, nullptr);
    NumberData data;
    if(r > 0 && c > 0){
        data.id = id;
        data.s = re[1*c + 1];
    }
    
    sqlite3_free_table(re);
    
    return data;

}


bool DataControl::saveChapterScore(int id, int score, int miss){
    const string sql = "update Chapter set Score = " + ::uIntToStr(score)
    + ", Miss = " + ::uIntToStr(miss) + " where Id = " + ::uIntToStr(id) + ";";
    int result = sqlite3_exec(_pdb, sql.c_str(), nullptr, nullptr, nullptr);
    
    return result == SQLITE_OK;
}

int DataControl::getCurChapterId(){
    const string sql = "select Id from Chapter where Score < PassScore order by id limit 1";
    char** re;
    int r, c, id=0;
    sqlite3_get_table(_pdb, sql.c_str(), &re, &r, &c, nullptr);
    if(r > 0 && c > 0){
        id = ::atoi(re[1*c]);
    }
    sqlite3_free_table(re);
    
    return id;

}

int DataControl::getNextChapterId(int preId){
    const string sql = "select Id from Chapter where Id > " + ::uIntToStr(preId) + " order by id limit 1";
    char** re;
    int r, c, id=0;
    sqlite3_get_table(_pdb, sql.c_str(), &re, &r, &c, nullptr);
    if(r > 0 && c > 0){
        id = ::atoi(re[1*c]);
    }
    sqlite3_free_table(re);
    
    return id;
}

void DataControl::getPassedChapterId(std::vector<ChapterData>& store){
    const string sql = "select * from Chapter where Score >= PassScore";
    char** re;
    int r, c;
    sqlite3_get_table(_pdb, sql.c_str(), &re, &r, &c, nullptr);
    if(r > 0 && c > 0){
        for(int i = 1; i <= r; ++i){
            ChapterData data;
            readDataFromResult(re, c, i, data);
            store.push_back(data);
        }
    }
    sqlite3_free_table(re);
}

void DataControl::addCurChapterData(std::vector<ChapterData> & store){
    const string sql = "select * from Chapter where Score < PassScore order by Id limit 1";
    char** re;
    int r, c;
    sqlite3_get_table(_pdb, sql.c_str(), &re, &r, &c, nullptr);
    if(r > 0 && c > 0){
        ChapterData data;
        readDataFromResult(re, c, 1, data);
        store.push_back(data);
    }
    sqlite3_free_table(re);
}

void DataControl::readDataFromResult(char **re, int c, int row, ChapterData &data){
    data.id = ::atoi(re[row * c + CHAPTER_ROW_ID]);
    data.type = ::atoi(re[row * c + CHAPTER_ROW_TYPE]);
    data.name = re[row*c + CHAPTER_ROW_NAME];
    data.numId = ::atoi(re[row * c + CHAPTER_ROW_NUM_ID]);
    data.playId = ::atoi(re[row * c + CHAPTER_ROW_PLAY_ID]);
    data.userScore = ::atoi(re[row * c + CHAPTER_ROW_SCORE]);
    data.userMiss = ::atoi(re[row * c + CHAPTER_ROW_MISS]);
    data.passScore = ::atoi(re[row * c + CHAPTER_ROW_PASS_SCORE]);
    data.speed = ::atoi(re[row * c + CHAPTER_ROW_SPEED]);
    data.opeSet = re[row * c + CHAPTER_ROW_OPE_SET];
}

int DataControl::getSetting(int key, int defValue){
    string sql = "select * from Setting;";
    char** re;
    int r, c;
    int result = sqlite3_get_table(_pdb, sql.c_str(), &re, &r, &c, nullptr);
    if(result != SQLITE_OK){
        sql = "create table Setting(Key integer, Value integer);";
        sqlite3_exec(_pdb, sql.c_str(), nullptr, nullptr, nullptr);
        
    }
    
    int value = defValue;
    bool find = false;
    if(r > 0 && c > 0){
        for(int i = 1; i <= r; ++i){
            if(::atoi(re[i*c]) == key){
                value = ::atoi(re[i*c + 1]);
                find = true;
                break;
            }
        }
    }
    
    sqlite3_free_table(re);
    
    if(!find){
        sql = "insert into Setting Values(" +::uIntToStr(key) + "," + ::uIntToStr(defValue) + ");";
        sqlite3_exec(_pdb, sql.c_str(), nullptr, nullptr, nullptr);
    }
    return value;
}

void DataControl::setSetting(int id, int value){
    string sql = "update Setting set Value = " + ::uIntToStr(value)
    + " where Key = " + ::uIntToStr(id) + ";";
    
    sqlite3_exec(_pdb, sql.c_str(), nullptr, nullptr, nullptr);
    
}

