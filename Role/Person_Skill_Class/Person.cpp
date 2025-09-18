#include "Person.h"
#include "Skill.h"
#include <iostream>
#include<algorithm>
#include<cmath>

void Player::InitializeSkills() {
    // 添加玩家技能
    Skills.push_back(new FireArrow());//小火球
    Skills.push_back(new QuickAttack());//大火球
    Skills.push_back(new Heal());//治疗术
	Skills.push_back(new BloodSiphon());//汲血术
	Skills.push_back(new ManaBurst());//魔力爆裂
	Skills.push_back(new LifeConvert());//魔力再生
	Skills.push_back(new FateDice());//命运骰子
	Skills.push_back(new FrostNova());//冰霜陨星
	Skills.push_back(new BloodBath());//浴血术
	Skills.push_back(new ThunderStrike());//雷霆审判
	Skills.push_back(new AttributeSwap());//等价交换
	Skills.push_back(new ManaMeditation());//魔力凝聚
	Skills.push_back(new HolyLight());//神圣之光
	Skills.push_back(new DesperateEcho());//殊死回响
    std::cout << "技能初始化完成\n";
}

void Player::UpLevel() {
    if (Level >= 25) {
        // 已经满级，不再升级
        EXP = 0; // 清空当前经验
        return;
    }

    Level++;
    std::cout << "升级了！当前等级：" << Level << std::endl;

    // 应用属性增长
    MaxHP +=(std::max(Level,1)-1)*2;
    HP = MaxHP; // 升级后回满生命
    MaxMP += 20;
    MP = MaxMP; // 升级后回满魔法
    Attack += (std::max(Level, 1) -1);
    Defend += floor((std::max(Level, 1) -1)/2);

    // 阶梯式经验值增长公式
    if (Level == 1) {
        MaxEXP = 10;
    }
     else {
        MaxEXP = (Level+1)*10*(Level-1);
    }
}