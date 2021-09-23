#pragma once
#include <vector>

class ProjectNetworkTable;

class ProjectNetworkTableSolver
{
public:
	ProjectNetworkTableSolver(ProjectNetworkTable& table);
	
	void SolveSeveralEvents(const std::vector<int>& events, bool isStartEvents);

private:
	void AskAndDeleteEvent(const std::vector<int>& events);
	void AskAndCreateFakeStartEvent(const std::vector<int>& events);
	void AskAndCreateFakeEndEvent(const std::vector<int>& events);
	
	ProjectNetworkTable& table_;
};
