#include <iostream>
#include "GameField.h"
#include "utils.h"

int main() {
    cout << "Меню" << endl;
    cout << "Выберите режим ввода:" << endl;
    cout << "1. Ручной ввод" << endl;
    cout << "2. Случайная генерация" << endl;
    cout << endl;

    int mode_choice = get_valid_input<int>("Введите 1 или 2: ", 1, 2);

    if (mode_choice == 1) {
        int N, M, T;
        int R, W;

        N = get_valid_input<int>("Введите ширину поля N (1 - 1000): ", 1, 1000);
        M = get_valid_input<int>("Введите высоту поля M (1 - 1000): ", 1, 1000);
        T = get_valid_input<int>("Введите количество ходов T (0 - 10000): ", 0, 10000);
        R = get_valid_input<int>("Введите количество жертв R (0 - 10000): ", 0, 10000);
        W = get_valid_input<int>("Введите количество хищников W (0 - 10000): ", 0, 10000);

        GameField field(N, M);

        for (int i = 0; i < R; ++i) {
            int x = get_valid_input<int>("Введите координату x для жертвы " + to_string(i + 1) + " (0 - " + to_string(N - 1) + "): ", 0, N - 1);
            int y = get_valid_input<int>("Введите координату y для жертвы " + to_string(i + 1) + " (0 - " + to_string(M - 1) + "): ", 0, M - 1);
            int d = get_valid_input<int>("Введите направление d (0-UP, 1-RIGHT, 2-DOWN, 3-LEFT) для жертвы " + to_string(i + 1) + ": ", 0, 3);
            int k = get_valid_input<int>("Введите параметр k (1 - " + to_string(T) + ") для жертвы " + to_string(i + 1) + ": ", 1, T);
            field.add_animal(make_unique<Prey>(x, y, static_cast<Direction>(d), k));
        }

        for (int i = 0; i < W; ++i) {
            int x = get_valid_input<int>("Введите координату x для хищника " + to_string(i + 1) + " (0 - " + to_string(N - 1) + "): ", 0, N - 1);
            int y = get_valid_input<int>("Введите координату y для хищника " + to_string(i + 1) + " (0 - " + to_string(M - 1) + "): ", 0, M - 1);
            int d = get_valid_input<int>("Введите направление d (0-UP, 1-RIGHT, 2-DOWN, 3-LEFT) для хищника " + to_string(i + 1) + ": ", 0, 3);
            int k = get_valid_input<int>("Введите параметр k (1 - " + to_string(T) + ") для хищника " + to_string(i + 1) + ": ", 1, T);
            int p_type_int = get_valid_input<int>("Введите подвид (0 - Bear, 1 - Fox, 2 - Wolf) для хищника " + to_string(i + 1) + ": ", 0, 2);
            field.add_animal(make_unique<Predator>(x, y, static_cast<Direction>(d), k, static_cast<PredatorType>(p_type_int)));
        }

        field.simulate(T);

    } else if (mode_choice == 2) {
        int N, M, T;
        int R, W;

        N = get_valid_input<int>("Введите ширину поля N (1 - 1000): ", 1, 1000);
        M = get_valid_input<int>("Введите высоту поля M (1 - 1000): ", 1, 1000);
        T = get_valid_input<int>("Введите количество ходов T (0 - 10000): ", 0, 10000);
        R = get_valid_input<int>("Введите количество жертв R (0 - 10000): ", 0, 10000);
        W = get_valid_input<int>("Введите количество хищников W (0 - 10000): ", 0, 10000);

        GameField field = generate_random_field(N, M, R, W, T);

        field.simulate(T);

    } else {
        cout << "Неверный выбор. Завершение программы." << endl;
        return 1;
    }

    return 0;
}