#include "Person.h"
#include "Skill.h"
#include <iostream>

void Player::InitializeSkills() {
    // 添加玩家技能
    Skills.push_back(new FireArrow());
    Skills.push_back(new Heal());
    std::cout << "技能初始化完成\n";
}