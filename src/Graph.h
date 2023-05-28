#pragma once
#include "List.hxx"
#include "Pair.hxx"
#include "String.h"

class Graph
{
private:
	class Node
	{
	public:
		Node() = default;
		Node(const Node& rhs);
		Node(Node&& rhs) noexcept;
		explicit Node(const String& name);
		explicit Node(String&& name);
		~Node() = default;
		const String& GetName() const;
		Node& operator=(const Node& rhs);
		Node& operator=(Node&& rhs) noexcept;

	private:
		String _name;
	};

public:
	Graph() = default;
	explicit Graph(const List<String>& data);
	explicit Graph(List<String>&& data);
	~Graph() = default;
	void AddEdge(std::size_t start, std::size_t end, int weight);
	Pair<int, List<String>> FindPath(std::size_t start, std::size_t end);

private:
	using Edge = Pair<std::size_t, int>;
	List<List<Edge>> _edges;
	List<Node> _nodes;
};
