#pragma once
#include <map>
#include <vector>

class DFSUtils
{
public:
	DFSUtils(std::map<int, std::vector<int>> adj);
	std::vector<int> FindCycle();
private:
	bool DFS(int v);
	
	enum class Color
	{
		White, Gray, Black
	};
	
	int n;
	std::map<int, std::vector<int>> adj;
	std::map<int, Color> color;
	std::map<int, int> parent;
	int cycleStart, cycleEnd;
	bool isCycleFinded;
};
