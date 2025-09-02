#include "Inventory.h"
#include <iostream>
#include <algorithm>

Inventory::Inventory() {}

void Inventory::addItem(std::unique_ptr<Item> item, int quantity) {
    // 如果物品可堆叠，则查找是否已有同类物品
    if (item->stackable) {
        for (auto& slot : slots) {
            if (slot.item->id == item->id) {
                slot.quantity += quantity;
                return; // 找到并增加数量后直接返回
            }
        }
    }

    // 如果不可堆叠，或者没有找到可堆叠的同类物品，则创建新槽位
    slots.push_back({std::move(item), quantity});
}


void Inventory::removeItem(int list_index, int quantity) {
    if (list_index < 0 || list_index >= slots.size()) {
        std::cout << "无效的物品编号。\n";
        return;
    }

    slots[list_index].quantity -= quantity;

    // 如果数量小于等于0，则从背包中移除该物品槽
    if (slots[list_index].quantity <= 0) {
        slots.erase(slots.begin() + list_index);
    }
}

int Inventory::getItemCount() const {
    return slots.size();
}

InventorySlot* Inventory::getSlotByIndex(int list_index) {
    if (list_index < 0 || list_index >= slots.size()) {
        return nullptr;
    }
    return &slots[list_index];
}

void Inventory::display() const {
    std::cout << "\n-------- 背包 ---------\n";
    if (slots.empty()) {
        std::cout << "你的背包是空的。\n";
        return;
    }

    // 分类存储
    std::map<ItemType, std::vector<std::pair<int, const InventorySlot*>>> categorizedItems;
    for (int i = 0; i < slots.size(); ++i) {
        categorizedItems[slots[i].item->type].push_back({i, &slots[i]});
    }

    int display_counter = 1;

    // 显示装备
    if (categorizedItems.count(ItemType::EQUIPMENT)) {
        std::cout << "\n[装备]\n";
        for (const auto& pair : categorizedItems[ItemType::EQUIPMENT]) {
            std::cout << " " << display_counter++ << ". " << pair.second->item->name << "\n";
        }
    }

    // 显示消耗品
    if (categorizedItems.count(ItemType::CONSUMABLE)) {
        std::cout << "\n[消耗品]\n";
        for (const auto& pair : categorizedItems[ItemType::CONSUMABLE]) {
            std::cout << " " << display_counter++ << ". " << pair.second->item->name << " (数量: " << pair.second->quantity << ")\n";
        }
    }

    // 显示材料
    if (categorizedItems.count(ItemType::MATERIAL)) {
        std::cout << "\n[材料]\n";
        for (const auto& pair : categorizedItems[ItemType::MATERIAL]) {
            std::cout << " " << display_counter++ << ". " << pair.second->item->name << " (数量: " << pair.second->quantity << ")\n";
        }
    }
    std::cout << "---------------------------\n";
}
//

//
