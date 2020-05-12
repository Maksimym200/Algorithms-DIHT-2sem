#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using std::vector;
using std::pair;
using std::queue;

class suspendedGraph
{
public:
	suspendedGraph() = default;
	suspendedGraph(const int& vNumber)
	{
		eArray = vector<vector<int>>(vNumber, vector<int>(vNumber, 0));
	}
	suspendedGraph(const suspendedGraph& g)
	{
		eArray = g.eArray;
	}
	suspendedGraph(suspendedGraph&& g)
	{
		eArray = std::move(g.eArray);
	}
	suspendedGraph& operator= (const suspendedGraph& g)
	{
		eArray = g.eArray;
		return *this;
	}
	suspendedGraph& operator= (suspendedGraph&& g)
	{
		eArray = std::move(g.eArray);
		return *this;
	}
	int& weight(const int& fVertex, const int& sVertex)
	{
		return eArray[fVertex][sVertex];
	}
	int weight(const int& fVertex, const int& sVertex) const
	{
		return eArray[fVertex][sVertex];
	}
	vector<int> next(const int& index) const
	{
		vector<int> result;
		for (size_t i = 0; i < size(); i++)
		{
			if (eArray[index][i] > 0)
				result.push_back(i);
		}
		return std::move(result);
	}
	size_t size() const
	{
		return eArray.size();
	}
	void clear()
	{
		eArray.clear();
	}
	void reverse()
	{
		for (size_t i = 1; i < size(); i++)
		{
			for (size_t j = 0; j < i; j++)
				std::swap(eArray[i][j], eArray[j][i]);
		}
	}
private:
	suspendedGraph(vector<vector<int>> edges)
	{
		eArray = edges;
	}
	vector<vector<int>> eArray;
};

int getMaxLinearStreamWeight(suspendedGraph& g, const vector<pair<int, int>>& linearStream)
{
	int result = g.weight(linearStream[0].first, linearStream[0].second);
	for (size_t i = 1; i < linearStream.size(); i++)
	{
		result = std::min(g.weight(linearStream[i].first, linearStream[i].second), result);
	}
	return result;
}

void substractLinearStream(suspendedGraph& g, const vector<pair<int, int>>& linearStream, int streamWeight)
{
	for (auto p : linearStream)
	{
		g.weight(p.first, p.second) -= streamWeight;
		g.weight(p.second, p.first) += streamWeight;
	}
}

void substractStream(suspendedGraph& g, const suspendedGraph& stream)
{
	for (size_t i = 0; i < g.size(); i++)
	{
		for (size_t j = 0; j < g.size(); j++)
		{
			g.weight(i, j) -= stream.weight(i, j);
		}
	}
}

void addLinearStream(suspendedGraph& stream, const vector<pair<int, int>>& linearStream, int streamWeight)
{
	for (auto p : linearStream)
	{
		stream.weight(p.first, p.second) += streamWeight;
		stream.weight(p.second, p.first) -= streamWeight;
	}
}

bool tryFindLinearStream(const suspendedGraph& g, int start, int end, vector<pair<int, int>>& linearStream, vector<bool>& visited)
{
	if (start == end)
	{
		return true;
	}
	visited[start] = true;
	for (auto n : g.next(start))
	{
		if (visited[n] || !tryFindLinearStream(g, n, end, linearStream, visited))
			continue;
		linearStream.push_back(std::make_pair(start, n));
		return true;
	}
	return false;
}

bool tryFindLinearStream(const suspendedGraph& g, int start, int end, vector<pair<int, int>>& linearStream)
{
	linearStream.clear();
	vector<bool> visited(g.size(), false);
	return tryFindLinearStream(g, start, end, linearStream, visited);
}

int findMaxStream(suspendedGraph g, int input, int output, suspendedGraph& stream)
{
	vector<pair<int, int>> linearStream;
	int streamWeight = 0;
	stream.clear();
	stream = suspendedGraph(g.size());
	while (tryFindLinearStream(g, input, output, linearStream))
	{
		int linearStreamWeight = getMaxLinearStreamWeight(g, linearStream);
		substractLinearStream(g, linearStream, linearStreamWeight);
		addLinearStream(stream, linearStream, linearStreamWeight);
		streamWeight += linearStreamWeight;
	}
	return streamWeight;
}

struct visiter
{
	visiter(vector<int>& _v) : v(_v) { ; }
	void operator()(int vertex, const suspendedGraph& g) { v.push_back(vertex); }
	vector<int>& v;
};

template <class TVisit>
void BFS(const suspendedGraph& g, int start, TVisit& visit)
{
	vector<bool> visited(g.size(), false);
	queue<int> nextToVisit;
	nextToVisit.push(start);
	visited[start] = true;
	while (nextToVisit.size() != 0)
	{
		int vertex = nextToVisit.front();
		nextToVisit.pop();
		visit(vertex, g);
		const vector<int>& next = g.next(vertex);
		int nextSize = next.size();
		for (int i = 0; i < nextSize; i++)
		{
			if (!visited[next[i]])
			{
				nextToVisit.push(next[i]);
				visited[next[i]] = true;
			}
		}
	}
}

int getMinCutWeight(suspendedGraph g, int input, int output)
{
	suspendedGraph stream;
	int weight = findMaxStream(g, input, output, stream);
	substractStream(g, stream);
	return weight;
}

void findMinCut(suspendedGraph g, int input, int output, vector<int>& fSet, vector<int>& sSet)
{
	suspendedGraph stream;
	findMaxStream(g, input, output, stream);
	substractStream(g, stream);
	sSet.clear();
	fSet.clear();
	visiter fV(fSet);
	visiter sV(sSet);
	BFS<visiter>(g, input, fV);
	g.reverse();
	BFS<visiter>(g, output, sV);
}

void getPairs(const suspendedGraph& g, vector<int>& fSet, vector<int>& sSet)
{
	int minCutWeight = getMinCutWeight(g, 0, 1);
	size_t bestOuptput = 1;
	for (size_t i = 2; i < g.size(); i++)
	{
		int newWeight = getMinCutWeight(g, 0, i);
		if (newWeight < minCutWeight)
		{
			minCutWeight = newWeight;
			bestOuptput = i;
		}
	}
	findMinCut(g, 0, bestOuptput, fSet, sSet);
	vector<bool> visited(g.size(), false);
	for (auto v : fSet)
		visited[v] = true;
	for (auto v : sSet)
		visited[v] = true;
	for (size_t i = 0; i < g.size(); i++)
	{
		if (!visited[i])
			sSet.push_back(i);
	}
}

int main()
{
	int n;
	std::cin >> n;
	suspendedGraph g(n);
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			char c;
			std::cin >> c;
			g.weight(i, j) = c - '0';
		}
	}
	vector<int> fSet;
	vector<int> sSet;
	getPairs(g, fSet, sSet);
	for (int i : fSet)
		std::cout << i + 1 << " ";
	std::cout << "\n";
	for (int i : sSet)
		std::cout << i + 1 << " ";
}