#include "draw/DrawBasic.h"
#include "draw/MainShell.h"
#include "map/Map.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "static/story/StorySystem.h"
#include "Role/Person_Skill_Class/Person.h"
#include "Role/Person_Skill_Class/monster.h"
#include "Role/new_bag/Item.h"
#include "map/map_data/map.h"
#include "draw/GameSystem.h"

// 声明剧情系统初始化函数
void InitStorySystem();
void StartGameStory();

// 声明Boss数据初始化函数
void InitBossData();

// 全局玩家对象
Player player;
map<int, Person> monsters;
map<int, Person> boss;
// 全局地图管理器和事件管理器
MapManager g_mapManager;
EventManager g_eventManager;


int main() {
    std::cout << "游戏启动" << std::endl;
    // 初始化控制台
    InitConsole();
    DrawGameImg();
    // 为玩家添加一些默认物品
    player.GetBag().gainItem(std::make_unique<HealthPotion>(1, "生命药水", "恢复50点生命值", 50, 50), 3);
    player.GetBag().gainItem(std::make_unique<ManaPotion>(2, "魔法药水", "恢复30点魔法值", 30, 30), 2);
    
    // 为玩家添加一些初始装备
    player.GetBag().gainItem(std::make_unique<Equipment>(101, "木剑", "一把简单的木剑", 100, 
                                                        Rarity::COMMON, 1, 5, 0, EquipmentSlot::WEAPON));
    player.GetBag().gainItem(std::make_unique<Equipment>(102, "皮帽", "用皮革制成的帽子", 80, 
                                                        Rarity::COMMON, 1, 0, 2, EquipmentSlot::HELMET));
    player.GetBag().gainItem(std::make_unique<Equipment>(103, "皮甲", "用皮革制成的护甲", 120, 
                                                        Rarity::COMMON, 1, 0, 5, EquipmentSlot::CHEST));
    player.GetBag().gainItem(std::make_unique<Equipment>(104, "皮靴", "用皮革制成的靴子", 60, 
                                                        Rarity::COMMON, 1, 0, 3, EquipmentSlot::SHOES));
    // 初始化地图系统
    InitMapSystem();
    // 初始化Boss数据
    InitBossData();
    // 初始化剧情系统
    try {
        InitStorySystem();
    } catch (const std::exception& e) {
        std::cerr << "初始化剧情系统 - 异常: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "初始化剧情系统 - 未知异常" << std::endl;
    }
    // 初始化游戏系统（商店和宝箱）
    std::cout << "初始化游戏系统" << std::endl;
    GameSystem::getInstance()->init();
    
    // 启动游戏剧情
    if (running) {
        std::cout << "启动游戏剧情" << std::endl;
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
            case Model_t::ShopModel:
                DrawShopUI();
                break;
        }
        Sleep(50);
    }
    if (player.GetHP()==0) {
        player.SetHp(1);
    }
    // 清屏并显示退出消息
    system("cls");
    std::cout << "Game ended. Thanks for playing!\n";
    ShowCursor();
    return 0;
}

// 实现Boss数据初始化函数
void InitBossData() {
    // 使用相对路径加载boss.json文件
    std::string bossFilePath = "../Role/Person_Skill_Class/boss.json";
    boss = loadBossFromJson(bossFilePath);
    
    if (boss.empty()) {
        std::cerr << "错误：未能加载boss数据！请检查文件路径: " << bossFilePath << std::endl;
        // 可以选择在这里终止程序或采取其他处理措施
        // exit(1);
    } else {
        std::cout << "成功加载Boss数据，共加载 " << boss.size() << " 个Boss" << std::endl;
    }
}