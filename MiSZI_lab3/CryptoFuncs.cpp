#include "CryptoFuncs.h"

std::vector<std::string> Encrypt(std::vector<big_int> message, big_int exp, big_int mod) {
    std::vector<std::string> encrypted_message;
    //std::ofstream fout("encrypted_nums.txt");


    size_t maxLength = 200;

    for (big_int symb : message) {
        big_int encrypted = modExp(symb, exp, mod);


        std::ostringstream oss;
        oss << encrypted;
        std::string encryptedStr = oss.str();


        if (encryptedStr.length() < maxLength) {
            size_t padding = maxLength - encryptedStr.length();
            encryptedStr = std::string(padding, '0') + encryptedStr;
        }

        encrypted = big_int(encryptedStr.c_str());

        //fout << encryptedStr << "\n";
        encrypted_message.push_back(encryptedStr);
    }

    return encrypted_message;
}


std::vector<big_int> Decrypt(std::vector<std::string> encrypted_message, big_int d, big_int mod) {
    std::vector<big_int> decrypted_message;
    //std::ofstream fout("decrypted_nums.txt");

    for (std::string symb : encrypted_message) {
        //fout << symb << "\n";
        big_int num(symb.c_str());
        decrypted_message.push_back(modExp(num, d, mod));
    }

    return decrypted_message;
}