
#ifndef MAPDRAW_H
#define MAPDRAW_H
#include "DrawBasic.h"
#include "map/map.h"
#include "Person_Skill_Class/Person.h"
#include <Windows.h>

// 全局变量
extern bool maprunning;
extern COORD playerPos;
extern COORD prevPlayerPos;
extern bool playerMoved;
extern MapManager g_mapManager;
extern EventManager g_eventManager;

extern Player player;

// 静态地图绘制
void DrawStaticMap();

// 部分刷新 - 只更新玩家移动相关的部分
void PartialRefresh();

// 键盘的输入处理线程
void MapKeyInput();

// 主地图函数
void Map();

// 初始化地图系统函数（与map包衔接）
void InitMapSystem();

// 事件检测函数
void EventDetection();

// 检查并处理玩家位置的事件
void CheckPositionEvent(int x, int y);

// 设置玩家位置（同步Map包和Player对象）
void SetPlayerPosition(int x, int y);

#endif // MAPDRAW_H

