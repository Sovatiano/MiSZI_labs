#include "Utility.h"

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

    while (d % 2 == 0) {
        d /= 2;
        s += 1;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dist(2, UINT64_MAX);

    for (int i = 0; i < iterations; i++) {
        big_int a = dist(gen) % (num - 4) + 2;
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


big_int EuсlidAlgorithm(big_int a, big_int b, big_int &x, big_int &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }

    big_int x1, y1;
    big_int gcd = EuсlidAlgorithm(b, a % b, x1, y1);

    x = y1;
    y = x1 - (a / b) * y1;

    return gcd;
}


big_int EulerFunction(big_int first_prime, big_int second_prime) {
    return (first_prime - 1) * (second_prime - 1);
}


big_int modExp(big_int base, big_int exp, big_int mod) {
    big_int result = 1;
    base = base % mod;

    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;

        exp = exp / 2;
        base = (base * base) % mod;
    }

    return result;
}


std::vector<big_int> readFileAsBigInts(const std::string& filename, size_t blockSize) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }

    std::vector<uint8_t> fileBytes((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());

    std::vector<big_int> bigInts;
    for (size_t i = 0; i < fileBytes.size(); i += blockSize) {
        size_t currentBlockSize = std::min(blockSize, fileBytes.size() - i);

        std::vector<uint8_t> block(fileBytes.begin() + i, fileBytes.begin() + i + currentBlockSize);

        if (currentBlockSize < blockSize) {
            block.resize(blockSize, 0x00);
        }

        big_int num = 0;
        for (uint8_t byte : block) {
            num = num * 256 + byte;
        }
        bigInts.push_back(num);
    }

    return bigInts;
}


std::vector<uint8_t> bigIntToBytes(const big_int& num, size_t blockSize) {
    std::vector<uint8_t> bytes;
    big_int temp = num;

    for (size_t i = 1; i <= blockSize; ++i) {
        bytes.insert(bytes.begin(), static_cast<uint8_t>(temp % 256));
        temp /= 256;
    }

    return bytes;
}


void writeFileFromBigInts(const std::string& filename, std::vector<big_int> bigInts, size_t blockSize) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл для записи: " + filename);
    }

    for (const auto& num : bigInts) {
        std::vector<uint8_t> bytes = bigIntToBytes(num, blockSize);
        file.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
    }
}


bool isFileOpened(std::string& filename, std::string message) {
    std::cin.clear();
    std::cout << message;
    std::getline(std::cin, filename);
    std::ifstream fin(filename, std::ios::binary);
    while (!fin.is_open())
    {
        std::cin.clear();
        std::cout << "Файл не найден, попробуйте ещё раз: ";
        std::getline(std::cin, filename);
        std::ifstream fin(filename, std::ios::binary);
    }

    return true;
    fin.close();
}


std::vector<std::string> readFileInBlocks(const std::string& filename, size_t blockSize) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }

    std::vector<std::string> blocks;

    while (true) {
        std::string block(blockSize, '\0');
        file.read(&block[0], blockSize);
        size_t bytesRead = file.gcount();

        if (bytesRead == 0) {
            break;
        }

        block.resize(bytesRead);
        blocks.push_back(block);
    }

    return blocks;
}