
#include "Map.h"
#include "Person_Skill_Class/Person.h"
#include "map/map.h"
#include <string>
#include <iostream>

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
    // 加载事件和地图数据
    std::string eventPath = "d:\\code_practise\\project\\Home_worke\\map\\event.json";
    std::string chapterPath = "d:\\code_practise\\project\\Home_worke\\map\\chapters.json";
    
    if (!g_eventManager.load_data(eventPath)) {
        std::cerr << "Failed to load event data from: " << eventPath << std::endl;
    } else {
        g_eventManager.bulid_all_events();
        std::cout << "Event data loaded successfully." << std::endl;
    }
    
    if (!g_mapManager.loadFromFile(chapterPath)) {
        std::cerr << "Failed to load map data from: " << chapterPath << std::endl;
    } else {
        // 确保设置当前章节为1，以便测试事件显示
        g_mapManager.currentChapterId = 1;
        if (!g_mapManager.chapters.empty()) {
            playerPos={static_cast<SHORT>(player.GetX()+offset),static_cast<SHORT>(player.GetY()+offset)};
            prevPlayerPos=playerPos;
            MapHeight=g_mapManager.get_Current_Capters()->height;
            MapWidth=g_mapManager.get_Current_Capters()->width;
        }
    }
    std::cout << "Map system initialized successfully!" << std::endl;
}


void DrawStaticMap() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsoleOut, &csbi)) {
        return;
    }
    DWORD written;
    // 清屏
    FillConsoleOutputCharacter(hConsoleOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, {0, 0}, &written);
    // 绘制顶部边界
    SetCursorPosition(hConsoleOut, 0, 0);
    for (int i = 0; i < MapWidth + 2; i++) std::cout << "#";
    // 绘制底部边界
    SetCursorPosition(hConsoleOut, 0, MapHeight + 1);
    for (int i = 0; i < MapWidth + 2; i++) std::cout << "#";
    // 绘制左右边界
    for (int y = 1; y <= MapHeight; y++) {
        SetCursorPosition(hConsoleOut, 0, y);
        std::cout << "#";
        SetCursorPosition(hConsoleOut, MapWidth + 1, y);
        std::cout << "#";
    }
    // 绘制地图中的事件元素
    if (!g_mapManager.chapters.empty()) {
        const Chapter* currentChapter = g_mapManager.get_Current_Capters();
        if (currentChapter) {
            for (const auto& id : currentChapter->events) {
                Event e = g_eventManager.find_event(id);
                if (e.id == 0) continue; // 事件不存在，跳过
                    SetCursorPosition(hConsoleOut, e.x, e.y);
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

    // 绘制说明文本
    SetCursorPosition(hConsoleOut, 0, static_cast<SHORT>(MapHeight + 3));
    std::cout << "WASD来移动";
    SetCursorPosition(hConsoleOut, 0, static_cast<SHORT>(MapHeight + 4));
    std::cout << "触碰即触发，ESC退出地图";
    SetCursorPosition(hConsoleOut, 0, static_cast<SHORT>(MapHeight + 5));
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

    // 初始绘制玩家位置
    SetColor(ConsoleColor::BRIGHT_WHITE);
    SetCursorPosition(hConsoleOut, playerPos.X, playerPos.Y);
    std::cout << "@";

    // 将光标移出游戏区域
    SetCursorPosition(hConsoleOut, 0, static_cast<SHORT>(MapHeight + 5));
}

void PartialRefresh() {
    // 清除玩家之前的位置
    SetCursorPosition(hConsoleOut, prevPlayerPos.X, prevPlayerPos.Y);
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
    SetCursorPosition(hConsoleOut, playerPos.X, playerPos.Y);
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
                    if (newPos.X < MapWidth - 1) {
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

void EventDetection() {
    Chapter* current_Chapter=g_mapManager.get_Current_Capters();
    if (current_Chapter) {
        for (auto id:current_Chapter->events) {
            Event e=g_eventManager.find_event(id);
            if (e.x==playerPos.X && e.y==playerPos.Y) {
                    e.event_handeler();
            }
        }
    }
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
