#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>
#include <memory>

enum class ItemType {
    GENERIC,
    EQUIPMENT,
    CONSUMABLE,
    MATERIAL
};

enum class Rarity {
    COMMON,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY
};

inline std::string rarityToString(Rarity rarity) {
    switch (rarity) {
        case Rarity::COMMON: return "普通";
        case Rarity::UNCOMMON: return "非凡";
        case Rarity::RARE: return "稀有";
        case Rarity::EPIC: return "史诗";
        case Rarity::LEGENDARY: return "传说";
        default: return "未知";
    }
}


// 更新装备槽位
enum class EquipmentSlot {
    NONE,
    WEAPON,
    HELMET, // 新增
    CHEST,  // 将 ARMOR 改为 CHEST，更清晰
    SHOES   // 新增
};

// ... (Item, Consumable, Material 类保持不变) ...
class Item {
public:
    int id;
    std::string name;
    std::string description;
    int price;
    bool stackable;
    ItemType type;

    Item(int id, std::string name, std::string description, int price, bool stackable, ItemType type)
        : id(id), name(std::move(name)), description(std::move(description)), price(price), stackable(stackable), type(type) {}

    virtual ~Item() = default;

    virtual void display() const {
        std::cout << "名称: " << name << "\n";
        std::cout << "类型: 基础物品\n";
        std::cout << "描述: " << description << "\n";
        std::cout << "售价: " << price << " 金币\n";
    }
};


// 更新装备类
class Equipment : public Item {
public:
    Rarity rarity;
    int levelRequirement;
    int attackBonus;
    int defenseBonus;
    // int speedBonus; // 已移除
    EquipmentSlot slot;

    Equipment(int id, std::string name, std::string description, int price,
              Rarity rarity, int levelReq, int attack, int defense, /*int speed,*/ EquipmentSlot slot)
        : Item(id, std::move(name), std::move(description), price, false, ItemType::EQUIPMENT),
          rarity(rarity), levelRequirement(levelReq), attackBonus(attack), defenseBonus(defense), /*speedBonus(speed),*/ slot(slot) {}

    void display() const override {
        std::cout << "名称: " << name << " [" << rarityToString(rarity) << "]\n";
        std::cout << "类型: 装备\n";

        std::string slotName;
        switch(slot) {
            case EquipmentSlot::WEAPON: slotName = "武器"; break;
            case EquipmentSlot::HELMET: slotName = "头部"; break;
            case EquipmentSlot::CHEST:  slotName = "胸部"; break;
            case EquipmentSlot::SHOES:  slotName = "脚部"; break;
            default: slotName = "未知";
        }
        std::cout << "部位: " << slotName << "\n";

        if (attackBonus > 0) std::cout << "攻击力: +" << attackBonus << "\n";
        if (defenseBonus > 0) std::cout << "防御力: +" << defenseBonus << "\n";
        // if (speedBonus > 0) std::cout << "速度: +" << speedBonus << "\n"; // 已移除

        std::cout << "等级要求: " << levelRequirement << "\n";
        std::cout << "描述: " << description << "\n";
        std::cout << "售价: " << price << " 金币\n";
    }
};

class Consumable : public Item {
public:
    Consumable(int id, std::string name, std::string description, int price)
        : Item(id, std::move(name), std::move(description), price, true, ItemType::CONSUMABLE) {}

    void display() const override {
        std::cout << "名称: " << name << "\n";
        std::cout << "类型: 消耗品\n";
        std::cout << "描述: " << description << "\n";
        std::cout << "售价: " << price << " 金币\n";
    }
};

class Material : public Item {
public:
    Material(int id, std::string name, std::string description, int price)
        : Item(id, std::move(name), std::move(description), price, true, ItemType::MATERIAL) {}

    void display() const override {
        std::cout << "名称: " << name << "\n";
        std::cout << "类型: 材料\n";
        std::cout << "描述: " << description << "\n";
        std::cout << "售价: " << price << " 金币\n";
    }
};


#endif //ITEM_H