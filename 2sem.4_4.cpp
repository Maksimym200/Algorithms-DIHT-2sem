#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
using std::vector;
using std::pair;
using std::stack;

class graph
{
public:
	graph() = default;
	graph(const int& vNumber)
	{
		eArray = vector<vector<int>>(vNumber);
	}
	graph(const graph& g)
	{
		eArray = g.eArray;
	}
	graph& operator= (const graph& g)
	{
		eArray = g.eArray;
		return *this;
	}
	void addEdge(const int& fVertex, const int& sVertex)
	{
		eArray[fVertex].push_back(sVertex);
	}
	void addVertex()
	{
		eArray.push_back(vector<int>(0));
	}
	const vector<int>& next(const int& index) const
	{
		return eArray[index];
	}
	vector<int>& next(const int& index)
	{
		return eArray[index];
	}
	size_t size() const
	{
		return eArray.size();
	}
	void clear()
	{
		eArray.clear();
	}
private:
	graph(vector<vector<int>> edges)
	{
		eArray = edges;
	}
	vector<vector<int>> eArray;
};

struct visitor
{
	visitor(vector<int>& d, vector<int>& p) : depths(d), parents(p) { ; }
	bool operator()(const graph& g, bool isExit, pair<int, int> v)
	{
		if (isExit)
		{
			--depth;
		}
		else
		{
			depths[v.first] = depth;
			parents[v.first] = v.second;
			++depth;
		}
		return false;
	}
	int depth = 0;
	vector<int>& depths;
	vector<int>& parents;
};

template <class TVisit>
void DFS(const graph& g, int start, TVisit& visit)
{
	vector<int> colors(g.size(), 0);
	stack<pair<int, int>> toVisit;
	toVisit.push(std::make_pair(start, start));
	while (toVisit.size() != 0)
	{
		auto v = toVisit.top();
		toVisit.pop();
		if (colors[v.first] == 1)
		{
			if (visit(g, true, v))
				return;
			colors[v.first] = 2;
		}
		if (colors[v.first] == 0)
		{
			if (visit(g, false, v))
				return;
			colors[v.first] = 1;
			toVisit.push(v);
			vector<int> next = g.next(v.first);
			for (size_t j = 0; j < next.size(); j++)
			{
				if (colors[next[j]] == 0)
					toVisit.push(std::make_pair(next[j], v.first));
			}
		}
	}
}

class LCATable
{
public:
	LCATable(const graph& g)
	{
		depths = vector<int>(g.size());
		parents = vector<int>(g.size());
		visitor v(depths, parents);
		DFS(g, 0, v);
		size_t power = 1;
		depthPoints.push_back(parents);
		for (size_t i = 1; power < g.size(); i++)
		{
			depthPoints.push_back(vector<int>(g.size()));
			for (size_t j = 0; j < g.size(); j++)
			{
				depthPoints[i][j] = depthPoints[i - 1][depthPoints[i - 1][j]];
			}
			power *= 2;
		}
	}
	size_t LCA(size_t u, size_t v)
	{
		if (depths[v] > depths[u])
			std::swap(u, v);
		for (int i = depthPoints.size() - 1; i >= 0; i--)
		{
			if (depths[depthPoints[i][u]] - depths[v] >= 0)
				u = depthPoints[i][u];
		}
		if (v == u)
			return v;
		for (int i = depthPoints.size() - 1; i >= 0; i--)
		{
			if (depthPoints[i][v] != depthPoints[i][u])
			{
				v = depthPoints[i][v];
				u = depthPoints[i][u];
			}
		}
		return parents[v];
	}
private:
	vector<vector<int>> depthPoints;
	vector<int> depths;
	vector<int> parents;
};

int main()
{
	int n;
	int m;
	std::cin >> n >> m;
	graph tree(n);
	for (int i = 1; i < n; i++)
	{
		int j;
		std::cin >> j;
		tree.addEdge(j, i);
	}
	LCATable table(tree);
	unsigned long long a;
	unsigned long long b;
	std::cin >> a >> b;
	int x;
	int y;
	int z;
	std::cin >> x >> y >> z;
	unsigned long long result = 0;
	int v = 0;
	for (int i = 0; i < m; i++)
	{
		v = table.LCA((a + v) % n, b);
		result += v;
		a = (x * a + y * b + z) % n;
		b = (x * b + y * a + z) % n;
	}
	std::cout << result;
	/*for (int i = 0; i < m; i++)
	{
		int a;
		int b;
		std::cin >> a >> b;
		std::cout << table.LCA(a, b) << "\n";
	}*/
}