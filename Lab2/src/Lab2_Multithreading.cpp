#include <iostream>
#include "tasks.hpp"
#include <Windows.h>

int main() {

    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    while (true) {
        std::cout << "\n================ ОБЕРІТЬ ЗАВДАННЯ ================\n"
            << "1 - Завдання 1.2.1 (Синхронізація прапорцем і sleep_for)\n"
            << "2 - Завдання 1.2.2 (condition_variable та notify_all)\n"
            << "3 - Завдання 1.2.3 (condition_variable та notify_one)\n"
            << "4 - Завдання 1.2.4 (Заміна прапорця з 1.2.1 на condition_variable)\n"
            << "5 - Завдання 1.2.5 (std::async: deferred vs async)\n"
            << "6 - Завдання 1.2.6 (std::packaged_task та деки задач)\n"
            << "7 - Завдання 1.2.7 (std::promise та std::future)\n"
            << "0 - Вихід\n"
            << "Вибір: ";

        int choice;
        if (!(std::cin >> choice) || choice == 0) break;

        switch (choice) {
        case 1: runTask1(); break;
        case 2: runTask2(); break;
        case 3: runTask3(); break;
        case 4: runTask4(); break;
        case 5: runTask5(); break;
        case 6: runTask6(); break;
        case 7: runTask7(); break;
        default: std::cout << "Невірний вибір.\n"; break;
        }
    }
    return 0;
}

