//
//  OpeNodeData.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/20.
//
//

#ifndef OpeNodeData_hpp
#define OpeNodeData_hpp

#include <stdio.h>
#include "GameUtil.hpp"
struct OpeNodeData{
public:
//    enum TYPE{
//        TYPE_NUM,
//        TYPE_ADD,
//        TYPE_MIN,
//        TYPE_MULTI,
//        TYPE_NEGTIVE,
//    };
    
    int mNum;
    
    OpeNodeData(int num): mNum(num){}
    virtual ~OpeNodeData(){}
    virtual int ope(int num) = 0;
    virtual void addOpeStr(std::string& s) = 0;
    virtual std::string toString() = 0;
};

struct OpeNumData:public OpeNodeData{
    OpeNumData(int num):OpeNodeData(num){}
    
    int ope(int n){
        return n + mNum;
    }
    
    void addOpeStr(std::string& s){
//        if(!s.empty()){
//            s += "+";
//        }
        
        s += "+" + ::uIntToStr(mNum);
    }
    
    std::string toString(){
        return ::uIntToStr(mNum);
    }
    
};

struct OpeMinData:public OpeNodeData{
    OpeMinData(int num):OpeNodeData(num){}
    
    int ope(int n){
        return n - mNum;
    }
    
    void addOpeStr(std::string& s){
        s += "-" + ::uIntToStr(mNum);
    }
    
    std::string toString(){
        return "-" + ::uIntToStr(mNum);
    }
};

struct OpeMultiData:public OpeNodeData{
    OpeMultiData(int num):OpeNodeData(num){}
    
    int ope(int n){
        return n * mNum;
    }
    
    void addOpeStr(std::string& s){
        s ="(" + s + ")" +  "*" + ::uIntToStr(mNum);
    }
    
    std::string toString(){
        return "*" + ::uIntToStr(mNum);
    }
};

struct OpeNegtiveData:public OpeNodeData{
    OpeNegtiveData(int num):OpeNodeData(num){}
    
    int ope(int n){
        return - n;
    }
    
    void addOpeStr(std::string& s){
        s = "-(" + s + ")";
    }
    
    std::string toString(){
        return "-" ;
    }
};

struct SoundNodeData:public OpeNodeData{
    SoundNodeData():OpeNodeData(0){}
    
    int ope(int n){
        return n;
    }
    
    void addOpeStr(std::string& s){
        
    }
    
    std::string toString(){
        return "On";
    }
};



#endif /* OpeNodeData_hpp */
