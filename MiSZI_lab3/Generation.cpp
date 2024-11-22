#include "Generation.h"


big_int generateRandomNumber(int minBitLength, int maxBitLength) {
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

    big_int lowerBound = big_int(1) << (minBitLength - 1);
    big_int upperBound = (big_int(1) << maxBitLength) - 1;

    big_int result = lowerBound + (upperBound - lowerBound) / dist(gen);


    while (!millerRabinTest(result, 100)) {
        result += 1;
    }
    return result;
}


big_int GenerateMod(big_int first_prime, big_int second_prime) {
    return first_prime * second_prime;
}


big_int GenerateExponent(big_int euler_fun) {
    int exp_size = 10;
    big_int exp = generateRandomNumber(exp_size, exp_size * 1.5);
    while (!(euler_fun % exp != 0) and !(euler_fun > exp)) {
        big_int exp = generateRandomNumber(exp_size, exp_size * 1.5);
        exp_size += 1;
    }

    return exp;
}


big_int GenerateD(big_int exp, big_int euler_fun) {
    big_int x, y;
    big_int gcd = abs(EuñlidAlgorithm(exp, euler_fun, x, y));
    if (gcd != 1) {
        std::cout << "Ýêñïîíåíòà è Ôè íå âçàèìíî ïðîñòû" << std::endl;
    }
    if (x < 0) {
        x += euler_fun;
    }

    return x;
}