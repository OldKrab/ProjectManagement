#include <algorithm>
#include <cassert>
#include <iomanip>
#include "Table.h"

#include <iostream>

#include "DFSUtils.h"

void Table::DeleteEvent(int e)
{
	t_.erase(std::remove_if(t_.begin(), t_.end(),
		[e](Activity& act) {return act.GetStartEvent() == e || act.GetEndEvent() == e; }), t_.end());
}

void Table::DeleteActivity(Activity activ)
{
	auto it = std::find(t_.begin(), t_.end(), activ);
	if (it != t_.end())
		t_.erase(it);
}

void Table::CreateFakeStartEvent(int fakeE, const std::vector<int>& startEvents)
{
	for (auto nextE : startEvents)
		t_.emplace_back(fakeE, nextE, 0);
}

void Table::CreateFakeEndEvent(int fakeE, const std::vector<int>& endEvents)
{
	for (auto prevE : endEvents)
		t_.emplace_back(prevE, fakeE, 0);
}

void Table::InputTable(std::istream& in)
{
	size_t n = 1;
	in >> n;
	if (n < 1)
		throw "Таблица пустая!\n";
	t_.reserve(n);
	while (n--)
	{
		int startEvent, endEvent, time;
		in >> startEvent >> endEvent >> time;
		if(time < 0)
			throw "Время у работы меньше нуля!\n";
		t_.emplace_back(startEvent, endEvent, time);
	}
	if (in.fail())
		throw "Ошибка при чтении таблицы!\n";
}

void Table::Print(const std::string& title)
{
	std::streamsize width = 7; 
	std::cout << title << std::endl;
	std::cout << std::setw(width) << "A" << std::setw(width) << "B" << std::setw(width) << "t_AB" << std::endl;
	for (const auto& act : t_)
		std::cout << std::setw(width) << act << std::endl;
}

void Table::PartialSort()	
{
	auto incomeCount = FindIncomeActivsCount();
	auto rightSwapIt = t_.begin();
	auto leftSwapIt = rightSwapIt;
	while (rightSwapIt != t_.end())
	{
		// Find zero incom start event
		while (true) {
			auto zeroIt = rightSwapIt;
			while (zeroIt != t_.end() && incomeCount.at(zeroIt->GetStartEvent()) != 0)
				++zeroIt;
			if (zeroIt == t_.end())
				break;
			auto zeroStart = zeroIt->GetStartEvent();
			// swap every activ with this start
			for (auto actIt = zeroIt; actIt != t_.end(); ++actIt)
				if (actIt->GetStartEvent() == zeroStart) {
					std::iter_swap(rightSwapIt, actIt);
					++rightSwapIt;
				}
		}
		// decrease incom
		while (leftSwapIt != rightSwapIt)
		{
			incomeCount.at(leftSwapIt->GetEndEvent())--;
			leftSwapIt++;
		}
	}
}


std::set<int> Table::GetEvents() const
{
	auto events = std::set<int>();
	for (auto&& act : t_)
	{
		events.insert(act.GetStartEvent());
		events.insert(act.GetEndEvent());
	}
	return events;
}


std::map<int, int> Table::FindIncomeActivsCount() const
{
	auto events = GetEvents();
	auto incomCount = std::map<int, int>();
	for (auto e : events)
		incomCount.insert(std::make_pair(e, 0));
	for (auto&& act : t_)
		incomCount.at(act.GetEndEvent())++;
	return incomCount;
}

std::map<int, int> Table::FindOutcomeActivsCount() const
{
	auto events = GetEvents();
	auto outcomCount = std::map<int, int>();
	for (auto e : events)
		outcomCount.insert(std::make_pair(e, 0));
	for (auto&& act : t_)
		outcomCount.at(act.GetStartEvent())++;
	return outcomCount;
}

std::vector<int> Table::FindStartEvents()const
{ 
	auto incomCount = FindIncomeActivsCount();
	auto startEvents = std::vector<int>();
	for (auto&& [key, count] : incomCount)
		if (count == 0)
			startEvents.push_back(key);
	return startEvents;
}

std::vector<int> Table::FindEndEvents() const
{
	auto outcomCount = FindOutcomeActivsCount();
	auto endEvents = std::vector<int>();
	for (auto&& [key, count] : outcomCount)
		if (count == 0)
			endEvents.push_back(key);
	return endEvents;
}

std::vector<std::vector<Activity>> Table::FindMultipleActivs( ) const
{
	std::map<std::pair<int, int>, std::vector<Activity>> activs;
	for (auto act : t_)
		activs[std::make_pair(act.GetStartEvent(), act.GetEndEvent())].push_back(act);
	std::vector<std::vector<Activity>> res;
	std::transform(activs.begin(), activs.end(), std::inserter(res, res.end()),
		[](auto pair) {return pair.second; });
	res.erase(std::remove_if(res.begin(), res.end(), [](auto vec) {return vec.size() < 2; }), res.end());
	return res;
}

std::vector<Activity> Table::FindActivsToItself( ) const
{
	std::vector<Activity> activs;
	std::copy_if(t_.begin(), t_.end(), std::back_inserter(activs), [](Activity act) {return act.GetStartEvent() == act.GetEndEvent(); });
	return activs;
}

std::vector<Activity> Table::FindCycle() const
{
	auto adj = CalcAdjacencyList();
	DFSUtils cycleFinder(adj);
	auto cycle = cycleFinder.FindCycle();
	std::vector<Activity> activs;
	for(size_t i = 1; i < cycle.size(); i++)
		activs.push_back(GetActivity(cycle[i-1],cycle[i]));
	return activs;
}
 
std::vector<std::vector<int>> Table::FindAllPaths() const
{
	auto adj = CalcAdjacencyList();
	DFSUtils pathsFinder(adj);
	auto startEvents = FindStartEvents();
	assert(startEvents.size() == 1);
	return pathsFinder.FindAllPaths(startEvents[0]);
}

Activity Table::GetActivity( int startEvent, int endEvent) const
{
	return *std::find_if(t_.begin(), t_.end(), 
		[startEvent, endEvent](Activity act){return act.GetStartEvent() == startEvent && act.GetEndEvent() == endEvent;});
}

std::map<int, std::vector<int>> Table::CalcAdjacencyList() const
{
	auto events = GetEvents();
	std::map<int, std::vector<int>> adj;
	for(auto e: events)
		adj.insert(std::make_pair(e, std::vector<int>()));
	for(auto&& act: t_)
		adj[act.GetStartEvent()].push_back(act.GetEndEvent());
	return adj;
}
