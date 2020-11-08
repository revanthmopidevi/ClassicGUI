#include "MainMenuScene.h"

USING_NS_CC;

Scene* MainMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    auto layer = MainMenu::create();

    scene->addChild(layer);

    return scene;
}

bool MainMenu::init()
{
    if (!Layer::init())
    {
        return false;
    }

    auto logo = Sprite::create("BitsLogo.png");
    Size size = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    logo->setPosition(Point(origin.x + 320, origin.y + 240));
    logo->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(logo);

    auto action_up = MoveTo::create(2, Point(origin.x + 320, origin.x + 400));
    logo->runAction(action_up);

    return true;
}