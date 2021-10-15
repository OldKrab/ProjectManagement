#pragma once
#include <map>
#include <set>
#include <vector>
#include "Activity.h"


class Table
{
public:
	void DeleteEvent(int eventIndex);
	void DeleteActivity(Activity activ);

	void CreateFakeStartEvent(int fakeE, const std::vector<int>& startEvents);
	void CreateFakeEndEvent(int fakeE, const std::vector<int>& endEvents);

	void PartialSort();

	std::set<int> GetEvents() const;
	Activity GetActivity(int startEvent, int endEvent) const;
	std::map<int, std::vector<int>> CalcAdjacencyList() const;

	std::map<int, int> FindIncomeActivsCount() const;
	std::map<int, int> FindOutcomeActivsCount() const;
	std::vector<Activity> FindActivsToItself() const;
	std::vector<std::vector<Activity>> FindMultipleActivs() const;

	std::vector<int> FindStartEvents() const;
	std::vector<int> FindEndEvents() const;

	std::vector<Activity> FindCycle() const;
	std::vector<std::vector<int>> FindAllPaths() const;


	void InputTable(std::istream& in);
	void Print(const std::string& title = "Текущая таблица:");
private:



	std::vector<Activity> t_;
};
