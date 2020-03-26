#include <iostream>
#include <cassert>
#include <stack>
#include <queue>
#include <utility>
#include <algorithm>
#include <vector>
#include <unordered_set>
#include <unordered_map>
using std::stack;
using std::pair;
using std::vector;
using std::queue;
using std::unordered_set;
using std::unordered_map;

class graph
{
public:
	graph() = default;
	graph(int vertexNumber)
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
	const vector<int>& getNeighbors(int index) const
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
			vector<int> edges = getNeighbors(i);
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
	int vNumber;
	vector<vector<int>> eArray;
};

bool find(vector<int> vec, int value)
{
	for (int i = vec.size() - 1; i >= 0; i--)
	{
		if (vec[i] == value)
		{
			return true;
		}
	}
	return false;
}

class planaryGraph : public graph
{
public:
	planaryGraph(const vector<int>& circle, int vertexNumber) : graph(vertexNumber)
	{
		faces.push_back(face(circle));
		faces.push_back(face(circle));
		for (int i = circle.size() - 1; i > 0; i--)
		{
			addEdge(circle[i], circle[i - 1]);
			addEdge(circle[i - 1], circle[i]);
		}
		addEdge(circle[circle.size() - 1], circle[0]);
		addEdge(circle[0], circle[circle.size() - 1]);
	}
	class face
	{
	public:
		face() = default;
		face(vector<int> c) : circle(c) 
		{
			for (int i = circle.size() - 1; i >= 0; i--)
			{
				vSet.insert(circle[i]);
			}
		}
		face& operator= (const face& f)
		{
			circle = f.circle;
			vSet = f.vSet;
			return *this;
		}
		void clear()
		{
			circle.clear();
		}
		bool findEdge(int vF, int vS, int& indexVF, int& indexVS)
		{
			bool hasVF = false;
			bool hasVS = false;
			for (int i = circle.size() - 1; i >= 0; i--)
			{
				if (circle[i] == vF)
				{
					hasVF = true;
					indexVF = i;
				}
				if (circle[i] == vS)
				{
					hasVS = true;
					indexVS = i;
				}
			}
			return hasVF && hasVS;
		}
		vector<int> circle;
		unordered_set<int> vSet;
	};
	const vector<int>& getFace(int faceIndex)
	{
		return faces[faceIndex].circle;
	}
	bool hasVertexes(int faceIndex, const vector<int>& vertexes)
	{
		for (int i = vertexes.size() - 1; i >= 0; i--)
		{
			if (faces[faceIndex].vSet.count(vertexes[i]) == 0)
			{
				return false;
			}
		}
		return true;
	}
	void addLine(vector<int> line, int f)
	{
		assert(faces.size() > f);
		int indexF;
		int indexS;
		assert(faces[f].findEdge(line[0], line[line.size() - 1], indexF, indexS));
		vector<int> circleF;
		for (int i = indexF; ; i++)
		{
			if (i >= faces[f].circle.size())
			{
				i -= faces[f].circle.size();
			}
			circleF.push_back(faces[f].circle[i]);
			if (i == indexS)
			{
				break;
			}
		}
		for (int i = line.size() - 2; i > 0; i--)
		{
			circleF.push_back(line[i]);
		}
		vector<int> circleS;
		for (int i = line.size() - 2; i > 0; i--)
		{
			circleS.push_back(line[i]);
		}
		for (int i = indexS; ; i++)
		{
			if (i >= faces[f].circle.size())
			{
				i -= faces[f].circle.size();
			}
			circleS.push_back(faces[f].circle[i]);
			if (i == indexF)
			{
				break;
			}
		}
		faces[f] = face(circleF);
		faces.push_back(face(circleS));
	}
//private:
	vector<face> faces;
};

bool getCircle(const graph& g, const vector<int>& subGraph, vector<int>& arr)
{
	arr.clear();
	int size = g.size();
	int* colors = new int[size];
	for (int i = 0; i < size; i++)
	{
		colors[i] = 2;
	}
	for (int i = subGraph.size() - 1; i >= 0; i--)
	{
		colors[subGraph[i]] = 0;
	}
	stack<int> toVisit;
	stack<int> currentWay;
	currentWay.push(-1);
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
				currentWay.pop();
			}
			if (colors[v] == 0)
			{
				colors[v] = 1;
				toVisit.push(v);
				vector<int> next = g.getNeighbors(v);
				for (int j = next.size() - 1; j >= 0; j--)
				{
					if (colors[next[j]] == 1  && currentWay.top() != next[j])
					{
						currentWay.push(v);
						arr.push_back(next[j]);
						while (currentWay.top() != next[j])
						{
							arr.push_back(currentWay.top());
							currentWay.pop();
						}

						/* std::cout << "CIRCLE:\n";
						for (int i = 0; i < arr.size(); i++)
						{
							std::cout << arr[i] << " ";
						}
						std::cout << "\n\n"; */

						delete[] colors;
						return true;
					}
					else if(currentWay.top() != next[j])
					{
						toVisit.push(next[j]);
					}
				}
				currentWay.push(v);
			}
		}
	}
	delete[] colors;
	return false;
}

