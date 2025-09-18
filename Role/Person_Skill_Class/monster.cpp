//
// Created by Administrator on 2025/9/8.
//
// 从JSON解析怪物
#include "monster.h"
#include <fstream>
#include <string>
#include <map>
#include <stdexcept>

map<int ,Person> loadBossFromJson(const string& filename) {
    map<int, Person> boss;
    try {
        // 读取JSON文件
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << filename << endl;
            return boss;
        }
        // 解析JSON
        json jsonData;
        file >> jsonData;
        // 检查是否有"Boss"键
        if (jsonData.contains("Boss") && jsonData["Boss"].is_array()) {
            for (const auto& bossjson : jsonData["Boss"]) {
                int id = stoi(bossjson.value("id", "0"));
                int hp = bossjson.value("hp", 0);
                int damage = bossjson.value("damage", 0);
                string name = bossjson.value("name", "");
                // 构造Person对象（假设构造函数参数为：当前HP, 最大HP, 攻击力, 防御力, 名称, 等级, 经验值, 金币）
                Person bos(hp, hp, damage, 0, name, 0, 0, 0);
                boss[id] = bos;
            }
        }
    }
    catch (const exception& e) {
        cerr << "解析JSON时出错: " << e.what() << endl;
    }
    return  boss;
}

// 加载怪物数据到map，key为monster_id
map<int, Person> loadMonstersFromJson(const string& filename) {
    map<int, Person> monsters;
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
                int id = stoi(monsterJson.value("id", "0"));
                int hp = monsterJson.value("hp", 0);
                int damage = monsterJson.value("damage", 0);
                string name = monsterJson.value("name", "");
                // 构造Person对象（假设构造函数参数为：当前HP, 最大HP, 攻击力, 防御力, 名称, 等级, 经验值, 金币）
                Person monster(hp, hp, damage, 0, name, 0, 0, 0);
                monsters[id] = monster;
            }
        }
    }
    catch (const exception& e) {
        cerr << "解析JSON时出错: " << e.what() << endl;
    }
    return monsters;
}

// 加载事件与怪物的映射关系，key为event.id
map<int, Person> loadEventMonstersMap(const string& eventFilename, const string& monsterFilename) {
    map<int, Person> eventMonsters;

    // 首先加载怪物数据
    map<int, Person> monsters = loadMonstersFromJson(monsterFilename);

    try {
        // 读取event.json文件
        ifstream file(eventFilename);
        if (!file.is_open()) {
            cerr << "无法打开文件: " << eventFilename << endl;
            return eventMonsters;
        }

        // 解析JSON
        json jsonData;
        file >> jsonData;

        // 检查是否有"monsterEvent"键
        if (jsonData.contains("monsterEvent") && jsonData["monsterEvent"].is_array()) {
            for (const auto& eventJson : jsonData["monsterEvent"]) {
                int eventId = eventJson.value("id", 0);
                int monsterId = eventJson.value("monster_id", 0);

                // 检查怪物是否存在
                if (monsters.find(monsterId) != monsters.end()) {
                    eventMonsters[eventId] = monsters[monsterId];
                } else {
                    cerr << "警告: 找不到ID为" << monsterId << "的怪物，事件ID: " << eventId << endl;
                }
            }
        }
    } catch (const exception& e) {
        cerr << "解析event.json时出错: " << e.what() << endl;
    }

    return eventMonsters;
}

// 将怪物保存回JSON文件
void saveMonstersToJson(const vector<Person>& monsters, const string& filename) {
    json jsonData;
    jsonData["monsters"] = json::array();

    int id = 1; // 从1开始为怪物分配ID
    for (const auto& monster : monsters) {
        json monsterJson;
        monsterJson["id"] = to_string(id++);
        monsterJson["name"] = monster.GetName();
        monsterJson["damage"] = monster.GetAttack();
        monsterJson["hp"] = monster.GetHP();
        jsonData["monsters"].push_back(monsterJson);
    }

    ofstream file(filename);
    if (file.is_open()) {
        file << jsonData.dump(4); // 使用4空格缩进美化输出
        cout << "怪物数据已保存到: " << filename << endl;
    }
    else {
        cerr << "无法创建文件: " << filename << endl;
    }
}