#pragma once
#include "Include.h"


big_int pow_mod(big_int base, big_int exponent, const big_int& modulus);

bool millerRabinTest(const big_int& num, int iterations);

big_int EuñlidAlgorithm(big_int a, big_int b, big_int &x, big_int &y);

big_int EulerFunction(big_int first_prime, big_int second_prime);

big_int modExp(big_int base, big_int exp, big_int mod);

std::vector<big_int> readFileAsBigInts(const std::string& filename, size_t blockSize);

std::vector<uint8_t> bigIntToBytes(const big_int& num, size_t blockSize);

void writeFileFromBigInts(const std::string& filename, std::vector<big_int> bigInts, size_t blockSize);

bool isFileOpened(std::string& filename, std::string message);

std::vector<std::string> readFileInBlocks(const std::string& filename, size_t blockSize);