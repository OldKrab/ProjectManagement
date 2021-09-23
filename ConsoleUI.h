#pragma once
#include <string>
#include <iostream>
#include <functional>

class ConsoleUI
{
public:
	template <class T>
	static T AskValue(std::string question);
	template <class T>
	static T AskValue(std::string question, std::function<bool(T)> isValid, std::string wrongMessage);

	template<class T, class... Args>
	static void Print(T value, Args... tail);

private:
	static void Print();
};

template <class T>
T ConsoleUI::AskValue(std::string question, std::function<bool(T)> isValid,  std::string wrongMessage)
{
	T value;
	while (true)
	{
		std::cout << question;
		std::cin >> value;
		if(std::cin.fail()){
			std::cout << "Введено значение, не соответствующее типу!\n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else if(isValid(value))
			return value;
		else
			std::cout << wrongMessage << std::endl;
	}
}

template <class T>
T ConsoleUI::AskValue(std::string question)
{
	return AskValue<T>(question,[](T){return true;},  "Неверное значение.");
}

template <class T, class ... Args>
void ConsoleUI::Print(T value, Args... tail)
{
	std::cout << value;
	Print(tail...);
}



