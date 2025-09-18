//
// Created by Lenovo on 25-8-26.
//
#include "DrawBasic.h"
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <conio.h>
#include <algorithm> // for std::min

// --- 全局变量和枚举 ---
HANDLE hConsoleOut;
HANDLE hConsoleIn;
HWND hConsoleWnd;

bool running = false;
Model_t ModelState = Model_t::MainModel;

// --- 函数声明 ---
void SetCursorPosition(HANDLE hConsole, int x, int y);
void ClearScreen();
void LowerPrint(std::string line, bool instant = false);
void DrawBox(int x, int y, int width, int height, const std::string& title = "");

// --- 控制台初始化和工具函数 ---

void InitConsole() {
    // 获取控制台句柄
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
    hConsoleWnd = GetConsoleWindow();

    // 设置UTF-8编码，以正确显示特殊字符（如边框）
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    // 设置控制台窗口大小和缓冲区
    SMALL_RECT rect = {0, 0, ConsoleWidth - 1, ConsoleHeight - 1};
    SetConsoleWindowInfo(hConsoleOut, TRUE, &rect);
    COORD size = {ConsoleWidth, ConsoleHeight};
    SetConsoleScreenBufferSize(hConsoleOut, size);

    // 设置控制台标题
    SetConsoleTitleW(L"Game - The Quest for the Seed of Origin"); // 使用宽字符以支持更多标题

    // 隐藏光标
    HideCursor();
}

void HideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = {1, 0};
    SetConsoleCursorInfo(hConsoleOut, &cursorInfo);
}

void ShowCursor() {
    CONSOLE_CURSOR_INFO cursorInfo = {1, 1};
    SetConsoleCursorInfo(hConsoleOut, &cursorInfo);
}

void SetColor(ConsoleColor color) {
    SetConsoleTextAttribute(hConsoleOut, static_cast<int>(color));
}

void SetCursorPosition(HANDLE hConsole, int x, int y) {
    COORD coord = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(hConsole, coord);
}

void ClearScreen() {
    COORD coord = {0, 0};
    DWORD count;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsoleOut, &csbi);
    DWORD consoleSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsoleOut, ' ', consoleSize, coord, &count);
    FillConsoleOutputAttribute(hConsoleOut, static_cast<int>(ConsoleColor::WHITE), consoleSize, coord, &count);
    SetCursorPosition(hConsoleOut, 0, 0);
}

// --- 界面绘制函数 ---

/**
 * @brief 绘制一个带边框和可选标题的方框
 * @param x 方框左上角X坐标
 * @param y 方框左上角Y坐标
 * @param width 方框宽度
 * @param height 方框高度
 * @param title 标题（可选），会显示在顶边框中间
 */
void DrawBox(int x, int y, int width, int height, const std::string& title) {
    SetColor(ConsoleColor::CYAN); // 设置边框颜色
    std::string top = "╔" + std::string(width - 2, '═') + "╗";
    std::string mid = "║" + std::string(width - 2, ' ') + "║";
    std::string bot = "╚" + std::string(width - 2, '═') + "╝";

    SetCursorPosition(hConsoleOut, x, y);
    std::cout << top;
    for (int i = 1; i < height - 1; ++i) {
        SetCursorPosition(hConsoleOut, x, y + i);
        std::cout << mid;
    }
    SetCursorPosition(hConsoleOut, x, y + height - 1);
    std::cout << bot;

    if (!title.empty()) {
        int titleX = x + (width - title.length()) / 2;
        SetCursorPosition(hConsoleOut, titleX, y);
        SetColor(ConsoleColor::YELLOW); // 标题高亮
        std::cout << title;
    }
}


/**
 * @brief 绘制经过美化的游戏主菜单界面
 */
void DrawGameImg() {
    ClearScreen();

    // 绘制外层大边框
    DrawBox(0, 0, ConsoleWidth, ConsoleHeight, " MAIN MENU ");

    // LOGO 文本
    std::vector<std::string> logo = {
        "  #####   ######  ##   ##  #####  #######  ",
        "  ##  ##  ##      ### ###  ##     ##       ",
        "  #####   ######  ## # ##  #####  #######  ",
        "  ##  ##  ##      ##   ##     ##  ##       ",
        "  #####   ######  ##   ##  #####  #######  "
    };

    // 计算LOGO绘制的起始位置（使其在屏幕上半部分居中）
    int logoWidth = logo[0].length();
    int logoHeight = logo.size();
    int logoStartX = (ConsoleWidth - logoWidth) / 2;
    int logoStartY = ConsoleHeight / 4 - logoHeight / 2;

    SetColor(ConsoleColor::LIGHT_MAGENTA);
    for (int i = 0; i < logoHeight; ++i) {
        SetCursorPosition(hConsoleOut, logoStartX, logoStartY + i);
        std::cout << logo[i];
    }

    // 绘制菜单面板
    int menuPanelWidth = 30;
    int menuPanelHeight = 7;
    int menuPanelX = (ConsoleWidth - menuPanelWidth) / 2;
    int menuPanelY = ConsoleHeight / 2;
    DrawBox(menuPanelX, menuPanelY, menuPanelWidth, menuPanelHeight);

    // 显示提示信息
    SetColor(ConsoleColor::GRAY);
    std::string hint = "Use W/S or Arrow Keys to select, Enter to confirm.";
    int hintX = (ConsoleWidth - hint.length()) / 2;
    int hintY = menuPanelY + menuPanelHeight + 1;
    SetCursorPosition(hConsoleOut, hintX, hintY);
    std::cout << hint;

    GameStartInput(); // 调用输入处理
}


