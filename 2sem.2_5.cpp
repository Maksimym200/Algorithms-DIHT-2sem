#include <iostream>
#include <cassert>
#include <stack>
#include <queue>
#include <vector>
using std::stack;
using std::pair;
using std::vector;
using std::queue;

template <class TCost>
class suspendedGraph
{
public:
	suspendedGraph() = default;
	suspendedGraph(const int& vertexNumber)
	{
		vNumber = vertexNumber;
		eArray = vector<vector<pair<int, TCost>>>(vNumber);
	}
	suspendedGraph(const suspendedGraph& g)
	{
		vNumber = g.size();
		eArray = g.eArray;
	}
	suspendedGraph& operator= (const suspendedGraph& g)
	{
		vNumber = g.size();
		eArray = g.eArray;
		return *this;
	}
	void addEdge(const int& fVertex, const int& sVertex, const TCost& weight)
	{
		assert(fVertex < vNumber && sVertex < vNumber && fVertex >= 0 && sVertex >= 0);
		eArray[fVertex].push_back(pair<int, TCost>(sVertex, weight));
	}
	void addVertex()
	{
		eArray.push_back(vector<pair<int, TCost>>(0));
	}
	const vector<pair<int, TCost>>& next(const int& index) const
	{
		assert(index < vNumber && index >= 0);
		return eArray[index];
	}
	int size() const
	{
		return vNumber;
	}
private:
	suspendedGraph(vector<vector<pair<int, TCost>>> edges)
	{
		eArray = edges;
		vNumber = edges.size();
	}
	int vNumber;
	vector<vector<pair<int, TCost>>> eArray;
};

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

vector<int> findSCC(const graph& g, int vertex)
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
	for (int i = 0; i < SCCarr.size(); i++)
	{
		for (int j = 0; j < SCCarr[i].size(); j++)
		{
			if (SCCarr[i][j] == vertex)
			{
				return SCCarr[i];
			}
		}
	}
}

template <class TCost, class TDistance>
bool hasCircle(const suspendedGraph<TCost>& g, int start, TDistance(*distance) (const TDistance& d, const TCost& c),
	const TDistance& null, const TDistance& infinity)
{
	vector<TDistance> distances(g.size(), infinity);
	distances[start] = null;
	bool isChanged = true;
	for (int k = 0; k < g.size(); k++)
	{
		if (!isChanged)
		{
			break;
		}
		isChanged = false;
		for (int i = 0; i < g.size(); i++)
		{
			if (!(distances[i] == infinity))
			{
				vector<pair<int, TCost>> next = g.next(i);
				for (int j = 0; j < next.size(); j++)
				{
					if (!(distance(distances[i], next[j].second) == infinity)
						&& (distances[next[j].first] == infinity || distance(distances[i], next[j].second) < distances[next[j].first]))
					{
						isChanged = true;
						distances[next[j].first] = distance(distances[i], next[j].second);
					}
				}
			}
		}
	}
	return isChanged;
}

struct cost
{
	double rate;
	double comission;
};

struct money
{
	double value = 0;
	bool operator< (const money& b) const
	{
		return value > b.value;
	}
	bool operator== (const money& b) const
	{
		return (value == b.value);
	}
};

money distance(const money& m, const cost& c)
{
	money result;
	result.value = (m.value - c.comission) * c.rate;
	if (result.value < 0)
	{
		result.value = 0;
	}
	return result;
}

int main()
{
	int N;
	int M;
	int S;
	double V;
	std::cin >> N >> M >> S >> V;
	S--;
	suspendedGraph<cost> sG(N);
	for (int i = 0; i < M; i++)
	{
		int a;
		int b;
		std::cin >> a >> b;
		a--;
		b--;
		cost c;
		std::cin >> c.rate >> c.comission;
		sG.addEdge(a, b, c);
		std::cin >> c.rate >> c.comission;
		sG.addEdge(b, a, c);
	}
	money start;
	start.value = V;
	if (hasCircle<cost, money>(sG, S, distance, start, money()))
	{
		std::cout << "YES";
	}
	else
	{
		std::cout << "NO";
	}
}