#include <iostream>
#include <vector>
#include <algorithm>
using std::vector;
using std::pair;

class sparseTable
{
public:
	sparseTable(const vector<int>& arr)
	{
		table.push_back(vector<pair<int, size_t>>());
		for (size_t i = 0; i < arr.size(); i++)
			table[0].push_back(std::make_pair(arr[i], i));
		size_t twoPowered = 2;
		while (table[table.size() - 1].size() >= twoPowered / 2)
		{
			table.push_back(vector<pair<int, size_t>>());
			size_t prev = table.size() - 2;
			for (size_t i = 0; i + twoPowered / 2 < table[prev].size(); i++)
			{
				table[prev + 1].push_back(std::min(table[prev][i], table[prev][i + twoPowered / 2]));
			}
			twoPowered = (twoPowered << 1);
		}
		size_t power = 0;
		twoPowered = 1;
		for (size_t i = 0; i < arr.size(); i++)
		{
			if (twoPowered * 2 <= i + 1)
			{
				twoPowered *= 2;
				++power;
			}
			log.push_back(std::make_pair(power, twoPowered));
		}
	}
	int RMQ(size_t i, size_t j)
	{
		auto power = log[j - i];
		return std::min(table[power.first][i].first, table[power.first][j + 1 - power.second].first);
	}
	int RMQSecond(size_t i, size_t j)
	{
		auto power = log[j - i];
		size_t index = std::min(table[power.first][i], table[power.first][j + 1 - power.second]).second;
		if (index == i)
			return RMQ(i + 1, j);
		else if (index == j)
			return RMQ(i, j - 1);
		else
			return std::min(RMQ(i, index - 1), RMQ(index + 1, j));
	}
private:
	vector<vector<pair<int, size_t>>> table;
	vector<pair<size_t, size_t>> log;
};

int main()
{
	int N;
	int M;
	std::cin >> N >> M;
	std::vector<int> arr(N);
	for (int i = 0; i < N; i++)
		std::cin >> arr[i];
	sparseTable ST(arr);
	for (int i = 0; i < M; i++)
	{
		int a;
		int b;
		std::cin >> a >> b;
		std::cout << ST.RMQSecond(a - 1, b - 1) << "\n";
	}
}

