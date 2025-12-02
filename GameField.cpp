#include "GameField.h"

void GameField::add_animal(unique_ptr<Animal> animal) {
    animals.push_back(move(animal));
}

void GameField::simulate_step() {
    random_device rd;
    mt19937 gen;

    for (auto& animal : animals) {
        animal->move(width, height);
        animal->updateChangeCounter();
    }

    map<pair<int, int>, list<Animal*>> cell_contents;
    for (auto& animal : animals) {
        cell_contents[{animal->getX(), animal->getY()}].push_back(animal.get());
    }

    vector<unique_ptr<Animal>*> prey_to_remove;
    vector<unique_ptr<Animal>*> predator_to_remove;

    for (auto& [pos, cell_animals] : cell_contents) {
        vector<Predator*> predators_in_cell;
        vector<Prey*> prey_in_cell;

        for (auto* animal : cell_animals) {
            if (animal->getType() == PREDATOR) {
                predators_in_cell.push_back(static_cast<Predator*>(animal));
            } else if (animal->getType() == PREY) {
                prey_in_cell.push_back(static_cast<Prey*>(animal));
            }
        }

        if (predators_in_cell.size() > 1) {
            vector<Predator*> unique_types;
            for (auto* p : predators_in_cell) {
                bool found = false;
                for (auto* existing_p : unique_types) {
                    if (p->getPredatorType() == existing_p->getPredatorType()) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    unique_types.push_back(p);
                }
            }

            for (size_t i = 0; i < unique_types.size(); ++i) {
                for (size_t j = i + 1; j < unique_types.size(); ++j) {
                    if (unique_types[i]->getPredatorType() != unique_types[j]->getPredatorType()) {
                        Predator* p1 = nullptr;
                        Predator* p2 = nullptr;
                        for (auto* p : predators_in_cell) {
                            if (p->getPredatorType() == unique_types[i]->getPredatorType() && !p1) {
                                p1 = p;
                            } else if (p->getPredatorType() == unique_types[j]->getPredatorType() && !p2) {
                                p2 = p;
                            }
                            if (p1 && p2) break;
                        }

                        if (p1 && p2) {
                            uniform_int_distribution<> dist(0, 1);
                            if (dist(gen) == 0) {
                                auto it = find_if(animals.begin(), animals.end(),
                                    [p2](const unique_ptr<Animal>& a) { return a.get() == p2; });
                                if (it != animals.end()) {
                                    predator_to_remove.push_back(&(*it));
                                }
                            } else {
                                auto it = find_if(animals.begin(), animals.end(),
                                    [p1](const unique_ptr<Animal>& a) { return a.get() == p1; });
                                if (it != animals.end()) {
                                    predator_to_remove.push_back(&(*it));
                                }
                            }
                        }
                    }
                }
            }
        }

        bool predator_survived = false;
        Predator* feeding_predator = nullptr;
        for (auto* animal : cell_animals) {
            if (animal->getType() == PREDATOR) {
                auto it = find_if(predator_to_remove.begin(), predator_to_remove.end(),
                    [animal](unique_ptr<Animal>* ptr) { return ptr->get() == animal; });
                if (it == predator_to_remove.end()) {
                    predator_survived = true;
                    feeding_predator = static_cast<Predator*>(animal);
                    break;
                }
            }
        }

        if (predator_survived && !prey_in_cell.empty()) {
            for (auto* prey_animal : prey_in_cell) {
                auto it = find_if(animals.begin(), animals.end(),
                    [prey_animal](const unique_ptr<Animal>& a) { return a.get() == prey_animal; });
                if (it != animals.end()) {
                    prey_to_remove.push_back(&(*it));
                }
            }

            if (!prey_in_cell.empty() && feeding_predator != nullptr) {
                 feeding_predator->feed();
            }
        }
    }

    for (auto ptr_to_unique_ptr : prey_to_remove) {
        animals.erase(remove(animals.begin(), animals.end(), *ptr_to_unique_ptr), animals.end());
    }
    for (auto ptr_to_unique_ptr : predator_to_remove) {
         animals.erase(remove(animals.begin(), animals.end(), *ptr_to_unique_ptr), animals.end());
    }

    for (auto& animal : animals) {
        animal->aging();
    }

    vector<unique_ptr<Animal>> new_animals;
    for (auto& animal : animals) {
        animal->reproduce(new_animals, width, height);
    }
    for (auto& new_animal : new_animals) {
        animals.push_back(move(new_animal));
    }

    animals.erase(
        remove_if(animals.begin(), animals.end(),
            [](const unique_ptr<Animal>& animal) { return animal->isDead(); }),
        animals.end()
    );
}

