//
//  MainScene.hpp
//  JigsawPuzzle
//
//  Created by 江秋朋 on 16/1/7.
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include <stdio.h>
#include "cocos2d.h"

USING_NS_CC;

class MainScene:public Layer {
    
public:
    
    static MainScene *createScene(std::string photo, int rows = 3, int cols = 3, bool isRestart=true);
    
    CREATE_FUNC(MainScene);
    
    void restart(); // 重新开始游戏
    
private:
    Size _visibleSize;
    
    int _rows;
    int _cols;
    int _sliceWidth;
    int _sliceHeight;
    std::vector<std::vector<Sprite *>> _slices;  // 储存切片
    std::vector<std::vector<Sprite *>> _finishSlices;
    
    virtual bool initWithPhoto(std::string photo, int rows = 3, int cols = 3, bool isRestart=true);
    
    bool initSlices(std::string photo); // 切割图片
    
    void initTouchEventlistener(); // 初始化触摸事件
    
    void move(int x, int y); // 移动图块
    
    bool isFinish(); // 判断是否完成拼图
};

#endif /* MainScene_hpp */
