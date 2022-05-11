// C++ Сложные моменты. ДЗ №3.
// Author: Stepan Istomin

#include <iostream>
#include <numeric> //for accumulate
#include <list>
#include <vector>
#include <cmath> //for pow()

//1. Написать функцию, добавляющую в конец списка вещественных чисел элемент, значение которого равно среднему арифметическому всех его элементов.
template <typename T>
void task1(std::list<T>& _list) {
    const double sum = std::accumulate(_list.begin(), _list.end(), 0.0);
    _list.push_back(sum / _list.size());
}

//2. Создать класс, представляющий матрицу. Реализовать в нем метод, вычисляющий определитель матрицы. Для реализации используйте контейнеры из STL.
//template <typename T>
class Matrix
{
public:
    Matrix(int _size) : size(_size) {}; // создается пустая матрица с указанием только размера
    void Init() { // Заполнение матрицы случайными double значениями от -100 до +100
        for (size_t i = 0; i < size; ++i)
        {
            std::vector<double> tempvec;
            for (size_t j = 0; j < size; ++j)
                tempvec.push_back(double(rand() % (10000 - (-10000) + 1) + (-10000)) / 100);
            matrix.push_back(tempvec);
        }
    }
    void Print() { // Вывод матрицы на экран
        for (size_t i = 0; i < size; ++i) {
            for (size_t j = 0; j < size; ++j)
                std::cout << matrix[i][j] << "\t";
            std::cout << "\n";
        }
    }
    friend double det(Matrix _matrix);
    friend Matrix matMinor(Matrix _matrix, int _i, int j);
private:
    int size;
    std::vector<std::vector<double>> matrix;
};
double det(Matrix _matrix) { // рекурсивная функция определителя разложением по первой строке матрицы
    double deter = 0;
    if (_matrix.size == 2) { // при размере минора 2х2 уже можно посчитать определитель
        return _matrix.matrix[0][0] * _matrix.matrix[1][1] - _matrix.matrix[1][0] * _matrix.matrix[0][1];
    }
    else {
        for (size_t j = 0; j < _matrix.size; ++j) {
            deter += pow(-1,2+j)*_matrix.matrix[0][j] * det(matMinor(_matrix,0,j));
        }
        return deter;
    }
}
Matrix matMinor(Matrix _matrix, int _i, int j) { // Функция возвращает матрицу минора для элемента ij исходной матрицы
    Matrix tempmat(_matrix.size-1);
    int src_i = 0;
    for (size_t i = 0; i < tempmat.size; ++i){
        if (i == _i) src_i++;
        std::vector<double> tempvec = _matrix.matrix[src_i++];
        tempvec.erase(tempvec.begin() + j);
        tempmat.matrix.push_back(tempvec);
    }
    return tempmat;
}

//3. Сделать шаблонный итератор с возможностью записи
template <typename T>
class MyIter
{
public:
    MyIter(T& first):Ptr(&first) {};
    void operator=(T val) { *Ptr = val; }
    bool operator!= (const MyIter<T> vol) const { return Ptr != vol.Ptr; }
    bool operator== (const MyIter<T> vol) const { return Ptr == vol.Ptr; }
    T operator*() { return *Ptr; }
    void operator++() { Ptr++; } //Префиксная форма
    void operator--() { Ptr--; }
    T operator++ (int) { return Ptr++; }//Постфиксная форма
    T operator-- (int) { return Ptr--; }
private:
    T* Ptr;
};
template <typename T>
class Range //Для получения начала и конца
{
public:
    Range(T _from, T _to) :from(_from), to(_to) {};
    MyIter<T> begin() const { return MyIter<T>(from); }
    MyIter<T> end() const { return MyIter<T>(to); }
private:
    T from;
    T to;
};

int main()
{
    using namespace std;
    // Task 1
    list<float> list1{1.1, 2.2, 3.3, 4.4};
    for (auto it : list1) 
        cout << it << "\t";
    cout << endl;
    task1(list1);
    for (auto it : list1)
        cout << it << "\t";
    cout << endl<<endl;

    //Task 2
    Matrix task2(5);
    task2.Init();
    task2.Print();
    cout << "\n" << "det = " << det(task2) << endl << endl;

    //Task 3
    vector<int> V = { 1, 2, 3 };
    //int k = 10;
    //MyIter<int> it(k);
    //it = 100;
    //cout << *it << endl;
    for (MyIter<int> it1 : V)
        cout << *it1 << " ";
    cout << endl;
    for (MyIter<int> it1 : V)
        it1 = 0;
    for (MyIter<int> it1 : V)
        cout << *it1 << " ";
    cout << endl;
}