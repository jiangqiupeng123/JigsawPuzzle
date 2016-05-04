//
//  EndScene.hpp
//  JigsawPuzzle
//
//  Created by 江秋朋 on 16/1/8.
//
//

#ifndef EndScene_hpp
#define EndScene_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class EndScene:public LayerColor {
    
public:
    
    static Scene *createScene();
    
    CREATE_FUNC(EndScene);
    
private:
    virtual bool init();
    
};

#endif /* EndScene_hpp */