template <class TGraph, class TCollector>
void BFS(TGraph& g, int startIndex, void (*visit)(int i, int color, TGraph& g, TCollector& c), TCollector& collector, int* colors)
{
	queue<int> nextToVisit;
	nextToVisit.push(startIndex);
	colors[startIndex] = 1;
	while (nextToVisit.size() != 0)
	{
		int vertex = nextToVisit.front();
		nextToVisit.pop();
		visit(vertex, colors[vertex], g, collector);
		const vector<int>& next = g.getNeighbors(vertex);
		for (int i = next.size() - 1; i >= 0; i--)
		{
			if (colors[next[i]] == 0)
			{
				nextToVisit.push(next[i]);
				colors[next[i]] = 1;
			}
			if (colors[next[i]] > 1)
			{
				visit(next[i], colors[next[i]], g, collector);
				colors[next[i]] = 1;
			}
		}
	}
}

void collectVertex(int index, int color, const graph& g, pair<vector<int>, vector<int>>& arr)
{
	if (color == 1)
	{
		arr.first.push_back(index);
	}
	else if (color == 2)
	{
		arr.second.push_back(index);
	}
}

vector<pair<vector<int>, vector<int>>> getSegments(const graph& g, const vector<int>& subGraph, const vector<int>& chain, vector<vector<int>>& fewContactsSegments)
{
	vector<pair<vector<int>, vector<int>>> segments;
	fewContactsSegments.clear();
	int* colors = new int[g.size()];
	for (int i = g.size() - 1; i >= 0; i--)
	{
		colors[i] = 2;
	}
	for (int i = subGraph.size() - 1; i >= 0; i--)
	{
		colors[subGraph[i]] = 0;
	}
	for (int i = chain.size() - 1; i >= 0; i--)
	{
		colors[chain[i]] = 2;
	}

	for (int i = g.size() - 1; i >= 0; i--)
	{
		if (colors[i] == 0)
		{
			pair<vector<int>, vector<int>> segment;
			BFS(g, i, collectVertex, segment, colors);
			if (segment.second.size() >= 2)
			{
				segments.push_back(segment);
			}
			else
			{
				fewContactsSegments.push_back(segment.first);
			}
		}
	}

	for (int i = g.size() - 1; i >= 0; i--)
	{
		colors[i] = 2;
	}
	for (int i = subGraph.size() - 1; i >= 0; i--)
	{
		colors[subGraph[i]] = 0;
	}
	for (int i = chain.size() - 1; i >= 0; i--)
	{
		colors[chain[i]] = 2;
	}

	for (int i = chain.size() - 2; i > 0; i--)
	{
		const vector<int>& next = g.getNeighbors(chain[i]);
		for (int j = next.size() - 1; j >= 0; j--)
		{
			if (colors[next[j]] == 2 && chain[i + 1] != next[j] && chain[i - 1] != next[j])
			{
				segments.push_back(pair<vector<int>, vector<int>>(0, 0));
				segments[segments.size() - 1].second.push_back(chain[i]);
				segments[segments.size() - 1].second.push_back(next[j]);
			}
		}
		colors[chain[i]] = 2;
	}
	delete[] colors;
	for (int i = 0; i < segments.size(); i++)
	{
		std::cout << "SEGMENT " << i << "\n";
		for (int j = 0; j < segments[i].first.size(); j++)
		{
			std::cout << segments[i].first[j] << " ";
		}
		std::cout << "\n";
		std::cout << "CONTACT " << "\n";
		for (int j = 0; j < segments[i].second.size(); j++)
		{
			std::cout << segments[i].second[j] << " ";
		}
		std::cout << "\n\n";
	}

	for (int i = 0; i < fewContactsSegments.size(); i++)
	{
		std::cout << "FEW CONTACTS SEGMENT " << i << "\n";
		for (int j = 0; j < fewContactsSegments[i].size(); j++)
		{
			std::cout << fewContactsSegments[i][j] << " ";
		}
		std::cout << "\n";
	}
	return segments;
}

