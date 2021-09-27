#include <fstream>
#include <iostream>
#include "ProjectNetworkTable.h"

int main()
{
	setlocale(LC_ALL, "rus");
	std::ifstream fin("input.txt");
	try {
		ProjectNetworkTable table(fin);
		table.Analysis();
		table.PartialSort();
		table.Print("Таблица после частичной сортировки:");
		table.PrintAllPaths();
	}
	catch (const char* err)
	{
		std::cout << err;
	}

	return 0;
}
