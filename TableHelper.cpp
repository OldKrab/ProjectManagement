#include "TableHelper.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <set>

#include "CycleFinder.h"
#include "ProjectNetworkTable.h"

Activity::Activity() :startEvent(0), endEvent(0), time(0) {}
Activity::Activity(int startEvent, int endEvent, int time) : startEvent(startEvent), endEvent(endEvent), time(time) {}

bool Activity::operator==(const Activity& other)
{
	return startEvent == other.startEvent && endEvent == other.endEvent && time == other.time;
}


std::ostream& operator<<(std::ostream& out, const Activity& activity)
{
	auto curWidth = std::cout.width();
	out << std::setw(curWidth) << activity.startEvent << std::setw(curWidth) << activity.endEvent << std::setw(curWidth) << activity.time;
	return out;
}

std::set<int> TableHelper::GetEvents(const TableT& t)
{
	auto events = std::set<int>();
	for (auto&& act : t)
	{
		events.insert(act.startEvent);
		events.insert(act.endEvent);
	}
	return events;
}

void TableHelper::DeleteEvent(TableT& t, int e)
{
	t.erase(std::remove_if(t.begin(), t.end(),
		[e](Activity& act) {return act.startEvent == e || act.endEvent == e; }), t.end());
}

std::map<int, int> TableHelper::FindIncomeActivsCount(const TableT& t)
{
	auto events = GetEvents(t);
	auto incomCount = std::map<int, int>();
	for (auto e : events)
		incomCount.insert(std::make_pair(e, 0));
	for (auto&& act : t)
		incomCount.at(act.endEvent)++;
	return incomCount;
}

std::map<int, int> TableHelper::FindOutcomeActivsCount(const TableT& t)
{
	auto events = GetEvents(t);
	auto outcomCount = std::map<int, int>();
	for (auto e : events)
		outcomCount.insert(std::make_pair(e, 0));
	for (auto&& act : t)
		outcomCount.at(act.startEvent)++;
	return outcomCount;
}

std::vector<int> TableHelper::FindStartEvents(const TableT& t)
{
	auto incomCount = FindIncomeActivsCount(t);
	auto startEvents = std::vector<int>();
	for (auto&& [key, count] : incomCount)
		if (count == 0)
			startEvents.push_back(key);
	return startEvents;
}

std::vector<int> TableHelper::FindEndEvents(const TableT& t)
{
	auto outcomCount = FindOutcomeActivsCount(t);
	auto endEvents = std::vector<int>();
	for (auto&& [key, count] : outcomCount)
		if (count == 0)
			endEvents.push_back(key);
	return endEvents;
}

std::vector<std::vector<Activity>> TableHelper::FindMultipleActivs(const TableT& t)
{
	std::map<std::pair<int, int>, std::vector<Activity>> activs;
	for (auto act : t)
		activs[std::make_pair(act.startEvent, act.endEvent)].push_back(act);
	std::vector<std::vector<Activity>> res;
	std::transform(activs.begin(), activs.end(), std::inserter(res, res.end()),
		[](auto pair) {return pair.second; });
	res.erase(std::remove_if(res.begin(), res.end(), [](auto vec) {return vec.size() < 2; }), res.end());
	return res;
}

std::vector<Activity> TableHelper::FindActivsToItself(const TableT& t)
{
	std::vector<Activity> activs;
	std::copy_if(t.begin(), t.end(), std::back_inserter(activs), [](Activity act) {return act.startEvent == act.endEvent; });
	return activs;
}

std::vector<Activity> TableHelper::FindCycle(const TableT& t)
{
	auto adj = GetAdjacencyList(t);
	CycleFinder cycleFinder(adj);
	auto cycle = cycleFinder.FindCycle();
	std::vector<Activity> activs;
	for(size_t i = 1; i < cycle.size(); i++)
		activs.push_back(GetActivity(t, cycle[i-1],cycle[i]));
	return activs;
}

Activity TableHelper::GetActivity(const TableT& t, int startEvent, int endEvent)
{
	return *std::find_if(t.begin(), t.end(), 
		[startEvent, endEvent](Activity act){return act.startEvent == startEvent && act.endEvent == endEvent;});
}

std::map<int, std::vector<int>> TableHelper::GetAdjacencyList(const TableT& t)
{
	auto events = GetEvents(t);
	std::map<int, std::vector<int>> adj;
	for(auto e: events)
		adj.insert(std::make_pair(e, std::vector<int>()));
	for(auto&& act: t)
		adj[act.startEvent].push_back(act.endEvent);
	return adj;
}

