#include "Bag.h"
#include <iostream>

Bag::Bag(std::string name, int level)
    : name(std::move(name)),
      level(level),
      gold(1000),
      baseAttack(1),
      baseDefense(0),
      // baseSpeed(5), // 已移除
      equippedWeapon(nullptr),
      equippedHelmet(nullptr),
      equippedChest(nullptr),
      equippedShoes(nullptr) {
}

void Bag::equipItem(int inventory_list_index) {
    InventorySlot* slot = inventory.getSlotByIndex(inventory_list_index);
    if (!slot) {
        std::cout << "无效的物品编号。\n";
        return;
    }

    if (slot->item->type != ItemType::EQUIPMENT) {
        std::cout << "这个物品无法装备。\n";
        return;
    }

    Equipment* equipment_to_equip = static_cast<Equipment*>(slot->item.get());

    if (this->level < equipment_to_equip->levelRequirement) {
        std::cout << "你的等级不足，无法装备 " << equipment_to_equip->name << "。\n";
        return;
    }

    // 根据槽位进行装备，并打印卸下旧装备信息
    switch (equipment_to_equip->slot) {
        case EquipmentSlot::WEAPON:
            if (equippedWeapon) std::cout << "你卸下了 " << equippedWeapon->name << "。\n";
            equippedWeapon = equipment_to_equip;
            break;
        case EquipmentSlot::HELMET:
            if (equippedHelmet) std::cout << "你卸下了 " << equippedHelmet->name << "。\n";
            equippedHelmet = equipment_to_equip;
            break;
        case EquipmentSlot::CHEST:
            if (equippedChest) std::cout << "你卸下了 " << equippedChest->name << "。\n";
            equippedChest = equipment_to_equip;
            break;
        case EquipmentSlot::SHOES:
            if (equippedShoes) std::cout << "你卸下了 " << equippedShoes->name << "。\n";
            equippedShoes = equipment_to_equip;
            break;
        default:
            std::cout << "未知的装备槽位。\n";
            return; // 装备失败
    }
    std::cout << "你装备了 " << equipment_to_equip->name << "。\n";
}

void Bag::sellItem(int inventory_list_index, int quantity) {
    InventorySlot* slot = inventory.getSlotByIndex(inventory_list_index);
    if (!slot) {
        std::cout << "无效的物品编号。\n";
        return;
    }

    // 检查所有装备槽位
    if (slot->item.get() == equippedWeapon || slot->item.get() == equippedHelmet ||
        slot->item.get() == equippedChest || slot->item.get() == equippedShoes) {
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
    // std::cout << "速度:   " << getTotalSpeed() << " (" << baseSpeed << " + " << getTotalSpeed() - baseSpeed << ")\n"; // 已移除
    std::cout << "--- 装备 ---\n";
    std::cout << "武器: " << (equippedWeapon ? equippedWeapon->name : "无") << "\n";
    std::cout << "头盔: " << (equippedHelmet ? equippedHelmet->name : "无") << "\n";
    std::cout << "胸甲: " << (equippedChest ? equippedChest->name : "无") << "\n";
    std::cout << "鞋子: " << (equippedShoes ? equippedShoes->name : "无") << "\n";
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
    if (equippedWeapon) total += equippedWeapon->attackBonus;
    if (equippedHelmet) total += equippedHelmet->attackBonus;
    if (equippedChest) total += equippedChest->attackBonus;
    if (equippedShoes) total += equippedShoes->attackBonus;
    return total;
}

int Bag::getTotalDefense() const {
    int total = baseDefense;
    if (equippedWeapon) total += equippedWeapon->defenseBonus;
    if (equippedHelmet) total += equippedHelmet->defenseBonus;
    if (equippedChest) total += equippedChest->defenseBonus;
    if (equippedShoes) total += equippedShoes->defenseBonus;
    return total;
}

//这里是传状态的api
CharacterStats Bag::getCharacterStats() const {
    CharacterStats stats;

    // 填充基础信息
    stats.name = this->name;
    stats.level = this->level;
    stats.gold = this->gold;

    stats.baseAttack = this->baseAttack;
    stats.baseDefense = this->baseDefense;
    // stats.baseSpeed = this->baseSpeed; // 已移除

    // 计算并填充总体属性
    stats.totalAttack = this->getTotalAttack();
    stats.totalDefense = this->getTotalDefense();
    // stats.totalSpeed = this->getTotalSpeed(); // 已移除

    // 计算并填充加成属性
    stats.bonusAttack = stats.totalAttack - stats.baseAttack;
    stats.bonusDefense = stats.totalDefense - stats.baseDefense;
    // stats.bonusSpeed = stats.totalSpeed - stats.baseSpeed; // 已移除

    // 填充装备名称
    stats.weaponName = this->equippedWeapon ? this->equippedWeapon->name : "无";
    stats.helmetName = this->equippedHelmet ? this->equippedHelmet->name : "无";
    stats.chestName = this->equippedChest ? this->equippedChest->name : "无";
    stats.shoesName = this->equippedShoes ? this->equippedShoes->name : "无";

    return stats;
}