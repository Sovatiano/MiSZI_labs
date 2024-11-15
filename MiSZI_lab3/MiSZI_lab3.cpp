#include <iostream>
#define NOMINMAX
#include <Windows.h>
#include "Generation.h"

using namespace std;

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    setlocale(LC_CTYPE, "Russian");
    big_int key1024_1 = generateRandomNumber(512);
    cout << key1024_1 << endl;
    big_int key1024_2 = generateRandomNumber(512);
    cout << key1024_2 << endl;
}
