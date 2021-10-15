#pragma once
#include <ostream>

class Activity
{
public:
	Activity();
	Activity(int startEvent, int endEvent, int time);
	bool operator==(const Activity& other) const;
	

	int GetStartEvent() const;
	int GetEndEvent() const;
	int GetTime() const;
private:
	int startEvent, endEvent, time;
	
};

std::ostream& operator<<(std::ostream& out, const Activity& activity);
