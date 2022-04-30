// C++ Сложные моменты. ДЗ №2.
// Author: Stepan Istomin

#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <chrono>
#include "Timer.h"

//1. Реализуйте шаблонную функцию Swap, которая принимает два указателя и обменивает местами значения, на которые указывают эти указатели 
// (нужно обменивать именно сами указатели, переменные должны оставаться в тех же адресах памяти).
template <typename T>
void Swap(T& a, T& b) {
    //T* temp = *a;
    //*a = *b;
    //*b = temp;
    T temp(std::move(a));
    a = std::move(b);
    b = std::move(temp);
}

//2. Реализуйте шаблонную функцию SortPointers, которая принимает вектор указателей и сортирует указатели по значениям, на которые они указывают.
template <typename T>
void SortPointers(std::vector<T*>&vec) {
    sort(vec.begin(), vec.end(), [](const T* left, const T* right){ return *left < *right; });
}

/*3. Подсчитайте количество гласных букв в книге “Война и мир”. Для подсчета используйте 4 способа:
○ count_if и find
○ count_if и цикл for
○ цикл for и find
○ 2 цикла for
Замерьте время каждого способа подсчета и сделайте выводы.*/

const std::string vowel = "аоиеёэыуюяАОИЕЁЭЫУЮЯ"; // Список русских гласных букв

bool isVowelFind(char chr) {
    if (vowel.find(chr) != std::string::npos)
        return true;
    else
return false;
}

bool isVowelFor(char chr) {
    for (size_t i = 0; i < vowel.length(); i++) {
        if (chr == vowel[i])
            return true;
    }
    return false;
}

int find1(std::ifstream& _file) { // std::count_if + find
    std::string str;
    int vowelCount = 0;
    while (getline(_file, str)) { //Построковое чтение
        vowelCount += std::count_if(str.begin(), str.end(), isVowelFind);
    }
    return vowelCount;
}

int find2(std::ifstream& _file) { // std::count_if + for
    std::string str;
    int vowelCount = 0;
    while (getline(_file, str)) { //Построковое чтение
        vowelCount += std::count_if(str.begin(), str.end(), isVowelFor);
    }
    return vowelCount;
}

//int find22(std::ifstream& _file) { // for + std::count 
//    std::string str;
//    int vowelCount = 0;
//    while (getline(_file, str)) { //Построковое чтение
//        for (size_t i = 0; i < strlen(vowel); i++) {
//            vowelCount += std::count(str.begin(), str.end(), vowel[i]);
//        }
//    }
//    return vowelCount;
//}

//int find33(std::ifstream& _file) { // for + std::find
//    std::string str;
//    int vowelCount = 0;
//    while (getline(_file, str)) {
//        for (size_t i = 0; i < strlen(vowel); i++) {
//            std::string::iterator it; // объявляем итератор
//            it = str.begin();
//            while (it != str.end()) {
//                auto found{ std::find(it, str.end(), vowel[i]) };
//                if (found != str.end()) {
//                    vowelCount++;
//                    it = found+1;
//                }
//                else break;
//            }
//        }
//    }
//    return vowelCount;
//}

int find3(std::ifstream& _file) { // for + string.find
    std::string str;
    int vowelCount = 0;
    while (getline(_file, str)) { //Построковое чтение
        for (size_t i = 0; i < vowel.length(); i++) {
            std::string tempStr = str; // Временная строка
            while (tempStr.find(vowel[i]) != std::string::npos) {
                    vowelCount++;
                    tempStr = tempStr.substr(tempStr.find(vowel[i])+1);
                }
            }
        }
    return vowelCount;
}

int find4(std::ifstream& _file) {
    std::string str;
    int vowelCount = 0;
    while (getline(_file, str)) {
        for (size_t i = 0; i < vowel.length(); i++) {
            for (size_t j = 0; j < str.length(); j++) {
                if (vowel[i] == str[j])
                    vowelCount++;
            }
        }
    }
    return vowelCount;
}

int main()
{
    using namespace std;
    setlocale(LC_ALL, "rus");
     //Task 1
    {
        int x = 20;
        int y = 10;
        int* x_ptr = &x;
        int* y_ptr = &y;
        cout << "int x " << x << " " << &x << "\tptr int x " << *x_ptr << " " << x_ptr << endl;
        cout << "int y " << y << " " << &y << "\tptr int y " << *y_ptr << " " << y_ptr << endl;
        Swap(x_ptr, y_ptr);
        cout << "int x " << x << " " << &x << "\tptr int x " << *x_ptr << " " << x_ptr << endl;
        cout << "int y " << y << " " << &y << "\tptr int y " << *y_ptr << " " << y_ptr << endl;
        string str1 = "one";
        string str2 = "two";
        string* str1_ptr = &str1;
        string* str2_ptr = &str2;
        cout << "string str1 " << str1 << " " << &str1 << "\tptr string 1 " << *str1_ptr << " " << str1_ptr << endl;
        cout << "string str2 " << str2 << " " << &str2 << "\tptr string 2 " << *str2_ptr << " " << str2_ptr << endl;
        Swap(str1_ptr, str2_ptr);
        cout << "string str1 " << str1 << " " << &str1 << "\tptr string 1 " << *str1_ptr << " " << str1_ptr << endl;
        cout << "string str2 " << str2 << " " << &str2 << "\tptr string 2 " << *str2_ptr << " " << str2_ptr << endl;
    }

    // Task 2
    {
        int a = 5;
        int b = 2;
        int c = 8;
        int* ptr_a = &a;
        int* ptr_b = &b;
        int* ptr_c = &c;
        vector<int*> vecInt = { ptr_a, ptr_b, ptr_c };
        for (auto it : vecInt)
            cout << *it << " ";
        cout << endl;
        SortPointers(vecInt);
        for (auto it : vecInt)
            cout << *it << " ";
        cout << endl;
    }

    // Task 3
    {
    string path = "War and peace.txt";

    ifstream file1(path); // путь к файлу .txt
    auto start = chrono::steady_clock::now();
    cout << "Гласных: " << find2(file1);
    auto end = chrono::steady_clock::now();
    chrono::duration<double> Time = end - start;
    cout << " - count_if + find : " << Time.count() * 1000 << " мсек" << endl;

    ifstream file2(path); 
    start = chrono::steady_clock::now();
    cout << "Гласных: " << find2(file2);
    end = chrono::steady_clock::now();
    Time = end - start;
    cout << " - count_if + for : " << Time.count() * 1000 << " мсек" << endl;
    
    //ifstream file22(path); 
    //start = chrono::steady_clock::now();
    //cout << "Гласных: " << find2(file22);
    //end = chrono::steady_clock::now();
    //Time = end - start;
    //cout << " - count + for : " << Time.count() * 1000 << " мсек" << endl;

    ifstream file3(path); 
    start = chrono::steady_clock::now();
    cout << "Гласных: " << find3(file3);
    end = chrono::steady_clock::now();
    Time = end - start;
    cout << " - string.find + for : " << Time.count() * 1000 << " мсек" << endl;

    //ifstream file33(path);
    //start = chrono::steady_clock::now();
    //cout << "Гласных: " << find3(file33);
    //end = chrono::steady_clock::now();
    //Time = end - start;
    //cout << " - std::find + for : " << Time.count() * 1000 << " мсек" << endl;

    ifstream file4(path); 
    start = chrono::steady_clock::now();
    cout << "Гласных: " << find4(file4);
    end = chrono::steady_clock::now();
    Time = end - start;
    cout << " - for + for : " << Time.count() * 1000 << " мсек" << endl;
    }
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"
