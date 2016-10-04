//
//  GameControl.hpp
//  MyCppGame
//
//  Created by msempire on 16/8/3.
//
//

#ifndef GameControl_hpp
#define GameControl_hpp

class DataControl;
class GameControl{
    
public:
    static GameControl& getInstance(){
        static GameControl instance;
        
        return instance;
    }
    
    void startGame();
    void endGame();
    void stopGame();
    void resumeGame();
    
    void setCurChapterId(int id){ _curChapterId = id; }
    int getCurChapterId(){ return _curChapterId; }
    
    void setSound(bool on);
    bool getSound() { return _sound; };
    
    int getUpdateVer() { return _updateVer; }
private:
    GameControl():_curChapterId(0), _sound(true), _updateVer(0){
        init();
    };
    ~GameControl(){};
    GameControl(const GameControl&);
    const GameControl& operator=(const GameControl&);
    
    bool init();
    
private:
    int _curChapterId;
    bool _sound;
    int _updateVer;
    
};

#endif /* GameControl_hpp */
