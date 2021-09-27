#pragma once
#include <map>
#include <ostream>
#include <set>
#include <vector>

struct Activity
{
	Activity();
	Activity(int startEvent, int endEvent, int time);
	bool operator==(const Activity& other);
	int startEvent, endEvent, time;
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
	static std::vector<std::vector<Activity>> FindMultipleActivs(const TableT& t);
	static std::vector<Activity> FindActivsToItself(const TableT& t);
	static std::vector<Activity> FindCycle(const TableT& t);
	static Activity GetActivity(const TableT& t, int startEvent, int endEvent);
private:
	static std::map<int, std::vector<int>> GetAdjacencyList(const TableT& t);
};
