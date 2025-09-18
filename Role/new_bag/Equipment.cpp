#include "Item.h"
#include <iostream>

// Equipment类的一些辅助方法可以在这里实现
// 例如检查装备是否可以装备到特定等级的玩家
bool Equipment::CanEquip(int playerLevel) const {
    return playerLevel >= levelRequirement;
}

// 显示装备属性的格式化输出
void Equipment::ShowStats() const {
    std::cout << "装备属性：" << std::endl;
    if (attackBonus > 0) {
        std::cout << "  攻击加成: +" << attackBonus << std::endl;
    }
    if (defenseBonus > 0) {
        std::cout << "  防御加成: +" << defenseBonus << std::endl;
    }
    std::cout << "  等级要求: " << levelRequirement << std::endl;
}