#include"load.cpp"


int main(){

    EventManager event_manager;
    MapManager  map_manager;
    event_manager.load_data("C:\\Users\\zlh7v\\Desktop\\summer_cpp\\map\\event.json");
    event_manager.bulid_all_events();
    map_manager.loadFromFile("chapters.json");

    test(event_manager,map_manager);
    return 0;
}