#pragma once
#include <vector>
#include "Objects/Activity.h"
#include "Objects/Table.h"

class TableSolver
{
public:
	TableSolver(Table& table);
	
	void SolveSeveralEvents(const std::vector<int>& events, bool isStartEvents);
	void SolveMultipleActivs(std::vector<Activity> multipleActivsInxs);
	void SolveActivsToItself(std::vector<Activity> activsToItself);
	void SolveCycle(const std::vector<Activity>& cycle);
private:
	void AskAndDeleteEvent(const std::vector<int>& events);
	void AskAndCreateFakeStartEvent(const std::vector<int>& startEvents);
	void AskAndCreateFakeEndEvent(const std::vector<int>& endEvents);
	
	Table& table_;
};
