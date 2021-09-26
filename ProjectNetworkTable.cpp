#include <iomanip>
#include "ConsoleUI.h"
#include "ProjectNetworkTable.h"

#include <queue>

#include "ProjectNetworkTableSolver.h"

ProjectNetworkTable::ProjectNetworkTable(std::istream& in) : solver(*this)
{
	InputTable(in);
}

void ProjectNetworkTable::Analysis()
{
	AnalyzeSeveralStartPoints();
	AnalyzeSeveralEndPoints();
}

void ProjectNetworkTable::PartialSort()
{
	auto incomeCount = TableHelper::FindIncomeActivsCount(activs_);
	auto curIt = activs_.begin();
	while (curIt != activs_.end())
	{
		for (auto actIt = curIt + 1; actIt != activs_.end(); --actIt)
			if (incomeCount.at(actIt->startNode) == 0) {
				incomeCount.at(actIt->endNode)--;
				std::iter_swap(curIt, actIt);
				++curIt;
			}
	}
}

void ProjectNetworkTable::Print()
{
	std::cout << "Текущая таблица:\n";
	for (const auto& link : activs_)
		std::cout << std::setw(5) << link << std::endl;
}

void ProjectNetworkTable::AnalyzeSeveralStartPoints()
{
	auto startEvents = TableHelper::FindStartEvents(activs_);
	while (startEvents.size() > 1)
	{
		solver.SolveSeveralEvents(startEvents, true);
		startEvents = TableHelper::FindStartEvents(activs_);
	}
}

void ProjectNetworkTable::AnalyzeSeveralEndPoints()
{
	auto endEvents = TableHelper::FindEndEvents(activs_);
	while (endEvents.size() > 1)
	{
		solver.SolveSeveralEvents(endEvents, false);
		endEvents = TableHelper::FindEndEvents(activs_);
	}
}

void ProjectNetworkTable::DeleteEvent(int e)
{
	activs_.erase(std::remove_if(activs_.begin(), activs_.end(),
		[e](Activity& act) {return act.startNode == e || act.endNode == e; }), activs_.end());
}

void ProjectNetworkTable::CreateFakeStartEvent(int fakeE, const std::vector<int>& startEvents)
{
	for (auto nextE : startEvents)
		activs_.emplace_back(fakeE, nextE, 0);
}

void ProjectNetworkTable::CreateFakeEndEvent(int fakeE, const std::vector<int>& endEvents)
{
	for (auto prevE : endEvents)
		activs_.emplace_back(prevE, fakeE, 0);
}

void ProjectNetworkTable::InputTable(std::istream& in)
{
	size_t n;
	in >> n;
	activs_.reserve(n);
	while (n--)
	{
		int startEvent, endEvent, time;
		in >> startEvent >> endEvent >> time;
		if (in.fail())
			throw "Ошибка при чтении таблицы!\n";
		activs_.emplace_back(startEvent, endEvent, time);
	}
}
