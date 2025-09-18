#ifndef GAMESYSTEM_H
#define GAMESYSTEM_H

#include <vector>
#include <memory>
#include <string>
#include "../Role/new_bag/Shop.h"
#include "../Role/new_bag/Chest.h"

class GameSystem {
private:
    static GameSystem* instance;
    std::vector<std::unique_ptr<Shop>> shops;
    std::vector<std::unique_ptr<Chest>> chests;

    GameSystem();
    ~GameSystem();

public:
    // 获取单例实例
    static GameSystem* getInstance();

    // 初始化游戏系统
    void init();

    // 添加商店
    void addShop(std::unique_ptr<Shop> shop);

    // 添加宝箱
    void addChest(std::unique_ptr<Chest> chest);

    // 查找商店
    Shop* findShop(const std::string& name);

    // 查找宝箱
    Chest* findChest(int id);

    // 加载商店数据
    void loadShops(const std::string& directory);

    // 加载宝箱数据
    void loadChests(const std::string& directory);

    // 保存商店数据
    void saveShops(const std::string& directory) const;

    // 保存宝箱数据
    void saveChests(const std::string& directory) const;
};

#endif // GAMESYSTEM_H