
#include "Map.h"



bool maprunning=true;
COORD playerPos = {static_cast<SHORT>(MapWidth / 2), static_cast<SHORT>(MapHeight/ 2)};
COORD prevPlayerPos = playerPos; // 添加prevPlayerPos的定义
bool playerMoved=false;


void DrawStaticMap() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsoleOut, &csbi)) {
        return;
    }
    DWORD written;
    // 清屏
    FillConsoleOutputCharacter(hConsoleOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, {0, 0}, &written);
    // 绘制顶部边界
    SetConsoleCursorPosition(hConsoleOut, {0, 0});
    for (int i = 0; i < MapWidth + 2; i++) std::cout << "#";
    // 绘制底部边界
    SetConsoleCursorPosition(hConsoleOut, {0, static_cast<SHORT>(MapHeight + 1)});
    for (int i = 0; i < MapWidth + 2; i++) std::cout << "#";
    // 绘制左右边界
    for (int y = 1; y <= MapHeight; y++) {
        SetConsoleCursorPosition(hConsoleOut, {0, static_cast<SHORT>(y)});
        std::cout << "#";
        SetConsoleCursorPosition(hConsoleOut, {static_cast<SHORT>(MapWidth + 1), static_cast<SHORT>(y)});
        std::cout << "#";
    }

    // 绘制说明文本
    SetConsoleCursorPosition(hConsoleOut, {0, static_cast<SHORT>(MapHeight + 3)});
    std::cout << "WASD to move yourself";
    SetConsoleCursorPosition(hConsoleOut, {0, static_cast<SHORT>(MapHeight + 4)});
    std::cout << "Press 'Esc' to quit.";
    SetConsoleCursorPosition(hConsoleOut, {0, static_cast<SHORT>(MapHeight + 5)});
    SetColor(ConsoleColor::GREEN);
    std::cout<<"NPC---N";
    std::cout<<"  ";
    SetColor(ConsoleColor::RED);
    std::cout<<"Monster---M";
    // 初始绘制玩家位置
    SetColor(ConsoleColor::BRIGHT_WHITE);
    SetConsoleCursorPosition(hConsoleOut, {static_cast<SHORT>(playerPos.X + 1), static_cast<SHORT>(playerPos.Y + 1)});
    std::cout << "🧙";
    // 将光标移出游戏区域
    SetConsoleCursorPosition(hConsoleOut, {0, static_cast<SHORT>(MapHeight + 5)});
}

void PartialRefresh() {
    // 清除玩家之前的位置
    SetConsoleCursorPosition(hConsoleOut, {static_cast<SHORT>(prevPlayerPos.X + 1), static_cast<SHORT>(prevPlayerPos.Y + 1)});
    std::cout << " ";
    // 绘制玩家新位置
    SetConsoleCursorPosition(hConsoleOut, {static_cast<SHORT>(playerPos.X + 1), static_cast<SHORT>(playerPos.Y + 1)});
    std::cout << "🧙";
    // 更新玩家之前的位置
    prevPlayerPos = playerPos;
    // 将光标移出游戏区域
    SetConsoleCursorPosition(hConsoleOut, {0, static_cast<SHORT>(MapHeight + 5)});
}



void EventDetection(){}

void MapKeyInput() {
    MapkeyInput:
        if (int ch = _getch()) {
            // 转换为小写以便处理
            ch = tolower(ch);
            COORD newPos = playerPos;
            bool moved = false;
            switch (ch) {
                case 'w':
                    if (newPos.Y > 0) {
                        newPos.Y--;
                        moved = true;
                    }
                    break;
                case 's':
                    if (newPos.Y < MapHeight- 1) {
                        newPos.Y++;
                        moved = true;
                    }
                    break;
                case 'a':
                    if (newPos.X > 0) {
                        newPos.X--;
                        moved = true;
                    }
                    break;
                case 'd':
                    if (newPos.X < MapWidth - 1) {
                        newPos.X++;
                        moved = true;
                    }
                    break;
                case 27:
                    maprunning=false;
                    break;
                default:
                    goto MapkeyInput;
            }
            if (moved) {
                playerPos = newPos;
                playerMoved=true;
            }
        }
    Sleep(50);
    return;
}

void Map() {
    DrawStaticMap();
    maprunning=true;
    while (maprunning) {
        HideCursor();
        MapKeyInput();
        if (playerMoved) {
            PartialRefresh();
            playerMoved=false;
        }
    }
    ModelState=Model_t::MainModel;
    return;
}
