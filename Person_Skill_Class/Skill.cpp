#include "Skill.h"

// 火焰箭技能实现
bool FireArrow::Use(Player* user, Person* target) {
    if (!user || !target) return false;
    
    if (user->GetMP() < mpCost) return false;
    
    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);
    
    // 计算伤害
    int damage = effectValue + user->GetAttack() / 5;
    *target -= damage;
    
    return true;
}

// 治疗术技能实现
bool Heal::Use(Player* user, Person* target) {
    if (!user) return false;
    
    if (user->GetMP() < mpCost) return false;
    
    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);
    
    // 治疗效果
    int healAmount = effectValue + user->GetLevel();
    *user += healAmount;
    
    return true;
}

// 力量祝福技能实现
bool StrengthBlessing::Use(Player* user, Person* target) {
    if (!user) return false;
    
    if (user->GetMP() < mpCost) return false;
    
    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);
    
    // 这里简化处理，实际应该有状态系统
    user->SetAttack(user->GetAttack() + effectValue);
    
    return true;
}

// 冰霜新星技能实现
bool FrostNova::Use(Player* user, Person* target) {
    if (!user || !target) return false;
    
    if (user->GetMP() < mpCost) return false;
    
    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);
    
    // 计算伤害
    int damage = effectValue + user->GetAttack() / 4;
    *target -= damage;
    
    return true;
}

// 雷霆一击技能实现
bool ThunderStrike::Use(Player* user, Person* target) {
    if (!user || !target) return false;
    
    if (user->GetMP() < mpCost) return false;
    
    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);
    
    // 计算伤害
    int damage = effectValue + user->GetAttack() / 3;
    *target -= damage;
    
    return true;
}

// 牛x的一击技能实现
bool NBStrike::Use(Player* user, Person* target) {
    if (!user || !target) return false;
    
    if (user->GetMP() < mpCost) return false;
    
    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);
    
    // 计算伤害
    int damage = effectValue + user->GetAttack() / 2;
    *target -= damage;
    
    return true;
}