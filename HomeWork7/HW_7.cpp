// C++ Сложные моменты. ДЗ №7.
// Author: Stepan Istomin

#include <iostream>
#include <vector>
#include <string>
#include <optional> // для использования опциональных типов данных
#include <numeric> //for accumulate
#include <fstream>

namespace HomeWork7 {

	class FullName
	{
	public:
		FullName() {};
		void setName(std::string _name) { name = _name; }
		void setSurname(std::string _surname) { surname = _surname; }
		void setPatronym(std::string _patronym) { patronym = _patronym; }
		friend class StudentsGroup;
	private:
		std::string name;
		std::string surname;
		std::optional <std::string> patronym;
	};

	class Student
	{
	public:
		Student() {};
		Student(std::string _name, std::string _surname, std::string _patronym) {
			fullname.setName(_name);
			fullname.setSurname(_surname);
			fullname.setPatronym(_patronym);
		};
		Student(std::string _name, std::string _surname) {
			fullname.setName(_name);
			fullname.setSurname(_surname);
		};
		void addGrade(int _grade) {
			grades.push_back(_grade);
			avGrade = std::accumulate(grades.begin(), grades.end(), 0.0) / grades.size();
		}
		friend class StudentsGroup;
	private:
		FullName fullname;
		std::vector<int> grades;
		double avGrade = 0;
	};

	class StudentsGroup
	{
	public:
		StudentsGroup() {};
		void addStudent(const Student& _name) { students.push_back(_name); };
		void Save(std::string _filename) {
			std::ofstream WF(_filename, std::ios::binary);
			for (auto it : students)
			{
				char* A = (char*)&it.fullname;
				WF.write(A, sizeof(FullName));
				char* B = (char*)&it.avGrade;
				WF.write(B, sizeof(double));
				//char* C = (char*)&it.grades; // Так не работает
				//WF.write(C, sizeof(std::vector<int>));
			}
			WF.close();
		};
		void Open(std::string _filename) {
			std::ifstream RF(_filename, std::ios::binary);
			while (RF) {
				if (RF.peek() != -1) {
					Student* tempPtrS = new Student;
					RF.read(reinterpret_cast<char*>(&tempPtrS->fullname), sizeof(FullName));
					RF.read(reinterpret_cast<char*>(&tempPtrS->avGrade), sizeof(double));
					//RF.read(reinterpret_cast<char*>(&tempPtrS->grades), sizeof(std::vector<int>)); // Так не работает
					students.push_back(*tempPtrS);
				}
			}
			RF.close();
		};
		double GetAverageScore(const Student& _name) { return _name.avGrade; };
		std::string GetAllInfo(const Student& _name) {
			std::string info = "";
			info = _name.fullname.name + " " + _name.fullname.surname;
			if (_name.fullname.patronym) {
				info = info + " " + _name.fullname.patronym.value();
			}
			info = info + ", Average grade: " + std::to_string(_name.avGrade) + "\n";
			return info;
		};
		std::string GetAllInfo() {
			std::string info = "";
			for (auto it : students)
			{
				info = info + it.fullname.name + " " + it.fullname.surname;
				if (it.fullname.patronym) {
					info = info + " " + it.fullname.patronym.value();
				}
				info = info + ", Average grade: " + std::to_string(it.avGrade) + "\n";
			}
			return info;
		};
	private:
		std::vector<Student> students;
	};
}

int main()
{
	using namespace std;
	using namespace HomeWork7;
	string filename = "save.bin";

	Student stud1("Ivan", "Petrovich", "Sidorov");
	for (size_t i = 0; i < 10; i++)
		stud1.addGrade(rand() % 5 + 1);
	Student stud2("John", "Smith");
	for (size_t i = 0; i < 10; i++)
		stud2.addGrade(rand() % 5 + 1);
	StudentsGroup group;
	group.addStudent(stud1);
	group.addStudent(stud2);
	cout << group.GetAllInfo();
	group.Save(filename);
	
	StudentsGroup* groupCopy = new StudentsGroup;
	
	//filename = "save.bin"; // можно прочитать из любого файла

	groupCopy->Open(filename);
	cout << endl << "reading" << endl << groupCopy->GetAllInfo();
}