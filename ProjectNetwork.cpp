#include <fstream>
#include "ProjectNetworkTable.h"

int main()
{
	setlocale(LC_ALL, "rus");
	std::ifstream fin("input.txt");
	ProjectNetworkTable table(fin);
	table.Analysis();
	table.Print();
	return 0;
}
