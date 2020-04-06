#include <iostream>
#include <cassert>
#include <vector>
using std::pair;
using std::vector;

class suspendedGraph
{
public:
	suspendedGraph() = default;
	suspendedGraph(const int& vertexNumber)
	{
		vNumber = vertexNumber;
		eArray = vector<vector<pair<int, int>>>(vNumber);
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
	void addEdge(const int& fVertex, const int& sVertex, const int& weight)
	{
		assert(fVertex < vNumber && sVertex < vNumber && fVertex >= 0 && sVertex >= 0);
		eArray[fVertex].push_back(pair<int, int>(sVertex, weight));
	}
	void addVertex()
	{
		eArray.push_back(vector<pair<int, int>>(0));
	}
	const vector<pair<int, int>>& next(const int& index) const
	{
		assert(index < vNumber && index >= 0);
		return eArray[index];
	}
	int size() const
	{
		return vNumber;
	}
private:
	suspendedGraph(vector<vector<pair<int, int>>> edges)
	{
		eArray = edges;
		vNumber = edges.size();
	}
	int vNumber;
	vector<vector<pair<int, int>>> eArray;
};

vector<int> findLessDistances(const suspendedGraph& g, const int& start, const int& k)
{
	vector<vector<int>> arr(k + 1, vector<int>(g.size(), -1));
	arr[0][start] = 0;
	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < g.size(); j++)
		{
			if (arr[i][j] != -1)
			{
				vector<pair<int, int>> next = g.next(j);
				for (int t = 0; t < next.size(); t++)
				{
					if (arr[i + 1][next[t].first] == -1 || arr[i + 1][next[t].first] > arr[i][j] + next[t].second)
					{
						arr[i + 1][next[t].first] = arr[i][j] + next[t].second;
					}
				}
				if (arr[i + 1][j] == -1 || arr[i + 1][j] > arr[i][j])
				{
					arr[i + 1][j] = arr[i][j];
				}
			}
		}
	}
	return arr[k];
}


int main()
{
	int N;
	int M;
	int k;
	int S;
	int F;
	std::cin >> N >> M >> k >> S >> F;
	suspendedGraph g(N);
	for (int i = 0; i < M; i++)
	{
		int start;
		int end;
		int cost;
		std::cin >> start >> end >> cost;
		g.addEdge(start - 1, end - 1, cost);
	}
	std::cout << findLessDistances(g, S - 1, k)[F - 1];
}