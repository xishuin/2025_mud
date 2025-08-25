#include <string>
using namespace std;

#ifndef PERSON_H
#define PERSON_H
class Person{
    protected:
        int HP, MaxHP, Attack, Defend, Index, x, y;
        /*
        HP: 生命值
        Index: 位置的索引
        */
        string Name;
    public:
        Person(int InHP, int InMaxHP, int InAttack, int InDefend, string InName, int InIndex, int Inx, int Iny):HP(InHP), MaxHP(IntMaxHP),Attack(InAttack), Defend(InDefend), Name(InName), index(InIndex), x(Inx), y(Iny){}
        //获取成员变量
        int GetHP(){ return HP; }
        int GetMaxHP(){ return MaxHP; }
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
        //重载运算符
        Person& operator+(int value) {
            this->HP += value;
            return *this;
        }

        Person& operator-(int value) {
            this->HP -= value;
            return *this;
        }
};

class Player: public Person{
    private:
        Package PlayerPackage;
        Weapons PlayerWeapons;
        Armor PlayerArmor;
        int MP, MaxMP, EXP, MaxEXP, Level, Money;
        /*
        MP: 蓝条
        EXP: 经验值
        */
        Skill PlayerSkill;
    public:
        //定义Player();
        //修改成员变量
        bool ChangeMP(int InMP); // 最大蓝条
        bool ChangeEXP(int InExp); // 涉及到超过限度后自动升级
        bool ChangeMoney(int InMoney);
        //获取成员变量
        int GetMP(){return MP;}
        int GetMaxMP(){return MaxMP;}
        int GetEXP(){return EXP;}
        int GetMaxEXP(){return MaxEXP;}
        int GetLevel(){return Level;}
        int GetMoney(){return Money;}
        bool UpLevel(); // 涉及到改蓝条、血条、技能
};
#endif //PERSON_H
