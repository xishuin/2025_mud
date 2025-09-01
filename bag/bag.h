#ifndef BAG_H
#define BAG_H

#include "Inventory.h"
#include <string>

struct CharacterStats {
    // 基础信息
    std::string name;
    int level;
    long long gold;

    int totalAttack;
    int totalDefense;
    // int totalSpeed; // 已移除

    int baseAttack;
    int bonusAttack;
    int baseDefense;
    int bonusDefense;
    // int baseSpeed; // 已移除
    // int bonusSpeed; // 已移除

    // 装备名称
    std::string weaponName;
    std::string helmetName;
    std::string chestName;
    std::string shoesName;
};
// --- 结束新增 ---


class Bag {
public:
    Bag(std::string name, int level = 1);

    void equipItem(int inventory_list_index);
    void sellItem(int inventory_list_index, int quantity = 1);
    void showStatus() const;
    void showInventory() const;
    void gainItem(std::unique_ptr<Item> item, int quantity = 1);

    int getTotalAttack() const;
    int getTotalDefense() const;
    // int getTotalSpeed() const; // 已移除

    // --- 新增: 获取角色属性的 API 函数 ---
    CharacterStats getCharacterStats() const;


    int getGold() const { return gold; }
    Inventory* getInventory() { return &inventory; }

public:
    std::string name;
    int level;
    long long gold;

    int baseAttack;
    int baseDefense;
    // int baseSpeed; // 已移除

    Inventory inventory;

    Equipment* equippedWeapon;
    Equipment* equippedHelmet;
    Equipment* equippedChest;
    Equipment* equippedShoes;
};

#endif //BAG_H