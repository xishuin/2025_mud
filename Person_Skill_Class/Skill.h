#ifndef SKILLS_H
#define SKILLS_H

#include <string>

using namespace std;


// 技能基类
class Skill {
protected:
    int id                //技能的编号
    string name;
    string description;
    int levelRequirement; // 学习所需的等级
    int mpCost;           // 消耗的MP
    int effectValue;      // 效果值（伤害值、治疗量等）
    int duration;         // 效果持续回合数（0表示即时效果）现在暂时用不上

public:
    Skill(int id,string n, string desc, int lvlReq, int mp, int effVal, int dur = 0)
        : name(n), description(desc), levelRequirement(lvlReq), mpCost(mp), 
          effectValue(effVal),duration(dur) {}
    
    virtual ~Skill() {}
    
    // 获取技能信息
    int Getid() const { return id ;}
    string GetName() const { return name; }
    string GetDescription() const { return description; }
    int GetLevelRequirement() const { return levelRequirement; }
    int GetMpCost() const { return mpCost; }
    int GetEffectValue() const { return effectValue; }
    int GetDuration() const { return duration; }
    
    // 使用技能（纯虚函数，由具体技能实现）
    virtual bool Use(Player* user, Person* target) = 0;
    
    // 检查是否可以学习
    virtual bool CanLearn(int playerLevel) const {
        return playerLevel >= levelRequirement;
    }
};

// 火焰箭技能 - 直接伤害
class FireArrow : public Skill {
public:
    FireArrow() : Skill(1,"火焰箭", "发射一支火焰箭矢对敌人造成伤害", 5, 10, 20) {}
    
//具体实现等待战斗系统
};

// 治疗术技能 - 治疗
class Heal : public Skill {
public:
    Heal() : Skill(2,"治疗术", "恢复自身或队友的生命值", 10, 15, 30) {}
    
//具体实现等待战斗系统
};

// 力量祝福技能 - 攻击增益
class StrengthBlessing : public Skill {
public:
    StrengthBlessing() : Skill(3,"力量祝福", "暂时提升攻击力", 15, 20, 5,3) {}

//具体实现等待战斗系统
};

// 冰霜新星技能 - 范围伤害
class FrostNova : public Skill {
public:
    FrostNova() : Skill(4,"冰霜新星", "释放冰霜能量对全体敌人造成伤害", 20, 25, 20) {}
    
//具体实现等待战斗系统
};


// 雷霆一击技能 - 高伤害
class ThunderStrike : public Skill {
public:
    ThunderStrike() : Skill(5,"雷霆一击", "召唤雷霆对敌人造成巨大伤害", 25, 40, 50) {}
    
//具体实现等待战斗系统
};


// 牛x的一击技能 - 高aoe
class NBStrike : public Skill {
public:
    NBStrike() : Skill(6,"牛x一击", "发动牛x的一击造成巨大全体伤害", 30, 40, 50) {}
    
//具体实现等待战斗系统
};

#endif // SKILLS_H
