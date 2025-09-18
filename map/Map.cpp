#include "Map.h"
#include "../Role/Person_Skill_Class/Person.h"
#include "../Role/new_bag/Chest.h"
#include "../map/map_data/map.h"
#include  "../Role/Person_Skill_Class/monster.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../Fight/Fight.h"
#include "../Role/new_bag/ItemManager.h"
#include "../Role/new_bag/bag.h"
// 全局变量
bool maprunning=true;
COORD playerPos;
COORD prevPlayerPos; // 添加prevPlayerPos的定义
bool playerMoved=false;
int MapWidth;
int MapHeight;
int offset=1;
bool chapter_is_changer=false;
// 初始化Map包的系统
void InitMapSystem() {
    // 加载事件和地图数据c
    std::string eventPath = "..\\map\\map_data\\event.json";
    std::string chapterPath = "..\\map\\map_data\\chapters.json";
    std::string monstersPath="..\\Role\\Person_Skill_Class\\monster.json";
    if (!g_eventManager.load_data(eventPath)) {
        std::cerr << "Failed to load event data from: " << eventPath << std::endl;
    } else {
        g_eventManager.bulid_all_events();
        std::cout << "Event data loaded successfully." << std::endl;
    }
    
    if (!g_mapManager.loadFromFile(chapterPath)) {
        std::cerr << "Failed to load map data from: " << chapterPath << std::endl;
    } else {
        // 使用玩家当前章节，不再硬编码为1
        g_mapManager.Update_Capters(player);
        if (!g_mapManager.chapters.empty()) {
            playerPos={static_cast<SHORT>(player.GetX()+offset),static_cast<SHORT>(player.GetY()+offset)};
            prevPlayerPos=playerPos;
            MapHeight=g_mapManager.get_Current_Capters()->height;
            MapWidth=g_mapManager.get_Current_Capters()->width;
        }
    }
    // 从正确的路径加载怪物数据
    monsters = loadEventMonstersMap(eventPath,monstersPath);
    int idx=0;
    std::cout << "Map system initialized successfully!" << std::endl;
}

void EventDetection() {
    Chapter* current_Chapter=g_mapManager.get_Current_Capters();
    if (current_Chapter) {
        for (auto id:current_Chapter->events) {
            Event e=g_eventManager.find_event(id);
                if (e.x==playerPos.X && e.y == playerPos.Y) {
                    switch (e.type) {
                        case EventType::MONSTER:{
                            int res=Fight(player,monsters[e.id]);
                            if (res==1) {
                                auto it=monsters.find(e.id);
                                monsters.erase(it);
                                g_eventManager.remove_event(id);
                                
                                if (e.id<10) {
                                    player.GainEXP(20);
                                } else if (e.id>=10 && e.id<20) {
                                    player.GainEXP(40);
                                } else if (e.id>=20 && e.id<30) {
                                    player.GainEXP(50);
                                } else if (e.id>=30 && e.id<40) {
                                    player.GainEXP(60);
                                } else if (e.id>=40 && e.id<50) {
                                    player.GainEXP(70);
                                } else if (e.id>=50 && e.id<60) {
                                    player.GainEXP(70);
                                }
                                DrawStaticMap();
                            } else if (res==2) {
                                DrawStaticMap();
                            } else if (res == 0) {
                                ModelState = Model_t::MainModel;
                                maprunning=false;
                                running=false;
                            }
                            break;
                        }
                        case EventType::CHEST: {
                            // 初始化随机数生成器
                            static bool randInitialized = false;
                            if (!randInitialized) {
                                srand(static_cast<unsigned int>(time(nullptr)));
                                randInitialized = true;
                            }
                            // 获取当前章节
                            int currentChapter = g_mapManager.currentChapterId;
                            // 加载物品数据
                            static bool itemsLoaded = false;
                            if (!itemsLoaded) {
                                ItemManager::getInstance()->loadItemsFromJson("../static/json/items.json");
                                itemsLoaded = true;
                            }
                            // 创建宝箱
                            Chest chest(e.id, "宝箱", 0); // 简化实现，不考虑钥匙
                            // 生成随机物品（每个宝箱2个物品）
                            for (int i = 0; i < 2; ++i) {
                                std::unique_ptr<Item> randomItem = ItemManager::getInstance()->createRandomItemByChapter(currentChapter);
                                if (randomItem) {
                                    chest.addItem(std::move(randomItem), 1);
                                }
                            }
                            bool opened = chest.open(player);
                            if (opened) {
                                g_eventManager.remove_event(e.id);
                                SetCursorPosition(hConsoleOut, 0, static_cast<SHORT>(MapHeight + 11)); // 考虑偏移
                                std::cout<<"你获得了"<<chest.Getname();
                                Sleep(100);
                                SetCursorPosition(hConsoleOut, 0, static_cast<SHORT>(MapHeight + 11));
                                std::cout<<"                                                            ";
                                DrawStaticMap();
                            }
                            break;
                        }
                        default:
                            return;
                    }
                }
        }
    }
}



