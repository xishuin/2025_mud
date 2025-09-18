#include "Fight.h"
#include <iostream>
#include <conio.h>
#include "../draw/DrawBasic.h"

// 全局战斗状态定义
FightState g_fightState;
int prevY = 20;

// 辅助函数：绘制战斗日志
void DrawBattleLog() {
    SetCursorPosition(hConsoleOut, 35, 5);
    SetColor(ConsoleColor::LIGHT_YELLOW);
    std::cout << "战斗日志：" << std::endl;
    SetColor(ConsoleColor::WHITE);
    
    int startLine = 6;
    int maxLines = 10;
    int logIndex = g_fightState.battleLog.size() - maxLines;
    if (logIndex < 0) logIndex = 0;
    
    for (int i = logIndex; i < g_fightState.battleLog.size(); ++i) {
        SetCursorPosition(hConsoleOut, 35, startLine++);
        
        // 根据日志内容设置不同颜色
        const std::string& log = g_fightState.battleLog[i];
        if (log.find("造成了") != std::string::npos || log.find("击败了") != std::string::npos || log.find("逃跑失败") != std::string::npos) {
            SetColor(ConsoleColor::LIGHT_RED); // 伤害和战斗结果用红色
        } else if (log.find("使用了") != std::string::npos || log.find("成功逃跑") != std::string::npos) {
            SetColor(ConsoleColor::LIGHT_GREEN); // 使用技能/物品和成功逃跑用绿色
        } else if (log.find("警告") != std::string::npos || log.find("Boss") != std::string::npos) {
            SetColor(ConsoleColor::LIGHT_MAGENTA); // Boss相关信息用洋红色
        } else if (log.find("MP不足") != std::string::npos || log.find("失败") != std::string::npos) {
            SetColor(ConsoleColor::RED); // 失败信息用暗红色
        } else if (log.find("战斗开始") != std::string::npos || log.find("战斗中断") != std::string::npos) {
            SetColor(ConsoleColor::CYAN); // 战斗状态信息用青色
        }
        
        std::cout << log << std::endl;
        SetColor(ConsoleColor::WHITE); // 恢复默认颜色
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

// 计算玩家总攻击力（基础攻击力+装备加成）
int GetPlayerTotalAttack(const Player& player) {
    return player.GetAttack() + player.GetBag().getTotalAttack();
}

// 计算玩家总防御力（基础防御力+装备加成）
int GetPlayerTotalDefense(const Player& player) {
    return player.GetDefend() + player.GetBag().getTotalDefense();
}

// 战斗主函数
int Fight(Player &player, Person &enemy, bool isBoss) {
    // 初始化随机数生成器
    srand(static_cast<unsigned int>(time(nullptr)));
    
    // 初始化战斗状态
    g_fightState.isRunning = true;
    g_fightState.selectedAction = FightAction::Attack;
    g_fightState.selectedIndex = 0;
    g_fightState.player = &player;
    g_fightState.enemy = &enemy;
    g_fightState.battleLog.clear();
    g_fightState.isBossFight = isBoss;
    g_fightState.bossPhase = 1;
    g_fightState.bossMaxHP = enemy.GetMaxHP();

    // Boss战斗初始化
    if (isBoss) {
        AddBattleLog("警告：这是一场Boss战！敌人拥有强大的力量！");
        AddBattleLog("Boss特性：随着血量降低，会变得更加强大！");
    }
    
    ClearScreen();
    std::cout << "=====================================" << std::endl;
    std::cout << "           战斗界面              " << std::endl;
    std::cout << "=====================================" << std::endl;
    
    // 添加战斗开始日志
    AddBattleLog("战斗开始！");
    AddBattleLog("你遇到了" + enemy.GetName() + "！");
    
    // 初始化默认的动作处理器
    DrawFightOptions();
    
    // 主战斗循环
    while (g_fightState.isRunning) {
        // 绘制战斗状态
        SetCursorPosition(hConsoleOut, 2, 5);
        SetColor(ConsoleColor::LIGHT_CYAN);
        std::cout << "玩家：" << player.GetName() << " (等级：" << player.GetLevel() << ")" << std::endl;
        SetColor(ConsoleColor::WHITE);
        
        SetCursorPosition(hConsoleOut, 2, 6);
        std::cout << "HP：";
        Draw_Progress_Bar(5, 6, 20, player.GetHP(), player.GetMaxHP(), ConsoleColor::LIGHT_RED);
        SetCursorPosition(hConsoleOut, 27, 6);
        std::cout << player.GetHP() << "/" << player.GetMaxHP() << std::endl;
        
        SetCursorPosition(hConsoleOut, 2, 7);
        std::cout << "MP：";
        Draw_Progress_Bar(5, 7, 20, player.GetMP(), player.GetMaxMP(), ConsoleColor::BLUE);
        SetCursorPosition(hConsoleOut, 27, 7);
        std::cout << player.GetMP() << "/" << player.GetMaxMP() << std::endl;
        
        SetCursorPosition(hConsoleOut, 2, 10);
        SetColor(ConsoleColor::LIGHT_RED);
        std::cout << "敌人：" << enemy.GetName() << std::endl;
        SetColor(ConsoleColor::WHITE);
        
        SetCursorPosition(hConsoleOut, 2, 11);
        std::cout << "HP：";
        Draw_Progress_Bar(5, 11, 20, enemy.GetHP(), enemy.GetMaxHP(), ConsoleColor::RED);
        SetCursorPosition(hConsoleOut, 27, 11);
        std::cout << enemy.GetHP() << "/" << enemy.GetMaxHP() << std::endl;
        
        // 绘制战斗日志
        DrawBattleLog();
        DrawFightOptions();
        // 处理输入
        HandleInput();
        Optionsrefresh();
        // 检查战斗是否结束
        if (enemy.GetHP() <= 0) {
            AddBattleLog("你击败了" + enemy.GetName() + "！");
            DrawBattleLog();
            Sleep(50);
            g_fightState.isRunning = false;
            return 1;
        }
        else if (player.GetHP() <= 0) {
            AddBattleLog("你被击败了！");
            AddBattleLog("出师未捷身先死！");
            DrawBattleLog();
            Sleep(50);
            g_fightState.isRunning = false;
            return 0;
        }
    }
    
    // 等待用户按键继续
    SetCursorPosition(hConsoleOut, 2, 25);
    std::cout << "战斗结束。按任意键继续..." << std::endl;
    _getch();

    if ( g_fightState.player->GetHP() > 0 && enemy.GetHP() > 0) {
        return 2; // 2表示成功逃跑
    }
    
    return 0;
}


// 绘制战斗选项
void DrawFightOptions() {
    // 选项列表
    std::vector<std::string> options = {"攻击", "技能", "物品", "逃跑"};
    
    
    // 绘制选项框
    SetCursorPosition(hConsoleOut, 2, 19);
    std::cout << "┌────────────┐";
    for (int i = 0; i < options.size(); ++i) {
        int yPos = 20 + i;
        SetCursorPosition(hConsoleOut, 2, yPos);
        std::cout << "│            │";
    }
    SetCursorPosition(hConsoleOut, 2, 24);
    std::cout << "└────────────┘";
    
    // 绘制所有选项
    for (int i = 0; i < options.size(); ++i) {
        int yPos = 20 + i;
        
        // 设置选项颜色
        if (i == g_fightState.selectedIndex) {
            SetColor(ConsoleColor::LIGHT_GREEN);
        } else {
            SetColor(ConsoleColor::WHITE);
        }
        
        SetCursorPosition(hConsoleOut, 5, yPos);
        std::cout << options[i];
        SetColor(ConsoleColor::WHITE);
    }
    
    // 绘制选择指示器
    SetCursorPosition(hConsoleOut, 3, 20 + g_fightState.selectedIndex);
    SetColor(ConsoleColor::LIGHT_RED);
    std::cout << "▶";
    SetColor(ConsoleColor::WHITE);
}

void Optionsrefresh() {
    // 重新绘制整个选项界面以更新选择状态
    DrawFightOptions();
}

// 处理输入
void HandleInput() {
    bool keyPressed = false;
    
    while (!keyPressed) {
        if (_kbhit()) { // 检查是否有按键按下
            int ch = _getch();
            
            if (ch == 0 || ch == 0xE0) { // 扩展键
                ch = _getch(); // 获取扩展键码
                switch (ch) {
                    case 72: // 上箭头
                        // 向上选择
                        g_fightState.selectedIndex = (g_fightState.selectedIndex - 1 + 4) % 4;
                        g_fightState.selectedAction = static_cast<FightAction>(g_fightState.selectedIndex);
                        keyPressed = true;
                        break;
                    case 80: // 下箭头
                        // 向下选择
                        g_fightState.selectedIndex = (g_fightState.selectedIndex + 1) % 4;
                        g_fightState.selectedAction = static_cast<FightAction>(g_fightState.selectedIndex);
                        keyPressed = true;
                        break;
                }
            } else {
                ch = tolower(ch);
                
                switch (ch) {
                    case 'w':
                        // 向上选择
                        g_fightState.selectedIndex = (g_fightState.selectedIndex - 1 + 4) % 4;
                        g_fightState.selectedAction = static_cast<FightAction>(g_fightState.selectedIndex);
                        keyPressed = true;
                        break;
                    case 's':
                        // 向下选择
                        g_fightState.selectedIndex = (g_fightState.selectedIndex + 1) % 4;
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
                // Boss阶段变化逻辑
                if (g_fightState.isBossFight) {
                    int currentHP = enemy->GetHP();
                    int maxHP = g_fightState.bossMaxHP;
                    int newPhase = 1;
                    
                    if (currentHP <= maxHP * 0.2 && g_fightState.bossPhase < 3) {
                        newPhase = 3;
                    } else if (currentHP <= maxHP * 0.5 && g_fightState.bossPhase < 2) {
                        newPhase = 2;
                    }
                    
                    if (newPhase > g_fightState.bossPhase) {
                        g_fightState.bossPhase = newPhase;
                        int oldDamage = enemy->GetAttack();
                        int oldMaxHP = enemy->GetMaxHP();
                        
                        // 增强Boss能力
                        enemy->SetAttack(oldDamage + 10 * newPhase);
                        enemy->SetMaxHp(oldMaxHP + 50 * newPhase);
                        enemy->SetHp(currentHP + 30 * newPhase);
                        
                        AddBattleLog("Boss进入阶段 " + std::to_string(newPhase) + "！力量大幅提升！");
                    }
                }
                
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
            const auto& skills = player->GetSkills();
            int totalSkills = skills.size();
            int totalPages = (totalSkills + g_fightState.skillsPerPage - 1) / g_fightState.skillsPerPage;
            
            bool skillSelected = false;
            while (!skillSelected) {
                ClearScreen();
                
                // 绘制技能菜单标题
                SetColor(ConsoleColor::LIGHT_YELLOW);
                SetCursorPosition(hConsoleOut, 25, 1);
                std::cout << "==================== 技能菜单 ====================";
                SetColor(ConsoleColor::WHITE);
                
                // 显示当前MP
                SetCursorPosition(hConsoleOut, 25, 3);
                std::cout << "当前MP: " << player->GetMP() << "/" << player->GetMaxMP();
                
                // 显示返回提示
                SetCursorPosition(hConsoleOut, 25, 5);
                std::cout << "[ESC] 返回战斗界面";
                
                if (skills.empty()) {
                    SetCursorPosition(hConsoleOut, 25, 7);
                    std::cout << "你还没有学习任何技能！";
                    SetCursorPosition(hConsoleOut, 25, 8);
                    std::cout << "按任意键返回...";
                    _getch();
                    break;
                }
                
                // 显示当前页的技能
                int startIndex = g_fightState.currentSkillPage * g_fightState.skillsPerPage;
                int endIndex = std::min(startIndex + g_fightState.skillsPerPage, totalSkills);
                
                for (int i = startIndex; i < endIndex; ++i) {
                    int displayIndex = i - startIndex;
                    int yPos = 7 + displayIndex * 3;
                    
                    // 设置技能颜色（MP不足显示为红色）
                    if (player->GetMP() < skills[i]->GetMpCost()) {
                        SetColor(ConsoleColor::LIGHT_RED);
                    } else {
                        SetColor(ConsoleColor::LIGHT_CYAN);
                    }
                    
                    SetCursorPosition(hConsoleOut, 25, yPos);
                    std::cout << (displayIndex + 1) << ". " << skills[i]->GetName() << " (消耗: " << skills[i]->GetMpCost() << " MP)";
                    
                    SetColor(ConsoleColor::WHITE);
                    SetCursorPosition(hConsoleOut, 27, yPos + 1);
                    std::cout << skills[i]->GetDescription();
                }
                
                // 显示页码信息
                SetCursorPosition(hConsoleOut, 25, 7 + g_fightState.skillsPerPage * 3);
                std::cout << "页码: " << (g_fightState.currentSkillPage + 1) << "/" << totalPages;
                
                // 显示操作提示
                SetCursorPosition(hConsoleOut, 25, 8 + g_fightState.skillsPerPage * 3);
                std::cout << "[W/↑] 上一页 | [S/↓] 下一页 | [0] 返回 | [1-" << (endIndex - startIndex) << "] 选择技能";
                
                // 处理输入
                int ch = _getch();
                
                if (ch == 0 || ch == 0xE0) { // 扩展键
                    ch = _getch(); // 获取扩展键码
                    switch (ch) {
                        case 72: // 上箭头
                            if (g_fightState.currentSkillPage > 0) {
                                g_fightState.currentSkillPage--;
                            }
                            break;
                        case 80: // 下箭头
                            if (g_fightState.currentSkillPage < totalPages - 1) {
                                g_fightState.currentSkillPage++;
                            }
                            break;
                    }
                } else if (ch == VK_ESCAPE) {
                    // ESC键返回战斗界面
                    g_fightState.currentSkillPage = 0;
                    skillSelected = true;
                } else {
                    ch = tolower(ch);
                    
                    if (ch == 'w' && g_fightState.currentSkillPage > 0) {
                        g_fightState.currentSkillPage--;
                    } else if (ch == 's' && g_fightState.currentSkillPage < totalPages - 1) {
                        g_fightState.currentSkillPage++;
                    } else if (ch == '0' || ch == VK_ESCAPE) {
                        // 返回战斗界面
                        g_fightState.currentSkillPage = 0;
                        skillSelected = true;
                    } else if (ch >= '1' && ch <= '9') {
                        int choice = ch - '0';
                        if (choice <= endIndex - startIndex) {
                            int skillIndex = startIndex + choice - 1;
                            Skill* selectedSkill = skills[skillIndex];
                            
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
                            
                            g_fightState.currentSkillPage = 0;
                            skillSelected = true;
                        }
                    }
                }
            }
            
            // 返回战斗界面时清屏并重绘
            ClearScreen();
            break;
        }
        case FightAction::Items: {
            ClearScreen();
            
            // 绘制物品菜单标题
            SetColor(ConsoleColor::LIGHT_YELLOW);
            SetCursorPosition(hConsoleOut, 25, 1);
            std::cout << "==================== 物品栏 ====================";
            SetColor(ConsoleColor::WHITE);
            
            // 显示返回提示
            SetCursorPosition(hConsoleOut, 25, 3);
            std::cout << "[ESC] 返回战斗界面";
            
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
                SetCursorPosition(hConsoleOut, 25, 5);
                std::cout << "背包中没有可用的消耗品";
                SetCursorPosition(hConsoleOut, 25, 6);
                std::cout << "按任意键返回...";
                _getch();
                break;
            }
            
            // 显示物品列表
            SetCursorPosition(hConsoleOut, 25, 5);
            std::cout << "===== 可用物品 ====";
            
            for (size_t i = 0; i < consumables.size(); i++) {
                SetColor(ConsoleColor::LIGHT_GREEN);
                SetCursorPosition(hConsoleOut, 27, 7 + i * 2);
                std::cout << (i + 1) << ". " << consumables[i]->item->name;
                
                SetColor(ConsoleColor::WHITE);
                SetCursorPosition(hConsoleOut, 29, 8 + i * 2);
                std::cout << consumables[i]->item->description;
            }
            
            // 显示操作提示
            SetCursorPosition(hConsoleOut, 25, 7 + consumables.size() * 2 + 1);
            std::cout << "[0] 返回 | [1-" << consumables.size() << "] 选择物品";
            
            // 处理物品选择
            int itemIndex = -1;
            while (true) {
                char itemChoice = _getch();
                
                if (itemChoice == '0' || itemChoice == VK_ESCAPE) {
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
            if (itemIndex == -1) {
                ClearScreen();
                break;
            }
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
            ClearScreen();
            break;
        }
        case FightAction::Run: {
            // 逃跑逻辑
            int runChance = rand() % 100;
            if (runChance < 50) { // 50%的逃跑成功率
                AddBattleLog("成功逃跑了！");
                g_fightState.isRunning = false;
                // 设置一个特殊的标记来表示逃跑成功
                // 在Fight函数中会检查这个特殊情况
                if (g_fightState.isBossFight) {
                    AddBattleLog("你逃离了Boss的攻击范围！");
                    // 特殊处理：返回2表示成功逃跑
                    g_fightState.isRunning = false;
                    // 这里不直接返回，而是在主循环结束后处理
                }
            }
            else {
                AddBattleLog("逃跑失败！");
                // 敌人反击
                int enemyAttack = enemy->GetAttack();
                int playerTotalDefense = player->GetDefend() + player->GetBag().getTotalDefense();
                int enemyDamage = CalculateDamage(enemyAttack, playerTotalDefense);
                
                *player -= enemyDamage;
                AddBattleLog(enemy->GetName() + " 对 " + player->GetName() + " 造成了 " + std::to_string(enemyDamage) + " 点伤害！");
                
                // 检查玩家是否在逃跑失败后死亡
                if (player->GetHP() <= 0) {
                    AddBattleLog("你在逃跑过程中被击败了！");
                    g_fightState.isRunning = false;
                }
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

void InitializeDefaultHandlers() {
    // 初始化默认处理器的实现
}