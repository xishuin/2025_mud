#include "DrawBasic.h"
#include "MainShell.h"
#include "Map.h"
#include <iostream>
#include "Fight.h"
#include <cstdlib>
#include <ctime>

// 背包模型绘制函数

Player player;

int main() {
    // 初始化控制台
    InitConsole();
    DrawGameImg();
    // if (running) {
    //     GamePrologue();
    // }
    //srand(static_cast<unsigned int>(time(nullptr)));
    // 主循环
    //Fight();
    while (running) {
        switch (ModelState) {
            case Model_t::MainModel:
                MainGame();
                break;
            case Model_t::MapModel:
                Map();
                break;
            case Model_t::BoxModel:
                //DrawBox();
                break;
        }
        Sleep(50);
    }
    // 清屏并显示退出消息
    system("cls");
    std::cout << "Game ended. Thanks for playing!\n";
    ShowCursor();
    return 0;
}