#pragma once
#include <vector>
#include "Objects/Table.h"
#include "TableSolver.h"

std::ostream& operator<<(std::ostream& out, const Activity& activity);


class TableHandler
{
public:

	TableHandler(Table& table);

	void Analysis();

	void PrintAllPaths();

private:
	void AnalyzeSeveralStartPoints();
	void AnalyzeSeveralEndPoints();
	void AnalyzeMultipleActivs();
	void AnalyzeActivToItself();
	void AnalyzeCycle();

	Table& table_;

	friend TableSolver;
	TableSolver solver;
};
