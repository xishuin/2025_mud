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
int GetHP() { return HP; }
int GetMaxHP() { return MaxHP; }
string GetName() { return Name; }
int GetAttack() { return Attack; }
int GetDefend() { return Defend; }
int GetIndex() { return Index; }
int GetX() { return X; }
int GetY() { return Y; }

//主角专属
int GetMP() { return MP; }
int GetMaxMP() { return MaxMP; }
int GetEXP() { return EXP; }
int GetMaxEXP() { return MaxEXP; }
int GetLevel() { return Level; }
int GetMoney() { return Money; }
```

## 3. 修改成员变量

```C++
void ChangeIndex(int InIndex);
void ChangeXY(int InX, IntY);
// HP：重载+-

// 主角专属
bool ChangeMP(int InMP);
bool ChangeEXP(int InExp); // 会自动升级
bool ChangeMoney(int InMoney);
```

