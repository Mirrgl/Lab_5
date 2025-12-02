#ifndef ANIMAL_H
#define ANIMAL_H

#include <memory>
#include <vector>

using namespace std;

enum Direction { UP, RIGHT, DOWN, LEFT };
enum AnimalType { PREY, PREDATOR };
enum class PredatorType { BEAR, FOX, WOLF };

class Animal {
protected:
    int x, y;
    int age;
    Direction dir;
    int k;
    int changeCounter;
    AnimalType type;

public:
    Animal(int x, int y, Direction dir, int k, AnimalType type);

    virtual ~Animal() = default;

    virtual void move(int field_width, int field_height) = 0;
    virtual void feed() = 0;
    virtual void reproduce(vector<unique_ptr<Animal>>& new_animals, int field_width, int field_height) = 0;
    virtual void aging() = 0;
    virtual bool isDead() const = 0;
    virtual int getFoodCount() const = 0;
    virtual void resetFoodCount() = 0;

    int getX() const;
    int getY() const;
    int getAge() const;
    AnimalType getType() const;
    Direction getDir() const;
    int getK() const;

    void changeDirection();
    void updateChangeCounter();
};

class Prey : public Animal {
private:
    int breeding_counter;

public:
    Prey(int x, int y, Direction dir, int k);

    void move(int field_width, int field_height) override;
    void feed() override;
    void reproduce(vector<unique_ptr<Animal>>& new_animals, int field_width, int field_height) override;
    void aging() override;
    bool isDead() const override;
    int getFoodCount() const override;
    void resetFoodCount() override;
};

class Predator : public Animal {
protected:
    int food_count;
    PredatorType predator_type;

public:
    Predator(int x, int y, Direction dir, int k, PredatorType p_type);

    void move(int field_width, int field_height) override;
    void feed() override;
    void reproduce(vector<unique_ptr<Animal>>& new_animals, int field_width, int field_height) override;
    void aging() override;
    bool isDead() const override;
    int getFoodCount() const override;
    void resetFoodCount() override;

    PredatorType getPredatorType() const;
};

#endif