void GameField::print_state(int turn) const {
    cout << "Ход " << turn << endl;
    map<pair<int, int>, int> prey_counts;
    map<pair<int, int>, int> predator_counts;

    for (const auto& animal : animals) {
        auto pos = make_pair(animal->getX(), animal->getY());
        if (animal->getType() == PREY) {
            prey_counts[pos]++;
        } else if (animal->getType() == PREDATOR) {
            predator_counts[pos]++;
        }
    }

    for (int y = 0; y < height; ++y) {
        ostringstream oss_row;
        for (int x = 0; x < width; ++x) {
            auto pos = make_pair(x, y);
            int prey_count = prey_counts[pos];
            int pred_count = predator_counts[pos];

            bool has_animals = (prey_count > 0) || (pred_count > 0);

            if (prey_count > 0) {
                oss_row << "+" << prey_count;
            } else if (pred_count > 0) {
                oss_row << "-" << pred_count;
            } else {
                oss_row << "*";
            }
            
            if (x < width - 1) {
                if (has_animals) {
                    oss_row << " ";
                } else {
                    oss_row << "  ";
                }
            }
        }
        cout << oss_row.str() << "\n";
    }
    cout << endl;
}

void GameField::simulate(int turns) {
    print_state(0);
    for (int i = 1; i <= turns; ++i) {
        simulate_step();
        print_state(i);
    }
}

vector<string> GameField::get_result() const {
    vector<string> result;
    result.reserve(height);

    map<pair<int, int>, int> prey_counts;
    map<pair<int, int>, int> predator_counts;

    for (const auto& animal : animals) {
        auto pos = make_pair(animal->getX(), animal->getY());
        if (animal->getType() == PREY) {
            prey_counts[pos]++;
        } else if (animal->getType() == PREDATOR) {
            predator_counts[pos]++;
        }
    }

    for (int y = 0; y < height; ++y) {
        ostringstream oss_row;
        for (int x = 0; x < width; ++x) {
            auto pos = make_pair(x, y);
            int prey_count = prey_counts[pos];
            int pred_count = predator_counts[pos];

            bool has_animals = (prey_count > 0) || (pred_count > 0);

            if (prey_count > 0) {
                oss_row << "+" << prey_count;
            } else if (pred_count > 0) {
                oss_row << "-" << pred_count;
            } else {
                oss_row << "*";
            }
            
            if (x < width - 1) {
                if (has_animals) {
                    oss_row << " ";
                } else {
                    oss_row << "  ";
                }
            }
        }
        result.push_back(oss_row.str());
    }

    return result;
}

GameField::GameField(int w, int h) : width(w)
                                   , height(h) {}

GameField generate_random_field(int width, int height, int num_prey, int num_predator, int max_turns) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> x_dist(0, width - 1);
    uniform_int_distribution<> y_dist(0, height - 1);
    uniform_int_distribution<> dir_dist(0, 3);
    uniform_int_distribution<> k_dist(1, max_turns);
    uniform_int_distribution<> type_dist(0, 2);

    GameField field(width, height);

    for (int i = 0; i < num_prey; ++i) {
        int x = x_dist(gen);
        int y = y_dist(gen);
        Direction d = static_cast<Direction>(dir_dist(gen));
        int k = k_dist(gen);
        field.add_animal(make_unique<Prey>(x, y, d, k));
    }

    for (int i = 0; i < num_predator; ++i) {
        int x = x_dist(gen);
        int y = y_dist(gen);
        Direction d = static_cast<Direction>(dir_dist(gen));
        int k = k_dist(gen);
        field.add_animal(make_unique<Predator>(x, y, d, k, static_cast<PredatorType>(type_dist(gen))));
    }

    return field;
}