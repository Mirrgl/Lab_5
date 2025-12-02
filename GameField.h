#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Animal.h"
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <list>
#include <algorithm>
#include <sstream>
#include <random>
#include <iostream>

using namespace std;

class GameField {
private:
    int width, height;
    vector<unique_ptr<Animal>> animals;

public:
    GameField(int w, int h);

    void add_animal(unique_ptr<Animal> animal);
    void simulate_step();
    void print_state(int turn) const;
    void simulate(int turns);
    vector<string> get_result() const;
};

GameField generate_random_field(int width, int height, int num_prey, int num_predator, int max_turns);

#endif