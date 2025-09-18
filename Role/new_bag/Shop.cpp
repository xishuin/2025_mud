#include "Shop.h"
#include <iostream>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

Shop::Shop(std::string name) : name(std::move(name)) {}

Shop::~Shop() {}

void Shop::addItem(std::unique_ptr<Item> item, int quantity) {
    // 检查是否已有该物品
    for (auto& [existingItem, count] : itemsForSale) {
        if (existingItem->id == item->id) {
            count += quantity;
            return;
        }
    }
    // 如果没有，添加新物品
    itemsForSale.emplace_back(std::move(item), quantity);
}

bool Shop::removeItem(int itemId, int quantity) {
    for (auto& [item, count] : itemsForSale) {
        if (item->id == itemId) {
            if (count >= quantity) {
                count -= quantity;
                if (count == 0) {
                    // 移除该物品
                    auto it = find_if(itemsForSale.begin(), itemsForSale.end(),
                        [itemId](const auto& pair) { return pair.first->id == itemId; });
                    if (it != itemsForSale.end()) {
                        itemsForSale.erase(it);
                    }
                }
                return true;
            }
            return false; // 数量不足
        }
    }
    return false; // 未找到物品
}

void Shop::displayItems() const {
    cout << "===== " << name << " ======" << endl;
    cout << "ID | 名称 | 价格 | 数量 | 描述" << endl;
    cout << "----------------------------------------" << endl;

    for (const auto& [item, quantity] : itemsForSale) {
        cout << left << setw(3) << item->id
             << setw(10) << item->name
             << setw(6) << item->price
             << setw(6) << quantity
             << item->description << endl;
    }
    cout << "========================================" << endl;
}

bool Shop::buyItem(int itemId, int quantity, Bag& buyerBag) {
    for (const auto& [item, count] : itemsForSale) {
        if (item->id == itemId) {
            if (count >= quantity) {
                int totalCost = item->price * quantity;
                if (buyerBag.spendGold(totalCost)) { // 调用spendGold方法扣钱
                    // 增加物品到买家背包
                    for (int i = 0; i < quantity; ++i) {
                        // 创建物品副本
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
                        buyerBag.gainItem(std::move(newItem), 1);
                    }

                    // 记录购买历史
                    buyHistory.emplace_back(itemId, quantity);

                    // 减少商店库存
                    removeItem(itemId, quantity);

                    return true;
                } else {
                    cout << "金币不足！" << endl;
                    return false;
                }
            } else {
                cout << "商店中该物品数量不足！" << endl;
                return false;
            }
        }
    }
    cout << "未找到该物品！" << endl;
    return false;
}

bool Shop::sellItem(int itemId, int quantity, Bag& sellerBag) {
    // 遍历卖家背包中的所有物品槽位
    int totalSlots = sellerBag.getInventory()->getItemCount();
    bool found = false;
    int soldQuantity = 0;
    int totalValue = 0;
    
    // 首先检查物品是否存在并计算总价值
    for (int i = 0; i < totalSlots; ++i) {
        InventorySlot* slot = sellerBag.getInventory()->getSlotByIndex(i);
        if (slot && slot->item->id == itemId && slot->quantity > 0) {
            found = true;
            int sellQty = std::min(quantity - soldQuantity, slot->quantity);
            if (sellQty > 0) {
                int itemValue = static_cast<int>(slot->item->price * 0.7); // 出售价格为买入价格的70%
                totalValue += itemValue * sellQty;
                soldQuantity += sellQty;
                
                if (soldQuantity >= quantity) {
                    break; // 已达到请求的出售数量
                }
            }
        }
    }
    
    if (!found) {
        cout << "你没有这个物品可以出售！" << endl;
        return false;
    }
    
    if (soldQuantity == 0) {
        cout << "无法出售指定数量的物品！" << endl;
        return false;
    }
    
    // 实际移除物品并添加金币
    int remaining = quantity;
    for (int i = 0; i < totalSlots && remaining > 0; ++i) {
        InventorySlot* slot = sellerBag.getInventory()->getSlotByIndex(i);
        if (slot && slot->item->id == itemId && slot->quantity > 0) {
            int removeQty = std::min(remaining, slot->quantity);
            sellerBag.getInventory()->removeItem(i, removeQty);
            remaining -= removeQty;
        }
    }
    
    // 添加金币到卖家
    sellerBag.addGold(totalValue);
    
    // 查找物品名称
    std::string itemName = "未知物品";
    for (int i = 0; i < totalSlots; ++i) {
        InventorySlot* slot = sellerBag.getInventory()->getSlotByIndex(i);
        if (slot && slot->item->id == itemId) {
            itemName = slot->item->name;
            break;
        }
    }
    
    cout << "成功出售了 " << soldQuantity << " 个 " << itemName << "，获得了 " << totalValue << " 金币！" << endl;
    
    return true;
}

void Shop::loadFromJson(const std::string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "无法打开商店数据文件: " << filename << endl;
        return;
    }

    json data = json::parse(file);
    name = data["name"];

    itemsForSale.clear();
    for (const auto& itemData : data["items"]) {
        int id = itemData["id"];
        string name = itemData["name"];
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
                id, name, description, price,
                static_cast<Rarity>(rarity), levelReq, attack, defense,
                static_cast<EquipmentSlot>(slot)
            );
        } else {
            item = std::make_unique<Item>(
                id, name, description, price,
                stackable, static_cast<ItemType>(type)
            );
        }

        addItem(std::move(item), quantity);
    }
}

void Shop::saveToJson(const std::string& filename) const {
    json data;
    data["name"] = name;

    json itemsJson = json::array();
    for (const auto& [item, quantity] : itemsForSale) {
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

        itemsJson.push_back(itemData);
    }

    data["items"] = itemsJson;

    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "无法保存商店数据到文件: " << filename << endl;
        return;
    }

    file << data.dump(4);
}