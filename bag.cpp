#include "Bag.h"
#include <iostream>

Bag::Bag(std::string name, int level)
    : name(std::move(name)),
      level(level),
      gold(1000), // 初始1000金币
      baseAttack(1), // 假设基础攻击为1
      baseDefense(0), // 假设基础防御为0
      equippedWeapon(nullptr),
      equippedArmor(nullptr) {
}

void Bag::equipItem(int inventory_list_index) {
    InventorySlot* slot = inventory.getSlotByIndex(inventory_list_index);
    if (!slot) {
        std::cout << "无效的物品编号。\n";
        return;
    }

    // 检查物品是否是装备
    if (slot->item->type != ItemType::EQUIPMENT) {
        std::cout << "这个物品无法装备。\n";
        return;
    }

    // 将 item 指针转换为 Equipment 指针
    Equipment* equipment_to_equip = static_cast<Equipment*>(slot->item.get());

    // 检查等级要求
    if (this->level < equipment_to_equip->levelRequirement) {
        std::cout << "你的等级不足，无法装备 " << equipment_to_equip->name << "。\n";
        return;
    }

    // 根据装备槽位进行装备
    if (equipment_to_equip->slot == EquipmentSlot::WEAPON) {
        // 如果已有装备，先卸下
        if (equippedWeapon) {
            std::cout << "你卸下了 " << equippedWeapon->name << "。\n";
        }
        equippedWeapon = equipment_to_equip;
        std::cout << "你装备了 " << equippedWeapon->name << "。\n";
    } else if (equipment_to_equip->slot == EquipmentSlot::ARMOR) {
        if (equippedArmor) {
            std::cout << "你卸下了 " << equippedArmor->name << "。\n";
        }
        equippedArmor = equipment_to_equip;
        std::cout << "你装备了 " << equippedArmor->name << "。\n";
    } else {
        std::cout << "未知的装备槽位。\n";
    }
}

void Bag::sellItem(int inventory_list_index, int quantity) {
    InventorySlot* slot = inventory.getSlotByIndex(inventory_list_index);
    if (!slot) {
        std::cout << "无效的物品编号。\n";
        return;
    }

    // 不允许出售已装备的物品
    if (slot->item.get() == equippedWeapon || slot->item.get() == equippedArmor) {
        std::cout << "你不能出售已装备的物品！请先卸下它。\n";
        return;
    }

    int sell_quantity = std::min(quantity, slot->quantity);
    int total_price = slot->item->price * sell_quantity;

    std::cout << "你出售了 " << sell_quantity << " 个 " << slot->item->name << "，获得了 " << total_price << " 金币。\n";

    this->gold += total_price;
    inventory.removeItem(inventory_list_index, sell_quantity);
}

void Bag::showStatus() const {
    std::cout << "\n--- 角色状态 ---\n";
    std::cout << "名称: " << name << "\n";
    std::cout << "等级: " << level << "\n";
    std::cout << "金币: " << gold << "\n";
    std::cout << "--- 属性 ---\n";
    std::cout << "攻击力: " << getTotalAttack() << " (" << baseAttack << " + " << getTotalAttack() - baseAttack << ")\n";
    std::cout << "防御力: " << getTotalDefense() << " (" << baseDefense << " + " << getTotalDefense() - baseDefense << ")\n";
    std::cout << "--- 装备 ---\n";
    std::cout << "武器: " << (equippedWeapon ? equippedWeapon->name : "无") << "\n";
    std::cout << "防具: " << (equippedArmor ? equippedArmor->name : "无") << "\n";
    std::cout << "----------------\n";
}

void Bag::showInventory() const {
    inventory.display();
}

void Bag::gainItem(std::unique_ptr<Item> item, int quantity) {
    std::cout << "你获得了 " << quantity << " 个 " << item->name << "。\n";
    inventory.addItem(std::move(item), quantity);
}

int Bag::getTotalAttack() const {
    int total = baseAttack;
    if (equippedWeapon) {
        total += equippedWeapon->attackBonus;
    }
    if (equippedArmor) {
        total += equippedArmor->attackBonus;
    }
    return total;
}

int Bag::getTotalDefense() const {
    int total = baseDefense;
    if (equippedWeapon) {
        total += equippedWeapon->defenseBonus;
    }
    if (equippedArmor) {
        total += equippedArmor->defenseBonus;
    }
    return total;
}

//
// Created by huaji on 25-8-25.
//
