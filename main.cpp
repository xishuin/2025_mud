#include "Game.h"
#include <iostream>

int main() {
    // 设置控制台编码为UTF-8 (在Windows上可能需要)
    // system("chcp 65001");

    std::cout << "欢迎来到背包管理系统!\n";

    Game game("冒险者"); // 创建一个名为 "冒险者" 的游戏实例
    game.run();

    return 0;
}
//
// Created by huaji on 25-8-25.
//
