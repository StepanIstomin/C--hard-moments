// C++ Сложные моменты. ДЗ №6.
// Author: Stepan Istomin

#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <Windows.h>
#include <math.h>
#include <queue>
#include "Timer.h"

using namespace std::chrono_literals;

/*1. Создайте потокобезопасную оболочку для объекта cout. Назовите ее pcout. Необходимо, чтобы несколько потоков могли обращаться к pcout и информация выводилась в консоль. 
Продемонстрируйте работу pcout.*/

std::mutex m1;
template <typename T>
void pcout(T _print) {
    std::lock_guard m1_lg(m1); // works only at C++17
    std::cout << _print;
}
void func1() {
    for (size_t i = 0; i < 10; i++) {
        if (i == 5) {
            std::this_thread::sleep_for(1s);
        }
        pcout(i);           // thread non-dependent output
        //std::cout << i;   // thread dependent output
        std::cout << " ";
    }
}
void func2() {
    for (size_t i = 100; i < 110; i++) {
        pcout(i);           // thread non-dependent output
        //std::cout << i;   // thread dependent output 
        std::cout << " ";
    }
}
/*2. Реализовать функцию, возвращающую i-ое простое число (например, миллионное простое число равно 15485863). Вычисления реализовать во вторичном потоке. 
В консоли отображать прогресс вычисления (в основном потоке).*/
std::mutex m2;
std::queue<int> progress_queue;
std::condition_variable print_cond;
bool isPrime(size_t num) { // Prime number check func
    if (num < 2) return false;
    if (num == 2) return true;
    double limit = sqrt(num);
    for (size_t i = 3; i <= limit; i+=2) {
        if (num % i == 0) 
            return false;
    }
    return true;
}
int i_prime(int n) {
    if (n == 1) {
        std::lock_guard<std::mutex> pr_lock(m2);
        progress_queue.push(100);
        print_cond.notify_one();
        return 2;
    }
    size_t countPrime = 1;
    size_t progrCount = 0;
    for (size_t i = 3; ; i += 2) {
        if (isPrime(i)) {
            countPrime++;
            if (((countPrime * 100) / n - progrCount) >= 10) {
                progrCount += 10;
                std::lock_guard<std::mutex> pr_lock(m2);
                progress_queue.push(progrCount);
                print_cond.notify_one();

            }
        }
        if (countPrime == n) {
            std::lock_guard<std::mutex> pr_lock(m2);
            progress_queue.push(100);
            print_cond.notify_one();
            return i;
        }
    }
}
void printProgress(int n) {
    std::cout << "Progress: 0%";
    size_t iPr; //i-е простое число
    std::thread calc([&]() {iPr = i_prime(n); });
    calc.detach(); // Если делать join, то поток будет блокировать основной до своего окончания
    // !!! Когда присоединяем через detach, важно проверить что есть условия чтобы основной поток не завершился раньше чем второстепенный 
    while (true)    {
        std::unique_lock<std::mutex> pr_lock(m2);
        print_cond.wait(pr_lock, [] {return !progress_queue.empty(); });
        int status = progress_queue.front();
        progress_queue.pop();
        pr_lock.unlock();
        pcout("...");
        pcout(status);
        pcout("%");
        if (status == 100) { // <-----------  Это и есть условие чтобы поток основной поток завершился только когда второстепенный даст нам "100%"
            std::cout << std::endl;
            break;
        }
    }
    std::cout << n << "-th prime number: " << iPr;
}

/*3. Промоделировать следующую ситуацию. Есть два человека (2 потока): хозяин и вор. Хозяин приносит домой вещи (функция добавляющая случайное число в вектор 
с периодичностью 1 раз в секунду). При этом у каждой вещи есть своя ценность. Вор забирает вещи (функция, которая находит наибольшее число и удаляет из вектора 
с периодичностью 1 раз в 0.5 секунд), каждый раз забирает вещь с наибольшей ценностью.*/
std::mutex m3;
void owner(std::vector<int>& _vec){
    while (!_vec.empty())
    {
        std::this_thread::sleep_for(1s);
        std::lock_guard<std::mutex> th_lock(m3);
        _vec.push_back(rand()%100);
    }
}
void thief(std::vector<int>& _vec){
    while (!_vec.empty())
    {
        std::this_thread::sleep_for(500ms);
        std::lock_guard<std::mutex> th_lock(m3);
        _vec.erase(std::max_element(_vec.begin(), _vec.end()));
    }
}
void task3() {
    std::vector<int> things;
    for (size_t i = 0; i < 10; i++) // кладем 10 вещей в дом в начале
        things.push_back(rand() % 100);
    auto start = std::chrono::steady_clock::now();
    std::thread tOwner(owner, ref(things));
    std::thread tTheif(thief, ref(things));
    tOwner.detach();
    tTheif.detach();
    while (!things.empty())
    {
        std::this_thread::sleep_for(200ms);
        std::cout << "Amount of things :" << things.size()<< std::endl;
    }
    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> Time = end - start;
    std::cout << "thief stole all things in " << Time.count() << " sec" << std::endl;
}

int main()
{
    using namespace std;
    //cout << thread::hardware_concurrency() << endl; // Возвращает количество параллельных потоков,поддерживаемых реализацией.
    //task 1
    cout << "Task 1" << endl;
    thread th_1(func1);
    thread th_2(func2);
    th_1.join();
    th_2.join(); 
    //task 2
    cout << endl << "Task 2" << endl;
    size_t inp = 1'000'000;
    //cout << "Enter the ordinal number of a prime number: "; // На случай если вводить число вручную
    //cin >> inp;
    printProgress(inp);
    //task 3
    cout << endl << "Task 3" << endl;
    task3();
}