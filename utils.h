#ifndef UTILS_H
#define UTILS_H

#include <limits>
#include <string>
#include <iostream>

template<typename T>
T get_valid_input(const string& prompt, T min_val, T max_val) {
    T value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= min_val && value <= max_val) {
                return value;
            } else {
                cout << "Ошибка: Значение должно быть в диапазоне [" << min_val << ", " << max_val << "]. Повторите ввод.\n";
            }
        } else {
            cout << "Ошибка: Введите корректное число. Повторите ввод.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}
#endif