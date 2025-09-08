#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "json_wrapper.hpp"
#include "json.hpp"
#include "Person.h"

using json = nlohmann::json;
using namespace std;

// 从JSON解析怪物
vector<Monster> loadMonstersFromJson() {
    const string filename = "monsters.json";
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
                
                monsters.push_back(monster);
            }
        }
    } catch (const exception& e) {
        cerr << "解析JSON时出错: " << e.what() << endl;
    }
    
    return monsters;
}

// 将怪物保存回JSON文件
void saveMonstersToJson(const vector<Person>& monsters) {
    const string filename = "monsters.json";
    json jsonData;
    jsonData["monsters"] = json::array();

    for (const auto& monster : monsters) {
        json monsterJson;
        monsterJson["name"] = monster.GetName();
        monsterJson["damage"] = monster.GetAttack();
        monsterJson["hp"] = monster.GetHP();
        
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

