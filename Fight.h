//
// Created by Lenovo on 25-9-1.
//

#ifndef FIGHT_H
#define FIGHT_H
#include "DrawBasic.h"

enum class Fight_op {
    SimpleFight=0,
    Skills,
    Map,
    Skip
};
bool is_select=false;
void Fight_key();
void Handling_actions();
Fight_op op=Fight_op::SimpleFight;
int pre_x=2;
int pre_y=20;
bool fight_run=true;
void SetSelect(int x,int y);
void Fight() {
    ClearScreen();
    std::cout<<"================================";

    SetCursorPosition(hConsoleOut,3,20);
    std::cout<<"轻击";
    SetCursorPosition(hConsoleOut,3,21);
    std::cout<<"技能";
    SetCursorPosition(hConsoleOut,3,22);
    std::cout<<"逃跑";
    while (fight_run) {
        switch (op) {
            case Fight_op::SimpleFight:
                SetSelect(2,20);
                break;
            case Fight_op::Skills:
                SetSelect(2,21);
                break;
            case Fight_op::Skip:
                SetSelect(2,22);
                break;
        }
        Fight_key();
    }
}
void SetSelect(int x,int y) {
    SetCursorPosition(hConsoleOut,2,pre_y);
    std::cout<<" ";
    SetCursorPosition(hConsoleOut,x,y);
    std::cout<<"→";
    pre_y=y;
}
void Handling_actions() {

}
void Fight_key() {
    keyinput:
    if (int ch=_getch()) {
        ch=tolower(ch);
        switch (ch) {
            case 'w':
                if (op>Fight_op::SimpleFight) {
                    op=static_cast<Fight_op>(static_cast<int>(op)-1);
                }
                break;
            case 's':
                if (op<Fight_op::Skip) {
                    op=static_cast<Fight_op>(static_cast<int>(op)+1);
                }
                break;
            case VK_RETURN:
                is_select=true;
                break;
            default:
                goto keyinput;
                break;
        }
    }
}

#endif //FIGHT_H
