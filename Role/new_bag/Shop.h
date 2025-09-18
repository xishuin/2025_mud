#ifndef SHOP_H
#define SHOP_H

#include <vector>
#include <memory>
#include "Item.h"
#include "bag.h"

class Shop {
private:
    std::string name;
    std::vector<std::pair<std::unique_ptr<Item>, int>> itemsForSale; // 商品和数量
    std::vector<std::pair<int, int>> buyHistory; // 购买历史 (物品ID, 数量)

public:
    Shop(std::string name);
    ~Shop();

    // 添加商品到商店
    void addItem(std::unique_ptr<Item> item, int quantity = 1);

    // 从商店移除商品
    bool removeItem(int itemId, int quantity = 1);

    // 显示商店中的商品
    void displayItems() const;

    // 购买商品
    bool buyItem(int itemId, int quantity, Bag& buyerBag);

    // 出售商品
    bool sellItem(int itemId, int quantity, Bag& sellerBag);

    // 获取商店名称
    std::string getName() const { return name; }

    // 加载商店数据
    void loadFromJson(const std::string& filename);

    // 保存商店数据
    void saveToJson(const std::string& filename) const;
};

#endif // SHOP_H