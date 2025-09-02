#include"map.cpp"

//需要加载的数据有 events.json,chapters.json
void test(EventManager &event_manager,MapManager & map_manager){
    for(const auto & event : event_manager.all_events){
        cout<<event.name;
    }
    for(const auto & map : map_manager.chapters){
        cout<<map.name;
    }

}