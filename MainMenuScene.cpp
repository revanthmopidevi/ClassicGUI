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

    // GL View Dimensions
    Size size = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();


    // BITS Logo
    auto logo = Sprite::create("BitsLogo.png");
    logo->setPosition(Point(origin.x + 4 * (size.width / 5), origin.y + (size.height / 2)));
    logo->setAnchorPoint(Vec2(0.5, 0.5));
    this->addChild(logo);
    // auto logo_up = MoveTo::create(3, Point(origin.x + size.width / 2, origin.y + size.height));
    // logo->runAction(logo_up);
    // auto logo_fade = FadeOut::create(3);
    // logo->runAction(logo_fade);
    
    // Create Menu Items
    auto menu_item_1 = MenuItemFont::create("Sudoku", CC_CALLBACK_1(MainMenu::Sudoku, this));
    auto menu_item_2 = MenuItemFont::create("Maze", CC_CALLBACK_1(MainMenu::Maze, this));
    auto menu_item_3 = MenuItemFont::create("TicTacToe", CC_CALLBACK_1(MainMenu::TicTacToe, this));
    auto menu_item_4 = MenuItemImage::create("CloseNormal.png", "CloseSelected.png", CC_CALLBACK_1(MainMenu::exit, this));
    // Set Menu Items Position
    menu_item_1->setPosition(Point(origin.x + size.width / 2, origin.y + (size.height / 5) * 4));
    menu_item_2->setPosition(Point(origin.x + size.width / 2, origin.y + (size.height / 5) * 3));
    menu_item_3->setPosition(Point(origin.x + size.width / 2, origin.y + (size.height / 5) * 2));
    menu_item_4->setPosition(Point(origin.x + size.width / 2, origin.y + (size.height / 5) * 1));

    auto *menu = Menu::create(menu_item_1, menu_item_2, menu_item_3, menu_item_4, NULL);
    menu->setPosition(Point(0, 0));
    this->addChild(menu);


    return true;
}

void MainMenu::Sudoku(cocos2d::Ref *pSender) {
    CCLOG("Sudoku");
}

void MainMenu::Maze(cocos2d::Ref *pSender) {
    CCLOG("Maze");
}

void MainMenu::TicTacToe(cocos2d::Ref *pSender) {
    CCLOG("TicTacToe");
}

void MainMenu::exit(cocos2d::Ref* pSender) {
    CCLOG("exit");
}