#include "tasks.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

static std::mutex mtx2;
static std::condition_variable cv2;
static int i_var2 = 0;

static void Waits(int id) {
    std::unique_lock<std::mutex> lk(mtx2);
    std::cout << "[Thread " << id << "] Входить у стан очікування...\n";

    // Лямбда-функція перевіряє умова i_var2 == 1
    cv2.wait(lk, []() { return i_var2 == 1; });

    std::cout << "[Thread " << id << "] Очікування завершене!\n";
}

static void Awake() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "[Awake] Перше сповіщення (notify_all) без зміни i_var...\n";
    cv2.notify_all();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    {
        std::lock_guard<std::mutex> lk(mtx2);
        i_var2 = 1;
    }
    std::cout << "[Awake] Присвоєно i_var = 1. Друге сповіщення (notify_all)...\n";
    cv2.notify_all();
}

void runTask2() {
    i_var2 = 0;
    std::thread t1(Waits, 1);
    std::thread t2(Waits, 2);
    std::thread t3(Waits, 3);
    std::thread tAwake(Awake);

    t1.join();
    t2.join();
    t3.join();
    tAwake.join();
}
