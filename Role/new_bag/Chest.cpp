#include "Chest.h"
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "../Person_Skill_Class/Person.h"
#include "bag.h"

using json = nlohmann::json;
using namespace std;

Chest::Chest(int id, std::string name, int requiredKeyId) : id(id), name(std::move(name)), isOpen(false), requiredKeyId(requiredKeyId) {}

Chest::~Chest() {}

void Chest::addItem(std::unique_ptr<Item> item, int quantity) {
    // 检查是否已有该物品
    for (auto& [existingItem, count] : contents) {
        if (existingItem->id == item->id) {
            count += quantity;
            return;
        }
    }
    // 如果没有，添加新物品
    contents.emplace_back(std::move(item), quantity);
}

bool Chest::removeItem(int itemId, int quantity) {
    for (auto& [item, count] : contents) {
        if (item->id == itemId) {
            if (count >= quantity) {
                count -= quantity;
                if (count == 0) {
                    // 移除该物品
                    auto it = find_if(contents.begin(), contents.end(),
                        [itemId](const auto& pair) { return pair.first->id == itemId; });
                    if (it != contents.end()) {
                        contents.erase(it);
                    }
                }
                return true;
            }
            return false; // 数量不足
        }
    }
    return false; // 未找到物品
}

bool Chest::open(Player& player) {
    if (isOpen) {
        cout << "宝箱已经被打开过了！" << endl;
        return false;
    }

    // 检查是否需要钥匙
    if (requiredKeyId > 0) {
        // 检查玩家是否有钥匙
        // 假设Player类有一个hasItem方法
        // if (!player.hasItem(requiredKeyId)) {
        //     cout << "你没有打开这个宝箱所需的钥匙！" << endl;
        //     return false;
        // }
        // 简化实现
        bool hasKey = false;
        // 这里需要访问玩家背包
        cout << "检查钥匙功能尚未实现，假设玩家有钥匙" << endl;
        hasKey = true;

        if (!hasKey) {
            cout << "你没有打开这个宝箱所需的钥匙！" << endl;
            return false;
        }
    }

    // 打开宝箱
    isOpen = true;
    cout << "你打开了" << name << "！" << endl;

    // 给玩家物品
    for (const auto& [item, quantity] : contents) {
        cout << "获得了" << quantity << "个" << item->name << "！" << endl;
        // 添加物品到玩家背包
        // 假设Player类有一个getBag方法
        // Bag& bag = player.getBag();
        // 简化实现
        // 这里需要创建物品副本
        for (int i = 0; i < quantity; ++i) {
            std::unique_ptr<Item> newItem;
            if (item->type == ItemType::EQUIPMENT) {
                auto* equip = static_cast<Equipment*>(item.get());
                newItem = std::make_unique<Equipment>(
                    equip->id, equip->name, equip->description, equip->price,
                    equip->rarity, equip->levelRequirement, equip->attackBonus,
                    equip->defenseBonus, equip->slot
                );
            } else {
                newItem = std::make_unique<Item>(
                    item->id, item->name, item->description, item->price,
                    item->stackable, item->type
                );
            }
            // 获取玩家背包并添加物品
            Bag& bag = player.GetBag();
            bag.gainItem(std::move(newItem), 1);
        }
    }

    // 清空宝箱
    contents.clear();

    return true;
}

void Chest::loadFromJson(const std::string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开宝箱数据文件: " << filename << endl;
        return;
    }

    json data = json::parse(file);
    id = data["id"];
    name = data["name"];
    isOpen = data["isOpen"];
    requiredKeyId = data["requiredKeyId"];

    contents.clear();
    for (const auto& itemData : data["contents"]) {
        int itemId = itemData["id"];
        string itemName = itemData["name"];
        string description = itemData["description"];
        int price = itemData["price"];
        bool stackable = itemData["stackable"];
        int type = itemData["type"];
        int quantity = itemData["quantity"];

        std::unique_ptr<Item> item;
        if (type == static_cast<int>(ItemType::EQUIPMENT)) {
            int rarity = itemData["rarity"];
            int levelReq = itemData["levelRequirement"];
            int attack = itemData["attackBonus"];
            int defense = itemData["defenseBonus"];
            int slot = itemData["slot"];

            item = std::make_unique<Equipment>(
                itemId, itemName, description, price,
                static_cast<Rarity>(rarity), levelReq, attack, defense,
                static_cast<EquipmentSlot>(slot)
            );
        } else {
            item = std::make_unique<Item>(
                itemId, itemName, description, price,
                stackable, static_cast<ItemType>(type)
            );
        }

        addItem(std::move(item), quantity);
    }
}

void Chest::saveToJson(const std::string& filename) const {
    json data;
    data["id"] = id;
    data["name"] = name;
    data["isOpen"] = isOpen;
    data["requiredKeyId"] = requiredKeyId;

    json contentsJson = json::array();
    for (const auto& [item, quantity] : contents) {
        json itemData;
        itemData["id"] = item->id;
        itemData["name"] = item->name;
        itemData["description"] = item->description;
        itemData["price"] = item->price;
        itemData["stackable"] = item->stackable;
        itemData["type"] = static_cast<int>(item->type);
        itemData["quantity"] = quantity;

        if (item->type == ItemType::EQUIPMENT) {
            auto* equip = static_cast<Equipment*>(item.get());
            itemData["rarity"] = static_cast<int>(equip->rarity);
            itemData["levelRequirement"] = equip->levelRequirement;
            itemData["attackBonus"] = equip->attackBonus;
            itemData["defenseBonus"] = equip->defenseBonus;
            itemData["slot"] = static_cast<int>(equip->slot);
        }

        contentsJson.push_back(itemData);
    }

    data["contents"] = contentsJson;

    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "无法保存宝箱数据到文件: " << filename << endl;
        return;
    }

    file << data.dump(4);
}