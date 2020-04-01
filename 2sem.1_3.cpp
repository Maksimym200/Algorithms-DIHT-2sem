#include <iostream>
#include <cassert>
#include <stack>
#include <queue>
#include <vector>
using std::stack;
using std::pair;
using std::vector;
using std::queue;

class graph
{
public:
	graph() = default;
	graph(int vertexNumber)
	{
		vNumber = vertexNumber;
		eArray = vector<vector<int>>(vNumber);
	}
	void addEdge(int fVertex, int sVertex)
	{
		assert(fVertex < vNumber && sVertex < vNumber && fVertex >= 0 && sVertex >= 0);
		eArray[fVertex].push_back(sVertex);
	}
	void addVertex()
	{
		eArray.push_back(vector<int>(0));
	}
	const vector<int>& next(int index) const
	{
		assert(index < vNumber && index >= 0);
		return eArray[index];
	}
	int size() const
	{
		return vNumber;
	}
	void reverse()
	{
		vector<vector<int>> revEdges(vNumber);
		for (int i = vNumber - 1; i >= 0; i--)
		{
			vector<int> edges = next(i);
			for (int j = edges.size() - 1; j >= 0; j--)
			{
				revEdges[edges[j]].push_back(i);
			}
		}
		*this = graph(revEdges);
	}
private:
	graph(vector<vector<int>> edges)
	{
		eArray = edges;
		vNumber = edges.size();
	}
	int vNumber = 0;
	vector<vector<int>> eArray;
};

template <class TGraph, class TCollector>
void BFS(TGraph& g, int startIndex, void (*visit)(int i, int d, TGraph& g, TCollector& c), TCollector& collector, bool* visited)
{
	queue<pair<int, int>> nextToVisit;
	nextToVisit.push(pair<int, int>(startIndex, 0));
	visited[startIndex] = true;
	while (nextToVisit.size() != 0)
	{
		pair<int, int> vertex = nextToVisit.front();
		nextToVisit.pop();
		visit(vertex.first, vertex.second, g, collector);
		const vector<int>& next = g.next(vertex.first);
		int nextNumber = next.size();
		for (int i = 0; i < nextNumber; i++)
		{
			if (!visited[next[i]])
			{
				nextToVisit.push(pair<int, int>(next[i], vertex.second));
				visited[next[i]] = true;
			}
		}
	}
}

template <class TGraph, class TCollector>
void BFS(TGraph& g, int startIndex, void (*visit)(int i, int d, TGraph& g, TCollector& c), TCollector& collector)
{
	bool* visited = new bool[g.size()];
	for (int i = g.size() - 1; i >= 0; i--)
	{
		visited[i] = false;
	}
	BFS(g, startIndex, visit, collector, visited);
	delete[] visited;
}

bool topSort(const graph& g, vector<int>& arr)
{
	bool result = true;
	arr.clear();
	int size = g.size();
	int* colors = new int[size];
	for (int i = 0; i < size; i++)
	{
		colors[i] = 0;
	}
	stack<int> toVisit;
	for (int i = 0; i < size; )
	{
		if (colors[i] == 2)
		{
			i++;
			continue;
		}
		toVisit.push(i);
		while (toVisit.size() != 0)
		{
			int v = toVisit.top();
			toVisit.pop();
			if (colors[v] == 1)
			{
				colors[v] = 2;
				arr.push_back(v);
			}
			if (colors[v] == 0)
			{
				colors[v] = 1;
				toVisit.push(v);
				vector<int> next = g.next(v);
				for (int j = next.size() - 1; j >= 0; j--)
				{
					if (colors[next[j]] == 1)
					{
						result = false;
					}
					else
					{
						toVisit.push(next[j]);
					}
				}
			}
		}
	}
	delete[] colors;
	return result;
}

void collectVertex(int index, int depth, const graph& g, vector<int>& arr)
{
	arr.push_back(index);
}

vector<vector<int>> findSCC(const graph& g)
{
	vector<vector<int>> SCCarr;
	bool* visited = new bool[g.size()];
	for (int i = g.size() - 1; i >= 0; i--)
	{
		visited[i] = false;
	}
	graph gRev = g;
	gRev.reverse();
	vector<int> arr;
	topSort(gRev, arr);
	for (int i = arr.size() - 1; i >= 0; i--)
	{
		if (visited[arr[i]])
		{
			continue;
		}
		vector<int> SCC;
		SCCarr.push_back(SCC);
		BFS<const graph, vector<int>>(g, arr[i], collectVertex, SCCarr[SCCarr.size() - 1], visited);
	}
	delete[] visited;
	return SCCarr;
}

graph getSCCGraph(const graph& g)
{
	vector<vector<int>> SCCarr = findSCC(g);
	vector<int> SCCBelonging(g.size());
	for (int i = SCCarr.size() - 1; i >= 0; i--)
	{
		for (int j = SCCarr[i].size() - 1; j >= 0; j--)
		{
			SCCBelonging[SCCarr[i][j]] = i;
		}
	}
	graph SCCGraph(SCCarr.size());
	for (int i = SCCarr.size() - 1; i >= 0; i--)
	{
		vector<bool> edges(SCCarr.size(), false);
		for (int j = SCCarr[i].size() - 1; j >= 0; j--)
		{
			vector<int> neighbours = g.next(SCCarr[i][j]);
			for (int k = neighbours.size() - 1; k >= 0; k--)
			{
				if (!edges[SCCBelonging[neighbours[k]]] && SCCBelonging[neighbours[k]] != i)
				{
					SCCGraph.addEdge(i, SCCBelonging[neighbours[k]]);
				}
				edges[SCCBelonging[neighbours[k]]] = true;
			}
		}
	}
	return SCCGraph;
}

int distanceToSCG(const graph& g)
{
	graph SCCGraph = getSCCGraph(g);
	if (SCCGraph.size() == 1)
	{
		return 0;
	}
	int entranceVNumber = 0;
	int exitVNumber = 0;
	for (int i = SCCGraph.size() - 1; i >= 0; i--)
	{
		if (SCCGraph.next(i).size() == 0)
		{
			exitVNumber++;
		}
	}
	SCCGraph.reverse();
	for (int i = SCCGraph.size() - 1; i >= 0; i--)
	{
		if (SCCGraph.next(i).size() == 0)
		{
			entranceVNumber++;
		}
	}
	return std::max(exitVNumber, entranceVNumber);
}

int main()
{
	int n;
	int m;
	std::cin >> n >> m;
	graph g(n);
	for (int i = 0; i < m; i++)
	{
		int fVertex;
		int sVertex;
		std::cin >> fVertex >> sVertex;
		g.addEdge(fVertex - 1, sVertex - 1);
	}
	std::cout << distanceToSCG(g);
}