/**
 * @brief 处理主菜单的输入，带有选择光标
 */
void GameStartInput() {
    std::vector<std::string> menuItems = {"Start Game", "Quit Game"};
    int selectedIndex = 0;
    int menuX = (ConsoleWidth - 12) / 2; // "-> Start Game " 总长度大约12
    int menuY = ConsoleHeight / 2 + 2;

    auto printMenu = [&](int highlightIndex) {
        for (int i = 0; i < menuItems.size(); ++i) {
            SetCursorPosition(hConsoleOut, menuX, menuY + i * 2);
            if (i == highlightIndex) {
                SetColor(ConsoleColor::YELLOW);
                std::cout << "-> " << menuItems[i] << " <-";
            } else {
                SetColor(ConsoleColor::WHITE);
                std::cout << "   " << menuItems[i] << "   ";
            }
        }
    };

    printMenu(selectedIndex);

    int ch;
    while (true) {
        ch = _getch();
        if (ch == 224) { // 方向键
            ch = _getch();
            switch (ch) {
                case 72: // Up
                    selectedIndex = (selectedIndex - 1 + menuItems.size()) % menuItems.size();
                    break;
                case 80: // Down
                    selectedIndex = (selectedIndex + 1) % menuItems.size();
                    break;
            }
        } else {
            switch (ch) {
                case 'w': case 'W':
                    selectedIndex = (selectedIndex - 1 + menuItems.size()) % menuItems.size();
                    break;
                case 's': case 'S':
                    selectedIndex = (selectedIndex + 1) % menuItems.size();
                    break;
                case 13: // Enter key
                    if (selectedIndex == 0) { // Start Game
                        running = true;
                        return;
                    } else if (selectedIndex == 1) { // Quit Game
                        running = false;
                        return;
                    }
                    break;
            }
        }
        printMenu(selectedIndex);
    }
}

// --- 游戏流程函数 ---

/**
 * @brief 逐字打印效果
 * @param line 要打印的字符串
 * @param instant 如果为true，则立即打印，跳过逐字效果
 */
void LowerPrint(std::string line, bool instant) {
    if (instant) {
        std::cout << line << std::endl;
        return;
    }
    for (char c : line) {
        std::cout << c << std::flush; // 使用 flush 保证立即显示
        if (_kbhit()) { // 如果有按键，则加速显示
           _getch();
           instant = true; // 将后续内容立即显示
        }
        if (!instant) {
            Sleep(40); // 调整打字速度
        }
    }
    std::cout << std::endl;
}

/**
 * @brief 游戏序章，带有更好的翻页和文本显示逻辑
 */
void GamePrologue() {
    ClearScreen();
    DrawBox(0, 0, ConsoleWidth, ConsoleHeight, " PROLOGUE ");

    std::vector<std::string> prologueLines = {
        "【帝都 - 魔法学院顶楼】",
        "",
        "你站在帝国魔法学院的顶楼，俯瞰着繁华的帝都。你，罗汀·奥德瑞恩，是帝国的首席魔法师，",
        "拥有令人艳羡的天赋和财富。然而，你胸口的黑色结晶纹路正在隐隐作痛，那是你强行激活",
        "禁忌水晶后留下的“诅咒”，你的力量正在不可逆转地流失。",
        "",
        "【系统提示】",
        "你感到力量正在流失。你需要找到“始源之种”来解除诅咒。",
        "",
        "【NPC：导师艾利克斯】(☹)：",
        "“罗汀！你的状态越来越差了。我查阅了所有古籍，只有传说中的‘始源之种’",
        "才能稳定你的魔力。它位于迷雾森林深处的精灵遗迹中。你必须立刻动身！”"
    };

    int currentLine = 0;
    int topMargin = 2;
    int leftMargin = 4;
    bool fastForward = false;

    for (const auto& line : prologueLines) {
        SetCursorPosition(hConsoleOut, leftMargin, topMargin + currentLine);

        if (line.find("【") != std::string::npos) {
            SetColor(ConsoleColor::YELLOW);
        } else if (line.find("“") != std::string::npos) {
            SetColor(ConsoleColor::LIGHT_CYAN);
        }
        else {
            SetColor(ConsoleColor::WHITE);
        }

        LowerPrint(line, fastForward);

        if (_kbhit()) {
            _getch();
            fastForward = true;
        }

        currentLine++;
        // 可以在这里加入翻页逻辑，如果需要的话
    }

    // 章节结束提示
    SetColor(ConsoleColor::GRAY);
    std::string continuePrompt = "Press any key to continue...";
    SetCursorPosition(hConsoleOut, (ConsoleWidth - continuePrompt.length()) / 2, ConsoleHeight - 3);
    std::cout << continuePrompt;
    _getch();
}