vector<int> getChain(const graph& g, const vector<int>& segment, const vector<int>& contactVertexes)
{
	bool hasNotContacts = (segment.size() != 0);
	vector<int> chain;
	int* colors = new int[g.size()];
	for (int i = g.size() - 1; i >= 0; i--)
	{
		colors[i] = 1;
	}
	for (int i = segment.size() - 1; i >= 0; i--)
	{
		colors[segment[i]] = 0;
	}
	for (int i = contactVertexes.size() - 1; i > 0; i--)
	{
		colors[contactVertexes[i]] = -1;
	}
	colors[contactVertexes[0]] = -2;
	stack<int> toVisit;
	stack<int> currentWay;
	currentWay.push(-1);
	toVisit.push(contactVertexes[0]);
	while (toVisit.size() != 0)
	{
		int v = toVisit.top();
		toVisit.pop();
		if (colors[v] == -1)
		{
			chain.push_back(v);
			while (currentWay.top() != -1)
			{
				chain.push_back(currentWay.top());
				currentWay.pop();
			}
			delete[] colors;

			std::cout << "\nCHAIN:\n";
			for (int i = 0; i < chain.size(); i++)
			{
				std::cout << chain[i] << " ";
			}
			std::cout << "\n";

			return chain;
		}
		if (colors[v] == 1)
		{
			currentWay.pop();
			colors[v] = 2;
		}
		if (colors[v] <= 0)
		{
			toVisit.push(v);
			vector<int> next = g.getNeighbors(v);
			for (int j = next.size() - 1; j >= 0; j--)
			{
				if (colors[next[j]] == 0 || (colors[next[j]] < 0 && (colors[v] != -2 || !hasNotContacts)))
				{
					toVisit.push(next[j]);
				}
			}
			colors[v] = 1;
			currentWay.push(v);
		}
	}
}

