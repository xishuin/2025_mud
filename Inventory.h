#ifndef INVENTORY_H
#define INVENTORY_H

#include "Item.h"
#include <vector>
#include <map>

// 用于存储背包中的物品项
struct InventorySlot {
    std::unique_ptr<Item> item;
    int quantity;
};

class Inventory {
public:
    Inventory();

    // 添加物品到背包
    void addItem(std::unique_ptr<Item> item, int quantity = 1);

    // 通过索引移除物品
    void removeItem(int list_index, int quantity = 1);

    // 显示背包内容
    void display() const;

    // 获取背包中物品的总数
    int getItemCount() const;

    // 根据显示的列表索引获取物品槽
    InventorySlot* getSlotByIndex(int list_index);

private:
    std::vector<InventorySlot> slots;
};

#endif //INVENTORY_H

//
// Created by huaji on 25-8-25.
//

