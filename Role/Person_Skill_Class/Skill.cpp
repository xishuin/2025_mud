#include "Skill.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

// 小火球技能实现
bool FireArrow::Use(Player* user, Person* target) {
    if (!user || !target) return false;
    
    if (user->GetMP() < mpCost) return false;
    
    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);
    
    // 计算伤害
    int damage = effectValue + user->GetAttack();
    *target -= damage;
    return true;
}

// 大火球技能实现
bool QuickAttack::Use(Player* user, Person* target) {
    if (!user || !target) return false;
    
    if (user->GetMP() < mpCost) return false;
    
    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);
    
    // 计算伤害
    int damage = effectValue + user->GetAttack() ;
    *target -= damage;
    return true;
}

// 生命汲取技能实现
bool BloodSiphon::Use(Player* user, Person* target) {
    if (!user || !target) return false;

    if (user->GetMP() < mpCost) return false;

    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);

    // 计算伤害（基础效果值 + 用户攻击力的一半）
    int damage = effectValue + user->GetAttack() ;
    *target -= damage;

    // 计算回复量（伤害值的50%）
    int healAmount = damage * 0.5;
    *user += healAmount;

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

// 魔力爆裂技能实现
bool ManaBurst::Use(Player* user, Person* target) {
    if (!user || !target) return false;

    int currentMP = user->GetMP();
    if (currentMP == 0) return false;

    // 消耗所有MP
    user->SetMP(0);

    // 每点MP转化为2点伤害
    int damage = currentMP * 2;
    *target -= damage;

    return true;
}

// 冰霜陨星技能实现
bool FrostNova::Use(Player* user, Person* target) {
    if (!user || !target) return false;
    
    if (user->GetMP() < mpCost) return false;
    
    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);
    
    // 计算伤害
    int damage = effectValue + user->GetAttack() ;
    *target -= damage;
    return true;
}

// 魔力再生实现
bool LifeConvert::Use(Player* user, Person* target) {
    if (!user) return false;

    int hpCost = user->GetHP() * 0.2; // 消耗20%生命值
    if (hpCost <= 0) return false;

    *user -= hpCost; // 扣除生命值
    user->SetMP(user->GetMP() + hpCost); // 回复等量MP

    return true;
}

// 命运骰子技能实现
bool FateDice::Use(Player* user, Person* target) {
    if (!user || !target) return false;
    if (user->GetMP() < mpCost) return false;

    user->SetMP(user->GetMP() - mpCost);
    srand(time(nullptr));
    int effect = rand() % 3;

    switch (effect) {
    case 0: // 造成伤害
        *target -= (30 + user->GetAttack());
        break;
    case 1: // 治疗自己
        *user += (40 + user->GetLevel());
        break;
    case 2: // 回复MP
        user->SetMP(user->GetMP() + 30);
        break;
    }
    return true;
}

// 浴血术技能实现
bool BloodBath::Use(Player* user, Person* target) {
    if (!user || !target) return false;
    if (user->GetMP() < mpCost) return false;

    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);

    // 牺牲自身20%当前HP
    int hpCost = user->GetHP() * 0.2;
    if (hpCost < 1) hpCost = 1; // 至少消耗1点HP
    *user -= hpCost;

    // 计算伤害：基础值 + 消耗的HP值 + 用户攻击力
    int damage = effectValue + hpCost + user->GetAttack() ;
    *target -= damage;

    return true;
}

// 雷霆审判技能实现
bool ThunderStrike::Use(Player* user, Person* target) {
    if (!user || !target) return false;
    
    if (user->GetMP() < mpCost) return false;
    
    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);
    
    // 计算伤害
    int damage = effectValue + user->GetAttack() ;
    *target -= damage;
  
    
    return true;
}

// 等价交换技能实现
bool AttributeSwap::Use(Player* user, Person* target) {
    if (!user) return false;

    int currentHP = user->GetHP();
    int currentMP = user->GetMP();

    // 交换HP和MP
    user->SetHp(currentMP);
    user->SetMP(currentHP);

    // 确保不超过最大值
    if (user->GetHP() > user->GetMaxHP())
        user->SetHp(user->GetMaxHP());
    if (user->GetMP() > user->GetMaxMP())
        user->SetMP(user->GetMaxMP());

    return true;
}

// 魔法凝聚技能实现
bool ManaMeditation::Use(Player* user, Person* target) {
    if (!user) return false;

    int recoverAmount = user->GetMaxMP() * 0.3;
    user->SetMP(user->GetMP() + recoverAmount);

    if (user->GetMP() > user->GetMaxMP()) {
        user->SetMP(user->GetMaxMP());
    }

    return true;
}

// 神圣之光技能实现
bool HolyLight::Use(Player* user, Person* target) {
    if (!user) return false;
    
    if (user->GetMP() < mpCost) return false;
    
    // 扣除MP
    user->SetMP(user->GetMP() - mpCost);
    
    // 治疗效果
    int healAmount = effectValue + user->GetLevel();
    *user += healAmount;
   
    
    return true;
}

// 殊死回响技能实现
bool DesperateEcho::Use(Player* user, Person* target) {
    if (!user || !target) return false;
    if (user->GetHP() <= 1) return false; // 已经濒死无法使用

    // 消耗HP至仅剩1点
    int hpCost = user->GetHP() - 1;
    *user -= hpCost;

    // 计算伤害：基础值 + 消耗的HP * 系数
    int damage = effectValue + hpCost * 2 + user->GetAttack();
    *target -= damage;

    // 回复MP：基于消耗的HP量
    int mpRecover = hpCost * 1.5 + user->GetLevel() * 3;
    user->SetMP(user->GetMP() + mpRecover);

    // 确保不超过最大MP
    if (user->GetMP() > user->GetMaxMP()) {
        user->SetMP(user->GetMaxMP());
    }

    return true;
}


