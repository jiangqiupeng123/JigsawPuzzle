//
//  EndScene.cpp
//  JigsawPuzzle
//
//  Created by 江秋朋 on 16/1/8.
//
//

#include "EndScene.hpp"
#include "HelloWorldScene.h"

Scene *EndScene::createScene() {
    
    auto scene = Scene::create();
    
    auto layer = EndScene::create();
    
    scene->addChild(layer);
    
    return scene;
    
}

bool EndScene::init() {
    
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    auto label = Label::createWithSystemFont("Game Over", "fonts/Marker Felt.ttf", 60);
    addChild(label);
    label->setPosition(visibleSize / 2);
    label->setColor(Color3B(0, 0, 0));
    
    auto label2 = Label::createWithSystemFont("Restart", "fonts/Marker Felt.ttf", 40);
    addChild(label2);
    label2->setPosition(100, 100);
    label2->setColor(Color3B(0, 0, 0));
    
    auto listener = EventListenerTouchOneByOne::create();
    
    listener->onTouchBegan = [this](Touch *touch, Event *event){
        
        if (event->getCurrentTarget()->getBoundingBox().containsPoint(touch->getLocation())) {
            
            Director::getInstance()->replaceScene(TransitionZoomFlipAngular::create(0.5, HelloWorld::createScene()));
            
        }
        
        return true;
    };
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, label2);
    
    return true;
}





