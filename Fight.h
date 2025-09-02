//
// 优化后的战斗系统界面
//

#ifndef FIGHT_H
#define FIGHT_H
#include "DrawBasic.h"
#include "Person_Skill_Class/Person.h"
#include "Person_Skill_Class/Skill.h"
#include <functional>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>

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
    Player* player = nullptr;
    Person* enemy = nullptr;
    std::vector<std::string> battleLog;
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
// 辅助函数：绘制战斗日志
void DrawBattleLog() {
    SetCursorPosition(hConsoleOut, 35, 5);
    std::cout << "战斗日志：" << std::endl;
    
    int startLine = 6;
    int maxLines = 10;
    int logIndex = g_fightState.battleLog.size() - maxLines;
    if (logIndex < 0) logIndex = 0;
    
    for (int i = logIndex; i < g_fightState.battleLog.size(); ++i) {
        SetCursorPosition(hConsoleOut, 35, startLine++);
        std::cout << g_fightState.battleLog[i] << std::endl;
    }
}

// 辅助函数：添加战斗日志
void AddBattleLog(const std::string& log) {
    g_fightState.battleLog.push_back(log);
}

// 辅助函数：计算伤害
int CalculateDamage(int attackerAttack, int defenderDefense) {
    int damage = attackerAttack - defenderDefense / 2;
    if (damage < 1) damage = 1; // 至少造成1点伤害
    // 添加一些随机性
    damage = damage * (80 + rand() % 40) / 100;
    return damage;
}

