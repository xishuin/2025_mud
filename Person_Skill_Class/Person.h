#ifndef PERSON_H
#define PERSON_H
#include <string>
#include <iomanip>
#include <fstream>
#include "json.hpp"
#include "Skill.h"
#include <vector>
using namespace std;
using json = nlohmann::json;

class Person {
protected:
    int HP, MaxHP, Attack, Defend, Index, X, Y;
    /*
    HP: 生命值
    Index: 位置的索引
    */
    string Name;
public:
    Person(int InHP, int InMaxHP, int InAttack, int InDefend, string InName, int InIndex, int InX, int InY) :HP(InHP), MaxHP(InMaxHP), Attack(InAttack), Defend(InDefend), Name(InName), Index(InIndex), X(InX), Y(InY) {}
    Person() {
        ifstream file("player.json");
        if (!file.is_open()) {
            cerr << "人物初始化错误，Person类错误" << endl;
        }
        json data = json::parse(file);
        HP = data["hp"];
        MaxHP = data["max_hp"];
        Attack=data["attack"];
        Defend = data["defend"];
        Index = data["index"];
        X = data["x"];
        Y = data["y"];
        Name = data["name"];
    }
    virtual ~Person() = default;
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
    void ChangeIndex(int InIndex) { Index = InIndex; }
    void ChangeXY(int InX, int InY) {
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
    json GetData() {
        json data;
        data["hp"] = HP;
        data["max_hp"] = MaxHP;
        data["attack"] = Attack;
        data["defend"] = Defend;
        data["index"] = Index;
        data["x"] = X;
        data["y"] = Y;
        data["name"] = Name;
        return data;
    }
    void SaveToJson() {
        json data = GetData();
        ifstream file("player.json");
        if (!file.is_open()) {
            cerr<<"无法保存人物到数据中"<<endl;
        }
        else {
            json existing_data = json::parse(file);
            for (json::iterator it = data.begin(); it != data.end(); ++it) {
                existing_data[it.key()] = it.value();
            }
            file.close();
            ofstream out_file("player.json");
            if (!out_file.is_open()) {
                cerr << "无法保存人物到数据中"<<endl;
            }
            else {
                out_file << existing_data.dump(4);
                out_file.close();
            }
        }
    }
};

class Player : public Person {
private:
    /*Package PlayerPackage;
    Weapons PlayerWeapons;
    Armor PlayerArmor;*/
    int MP, MaxMP, EXP, MaxEXP, Level/*, Money*/;
    vector<Skill*> Skills;
    /*
    MP: 蓝条
    EXP: 经验值
    */
    /*Skill PlayerSkill;*/
    void UpLevel() {

    } // 涉及到改蓝条、血条

    void InitializeSkills() {
        // 清除现有技能
        for (Skill* skill : Skills) {
            delete skill;
        }
        Skills.clear();

        // 根据等级添加技能
        if (Level >= 5) {
            Skills.push_back(new FireArrow());
        }
        if (Level >= 10) {
            Skills.push_back(new Heal());
        }
        if (Level >= 15) {
            Skills.push_back(new StrengthBlessing());
        }
        if (Level >= 20) {
            Skills.push_back(new FrostNova());
        }
        if (Level >= 25) {
            Skills.push_back(new ThunderStrike());
        }
        if (Level >= 30) {
            Skills.push_back(new NBStrike());
        }
    }
public:
    Player()  : Person(){
        ifstream file("player.json");
        if (!file.is_open()) {
            cerr << "初始化玩家错误，Player类错误" << endl;
            //return 1;
        }
        json data = json::parse(file);
        file.close();
        MP = data["mp"];
        MaxMP = data["max_mp"];
        EXP = data["exp"];
        MaxEXP = data["max_exp"];
        Level = data["level"];
        InitializeSkills();
    }
    ~Player() {
        json data = Person::GetData();
        data["mp"] = MP;
        data["max_mp"] = MaxMP;
        data["exp"] = EXP;
        data["max_exp"] = MaxEXP;
        data["level"] = Level;
        Person::SaveToJson();

        ifstream file("player.json");
        if (!file.is_open()) {
            cerr<<"无法保存人物到数据中"<<endl;
        }
        else {
            json existing_data = json::parse(file);
            for (json::iterator it = data.begin(); it != data.end(); ++it) {
                existing_data[it.key()] = it.value();
            }
            file.close();
            ofstream out_file("player.json");
            if (!out_file.is_open()) {
                cerr << "无法保存人物到数据中"<<endl;
            }
            else {
                out_file << existing_data.dump(4);
                out_file.close();
            }
        }
    }

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
    // bool ChangeMoney(int InMoney) {
    //     if (Money + InMoney < 0) return false;
    //     else {
    //         Money += InMoney;
    //         return true;
    //     }
    // }
    //获取成员变量
    int GetMP() const { return MP; }
    int GetMaxMP() const { return MaxMP; }
    int GetEXP() const { return EXP; }
    int GetMaxEXP() const { return MaxEXP; }
    int GetLevel() const { return Level; }
    /*int GetMoney() const { return Money; }
    Package& GetPackage() { return PlayerPackage; }
    Weapons& GetWeapons() { return PlayerWeapons; }
    Armor& GetArmor() { return PlayerArmor; }*/
    Skill& GetSkills(int InIndex) { return *Skills[InIndex]; }
};
#endif //PERSON_H
