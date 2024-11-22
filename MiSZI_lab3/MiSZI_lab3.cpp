#include <iostream>
#define NOMINMAX
#include <Windows.h>
#include "Generation.h"
#include "CryptoFuncs.h"


void ShowMenu() {
    std::cout << "1. Сгенерировать пару открытый/закрытый ключ." << "\n"
        << "2. Шифровать данные." << "\n"
        << "3. Дешифровать данные." << "\n"
        << "0. Выход" << "\n";
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_CTYPE, "Russian");
    size_t blockSize = 50;

    while (true) {
        ShowMenu();
        int action;
        std::cout << "Введите номер интересующей опции: ";
        if (!(std::cin >> action) or action < 0 or action > 3) {
            std::cerr << "Неверный ввод, попробуйте ещё раз." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (action)
        {
        case 1:
        {
            big_int first_prime = generateRandomNumber(255, 256);
            big_int second_prime = generateRandomNumber(255, 256);
            big_int mod = GenerateMod(first_prime, second_prime);
            big_int euler_fun = EulerFunction(first_prime, second_prime);
            big_int exp = GenerateExponent(euler_fun);
            big_int d = GenerateD(exp, euler_fun);

            std::string output_file_name;
            std::cout << "Введите название файла для записи открытого ключа: ";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cin.clear();
            std::getline(std::cin, output_file_name);
            while (output_file_name == "")
            {
                std::cin.clear();
                std::cout << "Введите корректное имя файла: ";
                std::getline(std::cin, output_file_name);
            }

            std::ofstream fout;
            fout.open(output_file_name, std::ios::out);

            fout << mod << "\n" << exp;

            fout.close();

            std::cout << "Введите название файла для записи закытого ключа: ";
            std::cin.clear();
            std::getline(std::cin, output_file_name);
            while (output_file_name == "")
            {
                std::cin.clear();
                std::cout << "Введите корректное имя файла: ";
                std::getline(std::cin, output_file_name);
            }

            fout.open(output_file_name, std::ios::out);

            fout << mod << "\n" << d;

            fout.close();
            break;
        }
        case 2:
        {
            std::string line;
            std::string input_file_name;
            std::ifstream fin;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (isFileOpened(input_file_name, "Введите название файла с открытым ключом: ")) {
                fin.open(input_file_name);
            }

            std::getline(fin, line);
            big_int mod(line.c_str());
            std::getline(fin, line);
            big_int exp(line.c_str());

            std::vector<big_int> message;

            if (isFileOpened(input_file_name, "Введите название файла с текстом для шифрования: ")) {
                message = readFileAsBigInts(input_file_name, blockSize);
            }

            std::vector<std::string> encrypted_message = Encrypt(message, exp, mod);

            std::string output_file_name;
            std::ofstream fout;

            std::cout << "Введите название файла для записи зашифрованного текста: ";
            std::cin.clear();
            std::getline(std::cin, output_file_name);
            while (output_file_name == "")
            {
                std::cin.clear();
                std::cout << "Введите корректное имя файла: ";
                std::getline(std::cin, output_file_name);
            }

            fout.open(output_file_name, std::ios::out);

            for (std::string num : encrypted_message) {
                fout << num;
            }

            fout.close();

            break;
        }

        case 3:
        {
            std::string line;
            std::string input_file_name;
            std::ifstream fin;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (isFileOpened(input_file_name, "Введите название файла с закрытым ключом: ")) {
                fin.open(input_file_name);
            }

            std::getline(fin, line);
            big_int mod(line.c_str());
            std::getline(fin, line);
            big_int d(line.c_str());

            std::vector<std::string> encrypted_text;


            if (isFileOpened(input_file_name, "Введите название файла с текстом для дешифрования: ")) {
                encrypted_text = readFileInBlocks(input_file_name, 200);
            }

            std::vector<big_int> decrypted_message = Decrypt(encrypted_text, d, mod);

            std::string output_file_name;
            std::ofstream fout;

            std::cout << "Введите название файла для записи дешифрованного текста: ";
            std::cin.clear();
            std::getline(std::cin, output_file_name);
            while (output_file_name == "")
            {
                std::cin.clear();
                std::cout << "Введите корректное имя файла: ";
                std::getline(std::cin, output_file_name);
            }

            writeFileFromBigInts(output_file_name, decrypted_message, blockSize);

            break;
        }
        }


        //big_int first_prime = generateRandomNumber(255, 256);
        //big_int second_prime = generateRandomNumber(255, 256);
        ////big_int first_prime = 19;
        ////big_int second_prime = 41;
        //big_int mod = GenerateMod(first_prime, second_prime);
        //big_int euler_fun = EulerFunction(first_prime, second_prime);
        //big_int exp = GenerateExponent(euler_fun);
        //big_int d = GenerateD(exp, euler_fun);

        //std::string filename = "test.txt";

        //size_t blockSize = 50;

        //std::vector<big_int> text = readFileAsBigInts(filename, blockSize);


        //std::vector<big_int> encrypted_text = Encrypt(text, exp, mod);

        //std::vector<big_int> decrypted_text = Decrypt(encrypted_text, d, mod);

        //writeFileFromBigInts("decrypted.txt", decrypted_text, blockSize);

    }
}
