#include "tasks.hpp"
#include "helpers.hpp"
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

static std::queue<int> global_queue4;
static std::mutex mtx4;
static std::condition_variable cv4;
static bool input_finished4 = false;

static void DataPreparation4() {
    std::cout << "[DataPreparation] Введіть цілі числа (введіть -1 для завершення):\n";
    int val;
    while (std::cin >> val && val != -1) {
        {
            std::lock_guard<std::mutex> lock(mtx4);
            global_queue4.push(val);
        }
        cv4.notify_one();
    }
    {
        std::lock_guard<std::mutex> lock(mtx4);
        input_finished4 = true;
    }
    cv4.notify_all();
}

static void DataProcessing4() {
    std::cout << "[DataProcessing] Очікування на дані через condition_variable...\n";
    while (true) {
        std::unique_lock<std::mutex> lk(mtx4);
        // Чекаємо, поки черга не стане не порожньою або не завершиться введення
        cv4.wait(lk, []() { return !global_queue4.empty() || input_finished4; });

        while (!global_queue4.empty()) {
            int val = global_queue4.front();
            global_queue4.pop();
            if (isPrime(val)) {
                std::cout << "  [Просте число]: " << val << "\n";
            }
        }

        if (input_finished4 && global_queue4.empty()) {
            break;
        }
    }
}

void runTask4() {
    input_finished4 = false;
    while (!global_queue4.empty()) global_queue4.pop();

    std::thread t1(DataPreparation4);
    t1.detach();

    DataProcessing4();
}
