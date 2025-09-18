#include "MainShell.h"
#include <conio.h>
#include <iostream>
#include <windows.h>
#include "GameSystem.h"



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
            case 's':
                ModelState=Model_t::ShopModel;
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
    SetCursorPosition(hConsoleOut,StatePosX,StatePosY+4);
    std::cout<<"等级"<<player.GetLevel()<<":"<<player.GetEXP()<<"\\"<<player.GetMaxEXP();
    SetCursorPosition(hConsoleOut,StatePosX,StatePosY+5);
    SetColor(ConsoleColor::LIGHT_CYAN);
    std::cout <<"M:map       ";
    SetColor(ConsoleColor::YELLOW);
    std::cout<<"B:Box        ";
    SetColor(ConsoleColor::GREEN);
    std::cout<<"S:Shop       ";
    SetColor(ConsoleColor::RED);
    std::cout<<"Q:quit";
    SetColor(ConsoleColor::WHITE);
    //Plot的地方
    DrawPlots();
    if (player.GetHP()==0) {
        running=false;
        return;
    }
    //Plot的地方
    MainKeyInput();
}

void DrawBagUI() {
    while (ModelState == Model_t::BoxModel) {
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

        std::cout << "\n操作选项: " << std::endl;
        std::cout << "1. 选择物品" << std::endl;
        std::cout << "2. 返回主菜单" << std::endl;

        int ch = _getch();
        if (ch == 27) { // ESC键
            ModelState = Model_t::MainModel;
            break;
        }
        int choice = ch - '0';
        if (choice == 2) {
            ModelState = Model_t::MainModel;
            break;
        } else if (choice == 1) {
            std::cout << "\n请输入物品编号: ";
            int itemIndex;
            std::cin >> itemIndex;
            itemIndex--; // 转换为0索引

            InventorySlot* slot = player.GetBag().getInventory()->getSlotByIndex(itemIndex);
            if (!slot) {
                std::cout << "无效的物品编号。\n";
                _getch();
                continue;
            }

            // 显示物品详细信息
            std::cout << "\n--- 物品详情 ---\n";
            slot->item->display();
            std::cout << "数量: " << slot->quantity << "\n";
            
            // 根据物品类型提供不同选项
            if (slot->item->type == ItemType::EQUIPMENT) {
                Equipment* equipment = static_cast<Equipment*>(slot->item.get());
                std::cout << "\n操作: " << std::endl;
                std::cout << "1. 装备" << std::endl;
                std::cout << "2. 出售" << std::endl;
                std::cout << "3. 返回" << std::endl;
                
                int action = _getch();
                if (action == 27) { // ESC键
                    continue;
                }
                action = action - '0';
                if (action == 1) {
                    // 检查等级要求
                    if (equipment->CanEquip(player.GetLevel())) {
                        player.GetBag().equipItem(itemIndex);
                    } else {
                        std::cout << "等级不足，无法装备！\n";
                    }
                    _getch();
                } else if (action == 2) {
                    player.GetBag().sellItem(itemIndex, 1);
                    _getch();
                }
            } else if (slot->item->type == ItemType::CONSUMABLE) {
                Consumable* consumable = static_cast<Consumable*>(slot->item.get());
                std::cout << "\n操作: " << std::endl;
                std::cout << "1. 使用" << std::endl;
                std::cout << "2. 出售" << std::endl;
                std::cout << "3. 返回" << std::endl;
                
                int action = _getch();
                if (action == 27) { // ESC键
                    continue;
                }
                action = action - '0';
                if (action == 1) {
                    consumable->Use(&player);
                    player.GetBag().getInventory()->removeItem(itemIndex, 1);
                    _getch();
                } else if (action == 2) {
                    player.GetBag().sellItem(itemIndex, 1);
                    _getch();
                }
            } else {
                // 材料类物品
                std::cout << "\n操作: " << std::endl;
                std::cout << "1. 出售" << std::endl;
                std::cout << "2. 返回" << std::endl;
                
                int action = _getch();
                if (action == 27) { // ESC键
                    continue;
                }
                action = action - '0';
                if (action == 1) {
                    player.GetBag().sellItem(itemIndex, 1);
                    _getch();
                }
            }
        }
    }
}

