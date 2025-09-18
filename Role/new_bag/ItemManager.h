#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "Item.h"
#include "json.hpp"

using json = nlohmann::json;

class ItemManager {
private:
    std::unordered_map<int, std::unique_ptr<Item>> items;
    static ItemManager* instance;

    ItemManager() = default;

public:
    static ItemManager* getInstance() {
        if (!instance) {
            instance = new ItemManager();
        }
        return instance;
    }

    void loadItemsFromJson(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "无法打开物品数据文件: " << filename << std::endl;
            return;
        }

        json data = json::parse(file);
        items.clear();

        // 只加载前24个物品
        for (int i = 1; i <= 24; ++i) {
            if (data.find(std::to_string(i)) == data.end()) {
                continue;
            }

            const auto& itemData = data[std::to_string(i)];
            int id = i;
            std::string name = itemData["name"];
            std::string description = itemData["description"];
            int price = itemData["price"];
            std::string typeStr = itemData["type"];
            ItemType type = ItemType::NONE;

            if (typeStr == "Equipment") {
                type = ItemType::EQUIPMENT;
                std::string slotStr = itemData["slot"];
                EquipmentSlot slot = EquipmentSlot::NONE;
                if (slotStr == "Weapon") slot = EquipmentSlot::WEAPON;
                else if (slotStr == "Chest") slot = EquipmentSlot::CHEST;
                else if (slotStr == "Helmet") slot = EquipmentSlot::HELMET;
                else if (slotStr == "Shield") slot = EquipmentSlot::SHIELD;

                std::string rarityStr = itemData["rarity"];
                Rarity rarity = Rarity::COMMON;
                if (rarityStr == "UNCOMMON") rarity = Rarity::UNCOMMON;
                else if (rarityStr == "RARE") rarity = Rarity::RARE;
                else if (rarityStr == "EPIC") rarity = Rarity::EPIC;
                else if (rarityStr == "LEGENDARY") rarity = Rarity::LEGENDARY;
                else if (rarityStr == "MYTHIC") rarity = Rarity::MYTHIC;
                else if (rarityStr == "DIVINE") rarity = Rarity::DIVINE;
                else if (rarityStr == "GODLY") rarity = Rarity::GODLY;

                int levelReq = itemData["level_req"];
                int attack = itemData["attack"];
                int defense = itemData["defense"];

                items[id] = std::make_unique<Equipment>(
                    id, name, description, price,
                    rarity, levelReq, attack, defense, slot
                );
            } else if (typeStr == "Consumable") {
                type = ItemType::CONSUMABLE;
                bool stackable = itemData.value("stackable", true);
                items[id] = std::make_unique<Item>(
                    id, name, description, price, stackable, type
                );
            } else if (typeStr == "Material") {
                type = ItemType::MATERIAL;
                bool stackable = itemData.value("stackable", true);
                items[id] = std::make_unique<Item>(
                    id, name, description, price, stackable, type
                );
            }
        }

        std::cout << "成功加载了 " << items.size() << " 个物品数据" << std::endl;
    }

    std::unique_ptr<Item> createRandomItemByChapter(int chapter) {
        // 根据章节确定物品ID范围
        int minId = (chapter - 1) * 4 + 1;
        int maxId = chapter * 4;

        // 确保不超过24个物品
        maxId = std::min(maxId, 24);
        minId = std::min(minId, maxId);

        // 随机生成物品ID
        int randomId = minId + rand() % (maxId - minId + 1);

        // 查找并创建物品副本
        if (items.find(randomId) != items.end()) {
            Item* originalItem = items[randomId].get();
            std::unique_ptr<Item> newItem;

            if (originalItem->type == ItemType::EQUIPMENT) {
                Equipment* originalEquip = static_cast<Equipment*>(originalItem);
                newItem = std::make_unique<Equipment>(
                    originalEquip->id, originalEquip->name, originalEquip->description, originalEquip->price,
                    originalEquip->rarity, originalEquip->levelRequirement, originalEquip->attackBonus,
                    originalEquip->defenseBonus, originalEquip->slot
                );
            } else {
                newItem = std::make_unique<Item>(
                    originalItem->id, originalItem->name, originalItem->description, originalItem->price,
                    originalItem->stackable, originalItem->type
                );
            }

            return newItem;
        }

        return nullptr;
    }
};

// 初始化静态成员
ItemManager* ItemManager::instance = nullptr;