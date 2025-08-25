#ifndef BAG_H
#define BAG_H

#include "Inventory.h"
#include <string>

class Bag {
public:
    Bag(std::string name, int level = 1);

    // 装备一件物品 (通过背包列表的索引)
    void equipItem(int inventory_list_index);

    // 出售物品 (通过背包列表的索引)
    void sellItem(int inventory_list_index, int quantity = 1);

    // 显示角色/背包状态
    void showStatus() const;

    // 显示背包内容
    void showInventory() const;

    // 向背包添加物品 (用于加载和获取)
    void gainItem(std::unique_ptr<Item> item, int quantity = 1);

    // 计算总攻击力
    int getTotalAttack() const;

    // 计算总防御力
    int getTotalDefense() const;

    // 获取金币
    int getGold() const { return gold; }

    // 获取背包指针，用于外部操作
    Inventory* getInventory() { return &inventory; }

    // 为了简单起见，暂时将成员设为 public，方便 Game 类访问和修改
    // 在更复杂的项目中，会使用 getter/setter 和 friend 类
public:
    std::string name;
    int level;
    long long gold;

    int baseAttack;
    int baseDefense;

    Inventory inventory;

    // 指向背包中某件装备的裸指针，因为物品的所有权属于 inventory
    Equipment* equippedWeapon;
    Equipment* equippedArmor;
};


#endif //BAG_H

//
// Created by huaji on 25-8-25.
//
