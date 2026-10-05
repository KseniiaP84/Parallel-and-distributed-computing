#ifndef HELPERS_HPP
#define HELPERS_HPP

#include <cmath>

// Перевірка числа на простоту
inline bool isPrime(long long n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    for (long long i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

// Пошук N-го простого числа (1-indexed: 1st -> 2, 2nd -> 3, ...)
inline long long getNthPrime(int n) {
    if (n <= 0) return 0;
    int count = 0;
    long long num = 1;
    while (count < n) {
        num++;
        if (isPrime(num)) {
            count++;
        }
    }
    return num;
}

#endif // HELPERS_HPP
