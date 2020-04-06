#include <iostream>
#include <cassert>
#include <stack>
#include <vector>
using std::stack;
using std::pair;
using std::vector;

class graph
{
public:
	graph() = default;
	explicit graph(int vertexNumber)
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
private:
	int vNumber;
	vector<vector<int>> eArray;
};

bool tryTopSort(graph& g, vector<int>& arr)
{
	bool result = true;
	arr.clear();
	int size = g.size();
	int* colors = new int[size]; // 0 - not visited, 1 - in process of visiting, 2- visited
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
		g.addEdge(sVertex, fVertex);
	}
	vector<int> arr;
	if (tryTopSort(g, arr))
	{
		std::cout << "YES\n";
		for (int i = 0; i < n; i++)
		{
			std::cout << arr[i] << " ";
		}
	}
	else
	{
		std::cout << "NO";
	}
}
