#include <iostream>
#include <set>
#include <vector>
#include <cassert>
using std::pair;
using std::vector;
using std::set;

class suspendedGraph
{
public:
	suspendedGraph() = default;
	suspendedGraph(const int& vNumber)
	{
		eArray = vector<vector<pair<int, int>>>(vNumber);
	}
	suspendedGraph(const suspendedGraph& g)
	{
		eArray = g.eArray;
	}
	suspendedGraph& operator= (const suspendedGraph& g)
	{
		eArray = g.eArray;
		return *this;
	}
	void addEdge(const int& fVertex, const int& sVertex, const int& weight)
	{
		assert(fVertex < size() && sVertex < size() && fVertex >= 0 && sVertex >= 0);
		eArray[fVertex].push_back(pair<int, int>(sVertex, weight));
	}
	void addVertex()
	{
		eArray.push_back(vector<pair<int, int>>(0));
	}
	const vector<pair<int, int>>& next(const int& index) const
	{
		assert(index < size() && index >= 0);
		return eArray[index];
	}
	int size() const
	{
		return eArray.size();
	}
	void clear()
	{
		eArray.clear();
	}
private:
	suspendedGraph(vector<vector<pair<int, int>>> edges)
	{
		eArray = edges;
	}
	vector<vector<pair<int, int>>> eArray;
};

unsigned long long PrimsMST(const suspendedGraph& g, suspendedGraph& MST)
{
	MST.clear();
	MST = suspendedGraph(g.size());
	vector<bool> visited(g.size(), false);
	unsigned long long MSTweight = 0;
	set<pair<int, pair<int, int>>> closestVertexes;
	auto next = g.next(0);
	visited[0] = true;
	for (int i = next.size() - 1; i >= 0; i--)
	{
		if (!visited[next[i].first])
		{
			closestVertexes.insert(std::make_pair(next[i].second, std::make_pair(0, next[i].first)));
		}
	}
	while (!closestVertexes.empty())
	{
		auto closest = closestVertexes.begin();
		if (visited[closest->second.second])
		{
			closestVertexes.erase(closest);
			continue;
		}
		MSTweight += closest->first;
		visited[closest->second.second] = true;
		int thisVertex = closest->second.second;
		auto next = g.next(thisVertex);
		closestVertexes.erase(closest);
		for (int i = next.size() - 1; i >= 0; i--)
		{
			if (!visited[next[i].first])
			{
				closestVertexes.insert(pair<int, pair<int, int>>(next[i].second, pair<int, int>(thisVertex, next[i].first)));
			}
		}
	}
	return MSTweight;
}

int main()
{
	int n;
	int m;
	std::cin >> n >> m;
	suspendedGraph g(n);
	for (int i = 0; i < m; i++)
	{
		int b;
		int e;
		int w;
		std::cin >> b >> e >> w;
		g.addEdge(b - 1, e - 1, w);
		g.addEdge(e - 1, b - 1, w);
	}
	suspendedGraph MST;
	std::cout << PrimsMST(g, MST);
}
