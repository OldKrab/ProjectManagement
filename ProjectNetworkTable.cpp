#include <iomanip>
#include "ConsoleUI.h"
#include "ProjectNetworkTable.h"

#include <queue>

#include "ProjectNetworkTableSolver.h"


Activity::Activity() :startNode(0), endNode(0), time(0) {}
Activity::Activity(Event* startNode, Event* endNode, int time) : startNode(startNode), endNode(endNode), time(time) {}

std::ostream& operator<<(std::ostream& out, const Activity& activity)
{
	auto curWidth = std::cout.width();
	out << std::setw(curWidth) << activity.startNode->key << std::setw(curWidth) << activity.endNode->key << std::setw(curWidth) << activity.time;
	return out;
}

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
	TableT newTable;
	GraphT grCopy = graph_;
	std::queue<int> que;
	auto startEvents = GraphHelper::FindStartEvents(grCopy);
	for(auto&& e:startEvents)
		que.push(e);
	while(!que.empty())
	{
		auto curE = que.back();
		que.pop();
		auto NextActivs = grCopy.at(curE).nextActivs;
		for(auto&& nextAct: NextActivs)
		if(nextAct->endNode->GetIncomingEventsCount() == 0){
			//newTable.emplace_back(curE, nextE->key, )
		}
	}
}

void ProjectNetworkTable::Print()
{
	std::cout << "Текущая таблица:\n";
	for (const auto& link : table_)
		std::cout << std::setw(5) << link << std::endl;
}

void ProjectNetworkTable::AnalyzeSeveralStartPoints()
{
	auto startEvents =  GraphHelper::FindStartEvents(graph_);
	while (startEvents.size() > 1)
	{
		solver.SolveSeveralEvents(startEvents, true);
		startEvents =  GraphHelper::FindStartEvents(graph_);
	}
}

void ProjectNetworkTable::AnalyzeSeveralEndPoints()
{
	auto endEvents =  GraphHelper::FindEndEvents(graph_);
	while (endEvents.size() > 1)
	{
		solver.SolveSeveralEvents(endEvents, false);
		endEvents = GraphHelper::FindEndEvents(graph_);
	}
}

void ProjectNetworkTable::AddActivity(int startEventKey, int endEventKey, int time)
{
	if (graph_.find(startEventKey) == graph_.end())
		graph_.emplace(std::make_pair(startEventKey, Event(startEventKey)));
	if (graph_.find(endEventKey) == graph_.end())
		graph_.emplace(std::make_pair(endEventKey, Event(endEventKey)));
	auto&& startE = graph_.at(startEventKey), &&endE = graph_.at(endEventKey);
	table_.emplace_back(&startE, &endE, time);
	startE.AddNextActiv(&table_.back());
}

void ProjectNetworkTable::DeleteEvent(int eIndex)
{
	GraphHelper::DeleteEvent(graph_, eIndex);
	table_.erase(std::remove_if(table_.begin(), table_.end(),
		[eIndex](const Activity& act) {return act.startNode->key == eIndex || act.endNode->key == eIndex; }), table_.end());
}

void ProjectNetworkTable::CreateFakeStartEvent(int fakeE, const std::vector<int>& startEvents)
{
	for (auto nextE : startEvents)
		AddActivity(fakeE,nextE,0);
}

void ProjectNetworkTable::CreateFakeEndEvent(int fakeE, const std::vector<int>& endEvents)
{
	for (auto prevE : endEvents)
		AddActivity(prevE, fakeE, 0);
}

void ProjectNetworkTable::InputTable(std::istream& in)
{
	size_t n;
	in >> n;
	table_.reserve(n);
	while (n--)
	{
		int startKey, endKey, time;
		in >> startKey >> endKey >> time;
		if (in.fail())
			throw "Ошибка при чтении таблицы!\n";
		AddActivity(startKey,endKey,time);
	}
}
