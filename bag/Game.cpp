#include "Game.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <iomanip> // 需要包含 setw

Game::Game(const std::string& player_name) : m_bag(player_name), m_isRunning(true) {
    loadItemDatabase("items.json");
    loadPlayerData("player.json");
}

void Game::run() {
    while (m_isRunning) {
        showMainMenu();
        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "无效输入，请输入数字。\n";
            continue;
        }

        switch (choice) {
            case 1:
                m_bag.showStatus();
                break;
            case 2:
                showBackpackMenu();
                break;
            case 3:
                savePlayerData("player.json");
                break;
            case 4:
                savePlayerData("player.json");
                m_isRunning = false;
                break;
            default:
                std::cout << "无效的选择，请重试。\n";
        }
    }
    std::cout << "感谢游玩！\n";
}

void Game::loadItemDatabase(const std::string& filepath) {
    std::ifstream f(filepath);
    if (!f.is_open()) {
        std::cerr << "错误: 无法打开物品数据库文件 " << filepath << std::endl;
        exit(1);
    }
    m_itemDatabase = json::parse(f);
    std::cout << "物品数据库加载成功。\n";
}

void Game::loadPlayerData(const std::string& filepath) {
    std::ifstream f(filepath);
    if (!f.is_open()) {
        std::cout << "未找到玩家存档，将创建新角色。\n";
        m_bag.gainItem(createItemById(101), 1);
        m_bag.gainItem(createItemById(301), 5);
        return;
    }

    json data = json::parse(f);

    m_bag.name = data["name"];
    m_bag.level = data["level"];
    m_bag.gold = data["gold"];

    for (const auto& item_data : data["inventory"]) {
        int id = item_data["id"];
        int quantity = item_data["quantity"];
        m_bag.inventory.addItem(createItemById(id), quantity);
    }

    auto equip_by_id = [&](int item_id) {
        if (item_id == 0) return;
        for (int i = 0; i < m_bag.inventory.getItemCount(); ++i) {
             if(m_bag.inventory.getSlotByIndex(i) && m_bag.inventory.getSlotByIndex(i)->item->id == item_id){
                 m_bag.equipItem(i);
                 return;
             }
        }
    };

    if (data.contains("equipped_weapon") && !data["equipped_weapon"].is_null()) equip_by_id(data["equipped_weapon"]);
    if (data.contains("equipped_helmet") && !data["equipped_helmet"].is_null()) equip_by_id(data["equipped_helmet"]);
    if (data.contains("equipped_chest") && !data["equipped_chest"].is_null()) equip_by_id(data["equipped_chest"]);
    if (data.contains("equipped_shoes") && !data["equipped_shoes"].is_null()) equip_by_id(data["equipped_shoes"]);

    std::cout << "玩家 " << m_bag.name << " 的数据加载成功。\n";
}

void Game::savePlayerData(const std::string& filepath) {
    json data;
    data["name"] = m_bag.name;
    data["level"] = m_bag.level;
    data["gold"] = m_bag.gold;

    data["equipped_weapon"] = m_bag.equippedWeapon ? json(m_bag.equippedWeapon->id) : json(nullptr);
    data["equipped_helmet"] = m_bag.equippedHelmet ? json(m_bag.equippedHelmet->id) : json(nullptr);
    data["equipped_chest"] = m_bag.equippedChest ? json(m_bag.equippedChest->id) : json(nullptr);
    data["equipped_shoes"] = m_bag.equippedShoes ? json(m_bag.equippedShoes->id) : json(nullptr);

    data["inventory"] = json::array();
    for (int i = 0; i < m_bag.inventory.getItemCount(); ++i) {
        InventorySlot* slot = m_bag.inventory.getSlotByIndex(i);
        if (slot) {
             data["inventory"].push_back({
                {"id", slot->item->id},
                {"quantity", slot->quantity}
            });
        }
    }
    std::ofstream o(filepath);
    o << std::setw(4) << data << std::endl;
    std::cout << "数据已保存至 " << filepath << "\n";
}

std::unique_ptr<Item> Game::createItemById(int id) {
    std::string id_str = std::to_string(id);
    if (!m_itemDatabase.contains(id_str)) {
        return nullptr;
    }
    const auto& data = m_itemDatabase[id_str];
    std::string type = data["type"];

    if (type == "Equipment") {
        return std::make_unique<Equipment>(
            id, data["name"], data["description"], data["price"],
            stringToRarity(data["rarity"]), data["level_req"],
            data["attack"], data["defense"], data["speed"], // 确保这里有9个参数 + id = 10个
            stringToSlot(data["slot"])
        );
    } else if (type == "Consumable") {
        return std::make_unique<Consumable>(
            id, data["name"], data["description"], data["price"]
        );
    } else if (type == "Material") {
        return std::make_unique<Material>(
            id, data["name"], data["description"], data["price"]
        );
    }
    return nullptr;
}

void Game::showMainMenu() {
    std::cout << "\n===== 主菜单 =====\n";
    std::cout << "1. 查看状态\n";
    std::cout << "2. 打开背包\n";
    std::cout << "3. 保存游戏\n";
    std::cout << "4. 保存并退出\n";
    std::cout << "====================\n";
    std::cout << "请输入你的选择: ";
}

void Game::showBackpackMenu() {
    m_bag.showInventory();
    if (m_bag.inventory.getItemCount() == 0) return;

    std::cout << "请输入物品编号进行操作 (输入 0 返回): ";
    int choice;
    std::cin >> choice;

    if (std::cin.fail() || choice < 0 || choice > m_bag.inventory.getItemCount()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "无效输入。\n";
        return;
    }

    if (choice > 0) {
        handleBackpackAction(choice - 1);
    }
}

void Game::handleBackpackAction(int list_index) {
    InventorySlot* slot = m_bag.inventory.getSlotByIndex(list_index);
    if (!slot) return;

    std::cout << "\n你选择了: " << slot->item->name << "\n";
    slot->item->display();

    std::cout << "\n--- 操作 ---\n";
    std::cout << "1. 穿戴/使用\n";
    std::cout << "2. 出售\n";
    std::cout << "0. 返回\n";
    std::cout << "-----------\n";
    std::cout << "请输入你的选择: ";

    int choice;
    std::cin >> choice;
    if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        choice = -1;
    }

    switch (choice) {
        case 1:
            if (slot->item->type == ItemType::EQUIPMENT) {
                m_bag.equipItem(list_index);
            } else if (slot->item->type == ItemType::CONSUMABLE) {
                std::cout << "你使用了 " << slot->item->name << "。(效果待实现)\n";
                m_bag.inventory.removeItem(list_index, 1);
            } else {
                std::cout << "这个物品无法使用。\n";
            }
            break;
        case 2:
            m_bag.sellItem(list_index, 1);
            break;
        case 0:
            break;
        default:
            std::cout << "无效的选择。\n";
    }
}

Rarity Game::stringToRarity(const std::string& s) {
    if (s == "COMMON") return Rarity::COMMON;
    if (s == "UNCOMMON") return Rarity::UNCOMMON;
    if (s == "RARE") return Rarity::RARE;
    if (s == "EPIC") return Rarity::EPIC;
    if (s == "LEGENDARY") return Rarity::LEGENDARY;
    return Rarity::COMMON;
}

EquipmentSlot Game::stringToSlot(const std::string& s) {
    if (s == "Weapon") return EquipmentSlot::WEAPON;
    if (s == "Helmet") return EquipmentSlot::HELMET;
    if (s == "Chest") return EquipmentSlot::CHEST;
    if (s == "Shoes") return EquipmentSlot::SHOES;
    return EquipmentSlot::NONE;
}