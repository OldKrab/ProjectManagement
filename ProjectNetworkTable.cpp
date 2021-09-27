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
	AnalyzeMultipleActivs();
	AnalyzeActivToItself();
	AnalyzeCycle();
	AnalyzeSeveralEndPoints();
}

void ProjectNetworkTable::PartialSort()
{
	auto incomeCount = TableHelper::FindIncomeActivsCount(activs_);
	auto rightSwapIt = activs_.begin();
	auto leftSwapIt = rightSwapIt;
	while (rightSwapIt != activs_.end())
	{
		// Find zero incom start event
		while (true) {
			auto zeroIt = rightSwapIt;
			while (zeroIt != activs_.end() && incomeCount.at(zeroIt->startEvent) != 0)
				++zeroIt;
			if (zeroIt == activs_.end())
				break;
			auto zeroStart = zeroIt->startEvent;
			// swap every activ with this start
			for (auto actIt = zeroIt; actIt != activs_.end(); ++actIt)
				if (actIt->startEvent == zeroStart) {
					std::iter_swap(rightSwapIt, actIt);
					++rightSwapIt;
				}
		}
		// decrease incom
		while (leftSwapIt != rightSwapIt)
		{
			incomeCount.at(leftSwapIt->endEvent)--;
			leftSwapIt++;
		}
	}
}

void ProjectNetworkTable::Print(const std::string& title)
{
	std::cout << title << std::endl;
	for (const auto& act : activs_)
		std::cout << std::setw(5) << act << std::endl;
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

void ProjectNetworkTable::AnalyzeMultipleActivs()
{
	auto multipleActivsVector = TableHelper::FindMultipleActivs(activs_);
	for (auto multipleActivs : multipleActivsVector)
		solver.SolveMultipleActivs(multipleActivs);
}

void ProjectNetworkTable::AnalyzeActivToItself()
{
	auto activsToItself = TableHelper::FindActivsToItself(activs_);
	if (activsToItself.size() > 0)
		solver.SolveActivsToItself(activsToItself);

}

void ProjectNetworkTable::AnalyzeCycle()
{
	auto cycle = TableHelper::FindCycle(activs_);
	while (cycle.size() > 0)
	{
		solver.SolveCycle(cycle);
		cycle = TableHelper::FindCycle(activs_);
	}
}

void ProjectNetworkTable::DeleteEvent(int e)
{
	activs_.erase(std::remove_if(activs_.begin(), activs_.end(),
		[e](Activity& act) {return act.startEvent == e || act.endEvent == e; }), activs_.end());
}

void ProjectNetworkTable::DeleteActivity(Activity activ)
{
	auto it = std::find(activs_.begin(), activs_.end(), activ);
	if (it != activs_.end())
		activs_.erase(it);
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
