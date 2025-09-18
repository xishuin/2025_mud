#ifndef PERSON_H
#define PERSON_H
#include <string>
#include <iomanip>
#include <fstream>
#include "../../lib/json_wrapper.hpp"
#include <vector>
// 包含Bag类的完整定义
#include "../new_bag/bag.h"
using namespace std;
using json = nlohmann::json;

// 前向声明Skill类，避免循环包含
class Skill;

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
        // 使用当前目录下的player.json路径
        ifstream file("../static/json/player.json");
        if (!file.is_open()) {
            cerr << "can not open player.json" << endl;
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
    void SetAttack(int attack){ Attack=attack;}
    void SetDefend(int InDefend) { Defend = InDefend;}
    void SetMaxHp(int max_hp){MaxHP=max_hp;}
    void SetHp(int _hp){HP=_hp;}
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
        ifstream file("../static/json/player.json");
        if (!file.is_open()) {
            cerr<<"can not save to player.json"<<endl;
        }
        else {
            json existing_data = json::parse(file);
            for (json::iterator it = data.begin(); it != data.end(); ++it) {
                existing_data[it.key()] = it.value();
            }
            file.close();
            ofstream out_file("../static/json/player.json");
            if (!out_file.is_open()) {
                cerr << "can not save to player.json"<<endl;
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
    Bag PlayerBag;
    int MP, MaxMP, EXP, MaxEXP, Level/*, Money*/;
    vector<Skill*> Skills;
    int chapter;
    /*
    MP: 蓝条
    EXP: 经验值
    */
    /*Skill PlayerSkill;*/
    void UpLevel();// 涉及到改蓝条、血条
    void InitializeSkills();
public:
    Player()  : Person(), PlayerBag(){
        ifstream file("../static/json/player.json");
        if (!file.is_open()) {
            cerr << "1" << endl;
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
        chapter=1;
    }
    ~Player() {
        json data = Person::GetData();
        data["mp"] = MP;
        data["max_mp"] = MaxMP;
        data["exp"] = EXP;
        data["max_exp"] = MaxEXP;
        data["level"] = Level;
        Person::SaveToJson();

        ifstream file("../static/json/player.json");
        if (!file.is_open()) {
            cerr<<"can not save to player.json"<<endl;
        }
        else {
            json existing_data = json::parse(file);
            for (json::iterator it = data.begin(); it != data.end(); ++it) {
                existing_data[it.key()] = it.value();
            }
            file.close();
            ofstream out_file("../static/json/player.json");
            if (!out_file.is_open()) {
                cerr << "can not save to player.json"<<endl;
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
    // 对外API：增加经验值并自动执行升级逻辑
    bool GainEXP(int InEXP) {
        return ChangeEXP(InEXP);
    }

    // 内部方法：更改经验值
    bool ChangeEXP(int InEXP) {
        if (EXP + InEXP < 0) return false;
        EXP += InEXP;
        while (EXP > MaxEXP && Level < 25) {
            EXP -= MaxEXP;
            UpLevel();
        }
        // 如果已经满级，多余的经验值清空
        if (Level >= 25) {
            EXP = 0;
        }
        return true;
    }
    int GetLevel() const { return Level; }
    int GetMP() const { return MP; }
    int GetMaxMP() const { return MaxMP; }
    int GetEXP() const { return EXP; }
    int GetMaxEXP() const { return MaxEXP; }
    const std::vector<Skill*>& GetSkills() const { return Skills; }
    void SetMP(int mp) { MP = mp; }
    void SetAttack(int attack) { Attack = attack; }

    // int GetMoney() const { return Money; }
    const Bag& GetBag() const { return PlayerBag; }
    Bag& GetBag() { return PlayerBag; }
    Skill& GetSkills(int InIndex) { return *Skills[InIndex]; }
    void SetChapter(int _c) { chapter=_c;}
    int GetChapter() const{ return chapter;}
};
// 在类定义之后包含必要的头文件以获取完整定义
#include "../new_bag/bag.h"
#include "Skill.h"

// InitializeSkills方法的实现将在Person.cpp中提供

#endif //PERSON_H
