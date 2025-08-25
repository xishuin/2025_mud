
#ifndef ITEM_H
#define ITEM_H

#include <iostream>
#include <string>
#include <memory>

// 物品类型
enum class ItemType {
    GENERIC,
    EQUIPMENT,
    CONSUMABLE,
    MATERIAL
};

// 装备稀有度
enum class Rarity {
    COMMON,
    UNCOMMON,
    RARE,
    EPIC,
    LEGENDARY
};

// 装备槽位
enum class EquipmentSlot {
    NONE,
    WEAPON,
    ARMOR
};

// Rarity 和 EquipmentSlot 的辅助函数，用于字符串转换
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

// 物品基类
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

    // 使用虚析构函数以确保派生类正确析构
    virtual ~Item() = default;

    // 虚函数，用于显示物品的详细信息
    virtual void display() const {
        std::cout << "名称: " << name << "\n";
        std::cout << "类型: 基础物品\n";
        std::cout << "描述: " << description << "\n";
        std::cout << "售价: " << price << " 金币\n";
    }
};

// 装备类
class Equipment : public Item {
public:
    Rarity rarity;
    int levelRequirement;
    int attackBonus;
    int defenseBonus;
    EquipmentSlot slot;

    Equipment(int id, std::string name, std::string description, int price,
              Rarity rarity, int levelReq, int attack, int defense, EquipmentSlot slot)
        : Item(id, std::move(name), std::move(description), price, false, ItemType::EQUIPMENT), // 装备默认不可堆叠
          rarity(rarity), levelRequirement(levelReq), attackBonus(attack), defenseBonus(defense), slot(slot) {}

    void display() const override {
        std::cout << "名称: " << name << " [" << rarityToString(rarity) << "]\n";
        std::cout << "类型: 装备\n";
        if (slot == EquipmentSlot::WEAPON) {
            std::cout << "部位: 武器\n";
            std::cout << "攻击力: +" << attackBonus << "\n";
        } else if (slot == EquipmentSlot::ARMOR) {
            std::cout << "部位: 防具\n";
            std::cout << "防御力: +" << defenseBonus << "\n";
        }
        std::cout << "等级要求: " << levelRequirement << "\n";
        std::cout << "描述: " << description << "\n";
        std::cout << "售价: " << price << " 金币\n";
    }
};

// 消耗品类
class Consumable : public Item {
public:
    Consumable(int id, std::string name, std::string description, int price)
        : Item(id, std::move(name), std::move(description), price, true, ItemType::CONSUMABLE) {} // 消耗品默认可堆叠

    void display() const override {
        std::cout << "名称: " << name << "\n";
        std::cout << "类型: 消耗品\n";
        std::cout << "描述: " << description << "\n";
        std::cout << "售价: " << price << " 金币\n";
    }
};

// 材料类
class Material : public Item {
public:
    Material(int id, std::string name, std::string description, int price)
        : Item(id, std::move(name), std::move(description), price, true, ItemType::MATERIAL) {} // 材料默认可堆叠

    void display() const override {
        std::cout << "名称: " << name << "\n";
        std::cout << "类型: 材料\n";
        std::cout << "描述: " << description << "\n";
        std::cout << "售价: " << price << " 金币\n";
    }
};

#endif //ITEM_H
//
// Created by huaji on 25-8-25.
//