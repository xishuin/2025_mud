
#ifndef MAINDRAW_H
#define MAINDRAW_H
#include "DrawBasic.h"
#include "StorySystem.h"

// 前向声明Player类
class Player;
// 声明外部全局变量player
extern Player player;

// 声明全局剧情系统
extern StorySystem g_storySystem;

void DrawPlots();
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
    DrawPlots();
    //Plot的地方
    MainKeyInput();
}

void DrawBagUI() {
    ClearScreen();
    std::cout << "================================" << std::endl;
    std::cout << "           背包系统              " << std::endl;
    std::cout << "================================" << std::endl;

    // 显示角色状态
    std::cout << "角色: " << player.GetName() << std::endl;
    std::cout << "等级: " << player.GetLevel() << std::endl;
    std::cout << "金币: " << player.GetBag().getGold() << std::endl;
    std::cout << "\n装备状态: " << std::endl;
    std::cout << "总攻击力: " << player.GetBag().getTotalAttack() + player.GetAttack() << std::endl;
    std::cout << "总防御力: " << player.GetBag().getTotalDefense() + player.GetDefend() << std::endl;

    // 显示物品栏
    std::cout << "\n物品栏: " << std::endl;
    player.GetBag().getInventory()->display();

    std::cout << "\n使用说明: " << std::endl;
    std::cout << "按任意键返回主菜单" << std::endl;

    _getch(); // 等待用户按键
    ModelState=Model_t::MainModel;
}
void DrawPlots() {
    // 检查剧情是否活跃
    if (g_storySystem.isActive()) {
        // 绘制剧情文本
        const auto& currentNode = g_storySystem.getCurrentNode();
        SetCursorPosition(hConsoleOut, 2, 1);
        std::cout << "\n剧情：" << currentNode.getText() << std::endl;
        // 绘制选项
        const auto& choices = currentNode.getChoices();
        if (!choices.empty()) {
            SetCursorPosition(hConsoleOut, 2, 12);
            std::cout << "请选择：" << std::endl;
            for (size_t i = 0; i < choices.size(); ++i) {
                SetCursorPosition(hConsoleOut, 4, 13 + i);
                std::cout << i + 1 << ". " << choices[i]->text << std::endl;
            }
            // 处理选项输入
            if (int ch = _getch() - '0') {
                if (ch >= 1 && ch <= static_cast<int>(choices.size())) {
                    g_storySystem.chooseOption(ch - 1);
                    // 如果是结束节点，结束剧情
                    if (currentNode.isEnd()) {
                        g_storySystem.endStory();
                    }
                }
            }
        } else {
            // 没有选项的剧情，按任意键继续
            SetCursorPosition(hConsoleOut, 2, 12);
            std::cout << "按任意键继续..." << std::endl;
            if (_kbhit()) {
                _getch();
                g_storySystem.endStory();
            }
        }
    } else {
        // 剧情未激活，检查地图触发条件
        static int debugCounter = 0;
        if (debugCounter % 100 == 0) {
            // 每100帧输出一次调试信息
            SetCursorPosition(hConsoleOut, 2, 10);
            std::cout << "玩家位置: (" << player.GetX() << ", " << player.GetY() << ")" << std::endl;
            std::cout << "等级: " << player.GetLevel() << std::endl;
        }
        debugCounter++;
        // 检查是否按下P键手动触发剧情
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 'p' || ch == 'P') {
                g_storySystem.startStory(0);
            }
        }
        // 检查地图触发条件
        if (g_storySystem.checkMapTrigger(player.GetX(), player.GetY(), player)) {
            // 剧情已触发
            SetCursorPosition(hConsoleOut, 2, 15);
            std::cout << "触发剧情!" << std::endl;
        }
    }
}

#endif //MAINDRAW_H
