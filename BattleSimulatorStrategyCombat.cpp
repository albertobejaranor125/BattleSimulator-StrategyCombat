#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include <ctime>

// Enum para estados de unidad
enum class UnitState {
    IDLE,
    ATTACKING,
    DEAD
};

// Declaración adelantada
class Unit;


// Clase base de las unidades
class Unit {
protected:
    std::string name;
    int health;
    int damage;
    int initiative;
    UnitState state;
    bool shielded = false;

public:
    Unit(const std::string& name, int health, int damage, int initiative, UnitState state)
        : name(name), health(health), damage(damage), initiative(initiative), state(state) {
    }

    void ActivateShield() { shielded = true; }
    void MoreDamage(int amount) { damage = std::min(damage + amount, 100); }
    void Heal(int amount) { health = std::min(health + amount, 100); }
    int GetHealth() { return health; }
    virtual void Attack(std::vector<std::unique_ptr<Unit>>& units) = 0;

    virtual void TakeDamage(int amount) {
        if (shielded) {
            amount /= 2;
            shielded = false;
        }
        health = std::max(0, health - amount);
    }

    bool IsAlive() const { return health > 0; }
    std::string GetName() const { return name; }
    UnitState GetState() const { return state; }
    int GetInitiative() const { return initiative; }

    void UpdateState() {
        if (!IsAlive()) state = UnitState::DEAD;
        else if (state == UnitState::IDLE) state = UnitState::ATTACKING;
        else if (state == UnitState::ATTACKING) state = UnitState::IDLE;
    }

    virtual void PrintStatus() const {
        system("Color 0A");
        std::cout << name << " - HP: " << health << ", Damage: " << damage;
        if (state == UnitState::IDLE) std::cout << " (Idle)";
        else if (state == UnitState::ATTACKING) std::cout << " (Attacking)";
        else if (state == UnitState::DEAD) std::cout << " (Dead)";
        std::cout << "\n";
    }
   
    virtual ~Unit() = default;
};
class Item {
public:
    virtual void Use(Unit& user) = 0;
    virtual std::string GetName() const = 0;
    virtual ~Item() = default;
};
class Potion : public Item {
    std::string GetName() const override {
        return "Potion";
    }
    void Use(Unit& user) {
        user.Heal(30);
        std::cout << user.GetName() << " uses a potion and heals 30 HP!\n";
    }
};
class Buff : public Item {
    std::string GetName() const override {
        return "Buff";
    }
    void Use(Unit& user) {
        user.MoreDamage(10);
        std::cout << user.GetName() << " uses a buff and increase 10 Damage!\n";
    }
};
class Shield : public Item {
    std::string GetName() const override {
        return "Shield";
    }
    void Use(Unit& user) {
        user.ActivateShield();
        std::cout << user.GetName() << " raises a shield and will take half damage!\n";
    }
};
Unit* FindStrongestTarget(const Unit& self, const std::vector<std::unique_ptr<Unit>>& units) {
    Unit* strongest = nullptr;
    int maxHealth = -1;

    for (const auto& u : units) {
        if (u.get() == &self) continue;                // No atacarse a sí mismo
        if (!u->IsAlive()) continue;                   // Saltar si está muerto

        if (u->GetHealth() > maxHealth) {
            maxHealth = u->GetHealth();
            strongest = u.get();
        }
    }

    return strongest;
}
// Clases Hero y Monster irán aquí, cada una con su lógica de ataque adaptada
class Hero : public Unit {
public:
    std::vector<std::unique_ptr<Item>> inventory;
    Hero(const std::string& name)
        : Unit(name, 100, 25, 6, UnitState::IDLE) {
        inventory.push_back(std::make_unique<Potion>());
        inventory.push_back(std::make_unique<Buff>());
        inventory.push_back(std::make_unique<Shield>());
    }
    void UseItemIfNeeded() {
        size_t i = 0;
        while (i < inventory.size()) {
            if (health < 40 && inventory[i].get()->GetName() == "Potion") {
                inventory[i]->Use(*this);
                inventory.erase(inventory.begin() + i);
                return;
            }
            else if (inventory[i].get()->GetName() == "Buff") {
                inventory[i]->Use(*this);
                inventory.erase(inventory.begin() + i);
                return;
            }
            i++;
        }
    }
    void Attack(std::vector<std::unique_ptr<Unit>>& units) override {
        UseItemIfNeeded();
        Unit* target = FindStrongestTarget(*this, units);
        if (!target) return;
        int dmg = (health > 50) ? 25 : 15;
        std::cout << name << " is attacking to " << target->GetName() << std::endl;
        target->TakeDamage(dmg);
    }
};
class Monster : public Unit {
public:
    Monster(const std::string& name)
        : Unit(name, 80, 20, 5, UnitState::IDLE) {
    }

    void Attack(std::vector<std::unique_ptr<Unit>>& units) override {
        Unit* target = FindStrongestTarget(*this, units);
        if (!target) return;
        int dmg = 20;
        if (name == "Goblin") dmg = 12;
        else if (name == "Troll") dmg = 15;
        if (health < 30) {
            int decision = rand() % 2;
            if (decision == 0) {
                std::cout << name << " is attacking to " << target->GetName() << std::endl;
                target->TakeDamage(dmg);
                return;
            }
            else if (decision == 1) {
                std::cout << name << " is trying to heal but he can't attack!\n";
                return;
            }
        }
        else {
            std::cout << name << " is attacking to " << target->GetName() << std::endl;
            target->TakeDamage(dmg);
        }
    }
};
// Clase Game que maneja la simulación
class Game {
private:
    std::vector<std::unique_ptr<Unit>> units;

public:
    Game(); // Definir constructor en .cpp (fuera)
    
    void Run() {
        while (!IsGameOver()) {
            SortByInitiative();

            for (auto& u : units) {
                if (u->IsAlive()) {
                    u->Attack(units);
                }
            }

            UpdateStates();
            PrintStatus();
        }

        std::cout << (units[0]->IsAlive() ? "Hero wins!" : "Monsters win!") << std::endl;
    }
    void SortByInitiative() {
        std::sort(units.begin(), units.end(),
            [](const std::unique_ptr<Unit>& a, const std::unique_ptr<Unit>& b) {
                return a->GetInitiative() > b->GetInitiative();
            });
    }

    void UpdateStates() {
        for (auto& u : units) {
            u->UpdateState();
        }
    }
    void PrintStatus() const {
        for (const auto& u : units) {
            u->PrintStatus();
        }
        std::cout << "-----\n";
    }

    bool IsGameOver() const {
        int monsterDead = 0;
        for (size_t i = 1; i < units.size(); ++i) {
            if (!units[i]->IsAlive()) monsterDead++;
        }
        return !units[0]->IsAlive() || monsterDead == units.size() - 1;
    }
};
Game::Game() {
    srand(static_cast<unsigned>(time(nullptr)));
    units.push_back(std::make_unique<Hero>("Arthur"));
    units.push_back(std::make_unique<Monster>("Goblin"));
    units.push_back(std::make_unique<Monster>("Orc"));
    units.push_back(std::make_unique<Monster>("Troll"));
}
// main()
int main() {
    Game game;
    game.Run();
    return 0;
}
