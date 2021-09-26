﻿#pragma once
#include <map>
#include <ostream>
#include <set>
#include <vector>

struct Activity
{
	Activity();
	Activity(int startNode, int endNode, int time);
	int startNode, endNode, time;
};

std::ostream& operator<<(std::ostream& out, const Activity& activity);

using TableT = std::vector<Activity>;

class TableHelper
{
public:
	static std::set<int> GetEvents(const TableT& t);
	static void DeleteEvent(TableT& t, int eventIndex);
	static std::map<int, int> FindIncomeActivsCount(const TableT& t);
	static std::map<int, int> FindOutcomeActivsCount(const TableT& t);
	static std::vector<int> FindStartEvents(const TableT& t);
	static std::vector<int> FindEndEvents(const TableT& t);
};
