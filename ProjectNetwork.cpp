#include <fstream>
#include "ProjectNetworkTable.h"

int main()
{
	setlocale(LC_ALL, "rus");
	std::ifstream fin("input.txt");
	ProjectNetworkTable table(fin);
	table.Analysis();
	table.PartialSort();
	table.Print("Таблица после частичной сортировки:");
	return 0;
}
