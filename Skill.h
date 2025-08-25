#ifndef SKILLS_H
#define SKILLS_H

#include <string>
#include "Person.h" 

using namespace std;


// 技能基类
class Skill {
protected:
    string name;
    string description;
    int levelRequirement; // 学习所需的等级
    int mpCost;           // 消耗的MP
    int effectValue;      // 效果值（伤害值、治疗量等）
    int duration;         // 效果持续回合数（0表示即时效果）

public:
    Skill(string n, string desc, int lvlReq, int mp, int effVal, int dur = 0)
        : name(n), description(desc), levelRequirement(lvlReq), mpCost(mp), 
          effectValue(effVal),duration(dur) {}
    
    virtual ~Skill() {}
    
    // 获取技能信息
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
    FireArrow() : Skill("火焰箭", "发射一支火焰箭矢对敌人造成伤害", 5, 10, 20) {}
    
    bool Use(Player* user, Person* target) override {
        if (user->GetMP() < mpCost) return false;
        
        user->ChangeMP(-mpCost); // 消耗MP
        int damage = effectValue - target->GetDefend();//先这样，数值计算后面再细化
        if (damage < 0) damage = 0;
        *target -= damage; // 使用重载的-运算符减少HP
        
        return true;
    }
};

// 治疗术技能 - 治疗
class Heal : public Skill {
public:
    Heal() : Skill("治疗术", "恢复自身或队友的生命值", 10, 15, 30) {}
    
    bool Use(Player* user, Person* target) override {
        if (user->GetMP() < mpCost) return false;
        
        user->ChangeMP(-mpCost); // 消耗MP
        *target += effectValue; // 使用重载的+运算符增加HP
        
        return true;
    }
};

// 力量祝福技能 - 攻击增益
class StrengthBlessing : public Skill {
public:
    StrengthBlessing() : Skill("力量祝福", "暂时提升攻击力", 15, 20, 5,3) {}
    
    bool Use(Player* user, Person* target) override {
        if (user->GetMP() < mpCost) return false;
        
        user->ChangeMP(-mpCost); // 消耗MP
        // 这里需要实现buff效果等战斗系统再做
        
        return true;
    }
};

// 冰霜新星技能 - 范围伤害
class FrostNova : public Skill {
public:
    FrostNova() : Skill("冰霜新星", "释放冰霜能量对全体敌人造成伤害", 20, 25, 20) {}
    
    bool Use(Player* user, Person* target) override {
        if (user->GetMP() < mpCost) return false;
        
        user->ChangeMP(-mpCost); // 消耗MP
        int damage = effectValue - target->GetDefend();
        if (damage < 0) damage = 0;
        *target -= damage;
        
        return true;
    }
};


// 雷霆一击技能 - 高伤害
class ThunderStrike : public Skill {
public:
    ThunderStrike() : Skill("雷霆一击", "召唤雷霆对敌人造成巨大伤害", 25, 40, 50) {}
    
    bool Use(Player* user, Person* target) override {
        if (user->GetMP() < mpCost) return false;
        
        user->ChangeMP(-mpCost); // 消耗MP
        int damage = effectValue - target->GetDefend();
        if (damage < 0) damage = 0;
        *target - damage;
        
        return true;
    }
};


// 牛x的一击技能 - 高aoe
class NBStrike : public Skill {
public:
    NBStrike() : Skill("牛x一击", "发动牛x的一击造成巨大全体伤害", 30, 40, 50) {}
    
    bool Use(Player* user, Person* target) override {
        if (user->GetMP() < mpCost) return false;
        
        user->ChangeMP(-mpCost); // 消耗MP
        int damage = effectValue - target->GetDefend();
        if (damage < 0) damage = 0;
        *target -= damage;
        
        return true;
    }
};

#endif // SKILLS_H
