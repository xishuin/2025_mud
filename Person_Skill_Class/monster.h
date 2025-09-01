#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

// 定义怪物结构体
struct Monster {
    string name;
    string damage;
    int hp;
    int level;
};

// 从JSON解析怪物
vector<Monster> loadMonstersFromJson(const string& filename) {
    vector<Monster> monsters;
    
    try {
        // 读取JSON文件
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return monsters;
        }
        
        // 解析JSON
        json jsonData;
        file >> jsonData;
        
        // 检查是否有"monsters"键
        if (jsonData.contains("monsters") && jsonData["monsters"].is_array()) {
            for (const auto& monsterJson : jsonData["monsters"]) {
                Monster monster;
                monster.name = monsterJson.value("name", "");
                monster.damage = monsterJson.value("damage", "");
                monster.hp = monsterJson.value("hp", 0);
                monster.level = monsterJson.value("level", 0);
                
                monsters.push_back(monster);
            }
        }
    } catch (const exception& e) {
        cerr << "解析JSON时出错: " << e.what() << endl;
    }
    
    return monsters;
}

// 将怪物保存回JSON文件
void saveMonstersToJson(const vector<Monster>& monsters, const string& filename) {
    json jsonData;
    jsonData["monsters"] = json::array();
    
    for (const auto& monster : monsters) {
        json monsterJson;
        monsterJson["name"] = monster.name;
        monsterJson["damage"] = monster.damage;
        monsterJson["hp"] = monster.hp;
        monsterJson["level"] = monster.level;
        
        jsonData["monsters"].push_back(monsterJson);
    }
    
    ofstream file(filename);
    if (file.is_open()) {
        file << jsonData.dump(4); // 使用4空格缩进美化输出
        cout << "怪物数据已保存到: " << filename << endl;
    } else {
        cerr << "无法创建文件: " << filename << endl;
    }
}

// 示例：在内存中修改怪物数据
void modifyMonsters(vector<Monster>& monsters) {
    for (auto& monster : monsters) {
        // 示例修改：将所有怪物的HP增加10%
        monster.hp = static_cast<int>(monster.hp * 1.1);
    }
}

// 打印怪物信息
void printMonsters(const vector<Monster>& monsters) {
    cout << "怪物列表:" << endl;
    cout << "----------------------------------------" << endl;
    for (const auto& monster : monsters) {
        cout << "名称: " << monster.name << endl;
        cout << "伤害: " << monster.damage << endl;
        cout << "生命值: " << monster.hp << endl;
        cout << "等级: " << monster.level << endl;
        cout << "----------------------------------------" << endl;
    }
}
