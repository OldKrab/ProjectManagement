#include "Activity.h"

#include <iomanip>

std::ostream& operator<<(std::ostream& out, const Activity& activity)
{
	auto curWidth = out.width();
	out << std::setw(curWidth) << activity.GetStartEvent() << std::setw(curWidth) << activity.GetEndEvent() << std::setw(curWidth) << activity.GetTime();
	return out;
}

Activity::Activity() :startEvent(0), endEvent(0), time(0)
{}

Activity::Activity(int startEvent, int endEvent, int time) : startEvent(startEvent), endEvent(endEvent), time(time)
{}

bool Activity::operator==(const Activity& other) const
{
	return startEvent == other.startEvent && endEvent == other.endEvent && time == other.time;
}

int Activity::GetStartEvent() const
{
	return startEvent;
}

int Activity::GetEndEvent() const
{
	return endEvent;
}

int Activity::GetTime() const
{
	return time;
}
