//
//  MainScene.cpp
//  JigsawPuzzle
//
//  Created by 江秋朋 on 16/1/7.
//
//

#include "MainScene.hpp"
#include "EndScene.hpp"

MainScene *MainScene::createScene(std::string photo, int rows, int cols, bool isRestart) {
    
    auto scene = new MainScene();
    
    if (scene&&scene->initWithPhoto(photo, rows, cols, isRestart)) {
        scene->autorelease();
    }
    else {
        
        CC_SAFE_DELETE(scene);
        
        return nullptr;
    }
    
    return scene;
    
}

bool MainScene::initWithPhoto(std::string photo, int rows, int cols, bool isRestart) {
    
    if (!Layer::init()) {
        return false;
    }
    
    _rows = rows;
    _cols = cols;
    _visibleSize = Director::getInstance()->getVisibleSize();
    
    if (!initSlices(photo)) {
        return false;
    }
    
    if (isRestart) {
        initTouchEventlistener();
        
        restart();
    }
    
    return true;
    
}

bool MainScene::initSlices(std::string photo) {
    
    Texture2D *texture = Director::getInstance()->getTextureCache()->addImage(photo);
    
    if (texture == nullptr) {
        return false;
    }
    
    // 初始化切片数组
    for (int i = 0; i < _rows; i++) {
        std::vector<Sprite *> row;
        for (int j = 0; j < _cols; j++) {
            row.push_back(nullptr);
        }
        _slices.push_back(row);
        _finishSlices.push_back(row);
    }
    
    // 计算图片的宽与高
    auto size = texture->getContentSize();
    _sliceWidth = size.width / _rows;
    _sliceHeight = size.height / _cols;
    
    this->setContentSize(size);  // 设置Layer的大小
    
    // 挖出最后一块切片
//    auto endSlice = Sprite::createWithTexture(texture, Rect(_sliceWidth * (_rows - 1), _sliceHeight * (_cols - 1), _sliceWidth, _sliceHeight));
//    endSlice->setAnchorPoint(Vec2(0, 0));
//    endSlice->setPosition(_sliceWidth * (_rows - 1), 0);
//    endSlice->setOpacityModifyRGB(100);
//    addChild(endSlice);
    
    for (int i = 0; i < _rows; i++) {
        for (int j = 0; j < _cols; j++) {
            
            auto *slice = Sprite::createWithTexture(texture, Rect(i * _sliceWidth, j * _sliceHeight, _sliceWidth, _sliceHeight));
            slice->setAnchorPoint(Vec2(0, 0));
            slice->setPosition(Vec2(i * _sliceWidth, size.height - (j+1) * _sliceHeight));
            this->addChild(slice);
            
            _slices[i][j] = slice;
            _finishSlices[i][j] = slice;
            
            if (i == _rows-1 && j == _cols-1) {
                _slices[i][j] = nullptr;
                _finishSlices[i][j] = nullptr;
                slice->removeFromParent();
            }
            
        }
    }
    
    return true;
}

void MainScene::initTouchEventlistener() {
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = [this](Touch *touch, Event *event){
        
        Point locationInNode = this->convertToNodeSpace(touch->getLocation());
        
        Size size = this->getBoundingBox().size;
        Rect rect = Rect(0, 0, size.width, size.height);
        if (rect.containsPoint(locationInNode)) {
            return true;
        }
        
        return false;
    };
    
    listener->onTouchEnded = [this](Touch *touch, Event *event){
        
        Point locationInNode = this->convertToNodeSpace(touch->getLocation());
        
        int x = static_cast<int>(floor(locationInNode.x/_sliceWidth));
        int y = static_cast<int>(floor(locationInNode.y/_sliceHeight));
        
        this->move(x, y);
        
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void MainScene::move(int x, int y) {
    
    // Y轴坐标反转
    y = _cols - y - 1;
    
    auto slice = _slices[x][y];
    
    if (!slice) {
        return;
    }
    
    // 判断周围是否有空余的格子
    bool isFind = false;
    Point targetPos;
    if (y < _cols-1 && _slices[x][y+1] == nullptr) { //上
        targetPos.x = x;
        targetPos.y = y+1;
        isFind = true;
    } else if (y > 0 && _slices[x][y-1] == nullptr) { //下
        targetPos.x = x;
        targetPos.y = y-1;
        isFind = true;
    } else if (x > 0 && _slices[x-1][y] == nullptr) { //左
        targetPos.x = x-1;
        targetPos.y = y;
        isFind = true;
    } else if (x < _rows-1 && _slices[x+1][y] == nullptr) { //右
        targetPos.x = x+1;
        targetPos.y = y;
        isFind = true;
    }
    
    if (isFind) {
        slice->runAction(Sequence::create(MoveTo::create(0.25, Vec2(targetPos.x * _sliceWidth, _sliceHeight * _cols - ((targetPos.y+1) * _sliceHeight))), CallFunc::create([this](){
            
            
            
            if (this->isFinish()) {
                sleep(1);
                Director::getInstance()->replaceScene(TransitionZoomFlipAngular::create(0.5, EndScene::createScene()));
            }
            
        }), NULL));
        _slices[x][y] = nullptr;
        _slices[targetPos.x][targetPos.y] = slice;
    }
}

void MainScene::restart() {
    
    Vector<Sprite *> list;
    
    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            auto slice = _slices[i][j];
            if (slice) {
                list.pushBack(slice);
            }
        }
    }
    
    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            if (i==_rows-1 && j==_cols-1) {
                _slices[i][j]=nullptr;
                break;
            }
            
            auto length = list.size();
            auto value = static_cast<int>(CCRANDOM_0_1()*length);
            
            auto slice = list.at(value);
            list.erase(value);
            
            _slices[i][j] = slice;
            
            slice->cocos2d::Node::setPosition(i*_sliceWidth, _sliceHeight*_cols - ((j+1)*_sliceHeight));
        }
    }
    
}

bool MainScene::isFinish() {
    
    for (int i=0; i<_rows; i++) {
        for (int j=0; j<_cols; j++) {
            
            if (_slices[i][j]&&_finishSlices[i][j]) {
                if (_slices[i][j]->getPosition() != _finishSlices[i][j]->getPosition()) {
                    return false;
                }
            } else if (_slices[i][j] != _finishSlices[i][j]) {
                return false;
            }
            
        }
    }
    
    return true;
    
}


