#include <iostream>
#include <cassert>
#include <vector>
using std::pair;
using std::vector;

template <class T>
class heap
{
public:

	heap(bool (*function) (const T& a, const T& b))
	{
		N = 0;
		buffer = vector<T>(0);
		isMore = *function;
	}
	heap(const vector<T>& arr, bool (*function) (const T& a, const T& b))
	{
		isMore = *function;
		N = arr.size();
		buffer = arr;
		for (int i = (arr.size() - 1) / 2; i >= 0; i--)
		{
			checkHeapDown(i);
		}
	}

	heap& operator= (heap& oldHeap)
	{
		N = oldHeap.N;
		buffer = oldHeap.buffer;
		isMore = oldHeap.isMore;
		return*this;
	}

	void push(const T& element)
	{
		if (N == buffer.size())
		{
			buffer.push_back(element);
		}
		N++;
		buffer[N - 1] = element;
		checkHeapUp(N - 1);
	}

	T pop()
	{
		T element = buffer[0];
		buffer[0] = buffer[N - 1];
		N--;
		checkHeapDown(0);
		return element;
	}

	int top() const
	{
		return buffer[0];
	}

	int size() const
	{
		return N;
	}

private:
	vector<T> buffer;
	int N;
	bool (*isMore) (const T& a, const T& b);
	int bestRoot(int indexA, int indexB)
	{
		if (indexA <= N - 1)
		{
			if (indexB <= N - 1)
			{
				if (isMore(buffer[indexA], buffer[indexB]))
				{
					buffer[(indexA - 1) / 2] = buffer[indexA];
					return indexA;
				}
				else
				{
					buffer[(indexA - 1) / 2] = buffer[indexB];
					return indexB;
				}
			}
			return indexA;
		}
		return (indexA - 1) / 2;
	}
	void checkHeapUp(int index)
	{
		if (index != 0)
		{
			if (isMore(buffer[index], buffer[(index - 1) / 2]))
			{
				std::swap(buffer[index], buffer[(index - 1) / 2]);
				checkHeapUp((index - 1) / 2);
			}
		}
	}
	void checkHeapDown(int index)
	{
		T element = buffer[index];
		for (int newIndex = bestRoot(2 * index + 1, 2 * index + 2); (newIndex != index)
			&& (isMore(buffer[newIndex], element)); newIndex = bestRoot(2 * index + 1, 2 * index + 2))
		{
			buffer[index] = buffer[newIndex];
			index = newIndex;
		}
		buffer[index] = element;
	}
};

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

bool isCloser(const pair<int, double>& a, const pair<int, double>& b)
{
	return (a.second > b.second);
}

vector<double> findProbability(const suspendedGraph& g, const int& start)
{
	vector<bool> visited(g.size(), false);
	vector<double> distance(g.size(), 0);
	heap<pair<int, double>> closestVertexes(isCloser);
	distance[start] = 1;
	closestVertexes.push(pair<int, double>(start, 1));
	while (closestVertexes.size() != 0)
	{
		int closest = closestVertexes.pop().first;
		if (visited[closest])
		{
			continue;
		}
		visited[closest] = true;
		vector<pair<int, int>> next = g.next(closest);
		for (int i = next.size() - 1; i >= 0; i--)
		{
			if (!visited[next[i].first] &&
				(distance[next[i].first] < distance[closest] * double(100 - next[i].second) / 100))
			{
				distance[next[i].first] = distance[closest] * double(100 - next[i].second) / 100;
				closestVertexes.push(pair<int, double>(next[i].first, distance[next[i].first]));
			}
		}
	}
	return distance;
}

int main()
{
	int N;
	int M;
	int S;
	int F;
	std::cin >> N >> M >> S >> F;
	suspendedGraph g(N);
	for (int i = 0; i < M; i++)
	{
		int start;
		int end;
		int cost;
		std::cin >> start >> end >> cost;
		g.addEdge(start - 1, end - 1, cost);
		g.addEdge(end - 1, start - 1, cost);
	}
	std::cout << 1 - findProbability(g, S - 1)[F - 1];
}