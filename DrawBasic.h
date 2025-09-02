#pragma once
#ifndef DRAWBASIC_H
#define DRAWBASIC_H
#include <windows.h>
#include <iostream>
#include <string>
#include <sstream>
#include <atomic>
#include <thread>
#include <conio.h>
#include <vector>
#include <time.h>
#include "Person.h"
#define ConsoleWidth     120
#define ConsoleHeight    40
// DrawBasic.h
enum class Model_t {
    MainModel,
    MapModel,
    BoxModel
};
enum class ConsoleColor {
    BLACK        = 0,
    BLUE         = 1,
    GREEN        = 2,
    CYAN         = 3,
    RED          = 4,
    MAGENTA      = 5,
    YELLOW       = 6,
    WHITE        = 7,
    GRAY         = 8,
    LIGHT_BLUE   = 9,
    LIGHT_GREEN  = 10,
    LIGHT_CYAN   = 11,
    LIGHT_RED    = 12,
    LIGHT_MAGENTA= 13,
    LIGHT_YELLOW = 14,
    BRIGHT_WHITE = 15
};
extern HANDLE hConsoleOut;
extern HANDLE hConsoleIn;
extern HWND hConsoleWnd;
extern bool running;
extern Model_t ModelState;
// 初始化控制台和绘制静态元素
void InitConsole();
// 隐藏光标
void HideCursor();
// 显示光标
void ShowCursor();
void SetCursorPosition(HANDLE hConsole,int x, int y);
void ClearScreen();
void SetColor(ConsoleColor color);
void DrawGameImg();
void GameStartInput();
void StartPlots();
void GamePrologue();

#endif
