#include "GameSystem.h"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "../Role/new_bag/Shop.h"
#include "../Role/new_bag/Chest.h"
#include "../Role/new_bag/Item.h"

using namespace std;

// 初始化单例实例为nullptr
GameSystem* GameSystem::instance = nullptr;

GameSystem::GameSystem() {}

GameSystem::~GameSystem() {
    // 清理资源
    shops.clear();
    chests.clear();
}

GameSystem* GameSystem::getInstance() {
    if (instance == nullptr) {
        instance = new GameSystem();
    }
    return instance;
}

void GameSystem::init() {
    std::cout << "游戏系统初始化中..." << std::endl;

    // 创建默认商店
    auto generalShop = std::make_unique<Shop>("通用商店");
    // 添加一些默认商品
    generalShop->addItem(std::make_unique<Item>(1, "生命药水", "恢复50点生命值", 50, true, ItemType::CONSUMABLE), 100);
    generalShop->addItem(std::make_unique<Item>(2, "魔法药水", "恢复30点魔法值", 30, true, ItemType::CONSUMABLE), 100);
    generalShop->addItem(std::make_unique<Equipment>(101, "铁剑", "一把普通的铁剑", 100, Rarity::COMMON, 1, 5, 0, EquipmentSlot::WEAPON), 20);
    generalShop->addItem(std::make_unique<Equipment>(102, "皮甲", "一件普通的皮甲", 80, Rarity::COMMON, 1, 0, 3, EquipmentSlot::CHEST), 20);
    shops.push_back(std::move(generalShop));

    // 创建默认宝箱
    auto chest1 = std::make_unique<Chest>(1, "初学者宝箱");
    chest1->addItem(std::make_unique<Item>(1, "生命药水", "恢复50点生命值", 50, true, ItemType::CONSUMABLE), 3);
    chest1->addItem(std::make_unique<Item>(2, "魔法药水", "恢复30点魔法值", 30, true, ItemType::CONSUMABLE), 3);
    chests.push_back(std::move(chest1));

    auto chest2 = std::make_unique<Chest>(2, "精英宝箱", 1001); // 需要钥匙ID 1001
    chest2->addItem(std::make_unique<Equipment>(103, "钢剑", "一把坚固的钢剑", 200, Rarity::UNCOMMON, 5, 10, 0, EquipmentSlot::WEAPON), 1);
    chest2->addItem(std::make_unique<Item>(3, "高级生命药水", "恢复100点生命值", 100, true, ItemType::CONSUMABLE), 2);
    chests.push_back(std::move(chest2));

    std::cout << "游戏系统初始化完成！" << std::endl;
}

void GameSystem::addShop(std::unique_ptr<Shop> shop) {
    shops.push_back(std::move(shop));
}

void GameSystem::addChest(std::unique_ptr<Chest> chest) {
    chests.push_back(std::move(chest));
}

Shop* GameSystem::findShop(const std::string& name) {
    for (const auto& shop : shops) {
        if (shop->getName() == name) {
            return shop.get();
        }
    }
    return nullptr;
}

Chest* GameSystem::findChest(int id) {
    for (const auto& chest : chests) {
        if (chest->getId() == id) {
            return chest.get();
        }
    }
    return nullptr;
}

void GameSystem::loadShops(const std::string& directory) {
    // 简化版：不加载文件
    std::cout << "跳过加载商店数据" << std::endl;
}

void GameSystem::loadChests(const std::string& directory) {
    // 简化版：不加载文件
    std::cout << "跳过加载宝箱数据" << std::endl;
}

void GameSystem::saveShops(const std::string& directory) const {
    // 简化版：不保存文件
    std::cout << "跳过保存商店数据" << std::endl;
}

void GameSystem::saveChests(const std::string& directory) const {
    // 简化版：不保存文件
    std::cout << "跳过保存宝箱数据" << std::endl;
}