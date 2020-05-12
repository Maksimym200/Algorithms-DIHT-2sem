#include <iostream>
#include <vector>
#include <stack>
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
	visitor(int s, int e, vector<int>& i, vector<int>& w) : start(s), end(e), indexes(i), way(w) { ; }
	bool operator()(const graph& g, bool isExit, pair<int, pair<int, int>> v)
	{
		if (isExit)
		{
			if (v.first == start)
			{
				way.clear();
				return true;
			}
			indexes[v.second.first] = -1;
			way.pop_back();
			return false;
		}
		else
		{
			indexes[v.second.first] = v.second.second;
			way.push_back(v.first);
			if (v.first == end)
			{
				ended = true;
				return true;
			}
			return false;
		}
	}
	int end;
	int start;
	bool ended = false;
	vector<int>& indexes;
	vector<int>& way;
};

template <class TVisit>
void DFS(const graph& g, int start, TVisit& visit)
{
	vector<int> colors(g.size(), 0);
	stack<pair<int, pair<int, int>>> toVisit;
	toVisit.push(std::make_pair(start, std::make_pair(start, -1)));
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
					toVisit.push(std::make_pair(next[j], std::make_pair(v.first, j)));
			}
		}
	}
}

bool tryFindTwoWays(graph g, int start, int end, vector<int>& fWay, vector<int>& sWay)
{
	fWay.clear();
	sWay.clear();
	vector<int> indexes(g.size(), -1);
	visitor fV(start, end, indexes, fWay);
	DFS(g, start, fV);
	if (!fV.ended)
		return false;
	for (size_t i = 0; i < g.size(); i++)
	{
		if (indexes[i] >= 0)
			g.next(i).erase(g.next(i).begin() + indexes[i]);
	}
	vector<int> fNext(g.size(), -1);
	for (size_t i = 1; i < fWay.size(); i++)
	{
		fNext[fWay[i - 1]] = fWay[i];
		g.addEdge(fWay[i], fWay[i - 1]);
	}
	visitor sV(start, end, indexes, sWay);
	DFS(g, start, sV);
	if (!sV.ended)
		return false;
	graph ways(g.size());
	for (size_t i = 1; i < sWay.size(); i++)
	{
		if (fNext[sWay[i]] == sWay[i - 1])
			fNext[sWay[i]] = -1;
		else
			ways.addEdge(sWay[i - 1], sWay[i]);
	}
	for (size_t i = 0; i < g.size(); i++)
	{
		if (fNext[i] != -1)
			ways.addEdge(i, fNext[i]);
	}
	fWay.clear();
	sWay.clear();
	fWay.push_back(start);
	sWay.push_back(start);
	indexes = vector<int>(g.size(), -1);
	DFS(ways, ways.next(start)[0], fV);
	for (size_t i = 0; i < ways.size(); i++)
	{
		if (indexes[i] >= 0)
			ways.next(i).erase(ways.next(i).begin() + indexes[i]);
	}
	DFS(ways, ways.next(start)[1], sV);
	return true;
}

int main()
{
	int n;
	int m;
	int s;
	int t;
	std::cin >> n >> m >> s >> t;
	graph g(n);
	for (int i = 0; i < m; i++)
	{
		int x;
		int y;
		std::cin >> x >> y;
		g.addEdge(x - 1, y - 1);
	}
	vector<int> fWay;
	vector<int> sWay;
	if (tryFindTwoWays(g, s - 1, t - 1, fWay, sWay))
	{
		std::cout << "YES\n";
		for (int i : fWay)
			std::cout << i + 1 << " ";
		std::cout << "\n";
		for (int i : sWay)
			std::cout << i + 1 << " ";
	}
	else
	{
		std::cout << "NO\n";
	}
}