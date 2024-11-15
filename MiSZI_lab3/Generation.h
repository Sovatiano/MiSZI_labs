#include <arageli/big_int.hpp>
#include <random>
#include <ctime>
#include <chrono>

using namespace Arageli;


big_int pow_mod(big_int base, big_int exponent, const big_int& modulus);

bool millerRabinTest(const big_int& num, int iterations);

big_int generateRandomNumber(int bitLength);