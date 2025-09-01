//
// Created by Lenovo on 25-9-1.
//

#ifndef DRAWBOX_H
#define DRAWBOX_H
#include "DrawBasic.h"


void DrawBox() {
    ClearScreen();
    std::cout<<"============================================="<<std::endl;
    SetColor(ConsoleColor::LIGHT_YELLOW);
    std::cout<<"金币:"<<player.bag.getGold();
    SetColor(ConsoleColor::WHITE);
    std::cout<<"============================================="<<std::endl;

}

#endif //DRAWBOX_H
