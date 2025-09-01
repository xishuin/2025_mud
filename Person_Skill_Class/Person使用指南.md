# 人物使用指南

## 1. 定义

### npc:

```C++
Person(int InHP, int InMaxHP, int InAttack, int InDefend, string InName, int InIndex, int Inx, int Iny)
```

### 主角：

```C++
Player()
// 直接初始化主角
/*
结束后会自动保存，信息在player.json文件中，要求把player.json放在Person.h同一目录
*/
```

## 2. 获取信息

```C++
int GetHP() const { return HP; }
int GetMaxHP() const { return MaxHP; }
string GetName() const { return Name; }
int GetAttack() const { return Attack; }
int GetDefend() const { return Defend; }
int GetIndex() const { return Index; }
int GetX() const { return X; }
int GetY() const { return Y; }

//主角专属
int GetMP() const { return MP; }
int GetMaxMP() const { return MaxMP; }
int GetEXP() const { return EXP; }
int GetMaxEXP() const { return MaxEXP; }
int GetLevel() const { return Level; }

// int GetMoney() const { return Money; } // 有可能要取消
//以下目前未实现
// Package& GetPackage() { return PlayerPackage; }
// Weapons& GetWeapons() { return PlayerWeapons; }
// Armor& GetArmor() { return PlayerArmor; }
// Skill& GetSkill() { return PlayerSkill; }
```

## 3. 修改成员变量

```C++
void ChangeIndex(int InIndex);
void ChangeXY(int InX, int InY);
// HP：重载+-，使用必须依照以下方式
Person p(……);
p += 3
p -= 1
//不可以使用 p = p + 1这种形式
//HP达到最大值的时候为最大值，HP小于0的时候为0，如果是扣血的话，你们扣完之后需要GetHP()判断血是不是0，如果是0就死亡

// 主角专属
//这里输入改变的值，可以输入负数表示减
bool ChangeMP(int InMP);
bool ChangeEXP(int InExp); 
// bool ChangeMoney(int InMoney); // 有可能需要取消
/*
MP小于0会返回false，MP不会改变，MP达到上限后MP=MaxMP
EXP会自动升级，并且如果一次性得到很多经验，可以自动连续升好几级，EXP小于0也会返回false，EXP可以减，也可以不减
Money小于0后会返回false，Money不会改变
*/
```



