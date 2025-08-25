#ifndef PERSON_H
#define PERSON_H
#include <string>
using namespace std;

class Person{
    protected:
        int HP, MaxHP, Attack, Defend, Index, X, Y;
        /*
        HP: 生命值
        Index: 位置的索引
        */
        string Name;
    public:
        Person(int InHP, int InMaxHP, int InAttack, int InDefend, string InName, int InIndex, int InX, int InY):HP(InHP), MaxHP(InMaxHP),Attack(InAttack), Defend(InDefend), Name(InName), Index(InIndex), X(InX), Y(InY){}
        //获取成员变量
        int GetHP() const { return HP; }
        int GetMaxHP() const { return MaxHP; }
        string GetName() const { return Name; }
        int GetAttack() const { return Attack; }
        int GetDefend() const { return Defend; }
        int GetIndex() const { return Index; }
        int GetX() const { return X; }
        int GetY() const { return Y; }
        //修改成员变量
        void ChangeIndex(int InIndex){ Index = InIndex; }
        void ChangeXY(int InX, int InY){
            X = InX;
            Y = InY;
        }
        //重载运算符
        Person& operator+=(int value) {
            if (HP + value > MaxHP) {
                HP = MaxHP;
            }
            else {
                HP += value;
            }
            return *this;
        }

        Person& operator-=(int value) {
            if (HP - value < 0) {
                HP = 0;
            }
            else {
                HP -= value;
            }
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
        void UpLevel(); // 涉及到改蓝条、血条、技能
    public:
        //定义Player();
        //修改成员变量
        bool ChangeMP(int InMP) {
            if (MP + InMP < 0) return false;
            MP += InMP;
            if (MP > MaxMP) MP = MaxMP;
            return true;
        }
        bool ChangeEXP(int InEXP) {
            if (EXP + InEXP < 0) return false;
            EXP += InEXP;
            while (EXP > MaxEXP) {
                EXP -= MaxEXP;
                UpLevel();
            }
            return true;
        }
        bool ChangeMoney(int InMoney) {
            if (Money + InMoney < 0) return false;
            else {
                Money += InMoney;
                return true;
            }
        }
        //获取成员变量
        int GetMP() const {return MP;}
        int GetMaxMP() const {return MaxMP;}
        int GetEXP() const {return EXP;}
        int GetMaxEXP() const {return MaxEXP;}
        int GetLevel() const {return Level;}
        int GetMoney() const {return Money;}
        Package& GetPackage() { return PlayerPackage; }
        Weapons& GetWeapons() { return PlayerWeapons; }
        Armor& GetArmor() { return PlayerArmor; }
        Skill& GetSkill() { return PlayerSkill; }
};
#endif //PERSON_H
