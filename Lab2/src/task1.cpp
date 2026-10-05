#include "tasks.hpp"
#include "helpers.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>

static std::queue<int> global_queue;
static std::mutex global_mtx;
static bool input_finished = false;

static void DataPreparation() {
    std::cout << "[DataPreparation] Введіть цілі числа (введіть -1 для завершення):\n";
    int val;
    while (std::cin >> val && val != -1) {
        std::lock_guard<std::mutex> lock(global_mtx);
        global_queue.push(val);
    }

    {
        std::lock_guard<std::mutex> lock(global_mtx);
        input_finished = true;
    }
    std::cout << "[DataPreparation] Завершено введення даних.\n";
}

static void DataProcessing() {
    std::cout << "[DataProcessing] Очікування закінчення введення даних...\n";

    // Синхронізація через прапорець та sleep_for
    std::unique_lock<std::mutex> lk(global_mtx);
    while (!input_finished) {
        lk.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lk.lock();
    }

    std::cout << "[DataProcessing] Початок обробки черги:\n";
    while (!global_queue.empty()) {
        int val = global_queue.front();
        global_queue.pop();
        if (isPrime(val)) {
            std::cout << "  Просте число: " << val << "\n";
        }
    }
}

void runTask1() {
    input_finished = false;
    while (!global_queue.empty()) global_queue.pop();

    std::thread t1(DataPreparation);
    t1.detach(); // Від'єднуємо потік підготовки даних

    DataProcessing(); // Головна робота в цьому потоці
}
