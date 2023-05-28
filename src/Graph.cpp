#include "Graph.h"
#include "Utility.hxx"
#include "Pair.hxx"
#include "Triple.hxx"
#include "PriorityQueue.hxx"
#include <limits>

Graph::Graph(const List<String>& data)
	: _edges(data.Size()), _nodes(data.Size())
{
	for (std::size_t i = 0; i < _nodes.Size(); i++)
	{
		_nodes[i] = Node(data[i]);
	}
}

Graph::Graph(List<String>&& data)
	: _edges(data.Size()), _nodes(data.Size())
{
	for (std::size_t i = 0; i < _nodes.Size(); i++)
	{
		_nodes[i] = Node(Move(data[i]));
	}
}

void Graph::AddEdge(std::size_t start, std::size_t end, int weight)
{
	// TODO: _edges[start] can be ADT that has O(log n) insertion and lookup instead
	bool found = false;
	for (auto& edge : _edges[start])
	{
		if (edge.first == end)
		{
			found = true;
			if (edge.second > weight)
			{
				edge.second = weight;
			}
		}
	}
	if (!found)
	{
		_edges[start].Add({ end, weight });
	}
}

Pair<int, List<String>> Graph::FindPath(std::size_t start, std::size_t end)
{
	if (start == end)
	{
		return { 0, List<String>() };
	}

	auto less = [](const Pair<int, std::size_t>& lhs, const Pair<int, std::size_t>& rhs) -> bool
	{
		return lhs.first < rhs.first || (lhs.first == rhs.first && lhs.second < rhs.second);
	};
	PriorityQueue<Pair<int, std::size_t>, decltype(less)> queue(less);
	List<int> distances(_nodes.Size());
	List<std::size_t> paths(_nodes.Size());
	for (auto& distance : distances)
	{
		distance = std::numeric_limits<int>::max();
	}
	distances[start] = 0;
	paths[start] = start;
	queue.Add({ 0, start });

	// TODO: make each node have an index of previous
	//       so that the path from start to end can
	//       be reconstructed going backwards
	while (!(queue.Empty()))
	{
		int currentDistance = queue.Peek().first;
		std::size_t currentIndex = queue.Peek().second;
		queue.Remove();
		auto& edges = _edges[currentIndex];
		for (std::size_t i = 0; i < edges.Size(); i++)
		{
			if (currentDistance + edges[i].second < distances[edges[i].first])
			{
				paths[edges[i].first] = currentIndex;
				distances[edges[i].first] = currentDistance + edges[i].second;
				queue.Add({ distances[edges[i].first], edges[i].first });
			}
		}
	}
		
	if (distances[end] == std::numeric_limits<int>::max())
	{
		throw std::logic_error("No path between start and end has been found");
	}

	List<String> between;
	std::size_t current = paths[end];
	while (current != start)
	{
		between.Add(_nodes[current].GetName());
		current = paths[current];
	}
	return { Move(distances[end]), Move(between)};
}

Graph::Node::Node(const Node& rhs)
	: _name(rhs._name) {}

Graph::Node::Node(Node&& rhs) noexcept
	: _name(Move(rhs._name)) {}

Graph::Node::Node(const String& name)
	: _name(name) {}

Graph::Node::Node(String&& name)
	: _name(Move(name)) {}

const String& Graph::Node::GetName() const
{
	return _name;
}

Graph::Node& Graph::Node::operator=(const Node& rhs)
{
	Node tmp = rhs;
	Swap(_name, tmp._name);
	return *this;
}

Graph::Node& Graph::Node::operator=(Node&& rhs) noexcept
{
	Swap(_name, rhs._name);
	return *this;
}
