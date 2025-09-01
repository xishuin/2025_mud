
#ifndef MAINDRAW_H
#define MAINDRAW_H
#include "DrawBasic.h"

void MainKeyInput() {
    // 处理模型切换输入
    if (int ch = _getch()) {
        ch = tolower(ch);
        switch (ch) {
            case 'm':
                ModelState=Model_t::MapModel;
                break;
            case 'b':
                ModelState=Model_t::BoxModel;
                break;
            case 'q':
                running = false;
                break;
        }
    }
    Sleep(50);
}

void Draw_Progress_Bar(int x,int y,int width,int cur,int max,ConsoleColor color) {
    int fill=cur*width/max;
    SetCursorPosition(hConsoleOut,x,y);
    SetColor(color);
    int i=0;
    for (;i<fill;i++) {
        std::cout<<"█";
    }
    for (;i<width;i++) {
        std::cout<<"-";
    }
    SetColor(ConsoleColor::WHITE);
}
void MainGame() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsoleOut, &csbi)) {
        return;
    }
    DWORD written;
    FillConsoleOutputCharacter(hConsoleOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, {0, 0}, &written);
    SetConsoleCursorPosition(hConsoleOut,{0,0});
    std::cout <<"M:map";
    std::cout<<"B:Box";
    std::cout<<"Q:quit";
    int StatePosY=20;
    int StatePosX=2;
    SetCursorPosition(hConsoleOut,StatePosX,StatePosY-2);
    std::cout<<"player:"<<player.GetName();
    SetCursorPosition(hConsoleOut,StatePosX,StatePosY-1);
    std::cout<<"Level:"<<player.GetLevel();
    SetCursorPosition(hConsoleOut,StatePosX,StatePosY);
    std::cout<<"Hp:";
    Draw_Progress_Bar(StatePosX+3,StatePosY,10,player.GetHP(),player.GetMaxHP(),ConsoleColor::LIGHT_RED);
    SetCursorPosition(hConsoleOut,StatePosX,StatePosY+1);
    std::cout<<"Mp:";
    Draw_Progress_Bar(StatePosX+3,StatePosY+1,10,player.GetMP(),player.GetMaxMP(),ConsoleColor::BLUE);
    SetCursorPosition(hConsoleOut,StatePosX,StatePosY+2);
    std::cout<<"攻击力:"<<player.GetAttack();
    SetCursorPosition(hConsoleOut,StatePosX,StatePosY+3);
    std::cout<<"防御力:"<<player.GetDefend();
    SetColor(ConsoleColor::WHITE);
    //Plot的地方

    //Plot的地方
    MainKeyInput();
}
#endif //MAINDRAW_H
