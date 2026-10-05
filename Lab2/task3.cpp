#include "tasks.hpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

static std::mutex mtx3;
static std::condition_variable cv3;
static int i_var3 = 0;

static void ThreadFunc(int id) {
    std::unique_lock<std::mutex> lk(mtx3);
    cv3.wait(lk, []() { return i_var3 == 1; });
    std::cout << "Повідомлення з потоку " << id << "\n";
}

static void Notify() {
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    {
        std::lock_guard<std::mutex> lk(mtx3);
        i_var3 = 1;
    }
    std::cout << "[Notify] Викликаємо notify_one()...\n";
    cv3.notify_one();
}

void runTask3() {
    i_var3 = 0;
    std::thread t1(ThreadFunc, 1);
    std::thread t2(ThreadFunc, 2);
    std::thread t3(ThreadFunc, 3);
    std::thread tNotify(Notify);

    tNotify.join();

    // Щоб завершити інші потоки, розсилаємо сповіщення решті
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cv3.notify_all();

    t1.join();
    t2.join();
    t3.join();
}
