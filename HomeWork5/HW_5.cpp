// C++ Сложные моменты. ДЗ №5.
// Author: Stepan Istomin

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <sstream>
#include <set>
#include <algorithm>
#include <windows.h>
#include <map>

/*1. Создать шаблонную функцию, которая принимает итераторы на начало и конец последовательности слов, и выводящую в консоль список уникальных слов 
(если слово повторяется больше 1 раза, то вывести его надо один раз). Продемонстрировать работу функции, передав итераторы различных типов.*/
std::string filt_elems("\",.;:!?)(\\/"); // массив знаков препинания
bool isPunctMark(const char chr) {      // функция определения знаков препинания
    if (filt_elems.find(chr) != std::string::npos)
        return true;
    else
        return false;
}
template <typename It>
void ph_uniq(It _begin, It _end) {
    std::string text, word;             // text - строка, word - отдельное слово
    std::set<std::string> uniq_words;   // set из уникальных слов
    std::copy(_begin, _end, std::back_inserter(text));
    for (std::string::iterator it = text.begin(); it != text.end(); it++)   // Удаление знаков препинания из фразы
    {
        if (isPunctMark(*it))
            text.erase(static_cast<int>(it-- - text.begin()), 1); // !!! Необходимо уменьшать итератор т.к. строка уменьшается на 1 и нужно проверить следующий символ вставший на эту же позицию
    }
    std::stringstream ss(text); //Создаем поток ss из передаваемой string строки
    while (ss >> word)          // передаем из потока ss по одному слову, разделитель - пробел
        uniq_words.insert(word);
    std::for_each(uniq_words.begin(), uniq_words.end(), [](std::string el) {std::cout << el << std::endl; }); //вывод на экран
}
/*2. Используя ассоциативный контейнер, напишите программу, которая будет считывать данные введенные пользователем из стандартного потока ввода и разбивать их на предложения. 
Далее программа должна вывести пользователю все предложения, а также количество слов в них, отсортировав предложения по количеству слов.*/

std::vector<std::string> sent_ends = { ". ","! ","? ","... ","?! ","!? " };   // Варианты знаков препинания заказчивающих предложение

int words_count(const std::string& _inputStr) { //функция подсчета слов в предложении
    std::string tmp_text = _inputStr;
    return std::count(tmp_text.begin(), tmp_text.end(), ' ');
    //return std::count(tmp_text.begin(), tmp_text.end(), ' ') + 1; 
    //Этот вариант не используем, т.к. после окончания предложения остается пробел, 
    //надо использовать только если предложения обработаны функцией аналогом trim() из php, убирающей пробелы в начале и конце строки
}
void sentenсesCut(std::string  &_input) {
    std::multimap <int, std::string> sentenсes; // для хранения предложений отсортированных по кол-ву слов
    std::set<int> sentEndPos;                   // Позиции найденных концов предложений для последующей разбивки текста
    for (size_t i = 0; i < sent_ends.size(); i++) { // Цикл по различным вариантам конца предложения
        int posFind = 0;
        for (size_t j = 0; j < _input.size();)  // Цикл по тексту, начинаем искать с начала (j=0), при найденном конце предложения j меняется
        {
            posFind = _input.find(sent_ends[i], j);
            if (posFind != std::string::npos) {
                sentEndPos.insert(posFind + sent_ends[i].size()); //Вставляем позицию начала следующего предложения
                j = posFind + sent_ends[i].size();
            }
            else
                j = _input.size();
        }
    }
    //разбивка текста на предложения по найденным позициям
    int startPos = 0;   // Позиция с которой будет начало копируемого предложения
    for (auto it : sentEndPos) {
        std::string tmpSentence; // string для отдельного предложения
        tmpSentence.assign(_input, startPos, it - startPos);
        sentenсes.insert(std::pair<int, std::string>(words_count(tmpSentence), tmpSentence));
        startPos = it;
    }
    // вывод на экран кол-ва слов и предложения
    std::multimap<int, std::string>::iterator itr;
    for (itr = sentenсes.begin(); itr != sentenсes.end(); ++itr)
        std::cout << itr->first << " - " << itr->second << '\n';
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    using namespace std;
    setlocale(LC_ALL, "rus");

    //task 1
    string phrase = "Why do you cry, Willy? Why do you cry? Why, Willy? Why, Willy? Why, Willy? Why?"; // Tongue Twister
    vector<char> vec_phrase;
    deque<char> dec_phrase;
    copy(phrase.begin(), phrase.end(), back_inserter(vec_phrase));
    copy(phrase.begin(), phrase.end(), back_inserter(dec_phrase));
    cout << "\tTask1" << endl << endl;
    cout << "----- string pointer -----" << endl;
    ph_uniq(phrase.begin(), phrase.end());
    cout << "----- vector pointer -----" << endl;
    ph_uniq(vec_phrase.begin(), vec_phrase.end());
    cout << "----- deque pointer -----" << endl;
    ph_uniq(dec_phrase.begin(), dec_phrase.end());

   //task 2 
    string input;
    cout << "\tTask2" << endl << endl;
    cout << "Enter your text: ";
    getline(cin,input);
    sentenсesCut(input);
}