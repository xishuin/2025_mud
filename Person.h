#include <string>
using namespace std;

#ifndef PERSON_H
#define PERSON_H
class Person{
    protected:
        int HP, Attack, Defend, index;
        /*
        HP: 生命值
        index: 位置的索引
        */
        string Name;
    public:
        Person(int InHP, int InAttack, int InDefend, string InName, int Inindex):HP(InHP),Attack(InAttack), Defend(InDefend), Name(InName), index(Inindex){}
        int GetHP(){ return HP; }
        string GetName(){ return Name; }
        int GetAttack(){ return Attack; }
        int GetDefend(){ return Defend; }
        int GetIndex(){ return index; }
};
class Player: public Person{
    private:
        Package PlayerPackage;
        Weapons PlayerWeapons;
        Armor PlayerArmor;
        int MP, EXP, Level, SkillPoints, Money;
        /*
        MP: 蓝条
        EXP: 经验值
        SkillPoints: 技能点
        */
        Skill PlayerSkill;
    public:
        //定义Player();
        bool ChangeMP(int InMP);
        bool ChangeExp(int InExp);
        bool UpLevel();
        bool ChangeSkillPoints(int InSkillPoints);
        bool ChangeMoney(int InMoney);
};
#endif //PERSON_H
