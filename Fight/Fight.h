//
// 优化后的战斗系统界面
//


#ifndef FIGHT_H
#define FIGHT_H
#include "../draw/DrawBasic.h"
#include "../Role/Person_Skill_Class/Person.h"
#include "../Role/Person_Skill_Class/Skill.h"
#include <functional>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>


// 战斗操作枚举
enum class FightAction {
    Attack = 0,      // 攻击
    Skills,          // 技能
    Items,           // 物品
    Run              // 逃跑
};

// 动作处理函数接口
typedef std::function<void()> ActionHandler;

// 战斗选项结构体
struct FightOption {
    FightAction action;
    std::string name;
    ActionHandler handler;
};

// 战斗状态
struct FightState {
    bool isRunning = true;
    FightAction selectedAction = FightAction::Attack;
    int selectedIndex = 0;
    Player* player = nullptr;
    Person* enemy = nullptr;
    std::vector<std::string> battleLog;
    bool isBossFight = false;
    int bossPhase = 1;
    int bossMaxHP = 0;
    
    // 技能分页相关变量
    int currentSkillPage = 0;    // 当前页码，从0开始
    int skillsPerPage = 5;       // 每页显示的技能数量
};

// 全局战斗状态声明
extern FightState g_fightState;
extern int prevY;

// 函数声明
void DrawFightOptions();
void HandleInput();
void ProcessAction();
void SetActionHandler(FightAction action, const ActionHandler& handler);
void InitializeDefaultHandlers();
void Optionsrefresh();
void DrawBattleLog();
void AddBattleLog(const std::string& log);
int CalculateDamage(int attackerAttack, int defenderDefense);
int Fight(Player &player, Person &enemy, bool isBoss = false);


#endif //FIGHT_H
