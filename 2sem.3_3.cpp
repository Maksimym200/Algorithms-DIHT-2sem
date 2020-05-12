#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <cassert>
using std::pair;
using std::vector;
using std::map;

template <class T>
class DSU
{
public:
	bool find(const T& x, T& y)
	{
		if (index.find(x) == index.end())
		{
			return false;
		}
		y = object[indexFind(index[x])];
		return true;
	}
	void merge(const T& x, const T& y)
	{
		indexMerge(index[x], index[y]);
	}
	void makeSet(const T& x)
	{
		index.insert(std::make_pair(x, object.size()));
		object.push_back(x);
		rank.push_back(0);
		parent.push_back(parent.size());
	}
private:
	int indexFind(int index)
	{
		int startIndex = index;
		while (index != parent[index])
		{
			index = parent[index];
		}
		while (startIndex != index)
		{
			int par = parent[startIndex];
			parent[startIndex] = index;
			startIndex = par;
		}
		return index;
	}
	void indexMerge(int f, int s)
	{
		int fRoot = indexFind(f);
		int sRoot = indexFind(s);
		if (rank[fRoot] > rank[sRoot])
			parent[sRoot] = fRoot;
		else
		{
			if (rank[fRoot] == rank[sRoot])
			{
				++rank[sRoot];
			}
			parent[fRoot] = sRoot;
		}
	}
	vector<T> object;
	map<T, int> index;
	vector<int> parent;
	vector<int> rank;
};

class suspendedGraph
{
public:
	suspendedGraph() = default;
	suspendedGraph(const int& vNumber)
	{
		eArray = vector<vector<pair<int, unsigned long long>>>(vNumber);
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
	void addEdge(const int& fVertex, const int& sVertex, const unsigned long long& weight)
	{
		assert(fVertex < size() && sVertex < size() && fVertex >= 0 && sVertex >= 0);
		eArray[fVertex].push_back(pair<int, unsigned long long>(sVertex, weight));
	}
	void addVertex()
	{
		eArray.push_back(vector<pair<int, unsigned long long>>(0));
	}
	const vector<pair<int, unsigned long long>>& next(const int& index) const
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
	suspendedGraph(vector<vector<pair<int, unsigned long long>>> edges)
	{
		eArray = edges;
	}
	vector<vector<pair<int, unsigned long long>>> eArray;
};

unsigned long long KruskalMST(const suspendedGraph& g, suspendedGraph& MST)
{
	MST.clear();
	MST = suspendedGraph(g.size());
	unsigned long long MSTweight = 0;
	vector<pair<unsigned long long, pair<int, int>>> shortestEdges;
	DSU<int> pickedEdges;
	for (int i = 0; i < g.size(); i++)
	{
		pickedEdges.makeSet(i);
		auto next = g.next(i);
		for (int j = next.size() - 1; j >= 0; j--)
		{
			if (i < next[j].first)
			{
				shortestEdges.push_back(std::make_pair(next[j].second, std::make_pair(i, next[j].first)));
			}
		}
	}
	std::sort(shortestEdges.begin(), shortestEdges.end());
	for (size_t i = 0; i < shortestEdges.size(); i++)
	{
		int first;
		auto edge = shortestEdges[i];
		pickedEdges.find(edge.second.first, first);
		int second;
		pickedEdges.find(edge.second.second, second);
		if (first == second)
		{
			continue;
		}
		MSTweight += edge.first;
		MST.addEdge(edge.second.first, edge.second.second, edge.first);
		MST.addEdge(edge.second.second, edge.second.first, edge.first);
		pickedEdges.merge(first, second);
	}
	return MSTweight;
}

int main()
{
	int n;
	int m;
	std::cin >> n >> m;
	vector<unsigned long long> values(n);
	for (int i = 0; i < n; i++)
	{
		std::cin >> values[i];
	}
	suspendedGraph g(n);
	for (int i = 0; i < m; i++)
	{
		int x;
		int y;
		unsigned long long w;
		std::cin >> x >> y >> w;
		g.addEdge(x - 1, y - 1, w);
		g.addEdge(y - 1, x - 1, w);
	}
	int minVertex = 0;
	for (int i = 1; i < n; i++)
	{
		if (values[i] < values[minVertex])
			minVertex = i;
	}
	for (int i = 0; i < n; i++)
	{
		if (i == minVertex)
			continue;
		g.addEdge(minVertex, i, values[i] + values[minVertex]);
		g.addEdge(i, minVertex, values[i] + values[minVertex]);
	}
	suspendedGraph MST;
	std::cout << KruskalMST(g, MST);
}
