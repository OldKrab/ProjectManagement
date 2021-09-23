#include "GraphHelper.h"

#include "ProjectNetworkTable.h"


Event::Event(int key):key(key){}

void Event::AddNextActiv(Activity* pa)
{
	nextActivs.push_back(pa);
	pa->endNode->incomingEventsCount_++;
}

void Event::DeleteNextActiv(Activity* pa)
{
	auto it = std::find(nextActivs.begin(), nextActivs.end(), pa);
	if (it != nextActivs.end()) {
		nextActivs.erase(it);
		pa->endNode->incomingEventsCount_--;
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
			for(auto&& act: e.nextActivs)
				if(act->endNode == &delE)
					e.DeleteNextActiv(act);
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
		if (e.nextActivs.size() == 0)
			endEvents.push_back(key);
	return endEvents;
}

void GraphHelper::DeleteNextEvents(GraphT& gr, Event& e)
{
	for(auto nextAct: e.nextActivs){
		nextAct->endNode->incomingEventsCount_--;
		if(nextAct->endNode->GetIncomingEventsCount() == 0 && nextAct->endNode->nextActivs.size() == 0)
			gr.erase(nextAct->endNode->key);
	}
	e.nextActivs.clear();
}
