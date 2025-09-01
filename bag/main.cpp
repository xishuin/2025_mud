#include "Game.h"
#include <iostream>

//API例子
void displayCharacterPanel(const Bag& playerBag) {
    CharacterStats stats = playerBag.getCharacterStats();
    std::cout << "\n\n--- ==== 角色面板 ==== ---\n";
    std::cout << "  姓名: " << stats.name << " (等级 " << stats.level << ")\n";
    std::cout << "  金币: " << stats.gold << "\n";
    std::cout << "-------------------------------------\n";
    std::cout << "  攻击: " << stats.totalAttack
              << " (" << stats.baseAttack << " + " << stats.bonusAttack << ")\n";
    std::cout << "  防御: " << stats.totalDefense
              << " (" << stats.baseDefense << " + " << stats.bonusDefense << ")\n";
    std::cout << "-------------------------------------\n";
    std::cout << "  武器: " << stats.weaponName << "\n";
    std::cout << "  头盔: " << stats.helmetName << "\n";
    std::cout << "  胸甲: " << stats.chestName << "\n";
    std::cout << "  鞋子: " << stats.shoesName << "\n";
    std::cout << "--- ============================== ---\n";
}

// 只保留这个 main 函数
int main() {
    system("chcp 65001");
    system("cls");

    std::cout << "欢迎来到背包管理系统!\n";
    // displayCharacterPanel(); //API示例，不调用
    Game game("冒险者");
    game.run();

    return 0;
}