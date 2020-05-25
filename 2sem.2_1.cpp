#include <iostream>
#include <cassert>
#include <vector>
#include <set>
using std::pair;
using std::vector;
using std::set;
using std::make_pair;

class suspendedGraph
{
public:
	suspendedGraph() = default;
	suspendedGraph(const int& vertexNumber)
	{
		edges = vector<vector<pair<int, int>>>(vertexNumber);
	}
	suspendedGraph(const suspendedGraph& g)
	{
		edges = g.edges;
	}
	suspendedGraph& operator= (const suspendedGraph& g)
	{
		edges = g.edges;
		return *this;
	}
	void addEdge(const int& fVertex, const int& sVertex, const int& weight)
	{
		assert(fVertex < size() && sVertex < size() && fVertex >= 0 && sVertex >= 0);
		edges[fVertex].push_back(pair<int, int>(sVertex, weight));
	}
	void addVertex()
	{
		edges.push_back(vector<pair<int, int>>(0));
	}
	const vector<pair<int, int>>& next(const int& index) const
	{
		assert(index < size() && index >= 0);
		return edges[index];
	}
	int size() const
	{
		return edges.size();
	}
private:
	suspendedGraph(const vector<vector<pair<int, int>>>& e)
	{
		edges = e;
	}
	vector<vector<pair<int, int>>> edges;
};

vector<int> findDistances(const suspendedGraph& g, const int& start)
{
	vector<bool> visited(g.size(), false);
	vector<int> distance(g.size(), INT32_MAX);
	set<pair<int, int>> closestVertexes;
	distance[start] = 0;
	closestVertexes.insert(make_pair(0, start));
	while (!closestVertexes.empty())
	{
		auto closest = closestVertexes.begin();
		if (visited[closest->second])
		{
			closestVertexes.erase(closest);
			continue;
		}
		visited[closest->second] = true;
		int thisVertex = closest->second;
		auto next = g.next(thisVertex);
		closestVertexes.erase(closest);
		for (int i = next.size() - 1; i >= 0; i--)
		{
			if (!visited[next[i].first] && distance[next[i].first] > distance[thisVertex] + next[i].second)
			{
				closestVertexes.erase(make_pair(distance[next[i].first], next[i].first));
				distance[next[i].first] = distance[thisVertex] + next[i].second;
				closestVertexes.insert(make_pair(distance[next[i].first], next[i].first));
			}
		}
	}
	return distance;
}

int main()
{
	int a;
	int b;
	int M;
	int start;
	int end;
	std::cin >> a >> b >> M >> start >> end;
	suspendedGraph g(M);
	for (long long i = 0; i < M; i++)
	{
		g.addEdge(i, (i + 1) % M, a);
		g.addEdge(i, (i * i + 1) % M, b);
	}
	std::cout << findDistances(g, start)[end];
}
