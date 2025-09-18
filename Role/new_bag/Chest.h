#ifndef CHEST_H
#define CHEST_H

#include <vector>
#include <memory>
#include <string>
#include "Item.h"

class Player;

class Chest {
private:
    int id;
    std::string name;
    bool isOpen;
    std::vector<std::pair<std::unique_ptr<Item>, int>> contents; // 物品和数量
    int requiredKeyId; // 所需钥匙ID，0表示不需要钥匙

public:
    Chest(int id, std::string name, int requiredKeyId = 0);
    ~Chest();

    // 添加物品到宝箱
    void addItem(std::unique_ptr<Item> item, int quantity = 1);
    std::string Getname() {
        std::string res;
        for (auto& it:contents) {
            res+=it.first->name;
            res+=" ";
        }
        return res;
    }

    // 从宝箱移除物品
    bool removeItem(int itemId, int quantity = 1);

    // 打开宝箱
    bool open(Player& player);

    // 检查宝箱是否已打开
    bool getIsOpen() const { return isOpen; }

    // 获取宝箱ID
    int getId() const { return id; }

    // 获取宝箱名称
    std::string getName() const { return name; }

    // 加载宝箱数据
    void loadFromJson(const std::string& filename);

    // 保存宝箱数据
    void saveToJson(const std::string& filename) const;
};

#endif // CHEST_H