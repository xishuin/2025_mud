#ifndef MAPSYS_H
#define MAPSYS_H
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include <fstream>
#include "../json_wrapper.hpp" // 使用包装器解决byte命名冲突
#include "../Task.h"
using json = nlohmann::json;
using namespace std;

// 事件类型枚举
enum class EventType {
    NONE,      // 无事件
    MONSTER,   // 怪物事件
    CHEST,     // 宝箱事件
    NPC,       // NPC事件
    TRAP       // 陷阱事件
};

// 事件基类
struct Event {
    EventType type;         // 事件类型
    int id;              // 事件唯一标识
    string name;            // 事件名称
    string description;     // 事件描述
    int x,y;                //坐标
    int chapter_id;//位于哪个章节
    Task task;

    // 显式默认构造函数，使用空函数初始化Task
    Event() : type(EventType::NONE), id(0), x(0), y(0), chapter_id(0), task([]() {}) {
    }

    // 带参数的构造函数
    Event(EventType t, int i, const string& n, const string& d, int px, int py, int cid, Task tsk)
        : type(t), id(i), name(n), description(d), x(px), y(py), chapter_id(cid), task(std::move(tsk)) {
    }

    virtual ~Event() = default;
    // 将事件转换为JSON格式
    virtual json toJson() const;
    // 从JSON格式解析事件
    virtual void fromJson(const json& j);
    void set_handeler(Task);
    void event_handeler();
};

// 怪物事件
struct MonsterEvent : public Event {
    int monster_id;    //怪物id
    MonsterEvent() : Event(EventType::MONSTER, 0, "", "", 0, 0, 0, []() {}) {
        monster_id = 0;
    }

    // 带参数的构造函数
    MonsterEvent(int i, const string& n, const string& d, int px, int py, int cid, int mid, Task tsk)
        : Event(EventType::MONSTER, i, n, d, px, py, cid, std::move(tsk)), monster_id(mid) {
    }

    json toJson() const override;
    void fromJson(const json& j) override;
};

// 宝箱事件
struct ChestEvent : public Event {
    vector<int> items;  // 宝箱中的物品列表
    bool isLocked;         // 宝箱是否上锁
    ChestEvent() : Event(EventType::CHEST, 0, "", "", 0, 0, 0, []() {}),
        items(), isLocked(false) {
    }

    // 带参数的构造函数
    ChestEvent(int i, const string& n, const string& d, int px, int py, int cid, const vector<int>& its, bool locked, Task tsk)
        : Event(EventType::CHEST, i, n, d, px, py, cid, std::move(tsk)), items(its), isLocked(locked) {
    }

    json toJson() const override;
    void fromJson(const json& j) override;
};

// 地点类
class EventManager{
public:
bool load_data(const string& path);
void bulid_all_events();           //将所有的事件汇总，方便find
Event find_event(int id);          //根据id搜索事件

map<int,Event> chest_events;
map<int,Event> monster_events;
map<int,Event> all_events;     //事件总和

};

// 章节类
class Chapter {
public:
    int id;                         // 章节唯一标识
    string name;                    // 章节名称
    string description;             // 章节描述
    int width, height;              // 章节网格的宽度和高度
    vector<int> events;  // 储存事件id，在访问对象的时候使用GetEvent！！！
    // 构造函数
    Chapter(int id =0 , string name = "", string desc = "", int w = 10, int h = 10)
        : id(move(id)), name(move(name)), description(move(desc)), width(w), height(h) {
    }
    Event GetEvent(int index, EventManager& m);   
    bool isValidPosition(int x, int y) const; // 检查坐标是否有效
    json toJson() const;            // 转换为JSON格式
    void fromJson(const json& j);   // 从JSON格式解析
};

// 地图管理器类
class MapManager {
public:
    vector<Chapter>chapters;      // 存储所有章节，键为章节ID
    int currentChapterId;            // 当前章节ID
    pair<int, int> playerPosition;      // 玩家当前位置坐标

    MapManager(); // 构造函数
    Chapter* get_Current_Capters() {
        return &chapters[currentChapterId-1];
    }
    // 从文件加载地图数据
    bool loadFromFile(const string& filename);
};
#endif