void DrawPlots() {
    // 检查剧情是否活跃
    if (g_storySystem.isActive()) {
        // 绘制剧情文本
        const auto& currentNode = g_storySystem.getCurrentNode();
        std::string nodeText = currentNode.getText();
        SetCursorPosition(hConsoleOut, 2, 1);
        
        // 检查是否是Boss战相关剧情
        if (nodeText.find("Boss战") != std::string::npos ||
            nodeText.find("熔岩巨兽") != std::string::npos ||
            nodeText.find("冰霜巨龙") != std::string::npos ||
            nodeText.find("暗影领主") != std::string::npos) {
            SetColor(ConsoleColor::LIGHT_RED);
            std::cout << "\n【警告：Boss战即将开始！】\n" << std::endl;
            SetColor(ConsoleColor::YELLOW);
            std::cout << "剧情：" << nodeText << std::endl;
            SetColor(ConsoleColor::LIGHT_RED);
            std::cout << "\n【Boss拥有强大的力量和特殊能力，请注意应对！】\n" << std::endl;
            SetColor(ConsoleColor::WHITE);
        } else {
            std::cout << "\n剧情：" << nodeText << std::endl;
        }
        // 绘制选项
        const auto& choices = currentNode.getChoices();
        if (!choices.empty()) {
            SetCursorPosition(hConsoleOut, 2, 12);
            std::cout << "请选择：" << std::endl;
            for (size_t i = 0; i < choices.size(); ++i) {
                SetCursorPosition(hConsoleOut, 4, 13 + i);
                std::cout << i + 1 << ". " << choices[i]->text << std::endl;
            }
            if (int ch = _getch()) {
                ch-='0';
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
        // 剧情未激活，显示调试信息
        SetCursorPosition(hConsoleOut, 2, 1);
        std::cout << "剧情系统未激活。按P键开始剧情..." << std::endl;
        
        // 检查是否按下P键手动触发剧情
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 'p' || ch == 'P') {
                g_storySystem.startStory(0);
            }
        }
    }
}

void DrawShopUI() {
    while (ModelState == Model_t::ShopModel) {
        ClearScreen();
        std::cout << "================================" << std::endl;
        std::cout << "           商店系统              " << std::endl;
        std::cout << "================================" << std::endl;

        // 获取游戏系统中的通用商店
        Shop* generalShop = GameSystem::getInstance()->findShop("通用商店");
        
        if (!generalShop) {
            std::cout << "无法找到商店！" << std::endl;
            std::cout << "按任意键返回...";
            _getch();
            ModelState = Model_t::MainModel;
            break;
        }

        // 显示玩家金币
        std::cout << "玩家金币: " << player.GetBag().getGold() << std::endl;
        std::cout << "" << std::endl;

        // 显示商店商品
        generalShop->displayItems();

        // 显示操作选项
        std::cout << "\n操作选项: " << std::endl;
        std::cout << "1. 购买物品" << std::endl;
        std::cout << "2. 出售物品" << std::endl;
        std::cout << "3. 返回主菜单" << std::endl;

        int ch = _getch();
        if (ch == 27) { // ESC键
            ModelState = Model_t::MainModel;
            break;
        }
        int choice = ch - '0';
        
        if (choice == 3) {
            ModelState = Model_t::MainModel;
            break;
        } else if (choice == 1) {
            // 购买物品
            std::cout << "\n请输入要购买的物品ID (ESC退出): ";
            int itemId;
            std::cin >> itemId;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                if (_getch() == 27) { // 检查是否按下ESC键
                    continue;
                }
            }
            
            std::cout << "请输入购买数量: ";
            int quantity;
            std::cin >> quantity;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                continue;
            }
            
            // 调用商店的购买方法
            bool success = generalShop->buyItem(itemId, quantity, player.GetBag());
            
            if (success) {
                std::cout << "购买成功！" << std::endl;
            } else {
                std::cout << "购买失败！" << std::endl;
            }
            std::cout << "按任意键继续...";
            _getch();
        } else if (choice == 2) {
            // 出售物品
            ClearScreen();
            std::cout << "================================" << std::endl;
            std::cout << "           出售物品              " << std::endl;
            std::cout << "================================" << std::endl;
            
            // 显示玩家背包中的物品
            std::cout << "你的背包: " << std::endl;
            player.GetBag().getInventory()->display();
            
            std::cout << "\n请输入要出售的物品ID (ESC退出): ";
            int itemId;
            std::cin >> itemId;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                if (_getch() == 27) { // 检查是否按下ESC键
                    continue;
                }
            }
            
            std::cout << "请输入出售数量: ";
            int quantity;
            std::cin >> quantity;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                continue;
            }
            
            // 调用商店的出售方法
            bool success = generalShop->sellItem(itemId, quantity, player.GetBag());
            
            if (success) {
                std::cout << "出售成功！" << std::endl;
            } else {
                std::cout << "出售失败！" << std::endl;
            }
            std::cout << "按任意键继续...";
            _getch();
        }
    }
}