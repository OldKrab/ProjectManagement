#pragma once
#include <istream>
#include <map>
#include <vector>

#include "GraphHelper.h"
#include "ProjectNetworkTableSolver.h"



struct Activity
{
	Activity();
	Activity(Event* startNode, Event* endNode, int time);
	Event* startNode, *endNode;
	int time;
};

std::ostream& operator<<(std::ostream& out, const Activity& activity);


class ProjectNetworkTable
{
public:
	using TableT = std::vector<Activity>;

	ProjectNetworkTable(std::istream& in);

	void Analysis();

	void PartialSort();

	void Print();

private:
	void AnalyzeSeveralStartPoints();
	void AnalyzeSeveralEndPoints();
	
	void AddActivity(int startEventKey, int endEventKey, int time);
	
	void DeleteEvent(int eIndex);
	
	void CreateFakeStartEvent(int fakeE, const std::vector<int>& startEvents);
	void CreateFakeEndEvent(int fakeE, const std::vector<int>& endEvents);

	void InputTable(std::istream& in);

	TableT table_;
	GraphT graph_;

	friend ProjectNetworkTableSolver;
	ProjectNetworkTableSolver solver;
};