// 战斗主函数
void Fight(Player& player, Person& enemy) {
    // 初始化随机数生成器
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // 初始化战斗状态
    g_fightState.isRunning = true;
    g_fightState.selectedAction = FightAction::Attack;
    g_fightState.selectedIndex = 0;
    g_fightState.player = &player;
    g_fightState.enemy = &enemy;
    g_fightState.battleLog.clear();
    
    ClearScreen();
    std::cout << "=====================================" << std::endl;
    std::cout << "           战斗界面              " << std::endl;
    std::cout << "=====================================" << std::endl;
    
    // 添加战斗开始日志
    AddBattleLog("战斗开始！");
    AddBattleLog("你遇到了" + enemy.GetName() + "！");
    
    // 初始化默认的动作处理器
    InitializeDefaultHandlers();
    DrawFightOptions();
    
    // 主战斗循环
    while (g_fightState.isRunning) {
        // 绘制战斗状态
        SetCursorPosition(hConsoleOut, 2, 5);
        std::cout << "玩家：" << player.GetName() << " (等级：" << player.GetLevel() << ")" << std::endl;
        SetCursorPosition(hConsoleOut, 2, 6);
        std::cout << "HP：" << player.GetHP() << "/" << player.GetMaxHP() << std::endl;
        SetCursorPosition(hConsoleOut, 2, 7);
        std::cout << "MP：" << player.GetMP() << "/" << player.GetMaxMP() << std::endl;
        
        SetCursorPosition(hConsoleOut, 2, 10);
        std::cout << "敌人：" << enemy.GetName() << std::endl;
        SetCursorPosition(hConsoleOut, 2, 11);
        std::cout << "HP：" << enemy.GetHP() << "/" << enemy.GetMaxHP() << std::endl;
        
        // 绘制战斗日志
        DrawBattleLog();
        
        // 处理输入
        HandleInput();
        Optionsrefresh();
        
        // 检查战斗是否结束
        if (enemy.GetHP() <= 0) {
            AddBattleLog("你击败了" + enemy.GetName() + "！");
            DrawBattleLog();
            g_fightState.isRunning = false;
        }
        else if (player.GetHP() <= 0) {
            AddBattleLog("你被击败了！");
            DrawBattleLog();
            g_fightState.isRunning = false;
        }
    }
    
    // 等待用户按键继续
    SetCursorPosition(hConsoleOut, 2, 25);
    std::cout << "战斗结束。按任意键继续..." << std::endl;
    _getch();
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
    if (!g_fightState.player || !g_fightState.enemy) {
        AddBattleLog("战斗状态未初始化！");
        return;
    }
    
    Player* player = g_fightState.player;
    Person* enemy = g_fightState.enemy;
    
    switch (g_fightState.selectedAction) {
        case FightAction::Attack: {
            // 执行攻击动作
            int playerTotalAttack = player->GetAttack() + player->GetBag().getTotalAttack();
            int enemyDefense = enemy->GetDefend();
            int damage = CalculateDamage(playerTotalAttack, enemyDefense);
            
            *enemy -= damage;
            AddBattleLog(player->GetName() + " 对 " + enemy->GetName() + " 造成了 " + std::to_string(damage) + " 点伤害！");
            
            // 敌人反击
            if (enemy->GetHP() > 0) {
                int enemyAttack = enemy->GetAttack();
                int playerTotalDefense = player->GetDefend() + player->GetBag().getTotalDefense();
                int enemyDamage = CalculateDamage(enemyAttack, playerTotalDefense);
                
                *player -= enemyDamage;
                AddBattleLog(enemy->GetName() + " 对 " + player->GetName() + " 造成了 " + std::to_string(enemyDamage) + " 点伤害！");
            }
            break;
        }
        case FightAction::Skills: {
            // 技能菜单逻辑
            ClearScreen();
            std::cout << "================================" << std::endl;
            std::cout << "           技能菜单              " << std::endl;
            std::cout << "================================" << std::endl;
            
            const auto& skills = player->GetSkills();
            if (skills.empty()) {
                std::cout << "你还没有学习任何技能！" << std::endl;
                std::cout << "按任意键返回..." << std::endl;
                _getch();
                break;
            }
            
            for (size_t i = 0; i < skills.size(); ++i) {
                std::cout << i + 1 << ". " << skills[i]->GetName() << " (消耗: " << skills[i]->GetMpCost() << " MP)" << std::endl;
                std::cout << "   " << skills[i]->GetDescription() << std::endl;
            }
            
            std::cout << "0. 返回" << std::endl;
            std::cout << "请选择技能: ";
            
            int choice = 0;
            std::cin >> choice;
            
            if (choice > 0 && choice <= static_cast<int>(skills.size())) {
                Skill* selectedSkill = skills[choice - 1];
                if (player->GetMP() >= selectedSkill->GetMpCost()) {
                    // 使用技能
                    if (selectedSkill->Use(player, enemy)) {
                        AddBattleLog(player->GetName() + " 使用了 " + selectedSkill->GetName() + "！");
                        
                        // 敌人反击
                        if (enemy->GetHP() > 0) {
                            int enemyAttack = enemy->GetAttack();
                            int playerTotalDefense = player->GetDefend() + player->GetBag().getTotalDefense();
                            int enemyDamage = CalculateDamage(enemyAttack, playerTotalDefense);
                            
                            *player -= enemyDamage;
                            AddBattleLog(enemy->GetName() + " 对 " + player->GetName() + " 造成了 " + std::to_string(enemyDamage) + " 点伤害！");
                        }
                    }
                    else {
                        AddBattleLog("技能使用失败！");
                    }
                }
                else {
                    AddBattleLog("MP不足，无法使用技能！");
                }
            }
            break;
        }
        case FightAction::Items: {
            system("cls");
            
            // 获取玩家背包中的所有消耗品
            Inventory* inventory = g_fightState.player->GetBag().getInventory();
            std::vector<InventorySlot*> consumables;
            
            // 过滤出消耗品类物品
            for (int i = 0; i < inventory->getItemCount(); i++) {
                auto slot = inventory->getSlotByIndex(i);
                if (slot && slot->item->type == ItemType::CONSUMABLE) {
                    consumables.push_back(slot);
                }
            }
            
            if (consumables.empty()) {
                AddBattleLog("背包中没有可用的消耗品");
                break;
            }
            
            // 显示物品列表
            std::cout << "\n===== 物品栏 =====\n";
            for (size_t i = 0; i < consumables.size(); i++) {
                std::cout << i + 1 << ". " << consumables[i]->item->name << " - " << consumables[i]->item->description << "\n";
            }
            std::cout << "0. 返回\n";
            std::cout << "请选择物品: ";
            
            // 处理物品选择
            int itemIndex = -1;
            while (true) {
                char itemChoice = _getch();
                if (itemChoice == '0') {
                    g_fightState.selectedAction = FightAction::Attack;
                    itemIndex = -1;
                    break;
                }
                
                int choice = itemChoice - '0';
                if (choice > 0 && choice <= static_cast<int>(consumables.size())) {
                    itemIndex = choice - 1;
                    break;
                }
            }
            
            if (itemIndex == -1) break;
            
            // 使用选中的物品
            InventorySlot* selectedItem= consumables[itemIndex];
            if (selectedItem->item->type == ItemType::CONSUMABLE) {
                Consumable* consumable = static_cast<Consumable*>(selectedItem->item.get());
                if (consumable->Use(g_fightState.player)) {
                    AddBattleLog("使用了 " + selectedItem->item->name);
                    
                    // 从背包中移除已使用的物品
                    // 注意：这里需要找到物品在背包中的实际位置
                    for (int i = 0; i < inventory->getItemCount(); i++) {
                        if (inventory->getSlotByIndex(i) == selectedItem) {
                            inventory->removeItem(i);
                            break;
                        }
                    }
                } else {
                    AddBattleLog("无法使用 " + selectedItem->item->name);
                }
            }
            break;
        }
        case FightAction::Run: {
            // 逃跑逻辑
            int runChance = rand() % 100;
            if (runChance < 50) { // 50%的逃跑成功率
                AddBattleLog("成功逃跑了！");
                g_fightState.isRunning = false;
            }
            else {
                AddBattleLog("逃跑失败！");
                // 敌人反击
                int enemyAttack = enemy->GetAttack();
                int playerTotalDefense = player->GetDefend() + player->GetBag().getTotalDefense();
                int enemyDamage = CalculateDamage(enemyAttack, playerTotalDefense);
                
                *player -= enemyDamage;
                AddBattleLog(enemy->GetName() + " 对 " + player->GetName() + " 造成了 " + std::to_string(enemyDamage) + " 点伤害！");
            }
            break;
        }
    }
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
