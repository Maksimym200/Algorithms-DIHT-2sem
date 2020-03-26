#include <iostream>
#include <cassert>
#include <queue>
#include <utility>
#include <algorithm>
#include <vector>
using std::queue;
using std::pair;
using std::vector;

class graph
{
public:
	graph() = default;
	graph(int vertexNumber) : vNumber(vertexNumber), eArray(vector<vector<int>>(vertexNumber)) {}
	graph& operator= (const graph& g) = default;
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
private:
	graph(vector<vector<int>> edges)
	{
		eArray = edges;
		vNumber = edges.size();
	}
	int vNumber;
	vector<vector<int>> eArray;
};

template <class TVisit, class TCollector>
void BFS(graph& g, int startIndex, const TVisit& visit, TCollector& collector)
{
	bool* visited = new bool[g.size()];
	for (int i = g.size() - 1; i >= 0; i--)
	{
		visited[i] = false;
	}
	queue<pair<int, int>> nextToVisit;
	nextToVisit.push(pair<int, int>(startIndex, 0));
	visited[startIndex] = true;
	while (nextToVisit.size() != 0)
	{
		pair<int, int> vertex = nextToVisit.front();
		nextToVisit.pop();
		visit(vertex.first, vertex.second, g, collector);
		const vector<int>& next = g.next(vertex.first);
		int nextSize = next.size();
		for (int i = 0; i < nextSize; i++)
		{
			if (!visited[next[i]])
			{
				nextToVisit.push(pair<int, int>(next[i], vertex.second + 1));
				visited[next[i]] = true;
			}
		}
	}
	delete[] visited;
}

void visit(int index, int depth, graph& g, int* arr)
{
	arr[index] = depth;
}

int findMinRoadLength(graph& roads, int fPoint, int sPoint, int tPoint)
{
	int* fDist = new int[roads.size()];
	int* sDist = new int[roads.size()];
	int* tDist = new int[roads.size()];
	BFS(roads, fPoint, visit, fDist);
	BFS(roads, sPoint, visit, sDist);
	BFS(roads, tPoint, visit, tDist);
	int result = fDist[0] + sDist[0] + tDist[0];
	for (int i = roads.size() - 1; i > 0; i--)
	{
		result = std::min(result, fDist[i] + sDist[i] + tDist[i]);
	}
	delete[] fDist;
	delete[] sDist;
	delete[] tDist;
	return result;
}

int main()
{
	int n;
	int m;
	int leon;
	int matilda;
	int milk;
	std::cin >> n >> m >> leon >> matilda >> milk;
	graph roads(n);
	for (int i = 0; i < m; i++)
	{
		int fVertex;
		int sVertex;
		std::cin >> fVertex >> sVertex;
		roads.addEdge(fVertex - 1, sVertex - 1);
		roads.addEdge(sVertex - 1, fVertex - 1);
	}
	std::cout << findMinRoadLength(roads, leon - 1, matilda - 1, milk - 1);
}