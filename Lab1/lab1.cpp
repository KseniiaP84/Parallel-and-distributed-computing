#include <iostream>
#include <windows.h>
#include <thread>
#include <mutex>
#include <list>
#include <string>
#include <chrono>

using namespace std;

// 1.2.1

void Thread1()
{
    cout << "1" << endl;
}

void Thread2()
{
    cout << "2" << endl;
}

void task_1_2_1()
{
    cout << "\n===== 1.2.1 =====\n";

    thread t1(Thread1);
    thread t2(Thread2);
}


// 1.2.2
// Використання detach()

void Thread1_detach()
{
    cout << "1" << endl;
}

void Thread2_detach()
{
    cout << "2" << endl;
}

void task_1_2_2()
{
    cout << "\n===== 1.2.2 =====\n";

    thread t1(Thread1_detach);
    thread t2(Thread2_detach);

    t1.detach();
    t2.detach();

    this_thread::sleep_for(chrono::milliseconds(100));
}


// 1.2.3

list<int> l;

void AddToList(int value)
{
    for (int i = 0; i < 10; i++)
    {
        int number = value + i;

        l.push_back(number);

        cout << "Додано елемент: "
            << number << endl;

        this_thread::sleep_for(
            chrono::milliseconds(10)
        );
    }
}

void ListContains(int value)
{
    for (int i = 0; i < 10; i++)
    {
        bool contains = false;

        for (int element : l)
        {
            if (element == value)
            {
                contains = true;
                break;
            }
        }

        if (contains)
        {
            cout << "Перевірка "
                << i + 1
                << ": "
                << value
                << " входить у список"
                << endl;
        }
        else
        {
            cout << "Перевірка "
                << i + 1
                << ": "
                << value
                << " не входить у список"
                << endl;
        }

        this_thread::sleep_for(
            chrono::milliseconds(10)
        );
    }
}

void task_1_2_3()
{
    cout << "\n===== 1.2.3 =====\n";

    l.clear();

    int value = 5;

    thread t1(AddToList, value);
    thread t2(ListContains, value);

    t1.join();
    t2.join();

    cout << "Кінцевий список: ";

    for (int element : l)
    {
        cout << element << " ";
    }

    cout << endl;
}


// 1.2.4

list<int> l_mutex;
mutex m1;

void AddToList_mutex(int value)
{
    for (int i = 0; i < 10; i++)
    {
        int number = value + i;

        m1.lock();

        l_mutex.push_back(number);

        cout << "Додано елемент: "
            << number << endl;

        m1.unlock();

        this_thread::sleep_for(
            chrono::milliseconds(10)
        );
    }
}

void ListContains_mutex(int value)
{
    for (int i = 0; i < 10; i++)
    {
        m1.lock();

        bool contains = false;

        for (int element : l_mutex)
        {
            if (element == value)
            {
                contains = true;
                break;
            }
        }

        if (contains)
        {
            cout << "Перевірка "
                << i + 1
                << ": "
                << value
                << " входить у список"
                << endl;
        }
        else
        {
            cout << "Перевірка "
                << i + 1
                << ": "
                << value
                << " не входить у список"
                << endl;
        }

        m1.unlock();

        this_thread::sleep_for(
            chrono::milliseconds(10)
        );
    }
}

void task_1_2_4()
{
    cout << "\n===== 1.2.4 =====\n";

    l_mutex.clear();

    int value = 5;

    thread t1(AddToList_mutex, value);
    thread t2(ListContains_mutex, value);

    t1.join();
    t2.join();

    cout << "Кінцевий список: ";

    for (int element : l_mutex)
    {
        cout << element << " ";
    }

    cout << endl;
}


// 1.2.5

list<int> l_guard;
mutex m_guard;

void AddToList_guard(int value)
{
    lock_guard<mutex> lock(m_guard);

    l_guard.push_back(value);

    cout << "Додано елемент: "
        << value << endl;
}

void ListContains_guard(int value)
{
    lock_guard<mutex> lock(m_guard);

    bool contains = false;

    for (int element : l_guard)
    {
        if (element == value)
        {
            contains = true;
            break;
        }
    }

    if (contains)
    {
        cout << "Перевірка: "
            << value
            << " входить у список"
            << endl;
    }
    else
    {
        cout << "Перевірка: "
            << value
            << " не входить у список"
            << endl;
    }
}

void task_1_2_5()
{
    cout << "\n===== 1.2.5 =====\n";

    l_guard.clear();

    int value = 5;

    thread threads[20];

    for (int i = 0; i < 10; i++)
    {
        int addValue = value + i;

        threads[i * 2] =
            thread(AddToList_guard, addValue);

        threads[i * 2 + 1] =
            thread(ListContains_guard, value);
    }

    for (int i = 0; i < 20; i++)
    {
        threads[i].join();
    }

    cout << "Кінцевий список: ";

    for (int element : l_guard)
    {
        cout << element << " ";
    }

    cout << endl;
}


