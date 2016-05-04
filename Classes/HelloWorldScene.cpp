#include "HelloWorldScene.h"
#include "MainScene.hpp"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto photo1 = MainScene::createScene("Autumn.png", 3, 3);
    
    photo1->setAnchorPoint(Vec2(0 ,0));
    
    auto size1 = photo1->getContentSize();
    photo1->setPosition(visibleSize.width/2 - size1.width/2 - 200, visibleSize.height/2 - size1.height/2 + 150);
    
    addChild(photo1);
    
    auto photo2 = MainScene::createScene("Autumn.png", 3, 3, false);
    
    photo2->setAnchorPoint(Vec2(0 ,0));
    
    auto size2 = photo2->getContentSize();
    photo2->setPosition(visibleSize.width/2 - size2.width/2 - 200, visibleSize.height/2 - size2.height/2 - 150);
    
    addChild(photo2);
    
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