void DrawStaticMap() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsoleOut, &csbi)) {
        return;
    }
    DWORD written;
    // 清屏
    FillConsoleOutputCharacter(hConsoleOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, {0, 0}, &written);
    // 更新并获取当前章节信息
    g_mapManager.Update_Capters(player);
    const Chapter* currentChapter = g_mapManager.get_Current_Capters();
    
    // 显示当前地图信息
    SetCursorPosition(hConsoleOut, 0, 0);
    SetColor(ConsoleColor::CYAN);
    std::cout << "===== 当前地图信息 ======" << std::endl;
    std::cout << "地图名称: " << currentChapter->name << std::endl;
    std::cout << "地图描述: " << currentChapter->description << std::endl;
    std::cout << "地图大小: " << currentChapter->width << "x" << currentChapter->height << " 格" << std::endl;
    std::cout << "===== ====================" << std::endl;
    
    // 设置地图尺寸
    MapWidth = currentChapter->width;
    MapHeight = currentChapter->height;
    
    // 绘制顶部边界
    SetCursorPosition(hConsoleOut, 0, 5); // 从第5行开始绘制地图
    SetColor(ConsoleColor::WHITE);
    for (int i = 0; i < MapWidth + 2; i++) std::cout << "#";
    // 绘制底部边界
    SetCursorPosition(hConsoleOut, 0, MapHeight + 6); // 向下偏移5行
    for (int i = 0; i < MapWidth + 2; i++) std::cout << "#";
    // 绘制左右边界
    for (int y = 6; y <= MapHeight + 5; y++) { // 从第6行开始绘制
        SetCursorPosition(hConsoleOut, 0, y);
        std::cout << "#";
        SetCursorPosition(hConsoleOut, MapWidth + 1, y);
        std::cout << "#";
    }
    // 绘制地图中的事件元素
    if (!g_mapManager.chapters.empty()) {
        if (currentChapter) {
            for (const auto& id : currentChapter->events) {
                Event e = g_eventManager.find_event(id);
                if (e.id == 0) continue; // 事件不存在，跳过
                    SetCursorPosition(hConsoleOut, e.x, e.y + 5); // 考虑偏移
                    // 设置事件颜色
                    switch (e.type) {
                        case EventType::MONSTER:
                            SetColor(ConsoleColor::RED);
                            break;
                        case EventType::CHEST:
                            SetColor(ConsoleColor::YELLOW);
                            break;
                        case EventType::NPC:
                            SetColor(ConsoleColor::GREEN);
                            break;
                        case EventType::TRAP:
                            SetColor(ConsoleColor::MAGENTA);
                            break;
                        default:
                            SetColor(ConsoleColor::WHITE);
                            break;
                    }
                    // 绘制事件图标
                    switch (e.type) {
                        case EventType::MONSTER:
                            std::cout << "M";
                            break;
                        case EventType::CHEST:
                            std::cout << "C";
                            break;
                        case EventType::NPC:
                            std::cout << "N";
                            break;
                        case EventType::TRAP:
                            std::cout << "T";
                            break;
                        default:
                            std::cout << "?";
                            break;
                }
            }
        }
    }

    // 初始绘制玩家位置
    SetColor(ConsoleColor::BRIGHT_WHITE);
    SetCursorPosition(hConsoleOut, playerPos.X, playerPos.Y + 5); // 玩家位置向下偏移5行
    std::cout << "@";

    // 绘制说明文本
    SetCursorPosition(hConsoleOut, 0, static_cast<SHORT>(MapHeight + 8)); // 向下偏移5行
    std::cout << "WASD来移动";
    SetCursorPosition(hConsoleOut, 0, static_cast<SHORT>(MapHeight + 9));
    std::cout << "触碰即触发，ESC退出地图";
    SetCursorPosition(hConsoleOut, 0, static_cast<SHORT>(MapHeight + 10));
    SetColor(ConsoleColor::GREEN);
    std::cout << "NPC---N";
    std::cout << "  ";
    SetColor(ConsoleColor::RED);
    std::cout << "Monster---M";
    std::cout << "  ";
    SetColor(ConsoleColor::YELLOW);
    std::cout << "Chest---C";
    std::cout << "  ";
    SetColor(ConsoleColor::MAGENTA);
    std::cout << "Trap---T";

    // 将光标移出游戏区域
    SetCursorPosition(hConsoleOut, 0, static_cast<SHORT>(MapHeight + 10));
}

