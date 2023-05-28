#include "List.hxx"
#include "Pair.hxx"
#include "PriorityQueue.hxx"
#include "Dictionary.hxx"
#include "Map.h"
#include "Graph.h"
#include "Char.h"
#include <iostream>
#include <cstdio>

int main()
{
	std::size_t width = 0;
	std::size_t height = 0;
	std::cin >> width;
	std::cin >> height;

	if (width == 0 || height == 0)
	{
		std::cin.clear();
		return 0;
	}

	List<char> input(width * height);

	{
		// discard line feed
		int c = std::getchar();
		for (std::size_t i = 0; i < width * height; i++)
		{
			c = std::getchar();
			if (c != '\n')
			{
				input[i] = c;
			}
			else
			{
				i--;
			}
		}
	}

	Map map(Move(input), width);
	List<String> names;
	List<Map::Position> positions;
	Dictionary<std::size_t> cityIndices;

	bool seenRoad = false;
	{
		for (std::size_t y = 0; y < map.GetHeight(); y++)
		{
			for (std::size_t x = 0; x < map.GetWidth(); x++)
			{
				if (map.GetChar(x, y) == '#')
				{
					seenRoad = true;
				}
				if (!(Char::IsAlnum(map.GetChar(x, y))))
				{
					continue;
				}
				if (map.IsBetweenLetters(x, y))
				{
					continue;
				}
				if (!(map.IsAdjacentToCity(x, y)))
				{
					continue;
				}

				auto cityPosition = map.GetAdjacentCity(x, y);
				auto cityName = map.GetName(x, y);
				map.ClearName(x, y);
				cityIndices.Add(cityName, names.Size());
				names.Add(Move(cityName));
				positions.Add(cityPosition);
			}
		}
	}

	Graph graph(names);

	if (seenRoad)
	{
		List<Map::Position> adjacentList(4);
		List<int> weights(width * height);
		List<bool> seen(width * height);
		auto less = [](const Pair<Map::Position, int>& lhs, const Pair<Map::Position, int>& rhs) -> bool
		{
			return lhs.second < rhs.second;
		};
		for (std::size_t start = 0; start < positions.Size(); start++)
		{
			adjacentList.Zero();
			weights.Zero();
			seen.Zero();
			Map::Position position = positions[start];
			PriorityQueue<Pair<Map::Position, int>, decltype(less)> paths(less);
			paths.Add({ position, 0 });
			while (!paths.Empty())
			{
				adjacentList.Reset();
				auto currentPosition = paths.Peek().first;
				int weight = paths.Peek().second;
				paths.Remove();
				seen[currentPosition.second * width + currentPosition.first] = true;
				weights[currentPosition.second * width + currentPosition.first] = weight;
				map.GetAdjacentTraversables(currentPosition.first, currentPosition.second, adjacentList);
				for (auto& adjacent : adjacentList)
				{

					if (seen[adjacent.second * width + adjacent.first])
					{
						continue;
					}
					if (map.GetChar(adjacent.first, adjacent.second) == '#')
					{
						seen[adjacent.second * width + adjacent.first] = true;
						paths.Add({ adjacent, weight + 1 });
					}
					else if (map.GetChar(adjacent.first, adjacent.second) == '*')
					{
						seen[adjacent.second * width + adjacent.first] = true;
						weights[adjacent.second * width + adjacent.first] = weight + 1;
						for (std::size_t end = 0; end < positions.Size(); end++)
						{
							if (positions[end] == adjacent)
							{
								graph.AddEdge(start, end, weight + 1);
								graph.AddEdge(end, start, weight + 1);
								break;
							}
						}
					}
				}
			}
		}
	}

	{
		std::size_t flights = 0;
		std::cin >> flights;
		String beginning, destination;
		std::size_t start, end;
		int time = -1;
		for (std::size_t _ = 0; _ < flights; _++)
		{
			beginning = String::GetFromStdin();
			destination = String::GetFromStdin();
			std::cin >> time;
			start = cityIndices.Get(beginning);
			end = cityIndices.Get(destination);
			graph.AddEdge(start, end, time);
		}
	}

	{
		std::size_t queries = 0;
		std::cin >> queries;
		String beginning, destination;
		std::size_t start, end;
		int type = 0;
		for (std::size_t _ = 0; _ < queries; _++)
		{
			beginning = String::GetFromStdin();
			destination = String::GetFromStdin();
			std::cin >> type;
			start = cityIndices.Get(beginning);
			end = cityIndices.Get(destination);
			auto path = graph.FindPath(start, end);
			std::cout << path.first;
			if (type == 1)
			{
				// city names in between are reversed
				for (std::size_t i = 0; i < path.second.Size(); i++)
				{
					auto& cityName = path.second[path.second.Size() - i - 1];
					std::cout << " " << cityName;
				}
			}
			std::cout << std::endl;
		}
	}

	return 0;
}
