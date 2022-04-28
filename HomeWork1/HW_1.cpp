// HW_1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <tuple>    // кортеж, для использования tie
#include <optional> // для использования опциональных типов данных
#include <vector>
#include <algorithm>
#include <fstream>

//Task 1
struct Person
{
    std::string  surname; 
    std::string  name;
    std::optional <std::string>  patronym;
    friend std::ostream& operator<<(std::ostream& os, const Person& _p);
    friend bool operator <  (const Person& p1, const Person& p2);
    friend bool operator >  (const Person& p1, const Person& p2);
    friend bool operator == (const Person& p1, const Person& p2);
    friend bool operator != (const Person& p1, const Person& p2);
};
bool operator <  (const Person& p1, const Person& p2) {
    return std::tie(p1.surname, p1.name, p1.patronym) < std::tie(p2.surname, p2.name, p2.patronym);
}
bool operator >  (const Person& p1, const Person& p2) {
    return std::tie(p1.surname, p1.name, p1.patronym) > std::tie(p2.surname, p2.name, p2.patronym);
}
bool operator == (const Person& p1, const Person& p2) {
    return std::tie(p1.surname, p1.name, p1.patronym) == std::tie(p2.surname, p2.name, p2.patronym);
}
bool operator != (const Person& p1, const Person& p2) {
    return std::tie(p1.surname, p1.name, p1.patronym) != std::tie(p2.surname, p2.name, p2.patronym);
}
std::ostream& operator<<(std::ostream& os, const Person& _p)
{
    os << _p.surname;
    if (_p.surname.size() < 8)
        os << "\t\t";
    else
        os << "\t";
    os << _p.name;
    if (_p.name.size() < 8)
        os << "\t\t";
    else
        os << "\t";
    if (auto _patr = _p.patronym; _patr) {
        os << " " << *_patr;
    }
    else
        os << "\t";
    return os;
}
//Task 2
struct PhoneNumber
{
    int codeCountry;
    int codeCity;
    std::string number;
    std::optional<int> numberAdd;
    friend std::ostream& operator<<(std::ostream& os, const PhoneNumber& _n);
    friend bool operator <  (const PhoneNumber& p1, const PhoneNumber& p2);
    friend bool operator >  (const PhoneNumber& p1, const PhoneNumber& p2);
    friend bool operator == (const PhoneNumber& p1, const PhoneNumber& p2);
    friend bool operator != (const PhoneNumber& p1, const PhoneNumber& p2);
};
bool operator <  (const PhoneNumber& p1, const PhoneNumber& p2) {
    return std::tie(p1.codeCountry, p1.codeCity, p1.number, p1.numberAdd) < std::tie(p2.codeCountry, p2.codeCity, p2.number, p2.numberAdd);
}
bool operator >  (const PhoneNumber& p1, const PhoneNumber& p2) {
    return std::tie(p1.codeCountry, p1.codeCity, p1.number, p1.numberAdd) > std::tie(p2.codeCountry, p2.codeCity, p2.number, p2.numberAdd);
}
bool operator == (const PhoneNumber& p1, const PhoneNumber& p2) {
    return std::tie(p1.codeCountry, p1.codeCity, p1.number, p1.numberAdd) == std::tie(p2.codeCountry, p2.codeCity, p2.number, p2.numberAdd);
}
bool operator != (const PhoneNumber& p1, const PhoneNumber& p2) {
    return std::tie(p1.codeCountry, p1.codeCity, p1.number, p1.numberAdd) != std::tie(p2.codeCountry, p2.codeCity, p2.number, p2.numberAdd);
}
std::ostream& operator<<(std::ostream& os, const PhoneNumber& _n)
{
    os << "+" << _n.codeCountry << "(" << _n.codeCity << ")" << _n.number;
    if (auto _addnum = _n.numberAdd; _addnum) {
        os << " " << *_addnum;
    }
    return os;
}
//Task 3
class PhoneBook
{
public:
    PhoneBook(std::ifstream& _file) {
        std::string str;
        Person tempPers;
        PhoneNumber tempNumber;
        while (getline(_file, str)) { // читаем по строкам файл и помещаем в промежуточную переменную str
            std::string strName = str.substr(0, str.find("+")-1);// отделяем блок с именем
            std::string strPhone = str.substr(str.find("+")+1);// отделяем блок с телефоном
            //обработка блок имени
            tempPers.surname = strName.substr(0, strName.find(" "));
            strName = strName.substr(strName.find(" ")+1);
            tempPers.name = strName.substr(0, strName.find(" "));
            int marker1 = strName.find(" ");
            if (marker1 > 0) {
                tempPers.patronym = strName.substr(strName.find(" ")+1);
            }
            else
                tempPers.patronym = std::nullopt;
            //обработка блок телефон
            tempNumber.codeCountry = stoi(strPhone.substr(0, strPhone.find("(")));
            strPhone = strPhone.substr(strPhone.find("(") + 1);
            tempNumber.codeCity = stoi(strPhone.substr(0, strPhone.find(")")));
            strPhone = strPhone.substr(strPhone.find(")") + 1);
            int marker2 = strPhone.find(" ");
            if (marker2 > 0) {
            //if (str.find(" ") > 0) { // Вопрос к заданию №3
                tempNumber.number = strPhone.substr(0, strPhone.find(" "));
                tempNumber.numberAdd = stoi(strPhone.substr(strPhone.find(" ")+1));
            }
            else {
                tempNumber.number = strPhone;
                tempNumber.numberAdd = std::nullopt;
            }
            phonebook.push_back(std::make_pair(tempPers, tempNumber));
        }
    };
    void SortByName() {
        std::sort(phonebook.begin(), phonebook.end());
    }
    void SortByPhone() {
        std::sort(phonebook.begin(), phonebook.end(), [](std::pair<Person, PhoneNumber> _pb1, std::pair<Person, PhoneNumber> _pb2) {return _pb1.second < _pb2.second; });
    }
    std::pair<std::string, PhoneNumber>  GetPhoneNumber(std::string _name) {
        std::string resultStr;
        PhoneNumber resultPhN;
        std::vector<std::pair<Person, PhoneNumber>>::const_iterator it; // объявляем итератор только для чтения
        it = phonebook.begin(); // присваиваем ему начальный элемент вектора
        int count = 0; //Счетчик совпадений
        int pos; // Найденная позиция фамилии
        while (it != phonebook.end()) {
            if (it->first.surname == _name) {
                count++;
                pos = it - phonebook.begin();
            }
            it++;
        }
        if (count == 1) {
            resultStr = "";
            resultPhN = phonebook[pos].second;
        }
        else if (!count) // Не найден
            resultStr = "not found";
        else 
            resultStr = "found more than 1";
        return std::make_pair(resultStr, resultPhN);
    }
    void ChangePhoneNumber(Person _changePer, PhoneNumber _changePhN) {
        std::vector<std::pair<Person, PhoneNumber>>::const_iterator it; // объявляем итератор только для чтения
        it = phonebook.begin(); // присваиваем ему начальный элемент вектора
        while (it != phonebook.end()) {
            if (_changePer == it->first) {
                phonebook[it-phonebook.begin()].second = _changePhN;
            }
            it++;
        }
    }
    friend std::ostream& operator<<(std::ostream& os, const PhoneBook& _n);
private:
    std::vector<std::pair<Person, PhoneNumber>> phonebook;
};
std::ostream& operator<<(std::ostream& os, const PhoneBook& _n)
{
    std::vector<std::pair<Person, PhoneNumber>>::const_iterator it; // объявляем итератор только для чтения
    it = _n.phonebook.begin(); // присваиваем ему начальный элемент вектора
    while (it != _n.phonebook.end()) {
        os << it->first << "\t" << it++->second << "\n";
    }
    return os;
}


int main()
{
    using namespace std;
    string path = "PhoneBook.txt";
    ifstream file(path); // путь к файлу PhoneBook.txt
    PhoneBook book(file);
    cout << book;
    cout << "------SortByPhone-------" << endl;
    book.SortByPhone();
    cout << book;
    cout << "------SortByName--------" << endl;
    book.SortByName();
    cout << book;
    cout << "-----GetPhoneNumber-----" << endl;
    auto print_phone_number = [&book](const string& surname) {
        cout << surname << "\t";
        auto answer = book.GetPhoneNumber(surname);
        if (get<0>(answer).empty())
            cout << get<1>(answer);
        else
            cout << get<0>(answer);
        cout << endl;
    };
    // вызовы лямбды
    print_phone_number("Ivanov");
    print_phone_number("Zaitsev");
    print_phone_number("Solovev");
    cout << "----ChangePhoneNumber----" << endl;
    book.ChangePhoneNumber(Person{ "Kotov", "Vasilii", "Eliseevich" },
        PhoneNumber{ 7, 123, "15344458", nullopt });
    book.ChangePhoneNumber(Person{ "Mironova", "Margarita", "Vladimirovna" },
        PhoneNumber{ 16, 465, "9155448", 13 });
    cout << book;

}