// 1.2.6

class someData
{
public:

    string name;
    string surname;
    string address;
    int age;

    someData()
    {
        name = "";
        surname = "";
        address = "";
        age = 0;
    }

    void print()
    {
        cout << "Ім'я: " << name << endl;
        cout << "Прізвище: " << surname << endl;
        cout << "Адреса: " << address << endl;
        cout << "Вік: " << age << endl;
    }
};


class exchangePerson
{
public:

    someData data;
    mutex m;

    static void JohnDoe(exchangePerson& person)
    {
        lock_guard<mutex> lock(person.m);

        person.data.name = "John";
        person.data.surname = "Doe";
        person.data.address = "Unknown";
        person.data.age = 120;
    }

    static void JacobSmith(exchangePerson& person)
    {
        lock_guard<mutex> lock(person.m);

        person.data.name = "Jacob";
        person.data.surname = "Smith";
        person.data.address = "Known";
        person.data.age = 1;
    }

    static void Swap(
        exchangePerson& person1,
        exchangePerson& person2
    )
    {
        if (&person1 == &person2)
        {
            return;
        }

        lock(person1.m, person2.m);

        lock_guard<mutex> lock1(
            person1.m,
            adopt_lock
        );

        lock_guard<mutex> lock2(
            person2.m,
            adopt_lock
        );

        swap(person1.data, person2.data);
    }
};


void task_1_2_6()
{
    cout << "\n===== 1.2.6 =====\n";

    exchangePerson person1;
    exchangePerson person2;

    thread t1(
        exchangePerson::JohnDoe,
        ref(person1)
    );

    thread t2(
        exchangePerson::JacobSmith,
        ref(person2)
    );

    t1.detach();
    t2.detach();

 
    this_thread::sleep_for(
        chrono::milliseconds(100)
    );

    cout << "\nДо обміну:\n";

    cout << "\nPerson 1:\n";
    person1.data.print();

    cout << "\nPerson 2:\n";
    person2.data.print();


    thread swapThread(
        exchangePerson::Swap,
        ref(person1),
        ref(person2)
    );


    swapThread.join();


    cout << "\nПісля обміну:\n";

    cout << "\nPerson 1:\n";
    person1.data.print();

    cout << "\nPerson 2:\n";
    person2.data.print();
}



// 1.2.7

void SwapUniqueLock(
    exchangePerson& person1,
    exchangePerson& person2
)
{
    if (&person1 == &person2)
    {
        return;
    }

    unique_lock<mutex> lock1(
        person1.m,
        defer_lock
    );

    unique_lock<mutex> lock2(
        person2.m,
        defer_lock
    );



    lock(lock1, lock2);

    swap(person1.data, person2.data);
}


void task_1_2_7()
{
    cout << "\n===== 1.2.7 =====\n";

    exchangePerson person1;
    exchangePerson person2;

    exchangePerson::JohnDoe(person1);
    exchangePerson::JacobSmith(person2);

    cout << "\nДо обміну:\n";

    cout << "\nPerson 1:\n";
    person1.data.print();

    cout << "\nPerson 2:\n";
    person2.data.print();


    thread t(
        SwapUniqueLock,
        ref(person1),
        ref(person2)
    );

    t.join();


    cout << "\nПісля обміну:\n";

    cout << "\nPerson 1:\n";
    person1.data.print();

    cout << "\nPerson 2:\n";
    person2.data.print();
}


// 1.2.8

void SafeSwap(
    exchangePerson& person1,
    exchangePerson& person2
)
{
    if (&person1 == &person2)
    {
        return;
    }

    unique_lock<mutex> lock1(
        person1.m,
        defer_lock
    );

    unique_lock<mutex> lock2(
        person2.m,
        defer_lock
    );



    lock(lock1, lock2);

    swap(person1.data, person2.data);
}


void task_1_2_8()
{
    cout << "\n===== 1.2.8 =====\n";

    exchangePerson person1;
    exchangePerson person2;

    exchangePerson::JohnDoe(person1);
    exchangePerson::JacobSmith(person2);

    thread t1(
        SafeSwap,
        ref(person1),
        ref(person2)
    );

    thread t2(
        SafeSwap,
        ref(person2),
        ref(person1)
    );

    t1.join();
    t2.join();

    cout << "\nОбидва потоки завершили роботу.\n";

    cout << "\nPerson 1:\n";
    person1.data.print();

    cout << "\nPerson 2:\n";
    person2.data.print();
}



int main()
{

    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    task_1_2_2();

    task_1_2_3();

    task_1_2_4();

    task_1_2_5();

    task_1_2_6();

    task_1_2_7();

    task_1_2_8();

    return 0;
}
