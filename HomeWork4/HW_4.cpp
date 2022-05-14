// C++ Сложные моменты. ДЗ №4.
// Author: Stepan Istomin

#include <iostream>
#include <vector>
#include <algorithm>
#include "Timer.h"
#include <chrono>
#include <ctime>
#include <numeric>

/*1. Имеется отсортированный массив целых чисел. Необходимо разработать функцию insert_sorted, которая принимает вектор и новое число и вставляет новое число
в определенную позицию в векторе, чтобы упорядоченность контейнера сохранялась. 
Реализуйте шаблонную функцию insert_sorted, которая сможет аналогично работать с любым контейнером, содержащим любой тип значения.*/

template<typename T1, typename T2>
void insert_sorted(T1& _cont, T2 _vol) {
    auto iter = upper_bound(_cont.begin(), _cont.end(), _vol);
    _cont.emplace(iter, _vol);
    //_cont.insert(iter, _vol); // работает чуть чуть медленнее чем emplace()
}

//void insert_sorted_push(std::vector<int>& _vec, int _vol) { //Функция сделана для сравнения проверки времени выполнения через неоптимальный вариант
//    _vec.push_back(_vol);
//    std::sort(_vec.begin(), _vec.end());
//}

template<typename T>
void vecPrint(const std::vector<T>& _vec) {                 // функция для вывода вектора на экран без использования цикла
    //std::ostream_iterator<T> it_os = { std::cout, " " };  //вариант через итератор вывода
    //std::copy(_vec.begin(), _vec.end(), it_os);
    std::for_each(_vec.begin(), _vec.end(), [](T el) {std::cout << el << " "; });
    std::cout << std::endl;
}

int main()
{
    using namespace std;
    setlocale(LC_ALL, "rus");
    srand(time(0));

    //task 1
    {
        cout << "Task 1" << endl;
        vector<int> vec1(40);
        generate(vec1.begin(), vec1.end(), []() {return rand()% 100; });
        sort(vec1.begin(), vec1.end());
        int ins_vol = rand() % 100;
        cout << "Random number: " << ins_vol << endl;
        vecPrint(vec1);
        
        //auto start = chrono::steady_clock::now();
        insert_sorted(vec1, ins_vol);
        //auto end = chrono::steady_clock::now();
        //chrono::duration<double> Time = end - start;
        //cout << "upper_bound + emplace : " << Time.count() * 1000 << " msec" << endl;
        vecPrint(vec1);
    }

    //task 2
    /*Сгенерируйте вектор a, состоящий из 100 вещественных чисел, представляющий собой значения аналогового сигнала. На основе этого массива чисел создайте другой вектор
    целых чисел b, представляющий цифровой сигнал, в котором будут откинуты дробные части чисел. Выведите получившиеся массивы чисел. Посчитайте ошибку, которой обладает
    цифровой сигнал по сравнению с аналоговым по формуле: сигма (ai-bi)^2
    где:
    N - количество элементов в векторе,
    a - элемент вектора с дробными числами, b - элемент вектора с целыми числами
    Постарайтесь воспользоваться алгоритмическими функциями, не используя циклы.*/
    {
        cout << endl << "Task 2" << endl;
        int n = 100;        // количество элементов в векторе
        float error = 0;    // ошибка цифрового сигнала
        vector<float> a(n); // вектор значений аналогового сигнала
        vector<int> b;      // вектор значений цифрового сигнала
        generate(a.begin(), a.end(), []() {return static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 100.0)); }); //вектор заполняем значениями от 0.0 до 100.0
        cout << endl << "Vector a:" << endl;
        vecPrint(a);
        copy(a.begin(), a.end(), back_inserter(b)); // копирование вектора float в вектор int через адаптер
        cout << endl << "Vector b:" << endl;
        vecPrint(b);
        error = accumulate(a.begin(), a.end(), 0.0, [](float x, float y) {return x + pow(y - static_cast<int>(y), 2); });
        cout << endl << "Ошибка цифрового сигнала: " << error << endl;
    }
}