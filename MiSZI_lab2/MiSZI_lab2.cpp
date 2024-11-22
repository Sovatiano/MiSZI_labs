#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cmath>
#define NOMINMAX
#include <Windows.h>

using namespace std;


double calculateIndex(const vector<char> text) {
    map<char, int> frequency;
    int text_len = text.size();

    for (char ch : text) {
        frequency[tolower(ch)]++;
    }

    int numerator = 0;
    for (auto it = frequency.begin(); it != frequency.end(); it++)
    {
        numerator = numerator + it->second * (it->second - 1);
    }

    double denumerator = text_len * (text_len - 1);
    double result = numerator / denumerator;

    return result;
}


int calculateKeyLength(int lower, int upper, vector<char> text) {
    int bestKeyLength = 0;
    double bestInd = 0;
    

    for (int keyLength = 1; keyLength <= upper; ++keyLength) {
        double avgInd = 0.0;

        for (int i = 0; i < keyLength; ++i) {
            vector<char> segment;
            for (size_t j = i; j < text.size(); j += keyLength) {
                segment.push_back(text[j]);
            }
            avgInd += calculateIndex(segment);
        }
        avgInd /= keyLength;

        //double diff = abs(avgInd - baseInd);
        if (avgInd > bestInd) {
            avgInd = bestInd;
            bestKeyLength = keyLength;
        }
    }

    return bestKeyLength;

}


map<char, double> calculateFreq(vector<char> text) {
    map<char, double> frequency;

    for (int i = 0; i <= 255; i++) {
        frequency[char(i)] = 0;
    }

    for (char ch : text) {
        frequency[ch]++;
    }

    int text_len = text.size();

    for (auto it = frequency.begin(); it != frequency.end(); it++)
    {
        it->second = it->second / text_len;
    }

    return frequency;
}


vector<vector<char>> splitText(int keyLength, vector<char> text) {
    vector<vector<char>> splitedText(keyLength);
    for (int i = 0; i < text.size(); ++i) {
        splitedText[i % keyLength].push_back(text[i]);
    }

    return splitedText;
}

char findMax(map<char, double> freqMap) {
    double maxFreq = 0.0;
    char bestchar;
    for (auto it = freqMap.begin(); it != freqMap.end(); it++) {
        if (it->second > maxFreq) {
            maxFreq = it->second;
            bestchar = it->first;
        }
    }

    return bestchar;
}


int determineShift(const vector<char> segment, map<char, double> referenceFreq) {
    int bestShift = 0;
    double bestCorrelation = 100;
    
    map<char, double> segmentFreq = calculateFreq(segment);

    char most_often_segment = findMax(segmentFreq);
    char most_often_base = findMax(referenceFreq);
    bestShift = most_often_segment - most_often_base;
    /*for (int shift = 0; shift < 256; ++shift) {
        double correlation = 0.0;
        for (int i = -128; i < 128; ++i) {
            correlation += abs(segmentFreq[i] - referenceFreq[(i - shift) % 256]);
        }
        if (correlation < bestCorrelation) {
            bestCorrelation = correlation;
            bestShift = shift;
        }
    }*/


    return bestShift;
}


string GenerateKey(vector<char> text, string key) {
    int text_length = text.size();
    int key_length = key.size();

    for (int ind = 0; ind < text_length - key_length; ind++) {
        key.push_back(key[ind % key_length]);
    }

    return key;
}



string Decrypt(vector<char> text, string key) {
    string decrypted_text;
    int text_length = text.size();

    for (int ind = 0; ind < text_length; ind++) {
        decrypted_text.push_back((text[ind] - key[ind] + 256) % 256);
    }

    return decrypted_text;
}


void ShowMenu() {
    cout  << "1. Дешифровать данные" << "\n"
        << "0. Выход" << "\n";
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
            string base_file_name;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();
            cout << "Введите название файла с эталонным текстом: ";
            getline(cin, base_file_name);
            ifstream fin_base(base_file_name, ios::binary);
            while (!fin_base.is_open())
            {
                cin.clear();
                cout << "Файл не найден, попробуйте ещё раз: ";
                getline(cin, base_file_name);
                ifstream fin(base_file_name, ios::binary);
            }

            vector<char> base((istreambuf_iterator<char>(fin_base)), istreambuf_iterator<char>());
            string input_file_name;
            cout << "Введите название файла с данными для дешифрования: ";
            getline(cin, input_file_name);
            ifstream fin(input_file_name, ios::binary);
            while (!fin.is_open())
            {
                cin.clear();
                cout << "Файл не найден, попробуйте ещё раз: ";
                getline(cin, input_file_name);
                ifstream fin(input_file_name, ios::binary);
            }
            vector<char> text((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());

            fin.close();
            fin_base.close();

            cout << "Введите через пробел диапазон предполагаемой длины пароля: ";
            int left, right;
            cin >> left; cin >> right;

            int test_key_len = calculateKeyLength(left, right, text);
            cout << "Предполагаемая длина ключа:" << test_key_len << endl;

            map<char, double> reference = calculateFreq(base);
            vector<vector<char>> splitedText = splitText(test_key_len, text);
            string key;
            for (int i = 0; i < splitedText.size(); i++) {
                key += char(determineShift(splitedText[i], reference));
            }

            cout << "Предполагаемый пароль:" << key << endl;
            key = GenerateKey(text, key);
            string decrypted = Decrypt(text, key);

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.clear();

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
            fout << decrypted;
            fout.close();
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

