#include "DFSUtils.h"

#include "TableHelper.h"

bool DFSUtils::FindCycleRec(int v)
{
	used[v] = true;
	for (int u : adj[v])
	{
		parent[u] = v;
		if (!used[u]) {
			if (FindCycleRec(u))
				return true;
		}
		else
		{
			cycleStart = u;
			return true;
		}
	}
	used[v] = false;
	return false;
}

 std::vector<std::vector<int>> DFSUtils::FindAllPathsRec(int v)
{
	std::vector<std::vector<int>> curPaths;
	for (int u : adj[v])
		if (!used[u]){
			
			auto childPath = FindAllPathsRec(u);
			curPaths.insert(curPaths.end(), childPath.begin(), childPath.end());
		}
	for(auto&& path:curPaths)
		path.push_back(v);
	if(curPaths.size() == 0)
		curPaths.push_back({v});
	return curPaths;
}

void DFSUtils::Clear()
{
	for (auto [e, _] : this->adj)
	{
		used[e] = false;
		parent[e] = 0;
	}
}

DFSUtils::DFSUtils(std::map<int, std::vector<int>> adj) :adj(std::move(adj)) {}

std::vector<int> DFSUtils::FindCycle()
{
	Clear();
	bool isCycleFinded = false;
	for (auto [v, _] : adj)
		if (!used[v] && FindCycleRec(v)) {
			isCycleFinded = true;
			break;
		}

	if (!isCycleFinded)
		return std::vector<int>();

	std::vector<int> cycle;
	cycle.push_back(cycleStart);
	for (int v = parent[cycleStart]; v != cycleStart; v = parent[v])
		cycle.push_back(v);
	cycle.push_back(cycleStart);
	reverse(cycle.begin(), cycle.end());
	return cycle;
}

std::vector<std::vector<int>> DFSUtils::FindAllPaths(int startEvent)
{
	Clear();
	auto paths = FindAllPathsRec(startEvent);
	for(auto&& path: paths)
		reverse(path.begin(), path.end());
	return paths;
}
