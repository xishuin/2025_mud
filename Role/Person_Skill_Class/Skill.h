#ifndef SKILLS_H
#define SKILLS_H

#include <string>
using namespace std;

// 在实现技能类之前先包含Person.h以获取完整的Player和Person类定义
#include "Person.h"

// 技能基类
class Skill {
protected:
    int id;                //技能的编号
    string name;
    string description;
    int levelRequirement; // 学习所需的等级
    int mpCost;           // 消耗的MP
    int effectValue;      // 效果值（伤害值、治疗量等）

public:
    Skill(int id,string n, string desc, int lvlReq, int mp, int effVal)
        : id(id), name(n), description(desc), levelRequirement(lvlReq), mpCost(mp), 
          effectValue(effVal) {}
    
    virtual ~Skill() {}
    
    // 获取技能信息
    int Getid() const { return id ;}
    string GetName() const { return name; }
    string GetDescription() const { return description; }
    int GetLevelRequirement() const { return levelRequirement; }
    int GetMpCost() const { return mpCost; }
    int GetEffectValue() const { return effectValue; }
    
    // 使用技能（纯虚函数，由具体技能实现）
    virtual bool Use(Player* user, Person* target) = 0;
    
    // 检查是否可以学习
    virtual bool CanLearn(int playerLevel) const {
        return playerLevel >= levelRequirement;
    }
};

// 基础技能组 (1-5级)
// 大火球技能 - 直接伤害
class FireArrow : public Skill {
public:
    FireArrow() : Skill(1,"小火球", "发射一发小火球对敌人造成伤害ATK+6", 1, 5, 6) {}
    
    bool Use(Player* user, Person* target) override;
};

// 大火球技能 - 直接伤害
class QuickAttack : public Skill {
public:
    QuickAttack() : Skill(2,"大火球", "汇聚一发大火球对敌人造成伤害ATK+15", 2, 10, 15) {}
    
    bool Use(Player* user, Person* target) override;
};

// 汲血术技能 - 吸血
class BloodSiphon : public Skill {
public:
    BloodSiphon() : Skill(10, "汲血术", "吸取敌人的生命精华，造成伤害ATK+10并回复自身生命值50%damage", 3, 20, 10) {}

    bool Use(Player* user, Person* target) override;
};

// 治疗术技能 - 治疗
class Heal : public Skill {
public:
    Heal() : Skill(3,"治疗术", "恢复自身的生命值HP+25", 4, 8, 30) {}
    
    bool Use(Player* user, Person* target) override;
};

// 中级技能组 (6-15级)

// 冰霜陨星技能 - 较高伤害
class FrostNova : public Skill {
public:
    FrostNova() : Skill(4,"冰霜陨星", "释放冰霜能量对敌人造成中幅伤害ATK+25", 5,40 , 25) {}
    
    bool Use(Player* user, Person* target) override;
};

// 魔力爆裂技能 - 消耗所有MP进行高伤害
class ManaBurst : public Skill {
public:
    ManaBurst() : Skill(11, "魔力爆裂", "消耗所有MP，每点MP转化为2点伤害", 6, 0, 0) {}
    bool Use(Player* user, Person* target) override;
};

// 魔力再生 - 消耗生命值回复MP
class LifeConvert : public Skill {
public:
    LifeConvert() : Skill(12, "生命转换", "消耗20%生命值，回复等量MP", 6, 0, 0) {}
    bool Use(Player* user, Person* target) override;
};

// 命运骰子 - 随机效果
class FateDice : public Skill {
public:
    FateDice() : Skill(18, "命运骰子", "随机产生不同效果(伤害ATK+30/治疗Level+40/回复MP30)", 12, 20, 0) {}
    bool Use(Player* user, Person* target) override;
};

// 浴血术 - 牺牲生命值造成巨大伤害
class BloodBath : public Skill {
public:
    BloodBath() : Skill(23, "浴血术", "消耗MP并牺牲自身HP，造成巨额伤害(ATK+HP*20%)", 8, 20, 0) {}
    bool Use(Player* user, Person* target) override;
};

// 高级技能组 (16-25级)
// 雷霆审判技能 - 高伤害
class ThunderStrike : public Skill {
public:
    ThunderStrike() : Skill(6,"雷霆审判", "召唤远古雷神之力，降下毁灭性的雷霆裁决ATK+100", 8, 100, 100) {}
    
    bool Use(Player* user, Person* target) override;
};

// 等价交换 - 交换当前HP和MP
class AttributeSwap : public Skill {
public:
    AttributeSwap() : Skill(19, "等价交换", "交换当前的HP和MP数值", 14, 0, 0) {}
    bool Use(Player* user, Person* target) override;
};

// 魔力凝聚 - 恢复大量MP
class ManaMeditation : public Skill {
public:
    ManaMeditation() : Skill(18, "魔力冥想", "恢复最大MP的30%", 6, 0, 0) {}
    bool Use(Player* user, Person* target) override;
};


// 神圣之光技能 - 群体治疗
class HolyLight : public Skill {
public:
    HolyLight() : Skill(7,"神圣之光", "释放神圣光芒，治疗自身Level+100", 9, 80, 100) {}
    
    bool Use(Player* user, Person* target) override;
};

// 殊死回响 - 以生命值为代价造成巨大伤害并回复大量MP
class DesperateEcho : public Skill {
public:
    DesperateEcho() : Skill(26, "殊死回响", "耗尽生命至濒死状态，造成巨额伤害并回复大量MP", 18, 0, 0) {}
    bool Use(Player* user, Person* target) override;
};

// 技能实现已移至Skill.cpp文件中

//修改日志：大量增加丰富技能种类，平衡数值

#endif //SKILLS_H
