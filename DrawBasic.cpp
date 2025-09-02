//
// Created by Lenovo on 25-8-26.
//
#include "DrawBasic.h"

HANDLE hConsoleOut;
HANDLE hConsoleIn;
HWND hConsoleWnd;
bool running=false;
Model_t ModelState=Model_t::MainModel;





void InitConsole() {
    // 获取控制台句柄
    hConsoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
    hConsoleIn = GetStdHandle(STD_INPUT_HANDLE);
    hConsoleWnd = GetConsoleWindow();

    // 设置控制台窗口大小
    SMALL_RECT rect = {0, 0, ConsoleWidth - 1, ConsoleHeight - 1};
    SetConsoleWindowInfo(hConsoleOut, TRUE, &rect);
    COORD size = {ConsoleWidth, ConsoleHeight};
    SetConsoleScreenBufferSize(hConsoleOut, size);
    SetConsoleOutputCP(65001);
    // 设置控制台标题
    SetConsoleTitle("Game");
    // 设置文本颜色
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

void DrawGameImg() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsoleOut, &csbi)) {
        return;
    }
    DWORD written;
    // 清屏
    FillConsoleOutputCharacter(hConsoleOut, ' ', csbi.dwSize.X * csbi.dwSize.Y, {0, 0}, &written);
    // 移动光标到指定位置并打印LOGO和菜单
    COORD startPosLogo = { static_cast<SHORT>(csbi.dwSize.X / 2 - 10), static_cast<SHORT>(csbi.dwSize.Y / 2 - 5)}; // 调整LOGO起始位置
    SetCursorPosition(hConsoleOut, startPosLogo.X, startPosLogo.Y);
    std::cout << "#####   ######  ##   ##  #####  #######";
    SetCursorPosition(hConsoleOut, startPosLogo.X, static_cast<SHORT>(startPosLogo.Y+1));
    std::cout << "##  ##  ##      ### ###  ##     ##    ";
    SetCursorPosition(hConsoleOut, startPosLogo.X, static_cast<SHORT>(startPosLogo.Y + 2));
    std::cout << "#####   ######  ## # ##  #####  #######";
    SetCursorPosition(hConsoleOut, startPosLogo.X, static_cast<SHORT>(startPosLogo.Y + 3));
    std::cout << "##  ##  ##      ##   ##     ##  ##    ";
    SetCursorPosition(hConsoleOut, startPosLogo.X, static_cast<SHORT>(startPosLogo.Y + 4));
    std::cout << "#####   ######  ##   ##  #####  #######";
    // 打印游戏选项
    COORD startPosMenu = {static_cast<SHORT>(csbi.dwSize.X / 2 - 8), static_cast<SHORT>(csbi.dwSize.Y / 2 + 2)}; // 调整菜单起始位置
    SetCursorPosition(hConsoleOut, startPosMenu.X, startPosMenu.Y);
    std::cout << "1: StartGame";
    SetCursorPosition(hConsoleOut, startPosMenu.X, static_cast<SHORT>(startPosMenu.Y + 1));
    std::cout << "2: Quit Game";
    GameStartInput();
}

void GameStartInput() {
    gamestartinput:
        if (int ch = _getch()) {
            switch (ch) {
                case '1':
                    running=true;
                    break;
                case '2':
                    running=false;
                    break;
                default:
                    goto gamestartinput;
                    break;
            }
        }
        return;
}
void SetCursorPosition(HANDLE hConsole,int x, int y) {
    COORD coord = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(hConsole, coord);
}
void LowerPrint(std::string line) {
    size_t len=line.length();
    for (auto c:line) {
        std::cout<<c;
        Sleep(50);
    }

}
void GamePrologue() {
    std::vector<std::string> prologueLines = {
        "帝都 - 魔法学院顶楼",
        "你站在帝国魔法学院的顶楼，俯瞰着繁华的帝都。你，罗汀·奥德瑞恩，是帝国的首席魔法师，",
        "拥有令人艳羡的天赋和财富。然而，你胸口的黑色结晶纹路正在隐隐作痛，那是你强行激活",
        "禁忌水晶后留下的“诅咒”，你的力量正在不可逆转地流失。",
        "",
        "【系统提示】",
        "你感到力量正在流失。你需要找到“始源之种”来解除诅咒。",
        "",
        "【NPC：导师艾利克斯】（☹）：罗汀！你的状态越来越差了。我查阅了所有古籍，只有传说中的“始源之种”",
        "才能稳定你的魔力。它位于迷雾森林深处的精灵遗迹中。你必须立刻动身！"
    };
    const int totalLines = prologueLines.size();
    const int linesPerPage = 40;
    const int totalPages = (totalLines + linesPerPage - 1) / linesPerPage;
    for (int page = 0; page < totalPages; ++page) {
        ClearScreen();

        // 计算当前屏的起始和结束行
        int startLine = page * linesPerPage;
        int endLine = std::min(startLine + linesPerPage, totalLines);

        // 显示当前屏内容
        for (int i = startLine; i < endLine; ++i) {
            SetCursorPosition(hConsoleOut,0, i - startLine);
            if (_kbhit()) {
                std::cout<<prologueLines[i];
               _getch();
            }else {
                LowerPrint(prologueLines[i]);
            }
        }
        // 如果不是最后一屏，等待按键继续
        if (page < totalPages - 1) {
            SetCursorPosition(hConsoleOut,0, 39);
            printf("按任意键继续...");
            _getch();
        }
    }
    Sleep(150);
}
void ClearScreen() {
    COORD coord = {0, 0};
    DWORD count;
    FillConsoleOutputCharacter(hConsoleOut, ' ', 120*40, coord, &count);
    FillConsoleOutputAttribute(hConsoleOut, 0x07, 120*40, coord, &count);
    SetCursorPosition(hConsoleOut, coord.X, coord.Y);
}
