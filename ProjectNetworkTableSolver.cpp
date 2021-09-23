#include "ConsoleUI.h"
#include "ProjectNetworkTableSolver.h"
#include "ProjectNetworkTable.h"

ProjectNetworkTableSolver::ProjectNetworkTableSolver(ProjectNetworkTable& table) :table_(table) {}

void ProjectNetworkTableSolver::SolveSeveralEvents(const std::vector<int>& events, bool isStartEvents)
{
	table_.Print();
	if(isStartEvents)
		ConsoleUI::Print("Обнаружено несколько начальных событий: ");
	else
		ConsoleUI::Print("Обнаружено несколько конечных событий: ");
	for (const auto& event : events)
		ConsoleUI::Print(event, ' ');
	ConsoleUI::Print("\n");
	auto ans = ConsoleUI::AskValue<char>("Удалить определенное событие [d] или создать фиктивное событие [c]?: ",
		[](char ans) {return ans == 'd' || ans == 'c'; }, "Вы должны ввести d или с!");
	if (ans == 'd')
		AskAndDeleteEvent(events);
	else if(isStartEvents)
		AskAndCreateFakeStartEvent(events);
	else
		AskAndCreateFakeEndEvent(events);

}

void ProjectNetworkTableSolver::AskAndDeleteEvent(const std::vector<int>& events)
{
	auto event = ConsoleUI::AskValue<int>("Введите номер события, который будет удален: ",
		[&events](int event) {return std::find(events.begin(), events.end(), event) != events.end(); },
		"Это событие не является стартовым!");
	table_.DeleteEvent(event);
}

void ProjectNetworkTableSolver::AskAndCreateFakeStartEvent(const std::vector<int>& events)
{
	auto&& gr = table_.graph_;
	auto event = ConsoleUI::AskValue<int>("Введите номер начального фиктивного события: ",
		[&gr](int event) {return gr.find(event) == gr.end(); },
		"Это событие уже существует!");
	table_.CreateFakeStartEvent(event, events);
}

void ProjectNetworkTableSolver::AskAndCreateFakeEndEvent(const std::vector<int>& events)
{
	auto&& gr = table_.graph_;
	auto event = ConsoleUI::AskValue<int>("Введите номер конечного фиктивного события: ",
		[&gr](int event) {return gr.find(event) == gr.end(); },
		"Это событие уже существует!");
	table_.CreateFakeEndEvent(event, events);
}

