
#ifndef MAPDRAW_H
#define MAPDRAW_H
#include "DrawBasic.h"
#define MapWidth 50
#define MapHeight 20
extern bool maprunning;
extern COORD playerPos;
extern COORD prevPlayerPos;
extern bool playerMoved;
//静态地图
void DrawStaticMap();
// 部分刷新 - 只更新玩家移动相关的部分
void PartialRefresh();
//键盘的输入处理线程
void MapKeyInput();
//总函数
void Map();

#endif // MAPDRAW_H

