# 总体设计
类Event, 有MonsterEvent和ChestEvent两个子类，使用EventManager来管理

类Chapter，使用MapManager


### 使用前提
数据存储在chapter.json和event.json
使用前需要实例化EventManager和MapManager，并且使用

MapManager.loadFromFile(const string& filename);来加载数据

EventManager.load_data(const string& path)来加载数据
并且使用
EventManager.bulid_all_events();来总和数据

至此数据可以用

### 数据存储位置
对于chapters.json和event.json分别建立管理类 MapManager 和 EventManager

MapManager: 具有成员 vector<class Chapter\>，含有全部章节
EventManager:具有成因 vector<Event\>具有全部事件

目前两个数据成员都是public，可直接调用


