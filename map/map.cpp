#include "map.h"

// Event 序列化/反序列化
json Event::toJson() const {
    return {
        {"type", static_cast<int>(type)},
        {"id", id},
        {"name", name},
        {"description", description}
    };
}

void Event::fromJson(const json& j) {
    j.at("id").get_to(id);
    j.at("name").get_to(name);
    j.at("description").get_to(description);
}

// MonsterEvent 序列化/反序列化
json MonsterEvent::toJson() const {
    json j = Event::toJson();
    j["monster_id"] = monster_id;
    return j;
}

void MonsterEvent::fromJson(const json& j) {
    Event::fromJson(j);
    j.at("monster_id").get_to(monster_id);
}

// ChestEvent 序列化/反序列化
json ChestEvent::toJson() const {
    json j = Event::toJson();
    j["items"] = items;
    j["isLocked"] = isLocked;

    return j;
}

void ChestEvent::fromJson(const json& j) {
    Event::fromJson(j);
    for(const auto & item : j.at("items")){
        items.push_back(item.get<int>());
    }
    j.at("isLocked").get_to(isLocked);
}


//将event.json中的数据分类储存到EventManager中的monster_events和chest_events
bool EventManager::load_data(const string& path){
    ifstream file(path);
    if(!file.is_open()){
        cout<<"Event.json can not open!!"<<endl;
        return false;
    }
    json j;
    file>>j;
    json monster_events_json=j.at("monsterEvent");
    json chest_events_json=j.at("chestEvent");
    for(const auto & monster_event_json : monster_events_json){
        MonsterEvent monster_event;
        monster_event.fromJson(monster_event_json);
        monster_events.push_back(monster_event);
    }
    for(const auto & chest_event_json : chest_events_json){
        ChestEvent chest_event;
        chest_event.fromJson(chest_event_json);
        chest_events.push_back(chest_event);
    }
    return true;
}
void EventManager::bulid_all_events(){
    for(const auto&event : monster_events){
        all_events.push_back(event);
    }
    for(const auto&event : chest_events){
        all_events.push_back(event);
    }
}
Event EventManager::find_event(int id){
    for(const auto& event : all_events){
        if(event.id==id){return event;}
    }
    cout<<"Not find event!!"<<endl;
    Event e;
    return e;
}

bool Chapter::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

json Chapter::toJson() const {
    json j = {
        {"id", id},
        {"name", name},
        {"description", description},
        {"width", width},
        {"height", height}
    };
    for(const auto &event : events ){
        j["events"].push_back(event);
    }//将events的事件id都添加到json中
    return j;
}

Event Chapter::GetEvent(int id, EventManager& m){
    return m.find_event(id);
}

void Chapter::fromJson(const json& j) {
    j.at("id").get_to(id);
    j.at("name").get_to(name);
    j.at("description").get_to(description);
    j.at("width").get_to(width);
    j.at("height").get_to(height);
    //根据events的id创建对象
    for(const auto & event_id : j.at("events")){
        int id=event_id.get<int>();
        events.push_back(id);
    }
}

// MapManager 方法实现
MapManager::MapManager() : currentChapterId(), playerPosition({0, 0}) {}

bool MapManager::loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    try {
        json j;
        file >> j;
        chapters.clear();
        for (const auto& chapterJson : j.at("chapters")) {
            Chapter chapter;
            chapter.fromJson(chapterJson);
            chapters.push_back(chapter);
        }
        return true;
    } catch (const exception& e) {
        cerr << "Error loading map: " << e.what() << endl;
        return false;
    }
}


