#include <string>
using namespace std;

#ifndef PERSON_H
#define PERSON_H
class Person{
    protected:
        int HP, MaxHP, Attack, Defend, Index, X, Y;
        /*
        HP: 生命值
        Index: 位置的索引
        */
        string Name;
    public:
        Person(int InHP, int InMaxHP, int InAttack, int InDefend, string InName, int InIndex, int InX, int InY):HP(InHP), MaxHP(IntMaxHP),Attack(InAttack), Defend(InDefend), Name(InName), index(InIndex), X(InX), Y(InY){}
        //获取成员变量
        int GetHP(){ return HP; }
        int GetMaxHP(){ return MaxHP; }
        string GetName(){ return Name; }
        int GetAttack(){ return Attack; }
        int GetDefend(){ return Defend; }
        int GetIndex(){ return Index; }
        int GetX() { return X; }
        int GetY() { return Y; }
        //修改成员变量
        void ChangeIndex(int InIndex){ Index = InIndex; }
        void ChangeXY(int InX, IntY){
            X = InX;
            Y = InY;
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
        bool UpLevel(); // 涉及到改蓝条、血条、技能
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
};
#endif //PERSON_H