bool isPlanary(const graph& g, const vector<int>& subGraph)
{
	vector<int> circle;
	if (!getCircle(g, subGraph, circle))
	{
		return true;
	}
	planaryGraph pG(circle, g.size());
	vector<vector<int>> fewContactSegments;
	vector<pair<vector<int>, vector<int>>> segments = getSegments(g, subGraph, circle, fewContactSegments);
	for (int i = fewContactSegments.size() - 1; i >= 0; i--)
	{
		if (!isPlanary(g, fewContactSegments[i]))
		{
			return false;
		}
	}

	vector<vector<int>> segmentsNesting(segments.size());
	for (int i = segments.size() - 1; i >= 0; i--)
	{
		segmentsNesting[i].push_back(0);
		segmentsNesting[i].push_back(1);
	}
	vector<vector<int>> faces(2);
	for (int i = segments.size() - 1; i >= 0; i--)
	{
		faces[0].push_back(i);
		faces[1].push_back(i);
	}
	int unplacedSegments = segments.size();
	int leastNesting = 0;
	while (unplacedSegments != 0)
	{
		for (int i = segments.size() - 1; i >= 0; i--)
		{
			if (segmentsNesting[i][0] >= 0)
			{
				if (segmentsNesting[i].size() == 1)
				{
					leastNesting = i;
					break;
				}
				if (segmentsNesting[leastNesting][0] == -1 || segmentsNesting[i].size() < segmentsNesting[leastNesting].size())
				{
					leastNesting = i;
				}
			}
		}

	//	std::cout << leastNesting << "LN";

		vector<int> chain = getChain(g, segments[leastNesting].first, segments[leastNesting].second);
		pG.addLine(chain, segmentsNesting[leastNesting][0]);
		vector<pair<vector<int>, vector<int>>> newSegments = getSegments(g, segments[leastNesting].first, chain, fewContactSegments);
		for (int i = fewContactSegments.size() - 1; i >= 0; i--)
		{
			if (!isPlanary(g, fewContactSegments[i]))
			{
				return false;
			}
		}
		unplacedSegments += (newSegments.size() - 1);
		vector<int> newSegmentsInFace;
		faces.push_back(vector<int>(0));
		int newFace = segmentsNesting[leastNesting][0];
		std::cout << "REAL FACES IN MID:\n";
		for (int i = 0; i < pG.faces.size(); i++)
		{
			std::cout << "FACE " << i << "\n";
			for (int j = 0; j < pG.faces[i].circle.size(); j++)
			{
				std::cout << pG.faces[i].circle[j] << " ";
			}
			std::cout << "\n";
		}

		for (int i = faces[newFace].size() - 1; i >= 0; i--)
		{
			int thisSegment = faces[newFace][i];
			if (segmentsNesting[thisSegment][0] != -1)
			{
				if (thisSegment != leastNesting)
				{
					if (!pG.hasVertexes(newFace, segments[thisSegment].second))
					{
						for (int j = segmentsNesting[thisSegment].size() - 1; j >= 0; j--)
						{
							if (segmentsNesting[thisSegment][j] == newFace)
							{
								std::swap(segmentsNesting[thisSegment][j], segmentsNesting[thisSegment][segmentsNesting[thisSegment].size() - 1]);
								segmentsNesting[thisSegment].erase(segmentsNesting[thisSegment].end() - 1);
								break;
							}
						}
					}
					else
					{
						newSegmentsInFace.push_back(thisSegment);
					}
					if (pG.hasVertexes(faces.size() - 1, segments[thisSegment].second))
					{
						faces[faces.size() - 1].push_back(thisSegment);
						segmentsNesting[thisSegment].push_back(faces.size() - 1);
					}
					if (segmentsNesting[thisSegment].size() == 0)
					{
						return false;
					}
				}
			}
		}
		faces[newFace] = newSegmentsInFace;

		for (int i = newSegments.size() - 1; i >= 0; i--)
		{
			segmentsNesting.push_back(vector<int>(0));
			if (pG.hasVertexes(faces.size() - 1, newSegments[i].second))
			{
				segmentsNesting[segmentsNesting.size() - 1].push_back(faces.size() - 1);
				faces[faces.size() - 1].push_back(segmentsNesting.size() - 1);
			}
			if (pG.hasVertexes(newFace, newSegments[i].second))
			{
				segmentsNesting[segmentsNesting.size() - 1].push_back(newFace);
				faces[newFace].push_back(segmentsNesting.size() - 1);
			}
			if (segmentsNesting[segmentsNesting.size() - 1].size() == 0)
			{
				return false;
			}
			segments.push_back(newSegments[i]);
		}
		segmentsNesting[leastNesting].clear();
		segmentsNesting[leastNesting].push_back(-1);
		leastNesting = 0;
		std::cout << "\nFACES AFTER:\n";
		for (int i = 0; i < faces.size(); i++)
		{
			for (int j = 0; j < faces[i].size(); j++)
			{
				std::cout << faces[i][j] << " ";
			}
			std::cout << "\n";
		}

		std::cout << "SEGMENT NESTING AFTER:\n";
		for (int i = 0; i < segmentsNesting.size(); i++)
		{
			std::cout << "SEGMENT " << i << "\n";
			for (int j = 0; j < segmentsNesting[i].size(); j++)
			{
				std::cout << segmentsNesting[i][j] << " ";
			}
			std::cout << "\n";
		}

		std::cout << "REAL FACES AFTER:\n";
		for (int i = 0; i < pG.faces.size(); i++)
		{
			std::cout << "FACE " << i << "\n";
			for (int j = 0; j < pG.faces[i].circle.size(); j++)
			{
				std::cout << pG.faces[i].circle[j] << " ";
			}
			std::cout << "\n";
		}

		std::cout << "ALL SEGMENTS:\n";
		for (int i = 0; i < segments.size(); i++)
		{
			if (segmentsNesting[i][0] != -1)
			{
				std::cout << "SEGMENT " << i << "\n";
				for (int j = 0; j < segments[i].first.size(); j++)
				{
					std::cout << segments[i].first[j] << " ";
				}
				std::cout << "\n";
				std::cout << "CONTACT " << "\n";
				for (int j = 0; j < segments[i].second.size(); j++)
				{
					std::cout << segments[i].second[j] << " ";
				}
				std::cout << "\n\n";
			}
		}

	}
	return true;
}

bool isPlanary(const graph& g)
{
	vector<int> subGraph;
	for (int i = g.size() - 1; i >= 0; i--)
	{
		subGraph.push_back(i);
	}
	return isPlanary(g, subGraph);
}

int main()
{
	int n;
	int m;
	std::cin >> n >> m;
	graph g(n);
	bool** edges = new bool* [n];
	for (int i = 0; i < n; i++)
	{
		edges[i] = new bool[n];
		for (int j = i + 1; j < n; j++)
		{
			edges[i][j] = false;
		}
	}
	for (int i = 0; i < m; i++)
	{
		int fVertex;
		int sVertex;
		std::cin >> fVertex >> sVertex;
		edges[fVertex][sVertex] = true;
		edges[sVertex][fVertex] = true;
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = i + 1; j < n; j++)
		{
			if (edges[i][j])
			{
				g.addEdge(i, j);
				g.addEdge(j, i);
			}
		}
		delete[] edges[i];
	}
	delete[] edges;
	if (isPlanary(g))
	{
		std::cout << "YES";
	}
	else
	{
		std::cout << "NO";
	}
}