#pragma once
#include <map>
#include <vector>

struct Event
{
	explicit Event(int key);
	void AddNextEvent(Event* pe);
	void DeleteNextEvent(Event* pe);
	int GetIncomingEventsCount() const;


	std::vector<Event*> nextEvents;
	const int key;
private:
	int incomingEventsCount_ = 0;
	friend class GraphHelper;
};

using GraphT = std::map<int, Event>;

class GraphHelper
{
public:
	static void DeleteEvent(GraphT& gr, int eventIndex);
	static std::vector<int> FindStartEvents(const GraphT& gr);
	static std::vector<int> FindEndEvents(const GraphT& gr);
private:
	static void DeleteNextEvents(GraphT& gr, Event& e);
};
