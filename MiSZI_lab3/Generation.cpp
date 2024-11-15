#include "Generation.h"


big_int pow_mod(big_int base, big_int exponent, const big_int& modulus) {
    big_int result = 1;
    base %= modulus;

    while (exponent > 0) {
        if (exponent % 2 == 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent /= 2;
    }
    return result;
}


bool millerRabinTest(const big_int& num, int iterations) {
    if (num < 2) return false;
    if (num == 2 || num == 3) return true;
    if (num % 2 == 0) return false;

    big_int s = 0;
    big_int d = num - 1;

    // Разложение (num-1) на 2^s * d
    while (d % 2 == 0) {
        d /= 2;
        s += 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(2, UINT64_MAX);

    // Запускаем тест Миллера-Рабина для нескольких итераций
    for (int i = 0; i < iterations; i++) {
        big_int a = dist(gen) % (num - 4) + 2; // Выбираем a случайным образом из [2, num-2]
        big_int x = pow_mod(a, d, num);

        if (x == 1 || x == num - 1) continue;

        bool composite = true;
        for (big_int r = 1; r < s; r++) {
            x = pow_mod(x, 2, num);
            if (x == num - 1) {
                composite = false;
                break;
            }
        }
        if (composite) return false;
    }
    return true;
}

big_int generateRandomNumber(int bitLength) {
    std::random_device rd;
    std::mt19937::result_type seed = rd() ^ (
        (std::mt19937::result_type)
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count() +
        (std::mt19937::result_type)
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
        ).count());

    std::mt19937 gen(seed);
    std::uniform_int_distribution<unsigned long long> dist(2, 1000);

    // Генерация случайного числа в диапазоне [2^(bitLength-1), 2^bitLength - 1]

    big_int lowerBound = big_int(1) << (bitLength - 1);
    big_int upperBound = (big_int(1) << bitLength) - 1;

    big_int result = lowerBound + (upperBound - lowerBound) / dist(gen);


    while (!millerRabinTest(result, 100)) {
        result += 1;
    }
    return result;
}