#pragma once
#include "List.hxx"
#include "Pair.hxx"
#include "String.h"

// Map should always be a rectangle
// Width is located on the X axis
class Map
{
public:
	using Position = Pair<std::size_t, std::size_t>;

	Map(const List<char>& data, std::size_t width);
	Map(List<char>&& data, std::size_t width);
	~Map() = default;
	std::size_t GetWidth() const;
	std::size_t GetHeight() const;
	char GetChar(std::size_t x, std::size_t y) const;
	std::size_t CountAdjacentTraversables(std::size_t x, std::size_t y) const;
	List<Position> GetAdjacentTraversables(std::size_t x, std::size_t y) const;
	void GetAdjacentTraversables(std::size_t x, std::size_t y, List<Map::Position>& list) const;
	bool IsBetweenLetters(std::size_t x, std::size_t y) const;
	bool IsAdjacentToCity(std::size_t x, std::size_t y) const;
	Position GetAdjacentCity(std::size_t x, std::size_t y) const;
	String GetName(std::size_t x, std::size_t y) const;
	void ClearName(std::size_t x, std::size_t y);


private:
	List<char> _data;
	std::size_t _width;
};

bool operator==(const Map::Position& lhs, const Map::Position& rhs);
bool operator!=(const Map::Position& lhs, const Map::Position& rhs);
