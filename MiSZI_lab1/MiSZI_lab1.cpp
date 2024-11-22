#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>
#define NOMINMAX
#include <Windows.h>

using namespace std;


void ShowMenu() {
    cout << "1. Шифровать данные" << "\n"
        << "2. Дешифровать данные" << "\n"
        << "0. Выход" << "\n";
}

string GenerateKey(vector<char> text, string key) {
    int text_length = text.size();
    int key_length = key.size();

    for (int ind = 0; ind < text_length - key_length; ind++) {
        key.push_back(key[ind % key_length]);
    }

    return key;
}


string Encrypt(vector<char> text, string key) {
    string encrypted_text;
    int text_length = text.size();
    
    for (int ind = 0; ind < text_length; ind++) {
        encrypted_text.push_back((text[ind] + key[ind]));
    }

    return encrypted_text;
}


string Decrypt(vector<char> text, string key) {
    string decrypted_text;
    int text_length = text.size();

    for (int ind = 0; ind < text_length; ind++) {
        decrypted_text.push_back((text[ind] - key[ind]));
    }

    return decrypted_text;
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_CTYPE, "Russian");

    while (true) {
        ShowMenu();
        int action;
        cout << "Введите номер интересующей опции: ";
        if (!(cin >> action) or action < 0 or action > 2) {
            cerr << "Неверный ввод, попробуйте ещё раз." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (action)
        {
        case 1:
        {
            string input_file_name;
            string output_file_name;
            ifstream fin;
            ofstream fout;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            cout << "Введите название файла с данными для шифрования: ";
            getline(cin, input_file_name);
            fin.open(input_file_name, ios::in);
            while (!fin.is_open())
            {
                cin.clear();
                cout << "Файл не найден, попробуйте ещё раз: ";
                getline(cin, input_file_name);
                fin.open(input_file_name, ios::in);
            }
            if (fin.is_open()) {
                string key;
                ifstream t(input_file_name, ios::binary);
                vector<char> text((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
                cout << "Введите ключ: ";
                getline(cin, key);
                key = GenerateKey(text, key);
                string encrypted_text = Encrypt(text, key);

                string output_file_name;
                cout << "Введите название файла для сохранения результата: ";
                cin.clear();
                getline(cin, output_file_name);
                while (output_file_name == "")
                {
                    cin.clear();
                    cout << "Введите корректное имя файла: ";
                    getline(cin, output_file_name);
                }
                ofstream fout;
                fout.open(output_file_name, ios::out);
                fout << encrypted_text;
            }
            fout.close();
            fin.close();

            break;
        }

        case 2:
        {
            string input_file_name;
            string output_file_name;
            ifstream fin;
            ofstream fout;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            cout << "Введите название файла с данными для дешифрования: ";
            getline(cin, input_file_name);
            fin.open(input_file_name, ios::in);
            while (!fin.is_open())
            {
                cin.clear();
                cout << "Файл не найден, попробуйте ещё раз: ";
                getline(cin, input_file_name);
                fin.open(input_file_name, ios::in);
            }
            if (fin.is_open()) {
                string key;
                ifstream t(input_file_name, ios::binary);
                vector<char> text((istreambuf_iterator<char>(t)), istreambuf_iterator<char>());
                cout << "Введите ключ: ";
                getline(cin, key);
                key = GenerateKey(text, key);
                string decrypted_text = Decrypt(text, key);

                string output_file_name;
                cout << "Введите название файла для сохранения результата: ";
                cin.clear();
                getline(cin, output_file_name);
                while (output_file_name == "")
                {
                    cin.clear();
                    cout << "Введите корректное имя файла: ";
                    getline(cin, output_file_name);
                }
                ofstream fout;
                fout.open(output_file_name, ios::out);
                fout << decrypted_text;
            }
            fout.close();
            fin.close();

            break;
        }
            
        case 0: 
        {
            return 0;
        }
        }

    }

    return 0;
}