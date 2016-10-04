//
//  Singleton.h
//  MyCppGame
//
//  Created by msempire on 16/8/3.
//
//

#ifndef Singleton_h
#define Singleton_h

template <class T>
class Singleton {
public:
    
    static T& getInstance(){
        static T mInstance;
        
        return mInstance;
    }
    
    
protected:
    Singleton<T>(){};
    
    
private:
    Singleton<T>(Singleton<T> const &);
    void operator=(Singleton<T> const &);
    
    virtual ~Singleton(){}

};


#endif /* Singleton_h */
