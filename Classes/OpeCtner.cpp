//
//  OpeCtner.cpp
//  MyCppGame
//
//  Created by msempire on 16/9/18.
//
//

#include "OpeCtner.hpp"


void OpeCtner::guideData(const std::vector<bool>& data){
    auto childCount = getChildrenCount();
    if(data.size() < childCount){
        return;
    }
    
    auto children = getChildren();
    auto dIter = data.begin();
    auto iter = children.begin();
    for(; iter != children.end(); ++iter, ++dIter){
        auto child = dynamic_cast<GuideNode*>(*iter);
        child->setGuideState(*dIter);
    }
    guideIndex = -1;
}

bool OpeCtner::guideNext(){
    
    auto children = getChildren();
    auto childCount = getChildrenCount();
    guideIndex++;
    while(guideIndex < childCount){
        auto start = children.begin();
        auto child = dynamic_cast<GuideNode*>(*(start + guideIndex));
        
        if(child && child->guide()){
            break;
        }
        guideIndex++;
    }
    
    //past the last one
    if(guideIndex >= childCount){
        return false;
    }
    
    return true;
}