void PartialRefresh() {
    // 清除玩家之前的位置
    SetCursorPosition(hConsoleOut, prevPlayerPos.X, prevPlayerPos.Y + 5); // 考虑偏移
    // 检查这个位置是否有事件
    bool hasEvent = false;
    if (!g_mapManager.chapters.empty()) {
        const Chapter* currentChapter = g_mapManager.get_Current_Capters();
        if (currentChapter) {
            for (auto id : currentChapter->events) {
                Event e = g_eventManager.find_event(id);
                if (e.id == 0) continue; // 事件不存在，跳过
                if (e.x  == prevPlayerPos.X && e.y == prevPlayerPos.Y) {
                    hasEvent = true;
                    switch (e.type) {
                        case EventType::MONSTER:
                            SetColor(ConsoleColor::RED);
                            break;
                        case EventType::CHEST:
                            SetColor(ConsoleColor::YELLOW);
                            break;
                        case EventType::NPC:
                            SetColor(ConsoleColor::GREEN);
                            break;
                        case EventType::TRAP:
                            SetColor(ConsoleColor::MAGENTA);
                            break;
                        default:
                            SetColor(ConsoleColor::WHITE);
                            break;
                    }
                    switch (e.type) {
                        case EventType::MONSTER:
                            std::cout << "M";
                            break;
                        case EventType::CHEST:
                            std::cout << "C";
                            break;
                        case EventType::NPC:
                            std::cout << "N";
                            break;
                        case EventType::TRAP:
                            std::cout << "T";
                            break;
                        default:
                            std::cout << "?";
                            break;
                    }
                    break;
                }
            }
        }
    }

    // 如果没有事件，就输出空格
    if (!hasEvent) {
        std::cout << " ";
    }

    // 绘制玩家新位置
    SetColor(ConsoleColor::BRIGHT_WHITE);
    SetCursorPosition(hConsoleOut, playerPos.X, playerPos.Y + 5); // 考虑偏移
    std::cout << "@";

    // 更新玩家之前的位置
    prevPlayerPos = playerPos;

    // 将光标移出游戏区域
    SetCursorPosition(hConsoleOut, 0, static_cast<SHORT>(MapHeight + 5));
}
// 全局地图管理器和事件管理器在main.cpp中定义
// 此处只使用extern声明
extern MapManager g_mapManager;
extern EventManager g_eventManager;


// 设置玩家位置（同步Map包和Player对象）
void SetPlayerPosition(int x, int y) {
    // 更新Player对象的位置
    player.ChangeXY(x, y);
    // 更新控制台显示的玩家位置
    playerPos.X = static_cast<SHORT>(x);
    playerPos.Y = static_cast<SHORT>(y);
    playerMoved = true;
}

// 键盘输入处理函数
void MapKeyInput() {
    MapkeyInput:
        if (int ch = _getch()) {
            // 转换为小写以便处理
            ch = tolower(ch);
            COORD newPos = playerPos;
            bool moved = false;
            switch (ch) {
                case 'w':
                    if (newPos.Y > 1) {
                        newPos.Y--;
                        moved=true;
                    }
                    break;
                case 's':
                    if (newPos.Y < MapHeight - 1) {
                        newPos.Y++;
                        moved=true;
                    }
                    break;
                case 'a':
                    if (newPos.X > 1) {
                        newPos.X--;
                        moved=true;
                    }
                    break;
                case 'd':
                    if (newPos.X < MapWidth ) {
                        newPos.X++;
                        moved=true;
                    }
                    break;
                case 'e':
                    // 按E键检查周围是否有事件
                    break;
                case 27:
                    // Esc键退出地图模式
                    maprunning = false;
                    break;
                default:
                    goto MapkeyInput;
            }
            if (moved) {
                // 设置新的玩家位置，并同步所有系统
                SetPlayerPosition(newPos.X, newPos.Y);
            }
        }
    Sleep(50);
}


// 主地图函数
void Map() {
    DrawStaticMap();
    maprunning = true;
    while (maprunning) {
        HideCursor();
        MapKeyInput();
        
        if (playerMoved) {
            PartialRefresh();
            playerMoved = false;
        }
        EventDetection();
    }
    ModelState = Model_t::MainModel;
}
