#include "tasks.hpp"
#include "helpers.hpp"
#include <iostream>
#include <future>
#include <chrono>
#include <cmath>

void runTask5() {
    int n;
    std::cout << "Введіть n (номер простого числа, наприклад 500000): ";
    if (!(std::cin >> n)) return;

   
    // ТЕСТ 1: std::launch::deferred (Відкладений виклик)
    std::cout << "\n--- [1] Режим std::launch::deferred ---\n";

    // Створюємо відкладену задачу (обчислення НЕ починаються!)
    std::future<long long> f_def = std::async(std::launch::deferred, getNthPrime, n);

    std::cout << "Оберіть математичну операцію над n (1 - sqrt, 2 - sin, 3 - log): ";
    int choice;
    std::cin >> choice;

    double math_res = 0;
    if (choice == 1) math_res = std::sqrt(n);
    else if (choice == 2) math_res = std::sin(n);
    else math_res = std::log(n);
    std::cout << "Результат математичної функції: " << math_res << "\n";

    std::cout << "Викликаємо get() для deferred (обчислення починаються ЛИШЕ ЗАРАЗ у поточному потоці)...\n";

    // Засікаємо час безпосередньо перед і після get()
    auto start_def = std::chrono::high_resolution_clock::now();
    long long prime_def = f_def.get(); // Обчислення виконуються прямо тут
    auto end_def = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed_def = end_def - start_def;
    std::cout << n << "-те просте число: " << prime_def
        << " | Час очікування get(): " << elapsed_def.count() << " ms\n";

    // =======================================================
    // ТЕСТ 2: std::launch::async (Паралельний виклик)
    // =======================================================
    std::cout << "\n--- [2] Режим std::launch::async ---\n";

    // Обчислення починаються ОДРАЗУ в окремому потоці!
    auto start_async_total = std::chrono::high_resolution_clock::now();
    std::future<long long> f_async = std::async(std::launch::async, getNthPrime, n);

    std::cout << "Обчислення запущені у фоновому потоці!\n";
    std::cout << "Оберіть математичну операцію над n (1 - sqrt, 2 - sin, 3 - log): ";
    std::cin >> choice;

    if (choice == 1) math_res = std::sqrt(n);
    else if (choice == 2) math_res = std::sin(n);
    else math_res = std::log(n);
    std::cout << "Результат математичної функції: " << math_res << "\n";

    std::cout << "Викликаємо get() для async (чекаємо залишку обчислень)...\n";

    auto start_async_wait = std::chrono::high_resolution_clock::now();
    long long prime_async = f_async.get(); // Дочікуємося фонового потоку
    auto end_async_wait = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> elapsed_async_wait = end_async_wait - start_async_wait;
    std::chrono::duration<double, std::milli> elapsed_async_total = end_async_wait - start_async_total;

    std::cout << n << "-те просте число: " << prime_async
        << " | Залишок часу очікування у get(): " << elapsed_async_wait.count() << " ms\n"
        << "   (Повний час від запуску потоку: " << elapsed_async_total.count() << " ms)\n";
}
