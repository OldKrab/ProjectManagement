#include "DFSUtils.h"

bool DFSUtils::DFS(int v)
{
	color[v] = Color::Gray;
	for (int u : adj[v])
	{
		if (color[u] == Color::White)
		{
			parent[u] = v;
			if (DFS(u))
				return true;
		}
		else if (color[u] == Color::Gray)
		{
			cycleEnd = v;
			cycleStart = u;
			return true;
		}
	}
	color[v] = Color::Black;
	return false;
}

DFSUtils::DFSUtils(std::map<int, std::vector<int>> adj) :adj(std::move(adj)), isCycleFinded(false)
{
	for (auto [e, _] : this->adj)
	{
		color[e] = Color::White;
		parent[e] = 0;
	}
}

std::vector<int> DFSUtils::FindCycle()
{
	for (auto [v, _]: adj)
	{
		if (color[v] == Color::White && DFS(v)) {
			isCycleFinded = true;
			break;
		}
	}

	if (!isCycleFinded)
		return std::vector<int>();

	std::vector<int> cycle;
	cycle.push_back(cycleStart);
	for (int v = cycleEnd; v != cycleStart; v = parent[v])
		cycle.push_back(v);
	cycle.push_back(cycleStart);
	reverse(cycle.begin(), cycle.end());
	return cycle;
}
