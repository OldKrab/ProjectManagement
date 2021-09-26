#include "TableHelper.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <set>

#include "ProjectNetworkTable.h"

Activity::Activity() :startNode(0), endNode(0), time(0) {}
Activity::Activity(int startNode, int endNode, int time) : startNode(startNode), endNode(endNode), time(time) {}

bool Activity::operator==(const Activity& other)
{
	return startNode == other.startNode && endNode == other.endNode && time == other.time; 
}


std::ostream& operator<<(std::ostream& out, const Activity& activity)
{
	auto curWidth = std::cout.width();
	out << std::setw(curWidth) << activity.startNode << std::setw(curWidth) << activity.endNode << std::setw(curWidth) << activity.time;
	return out;
}

std::set<int> TableHelper::GetEvents(const TableT& t)
{
	auto events = std::set<int>();
	for (auto&& act : t)
	{
		events.insert(act.startNode);
		events.insert(act.endNode);
	}
	return events;
}

void TableHelper::DeleteEvent(TableT& t, int e)
{
	t.erase(std::remove_if(t.begin(), t.end(),
		[e](Activity& act) {return act.startNode == e || act.endNode == e; }), t.end());
}

std::map<int, int> TableHelper::FindIncomeActivsCount(const TableT& t)
{
	auto events = GetEvents(t);
	auto incomCount = std::map<int, int>();
	for(auto e :events)
		incomCount.insert(std::make_pair(e, 0));
	for (auto&& act : t)
		incomCount.at(act.endNode)++;
	return incomCount;
}

std::map<int, int> TableHelper::FindOutcomeActivsCount(const TableT& t)
{
	auto events = GetEvents(t);
	auto outcomCount = std::map<int, int>();
	for(auto e :events)
		outcomCount.insert(std::make_pair(e, 0));
	for (auto&& act : t)
		outcomCount.at(act.startNode)++;
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
	for(auto act: t)
		activs[std::make_pair(act.startNode, act.endNode)].push_back(act);
	std::vector<std::vector<Activity>> res;
	std::transform(activs.begin(), activs.end(), std::inserter(res, res.end()), 
		[](auto pair){return pair.second;});
	res.erase(std::remove_if(res.begin(), res.end(), [](auto vec){return vec.size() < 2;}), res.end());
	return res;
}

std::vector<Activity> TableHelper::FindActivsToItself(const TableT& t)
{
	std::vector<Activity> activs;
	std::copy_if(t.begin(), t.end(), std::back_inserter(activs), [](Activity act){return act.startNode == act.endNode;});
	return activs;
}

