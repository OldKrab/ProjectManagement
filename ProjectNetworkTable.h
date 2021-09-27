#pragma once
#include <vector>
#include "TableHelper.h"
#include "ProjectNetworkTableSolver.h"

std::ostream& operator<<(std::ostream& out, const Activity& activity);


class ProjectNetworkTable
{
public:

	ProjectNetworkTable(std::istream& in);

	void Analysis();

	void PartialSort();

	void Print(const std::string &title = "Текущая таблица:");

private:
	void AnalyzeSeveralStartPoints();
	void AnalyzeSeveralEndPoints();
	void AnalyzeMultipleActivs();
	void AnalyzeActivToItself();
	void AnalyzeCycle();

	void DeleteEvent(int e);
	void DeleteActivity(Activity activ);

	void CreateFakeStartEvent(int fakeE, const std::vector<int>& startEvents);
	void CreateFakeEndEvent(int fakeE, const std::vector<int>& endEvents);

	void InputTable(std::istream& in);

	TableT activs_;

	friend ProjectNetworkTableSolver;
	ProjectNetworkTableSolver solver;
};
