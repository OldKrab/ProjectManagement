#include "ConsoleUI.h"
#include "ProjectNetworkTableSolver.h"

#include <iomanip>

#include "ProjectNetworkTable.h"

ProjectNetworkTableSolver::ProjectNetworkTableSolver(ProjectNetworkTable& table) :table_(table) {}

void ProjectNetworkTableSolver::SolveSeveralEvents(const std::vector<int>& events, bool isStartEvents)
{
	table_.Print();
	if (isStartEvents)
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
	else if (isStartEvents)
		AskAndCreateFakeStartEvent(events);
	else
		AskAndCreateFakeEndEvent(events);

}

void ProjectNetworkTableSolver::SolveMultipleActivs(std::vector<Activity> multipleActivsInxs)
{
	table_.Print();
	ConsoleUI::Print("Обнаружены кратные работы:\n");
	auto activsCount = multipleActivsInxs.size();
	for (size_t i = 0; i < activsCount; i++)
		ConsoleUI::Print(i + 1, ") ", std::setw(7), multipleActivsInxs[i], '\n');

	auto inx = ConsoleUI::AskValue<size_t>("Введите порядковый номер работы, которая останется: ",
		[activsCount](size_t ans) {return 0 < ans && ans <= activsCount; }, "Неверный порядковый номер!");

	multipleActivsInxs[inx - 1] = multipleActivsInxs.back();
	multipleActivsInxs.pop_back();
	for (auto activ : multipleActivsInxs)
		table_.DeleteActivity(activ);
}

void ProjectNetworkTableSolver::SolveActivsToItself(std::vector<Activity> activsToItself)
{
	table_.Print();
	ConsoleUI::Print("Обнаружены работы, входящие в событие, из которой исходят:\n");
	auto activsCount = activsToItself.size();
	for (size_t i = 0; i < activsCount; i++)
		ConsoleUI::Print(i + 1, ") ", std::setw(7), activsToItself[i], '\n');

	ConsoleUI::Print("Данные работы удалены.\n");
	for (auto activ : activsToItself)
		table_.DeleteActivity(activ);
}

void ProjectNetworkTableSolver::SolveCycle(const std::vector<Activity>& cycle)
{
	table_.Print();
	ConsoleUI::Print("Обнаружен цикл:\n");
	auto activsCount = cycle.size();
	for (size_t i = 0; i < activsCount; i++)
		ConsoleUI::Print(i + 1, ") ", std::setw(7), cycle[i], '\n');

	auto inx = ConsoleUI::AskValue<size_t>("Введите порядковый номер работы, которая будет удалена: ",
		[activsCount](size_t ans) {return 0 < ans && ans <= activsCount; }, "Неверный порядковый номер!");

	table_.DeleteActivity(cycle[inx - 1]);
}

void ProjectNetworkTableSolver::AskAndDeleteEvent(const std::vector<int>& events)
{
	auto event = ConsoleUI::AskValue<int>("Введите номер события, который будет удален: ",
		[&events](int event) {return std::find(events.begin(), events.end(), event) != events.end(); },
		"Выберите один из удаляемых событий!");
	table_.DeleteEvent(event);
}

void ProjectNetworkTableSolver::AskAndCreateFakeStartEvent(const std::vector<int>& startEvents)
{
	auto&& allEvents = TableHelper::GetEvents(table_.activs_);
	auto event = ConsoleUI::AskValue<int>("Введите номер начального фиктивного события: ",
		[&allEvents](int event) {return allEvents.find(event) == allEvents.end(); },
		"Это событие уже существует!");
	table_.CreateFakeStartEvent(event, startEvents);
}

void ProjectNetworkTableSolver::AskAndCreateFakeEndEvent(const std::vector<int>& endEvents)
{
	auto&& allEvents = TableHelper::GetEvents(table_.activs_);
	auto event = ConsoleUI::AskValue<int>("Введите номер конечного фиктивного события: ",
		[&allEvents](int event) {return allEvents.find(event) == allEvents.end(); },
		"Это событие уже существует!");
	table_.CreateFakeEndEvent(event, endEvents);
}

