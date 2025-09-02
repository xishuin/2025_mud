#ifndef BAG_H
#define BAG_H

#include <string>
#include <memory>
#include <iostream>

// 前向声明Inventory、Equipment和Item类
class Inventory;
class Equipment;
class Item;

// struct CharacterStats {
//     // 基础信息
//     std::string name;
//     int level;
//     long long gold;
//
//     int totalAttack;
//     int totalDefense;
//     // int totalSpeed; // 已移除
//
//     int baseAttack;
//     int bonusAttack;
//     int baseDefense;
//     int bonusDefense;
//     // int baseSpeed; // 已移除
//     // int bonusSpeed; // 已移除
//
//     // 装备名称
//     std::string weaponName;
//     std::string helmetName;
//     std::string chestName;
//     std::string shoesName;
// };
// --- 结束新增 ---


class Bag {
public:
    Bag();
    ~Bag();
    void equipItem(int inventory_list_index);
    void sellItem(int inventory_list_index, int quantity = 1);
    // void showStatus() const;
    void showInventory() const;
    void gainItem(std::unique_ptr<Item> item, int quantity = 1);
    int getTotalAttack() const;
    int getTotalDefense() const;

    // int getTotalSpeed() const; // 已移除

    // --- 新增: 获取角色属性的 API 函数 ---
    // CharacterStats getCharacterStats() const;
    int getGold() const { return gold; }
    Inventory* getInventory() { return inventory; }

private:
    long long gold;
    Inventory* inventory;
    Equipment* equippedWeapon;
    Equipment* equippedHelmet;
    Equipment* equippedChest;
    Equipment* equippedShoes;
};

#include "Inventory.h"
#include "Item.h"

#endif //BAG_H