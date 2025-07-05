# BattleSimulator-StrategyCombat

This is a small turn-based battle simulator written in C++, inspired by classic RPG mechanics. It was developed as a personal and learning project focused on game programming fundamentals.

## 🎮 Description

You control a hero who must fight against three monsters, each with different behaviors and damage stats. The system is modular and extensible, ideal as a foundation for more complex gameplay logic.

### Core features include:

- **Initiative-based turn system**
- **Unit states**: `IDLE`, `ATTACKING`, `DEAD`
- **Basic monster AI**
- **Item system** with:
  - Potions: restore HP
  - Buffs: increase damage
  - Shields: reduce damage taken

## 🧠 Technologies Used

- Language: **C++**
- Standard libraries: `iostream`, `memory`, `vector`, `algorithm`, `cstdlib`, `ctime`
- OOP principles: inheritance, polymorphism, abstract classes
- Smart pointers (`std::unique_ptr`)

## 🧪 How to Compile

You can compile using any modern C++ compiler (C++11 or higher):

```bash
g++ -std=c++11 -o BattleSimulator BattleSimulatorStrategyCombat.cpp
./BattleSimulator
