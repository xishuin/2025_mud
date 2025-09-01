//
// 优化后的战斗系统界面
//

#ifndef FIGHT_H
#define FIGHT_H
#include "DrawBasic.h"
#include <functional>
#include <string>
#include <vector>

// 战斗操作枚举
enum class FightAction {
    Attack = 0,      // 攻击
    Skills,          // 技能
    Items,           // 物品
    Run              // 逃跑
};

// 动作处理函数接口
typedef std::function<void()> ActionHandler;

// 战斗选项结构体
struct FightOption {
    FightAction action;
    std::string name;
    ActionHandler handler;
};

// 战斗状态
struct FightState {
    bool isRunning = true;
    FightAction selectedAction = FightAction::Attack;
    int selectedIndex = 0;
};

// 全局战斗状态
FightState g_fightState;
int prevY = 20;
// 函数声明
void Fight();
void DrawFightOptions();
void HandleInput();
void ProcessAction();
void SetActionHandler(FightAction action, const ActionHandler& handler);
void InitializeDefaultHandlers();
void Optionsrefresh();
// 战斗主函数
void Fight() {
    ClearScreen();
    std::cout << "================================" << std::endl;
    std::cout << "           战斗界面              " << std::endl;
    std::cout << "================================" << std::endl;

    // 初始化默认的动作处理器
    InitializeDefaultHandlers();
    DrawFightOptions();
    // 主战斗循环
    while (g_fightState.isRunning) {
        HandleInput();
        Optionsrefresh();
    }
}

// 绘制战斗选项
void DrawFightOptions() {
    // 选项列表
    std::vector<std::string> options = {"攻击", "技能", "物品", "逃跑"};
    // 绘制所有选项
    for (int i = 0; i < options.size(); ++i) {
        int yPos = 20 + i;
        SetCursorPosition(hConsoleOut, 3, yPos);
        std::cout << options[i];
    }
    SetCursorPosition(hConsoleOut,0,prevY);
    std::cout<<"->";
}
void Optionsrefresh() {
    // 清空前一次的选择指示器
    SetCursorPosition(hConsoleOut, 0, prevY);
    std::cout << "  ";
    // 绘制当前选择指示器
    int currentY = 20 + g_fightState.selectedIndex;
    SetCursorPosition(hConsoleOut, 0, currentY);
    std::cout <<"->";
    prevY = currentY;
}
// 处理输入
void HandleInput() {
    bool keyPressed = false;
    
    while (!keyPressed) {
        if (int ch = _getch()) {
            ch = tolower(ch);
            
            switch (ch) {
                case 'w':
                    // 向上选择
                    g_fightState.selectedIndex = (g_fightState.selectedIndex - 1+4) % 4;
                    g_fightState.selectedAction = static_cast<FightAction>(g_fightState.selectedIndex);
                    keyPressed = true;
                    break;
                case 's':
                    // 向下选择
                    g_fightState.selectedIndex = (g_fightState.selectedIndex +1) % 4;
                    g_fightState.selectedAction = static_cast<FightAction>(g_fightState.selectedIndex);
                    keyPressed = true;
                    break;
                case VK_RETURN:
                    // 确认选择
                    ProcessAction();
                    keyPressed = true;
                    break;
            }
        }
    }
}

// 处理选中的动作
void ProcessAction() {
    // 这里会调用注册的动作处理器
    // 实际实现会在外部提供
    switch (g_fightState.selectedAction) {
        case FightAction::Attack:
            // 实际攻击逻辑会在这里执行
            break;
        case FightAction::Skills:
            // 技能菜单逻辑会在这里执行
            break;
        case FightAction::Items:
            // 物品栏逻辑会在这里执行
            break;
        case FightAction::Run:
            // 逃跑逻辑会在这里执行
            g_fightState.isRunning = false;
            break;
    }
    // 等待用户按键继续
    std::cout << "按任意键继续..." << std::endl;
    _getch();
}

// 设置动作处理器
void SetActionHandler(FightAction action, const ActionHandler& handler) {
    // 这个函数允许外部代码注册自定义的动作处理器
    // 实际实现需要一个映射表来存储处理器
    // 这里简化处理
}

// 初始化默认的动作处理器
void InitializeDefaultHandlers() {
    // 初始化默认的动作处理器
    // 在实际应用中，这里会注册各个动作的处理函数
}

#endif //FIGHT_H
