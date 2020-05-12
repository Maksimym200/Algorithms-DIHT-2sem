#include <iostream>
#include <set>
#include <vector>
#include <cassert>
using std::pair;
using std::vector;
using std::set;

class graph
{
public:
	graph() = default;
	explicit graph(int vertexNumber)
	{
		vNumber = vertexNumber;
		eArray = vector<vector<int>>(vNumber);
	}
	graph(const graph& g)
	{
		vNumber = g.size();
		eArray = g.eArray;
	}
	graph& operator= (const graph& g)
	{
		vNumber = g.size();
		eArray = g.eArray;
		return *this;
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
	explicit graph(vector<vector<int>> edges)
	{
		eArray = edges;
		vNumber = edges.size();
	}
	int vNumber;
	vector<vector<int>> eArray;
};

bool tryKuhn(const graph& g, int v, vector<int>& paired, vector<bool>& visited)
{
	if (visited[v])
	{
		return false;
	}
	visited[v] = true;
	for (int next : g.next(v))
	{
		if (paired[next] == -1 || tryKuhn(g, paired[next], paired, visited))
		{
			paired[next] = v;
			return true;
		}
	}
	return false;
}

bool Kuhn(const graph& g, int v, vector<int>& paired)
{
	vector<bool> visited(g.size(), false);
	return tryKuhn(g, v, paired, visited);
}

int numberOfMatchings(const graph& g, const vector<int>& left, const vector<int>& right)
{
	vector<int> paired(g.size(), -1);
	for (int i = 0; i < g.size(); i++)
	{
		Kuhn(g, i, paired);
	}
	for (int i = 0; i < paired.size(); i++)
	{
		std::cout << paired[i] << " ";
	}
	std::cout << "\n";
	int matchings = 0;
	for (int i = 0; i < right.size(); i++)
	{
		if (paired[right[i]] != -1)
		{
			matchings++;
		}
	}
	return matchings;
}

int main()
{
	int n;
	int m;
	int a;
	int b;
	std::cin >> n >> m >> a >> b;
	graph g(n * m);
	vector<int> left;
	vector<int> right;
	int placedNumber = 0;
	vector<char> prev(m + 1, '.');
	vector<char> curr(m + 1, '.');
	for (int i = 0; i < n; i++)
	{
		curr[0] = '.';
		for (int j = 0; j < m; j++)
		{
			char c;
			std::cin >> c;
			curr[j + 1] = '.';
			if (c == '*')
			{
				if ((i + j) % 2 == 0)
					left.push_back(i * m + j);
				else
					right.push_back(i * m + j);
				if (prev[j + 1] == '*')
				{
					g.addEdge(i * m + j, i * m + j - m);
					g.addEdge(i * m + j - m, i * m + j);
				}
				if (curr[j] == '*')
				{
					g.addEdge(i * m + j, i * m + j - 1);
					g.addEdge(i * m + j - 1, i * m + j);
				}
				curr[j + 1] = '*';
			}
			else
				++placedNumber;
		}
		prev = curr;
	}
	std::cout << "Answer: ";
	if (2 * b <= a)
	{
		std::cout << (m * n - placedNumber) * b;
	}
	else
	{
		int matching = numberOfMatchings(g, left, right);
		std::cout << matching * a + (m * n - placedNumber - matching * 2) * b;
	}
	std::cout << "\n";
	for (int i : right)
	{
		std::cout << i << " ";
	}
	std::cout << "\n";
	for (int i : left)
	{
		std::cout << i << " ";
	}
	std::cout << "\n";
}
