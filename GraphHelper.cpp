#include "GraphHelper.h"


Event::Event(int key):key(key){}

void Event::AddNextEvent(Event* pe)
{
	nextActivities.push_back(pe);
	pe->incomingEventsCount_++;
}

void Event::DeleteNextEvent(Event* pe)
{
	auto it = std::find(nextActivities.begin(), nextActivities.end(), pe);
	if (it != nextActivities.end()) {
		nextActivities.erase(it);
		pe->incomingEventsCount_--;
	}
}

int Event::GetIncomingEventsCount() const
{
	return incomingEventsCount_;
}

void GraphHelper::DeleteEvent(GraphT& gr, int eventIndex)
{
	auto& delE = gr.at(eventIndex);
	DeleteNextEvents(gr, delE);
	if(delE.GetIncomingEventsCount() > 0)
		for(auto&& [key, e]:gr)
			e.DeleteNextEvent(&delE);
	gr.erase(eventIndex);
}

std::vector<int> GraphHelper::FindStartEvents(const GraphT& gr) 
{
	auto startEvents = std::vector<int>();
	for (auto&& [key, e] : gr)
		if (e.GetIncomingEventsCount() == 0)
			startEvents.push_back(key);
	return startEvents;
}

std::vector<int> GraphHelper::FindEndEvents(const GraphT& gr) 
{
	auto endEvents = std::vector<int>();
	for (auto&& [key, e] : gr)
		if (e.nextActivities.size() == 0)
			endEvents.push_back(key);
	return endEvents;
}

void GraphHelper::DeleteNextEvents(GraphT& gr, Event& e)
{
	for(auto nextE: e.nextActivities){
		nextE->incomingEventsCount_--;
		if(nextE->GetIncomingEventsCount() == 0 && nextE->nextActivities.size() == 0)
			DeleteEvent(gr, nextE->key);
	}
	e.nextActivities.clear();
}
