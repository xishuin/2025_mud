
#ifndef MAINDRAW_H
#define MAINDRAW_H
#include "DrawBasic.h"
#include "../static/story/StorySystem.h"

// 前向声明Player类
class Player;
// 声明外部全局变量player
extern Player player;
extern map<int,Person> boss;
// 声明全局剧情系统
extern StorySystem g_storySystem;

void DrawPlots();
void MainKeyInput();

void Draw_Progress_Bar(int x,int y,int width,int cur,int max,ConsoleColor color);
void MainGame();

void DrawBagUI();
void DrawPlots();

#endif //MAINDRAW_H
