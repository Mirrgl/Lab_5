#include "Animal.h"

const int MAX_AGE_PREY = 15;
const int MAX_AGE_PREDATOR = 30;
const int FOOD_NEEDED_TO_BREED_PREDATOR = 2;

Animal::Animal(int x, int y, Direction dir, int k, AnimalType type) : x(x)
                                                                    , y(y)
                                                                    , age(0)
                                                                    , dir(dir)
                                                                    , k(k)
                                                                    , changeCounter(0)
                                                                    , type(type) {}

int Animal::getX() const { return x; }
int Animal::getY() const { return y; }
int Animal::getAge() const { return age; }
AnimalType Animal::getType() const { return type; }
Direction Animal::getDir() const { return dir; }
int Animal::getK() const { return k; }

void Animal::changeDirection() {
    dir = static_cast<Direction>((dir + 1) % 4);
}

void Animal::updateChangeCounter() {
    changeCounter++;
    if (changeCounter >= k) {
        changeDirection();
        changeCounter = 0;
    }
}

Prey::Prey(int x, int y, Direction dir, int k) : Animal(x, y, dir, k, PREY)
                                               , breeding_counter(0) {}

void Prey::move(int field_width, int field_height) {
    switch (dir) {
        case UP:    y = (y - 1 + field_height) % field_height; break;
        case RIGHT: x = (x + 1) % field_width; break;
        case DOWN:  y = (y + 1) % field_height; break;
        case LEFT:  x = (x - 1 + field_width) % field_width; break;
    }
}

void Prey::feed() {}

void Prey::reproduce(vector<unique_ptr<Animal>>& new_animals, int field_width, int field_height) {
    if ((age == 5 || age == 10) && breeding_counter < 2) {
        new_animals.emplace_back(make_unique<Prey>(x, y, dir, k));
        breeding_counter++;
    }
}

void Prey::aging() {
    age++;
}

bool Prey::isDead() const {
    return age > MAX_AGE_PREY;
}

int Prey::getFoodCount() const { return 0; }
void Prey::resetFoodCount() {}

Predator::Predator(int x, int y, Direction dir, int k, PredatorType p_type) : Animal(x, y, dir, k, PREDATOR)
                                                                            , food_count(0)
                                                                            , predator_type(p_type) {}

void Predator::move(int field_width, int field_height) {
    for (int i = 0; i < 2; ++i) {
        switch (dir) {
            case UP:    y = (y - 1 + field_height) % field_height; break;
            case RIGHT: x = (x + 1) % field_width; break;
            case DOWN:  y = (y + 1) % field_height; break;
            case LEFT:  x = (x - 1 + field_width) % field_width; break;
        }
    }
}

void Predator::feed() {
    food_count++;
}

void Predator::reproduce(vector<unique_ptr<Animal>>& new_animals, int field_width, int field_height) {
    if (food_count >= FOOD_NEEDED_TO_BREED_PREDATOR) {
        new_animals.emplace_back(make_unique<Predator>(x, y, dir, k, predator_type));
        food_count = 0;
    }
}

void Predator::aging() {
    age++;
}

bool Predator::isDead() const {
    return age > MAX_AGE_PREDATOR;
}

int Predator::getFoodCount() const { return food_count; }
void Predator::resetFoodCount() { food_count = 0; }

PredatorType Predator::getPredatorType() const { return predator_type; }