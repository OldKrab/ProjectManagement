#include <iomanip>
#include "ConsoleUI.h"
#include "TableHandler.h"
#include <utility>

#include "TableSolver.h"


TableHandler::TableHandler(Table& table):table_(table), solver(table_){}

void TableHandler::Analysis()
{
	AnalyzeMultipleActivs();
	AnalyzeActivToItself();
	AnalyzeCycle();
	AnalyzeSeveralStartPoints();
	AnalyzeSeveralEndPoints();
}

void TableHandler::PrintAllPaths()
{
	ConsoleUI::Print("Все пути:\n");
	auto paths = table_.FindAllPaths();
	for (auto&& path : paths) {
		for (auto&& e : path)
			ConsoleUI::Print(std::setw(7), e);
		ConsoleUI::Print('\n');
	}
}

void TableHandler::AnalyzeSeveralStartPoints()
{
	auto startEvents = table_.FindStartEvents();
	while (startEvents.size() > 1)
	{
		solver.SolveSeveralEvents(startEvents, true);
		startEvents = table_.FindStartEvents();
	}
}

void TableHandler::AnalyzeSeveralEndPoints()
{
	auto endEvents = table_.FindEndEvents();
	while (endEvents.size() > 1)
	{
		solver.SolveSeveralEvents(endEvents, false);
		endEvents = table_.FindEndEvents();
	}
}

void TableHandler::AnalyzeMultipleActivs()
{
	auto multipleActivsVector = table_.FindMultipleActivs();
	for (auto multipleActivs : multipleActivsVector)
		solver.SolveMultipleActivs(multipleActivs);
}

void TableHandler::AnalyzeActivToItself()
{
	auto activsToItself = table_.FindActivsToItself();
	if (activsToItself.size() > 0)
		solver.SolveActivsToItself(activsToItself);

}

void TableHandler::AnalyzeCycle()
{
	auto cycle = table_.FindCycle();
	while (cycle.size() > 0)
	{
		solver.SolveCycle(cycle);
		cycle = table_.FindCycle();
	}
}




