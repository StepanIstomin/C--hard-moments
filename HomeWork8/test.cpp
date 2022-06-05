#include "pch.h"
#include "HW_1.h"
#include <fstream>
//#include <optional> // дл€ использовани€ опциональных типов данных

TEST(PhoneBook, FileRead) // PhoneBook - тестируемый класс, FileRead - название теста
{
	std::ifstream file("PhoneBook.txt");
	ASSERT_NO_THROW(PhoneBook book(file));
	file.close();
}

TEST(PhoneBook, GetNumber) // ѕроверка метода GetPhoneNumber
{
	std::ifstream file("PhoneBook.txt");
	PhoneBook book(file);
	auto answer = book.GetPhoneNumber("Zaitsev");
	EXPECT_TRUE("+125(44)4164751");
	file.close();
}

TEST(PhoneBook, ChangePhoneNumber) // ѕроверка метода ChangePhoneNumber
{
	std::ifstream file("PhoneBook.txt");
	PhoneBook book(file);
	PhoneNumber PN{ 16, 465, "9155448", 13 };
	Person P{ "Mironova", "Margarita", "Vladimirovna" };
	ASSERT_NO_THROW(book.ChangePhoneNumber(P, PN));
	PhoneNumber PN2{ 7, 123, "15344458", std::nullopt };
	Person P2{ "Kotov", "Vasilii", "Eliseevich" };
	ASSERT_NO_THROW(book.ChangePhoneNumber(P, PN));
	file.close();
}
TEST(PhoneBook, SortByPhone) {
	std::ifstream file("PhoneBook.txt");
	PhoneBook book(file);
	ASSERT_NO_THROW(book.SortByName());
	file.close();
}
TEST(PhoneBook, SortByName) {
	std::ifstream file("PhoneBook.txt");
	PhoneBook book(file);
	ASSERT_NO_THROW(book.SortByPhone());
	file.close();
}


int main(int argc, char* argv[])
{
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}