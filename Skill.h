#ifndef SKILLS_H
#define SKILLS_H

#include <string>
#include <vector>
#include "Person.h" // 包含Person类的定义

using namespace std;

// 技能效果类型枚举
enum SkillEffectType {
    DAMAGE,     // 直接伤害
    HEAL,       // 治疗
    BUFF_ATK,   // 攻击增益
};

// 技能基类
class Skill {
protected:
    string name;
    string description;
    int levelRequirement; // 学习所需的等级
    int mpCost;           // 消耗的MP
    int effectValue;      // 效果值（伤害值、治疗量等）
    SkillEffectType effectType; // 效果类型
    int duration;         // 效果持续回合数（0表示即时效果）

public:
    Skill(string n, string desc, int lvlReq, int mp, int effVal, SkillEffectType type, int dur = 0)
        : name(n), description(desc), levelRequirement(lvlReq), mpCost(mp), 
          effectValue(effVal), effectType(type), duration(dur) {}
    
    virtual ~Skill() {}
    
    // 获取技能信息
    string GetName() const { return name; }
    string GetDescription() const { return description; }
    int GetLevelRequirement() const { return levelRequirement; }
    int GetMpCost() const { return mpCost; }
    int GetEffectValue() const { return effectValue; }
    SkillEffectType GetEffectType() const { return effectType; }
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
    FireArrow() : Skill("火焰箭", "发射一支火焰箭矢对敌人造成伤害", 5, 10, 20, DAMAGE) {}
    
    bool Use(Player* user, Person* target) override {
        if (user->GetMP() < mpCost) return false;
        
        user->ChangeMP(-mpCost); // 消耗MP
        int damage = effectValue - target->GetDefend();
        if (damage < 0) damage = 0;
        *target - damage; // 使用重载的-运算符减少HP
        
        return true;
    }
};

// 治疗术技能 - 治疗
class Heal : public Skill {
public:
    Heal() : Skill("治疗术", "恢复自身或队友的生命值", 10, 15, 30, HEAL) {}
    
    bool Use(Player* user, Person* target) override {
        if (user->GetMP() < mpCost) return false;
        
        user->ChangeMP(-mpCost); // 消耗MP
        *target + effectValue; // 使用重载的+运算符增加HP
        
        return true;
    }
};

// 力量祝福技能 - 攻击增益
class StrengthBlessing : public Skill {
public:
    StrengthBlessing() : Skill("力量祝福", "暂时提升攻击力", 15, 20, 5, BUFF_ATK, 3) {}
    
    bool Use(Player* user, Person* target) override {
        if (user->GetMP() < mpCost) return false;
        
        user->ChangeMP(-mpCost); // 消耗MP
        // 这里需要实现buff效果，可能需要修改Person类来支持buff/debuff
        
        return true;
    }
};

// 冰霜新星技能 - 范围伤害+减速
class FrostNova : public Skill {
public:
    FrostNova() : Skill("冰霜新星", "释放冰霜能量对周围敌人造成伤害并减速", 20, 25, 15, DAMAGE) {}
    
    bool Use(Player* user, Person* target) override {
        if (user->GetMP() < mpCost) return false;
        
        user->ChangeMP(-mpCost); // 消耗MP
        int damage = effectValue - target->GetDefend();
        if (damage < 0) damage = 0;
        *target - damage;
        
        // 这里需要实现减速效果，可能需要修改Person类来支持buff/debuff
        
        return true;
    }
};

// 圣光护盾技能 - 防御增益
class HolyShield : public Skill {
public:
    HolyShield() : Skill("圣光护盾", "创建一个神圣护盾吸收伤害", 25, 30, 0, BUFF_DEF, 5) {}
    
    bool Use(Player* user, Person* target) override {
        if (user->GetMP() < mpCost) return false;
        
        user->ChangeMP(-mpCost); // 消耗MP
        // 这里需要实现护盾效果，可能需要修改Person类来支持护盾机制
        
        return true;
    }
};

// 雷霆一击技能 - 高伤害+眩晕
class ThunderStrike : public Skill {
public:
    ThunderStrike() : Skill("雷霆一击", "召唤雷霆对敌人造成巨大伤害并可能眩晕", 30, 40, 50, DAMAGE) {}
    
    bool Use(Player* user, Person* target) override {
        if (user->GetMP() < mpCost) return false;
        
        user->ChangeMP(-mpCost); // 消耗MP
        int damage = effectValue - target->GetDefend();
        if (damage < 0) damage = 0;
        *target - damage;
        
        // 这里需要实现眩晕效果，可能需要修改Person类来支持控制效果
        
        return true;
    }
};

#endif // SKILLS_H
