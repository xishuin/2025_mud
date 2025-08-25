#include <string>
using namespace std;

#ifndef PERSON_H
#define PERSON_H
class Person{
    protected:
        int HP, Attack, Defend, Index, x, y;
        /*
        HP: 生命值
        Index: 位置的索引
        */
        string Name;
    public:
        Person(int InHP, int InAttack, int InDefend, string InName, int InIndex, int Inx, int Iny):HP(InHP),Attack(InAttack), Defend(InDefend), Name(InName), index(InIndex), x(Inx), y(Iny){}
        //获取成员变量
        int GetHP(){ return HP; }
        string GetName(){ return Name; }
        int GetAttack(){ return Attack; }
        int GetDefend(){ return Defend; }
        int GetIndex(){ return Index; }
        int Getx() { return x; }
        int Gety() { return y; }
        //修改成员变量
        void ChangeIndex(int InIndex){ Index = InIndex; }
        void ChangeXY(int Inx, Inty){
            x = Inx;
            y = Iny;
        }
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
        //修改成员变量
        bool ChangeMP(int InMP);
        bool ChangeExp(int InExp); // 涉及到超过限度后自动升级
        bool ChangeSkillPoints(int InSkillPoints); // 涉及到超过限度后自动升级
        bool ChangeMoney(int InMoney);
};
#endif //PERSON_H
