#include "tasks.hpp"
#include "helpers.hpp"
#include <iostream>
#include <deque>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <string>

static std::deque<std::packaged_task<long long()>> task_queue;
static std::deque<int> n_queue;
static std::mutex deque_mtx;
static std::condition_variable cv6;
static bool stop_worker = false;

static void WorkerThread() {
    while (true) {
        std::packaged_task<long long()> task;
        int n_val = 0;

        {
            std::unique_lock<std::mutex> lock(deque_mtx);
            cv6.wait(lock, []() { return !task_queue.empty() || stop_worker; });

            if (stop_worker && task_queue.empty()) break;

            task = std::move(task_queue.front());
            task_queue.pop_front();
            n_val = n_queue.front();
            n_queue.pop_front();
        }

        task(); // Виконуємо задачу
    }
}

void runTask6() {
    stop_worker = false;
    std::thread worker(WorkerThread);

    std::deque<std::future<long long>> futures;
    std::deque<int> requested_n;

    std::cout << "Введіть n (номери простих чисел) або 'stop' для завершення:\n";
    std::string input;
    while (true) {
        std::cout << "> ";
        std::cin >> input;
        if (input == "stop") break;

        try {
            int n = std::stoi(input);
            std::packaged_task<long long()> task([n]() { return getNthPrime(n); });
            std::future<long long> fut = task.get_future();

            {
                std::lock_guard<std::mutex> lock(deque_mtx);
                task_queue.push_back(std::move(task));
                n_queue.push_back(n);
            }
            cv6.notify_one();

            futures.push_back(std::move(fut));
            requested_n.push_back(n);
        }
        catch (...) {
            std::cout << "Некоректне введення. Спробуйте ще раз або введіть 'stop'.\n";
        }
    }

    // Сигналізуємо про завершення
    {
        std::lock_guard<std::mutex> lock(deque_mtx);
        stop_worker = true;
    }
    cv6.notify_all();

    // Очікуємо завершення обчислень та виводимо результати
    for (size_t i = 0; i < futures.size(); ++i) {
        long long res = futures[i].get();
        std::cout << "Результат для n = " << requested_n[i] << ": " << res << "\n";
    }

    if (worker.joinable()) worker.join();
}
