# 人物使用指南

## 1. 定义

### npc:

```C++
Person(int InHP, int InMaxHP, int InAttack, int InDefend, string InName, int InIndex, int Inx, int Iny)
```

### 主角：

```C++
Player() //目前未定义
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
int GetMoney() const { return Money; }
Package& GetPackage() { return PlayerPackage; }
Weapons& GetWeapons() { return PlayerWeapons; }
Armor& GetArmor() { return PlayerArmor; }
Skill& GetSkill() { return PlayerSkill; }
```

## 3. 修改成员变量

```C++
void ChangeIndex(int InIndex);
void ChangeXY(int InX, IntY);
// HP：重载+-，使用必须依照以下方式
Person p(……);
p += 3
p -= 1
//不可以使用 p = p + 1这种形式

// 主角专属
bool ChangeMP(int InMP);
bool ChangeEXP(int InExp); // 会自动升级，并且如果一次性得到很多经验，可以自动连续升好几级
bool ChangeMoney(int InMoney);
```



