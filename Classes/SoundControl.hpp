//
//  SoundControl.hpp
//  MyCppGame
//
//  Created by msempire on 16/9/2.
//
//

#ifndef SoundControl_hpp
#define SoundControl_hpp

class SoundControl{
public:
    static SoundControl* getInstance(){
        if(_instance == nullptr){
            //TODO: check null
            _instance = new SoundControl();
            if(!_instance->init()){
                //TODO:disable the sound
                _instance->_playSound = false;
            }
        }
        
        return _instance;
    }
    
    void playSound(int index);
    void initBgMusic();
    void pauseBgMusic();
    void resumeBgMusic();
    
    void startSound();
    void stopSound();
    void pauseSound();
    void resumeSound();
    void end();
    
    const int SOUND_MIN_INDEX;
    const int SOUND_MAX_INDEX;
    
private:
    SoundControl();
    ~SoundControl();
    
    bool init();
    
    void loadCallBack(bool load);
    
    static SoundControl* _instance;
    bool _playSound;
    bool _bgPlayed;
    
};


#endif /* SoundControl_hpp */
