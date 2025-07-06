# 🛡️ BattleSimulator - Strategy Combat

A turn-based battle simulator written in **C++**, inspired by classic RPG mechanics.  
Designed as a personal learning project to practice object-oriented programming, game logic, and basic AI.

---

## 🎮 Gameplay Overview

You control a **hero** who must defeat three monsters with unique behaviors.  
Each unit acts in order of **initiative**, switching between IDLE and ATTACKING states.

### 🔧 Core Features

- ⚔️ Initiative-based turn system
- 💀 Unit states: `IDLE`, `ATTACKING`, `DEAD`
- 🤖 Simple monster AI with random decisions (attack, defend, skip)
- 🎒 Item system:
  - 🧪 Potion → restores 30 HP
  - 💪 Buff → increases damage by 10
  - 🛡️ Shield → halves next incoming damage
- 📄 Battle log output to `battle_log.txt`
- 📊 Visual health bar in console (Windows)

---

## 🧠 Technologies Used

- **Language:** C++
- **Paradigms:** OOP (inheritance, polymorphism, encapsulation)
- **Memory:** Smart pointers (`std::unique_ptr`)
- **Standard libraries:** `iostream`, `vector`, `memory`, `algorithm`, `ctime`, `iomanip`, `fstream`
- **OS-specific:** Windows console coloring via `windows.h`

---

## 🧪 How to Compile & Run

Requires a modern C++ compiler (C++11 or later).

```bash
g++ -std=c++11 -o BattleSimulator BattleSimulatorStrategyCombat.cpp
./BattleSimulator
