#ifndef GAME_H
#define GAME_H

#include "Bag.h"
#include "json.hpp" // nlohmann/json 库

using json = nlohmann::json;

class Game {
public:
    Game(const std::string& player_name);
    void run();

private:
    // 初始化和数据处理
    void loadItemDatabase(const std::string& filepath);
    void loadPlayerData(const std::string& filepath);
    void savePlayerData(const std::string& filepath);
    std::unique_ptr<Item> createItemById(int id);

    // 菜单和UI
    void showMainMenu();
    void showBackpackMenu();
    void handleBackpackAction(int list_index);

    // 辅助函数
    Rarity stringToRarity(const std::string& s);
    EquipmentSlot stringToSlot(const std::string& s);

    Bag m_bag;
    json m_itemDatabase;
    bool m_isRunning;
};

#endif //GAME_H

//
// Created by huaji on 25-8-25.
//

