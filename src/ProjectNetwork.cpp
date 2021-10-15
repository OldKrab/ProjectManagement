#include <fstream>
#include <iostream>
#include "TableHandler.h"

int main()
{
	setlocale(LC_ALL, "rus");
	std::ifstream fin("input.txt");
	try {
		Table table;
		table.InputTable(fin);
		TableHandler handler(table);
		handler.Analysis();
		table.PartialSort();
		table.Print("Таблица после частичной сортировки:");
		handler.PrintAllPaths();
	}
	catch (const char* err)
	{
		std::cout << err;
	}

	return 0;
}
