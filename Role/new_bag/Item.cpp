#include "Item.h"
#include "../Person_Skill_Class/Person.h"

// HealthPotion::Use方法实现
bool HealthPotion::Use(Player* player) {
    if (!player) return false;
    
    *player += healAmount;
    return true;
}

// ManaPotion::Use方法实现
bool ManaPotion::Use(Player* player) {
    if (!player) return false;
    
    int currentMP = player->GetMP();
    int maxMP = player->GetMaxMP();
    
    if (currentMP >= maxMP) return false;
    
    int newMP = currentMP + manaAmount;
    if (newMP > maxMP) newMP = maxMP;
    
    player->ChangeMP(newMP - currentMP);
    return true;
}