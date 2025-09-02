#include "DrawBasic.h"
#include "MainShell.h"
#include "Map.h"
#include <iostream>
#include "Fight.h"
#include <cstdlib>
#include <ctime>
#include "StorySystem.h"
#include "Person_Skill_Class\Person.h"
#include "new_bag\Item.h"
#include "map/map.h"

// 声明剧情系统初始化函数
void InitStorySystem();
void StartGameStory();

// 全局玩家对象
Player player;

// 全局地图管理器和事件管理器
MapManager g_mapManager;
EventManager g_eventManager;


int main() {
    // 初始化控制台
    InitConsole();
    DrawGameImg();
    //Fight();

    // 为玩家添加一些默认物品
    player.GetBag().gainItem(std::make_unique<HealthPotion>(1, "生命药水", "恢复50点生命值", 50, 50), 3);
    player.GetBag().gainItem(std::make_unique<ManaPotion>(2, "魔法药水", "恢复30点魔法值", 30, 30), 2);
    // 初始化地图系统
    InitMapSystem();
    // 初始化剧情系统
    InitStorySystem();
    // 启动游戏剧情
    if (running) {
        StartGameStory();
    }
    
    // 设置随机数种子
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // 主循环
    while (running) {
        switch (ModelState) {
            case Model_t::MainModel:
                MainGame();
                break;
            case Model_t::MapModel:
                Map();
                break;
            case Model_t::BoxModel:
                DrawBagUI();
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