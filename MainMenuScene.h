#pragma once

#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(MainMenu);

    cocos2d::Sprite* logo;
    
    void Sudoku(cocos2d::Ref *pSender);
    void Maze(cocos2d::Ref *pSender);
    void TicTacToe(cocos2d::Ref *pSender);
    void exit(cocos2d::Ref* pSender);
}; 