#include "DrawBasic.h"
#include "MainShell.h"
#include "Map.h"
#include <iostream>
#include "Fight.h"
#include <cstdlib>
#include <ctime>
#include "StorySystem.h"

// 声明剧情系统初始化函数
void InitStorySystem();
void StartGameStory();

// 背包模型绘制函数

Player player;

int main() {
    // 初始化控制台
    InitConsole();
    DrawGameImg();
    //Fight();

    // 初始化剧情系统
    InitStorySystem();

    // 启动游戏剧情
    if (running) {
        StartGameStory();
    }
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