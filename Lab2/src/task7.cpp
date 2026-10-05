#include "tasks.hpp"
#include "helpers.hpp"
#include <iostream>
#include <thread>
#include <future>
#include <chrono>
#include <cmath>

static void FirstThread(int n, std::promise<long long> p_nth,
    std::promise<bool> p_signal,
    std::promise<long long> p_10nth)
{
    // 1. Обчислюємо n-те просте число
    long long nth_prime = getNthPrime(n);
    p_nth.set_value(nth_prime);

    // 2. Сигналізуємо другому потокові
    p_signal.set_value(true);

    // 3. Обчислюємо (n*10)-те просте число
    long long nth10_prime = getNthPrime(n * 10);
    p_10nth.set_value(nth10_prime);
}

static void SecondThread(int n, std::shared_future<bool> f_signal) {
    // Очікуємо на сигнал від першого потоку
    bool ready = f_signal.get();
    if (ready) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "\n[SecondThread] Квадратний корінь від n=" << n
            << " дорівнює: " << std::sqrt(n) << "\n";
    }
}

void runTask7() {
    int n;
    std::cout << "Введіть значення n: ";
    if (!(std::cin >> n)) return;

    std::promise<long long> prom_nth;
    std::promise<bool> prom_signal;
    std::promise<long long> prom_10nth;

    std::future<long long> fut_nth = prom_nth.get_future();
    std::shared_future<bool> fut_signal = prom_signal.get_future().share();
    std::future<long long> fut_10nth = prom_10nth.get_future();

    std::thread t1(FirstThread, n, std::move(prom_nth), std::move(prom_signal), std::move(prom_10nth));
    std::thread t2(SecondThread, n, fut_signal);

    std::cout << "[Main] " << n << "-те просте число: " << fut_nth.get() << "\n";
    std::cout << "[Main] " << (n * 10) << "-те просте число: " << fut_10nth.get() << "\n";

    t1.join();
    t2.join();
}
