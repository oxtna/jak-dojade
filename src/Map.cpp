#include "Map.h"
#include "Utility.hxx"
#include "Char.h"

Map::Map(const List<char>& data, std::size_t width)
	: _data(data), _width(width)
{
}

Map::Map(List<char>&& data, std::size_t width)
	: _data(Move(data)), _width(width)
{
}

std::size_t Map::GetWidth() const
{
	return _width;
}

std::size_t Map::GetHeight() const
{
	return _data.Size() / _width;
}

char Map::GetChar(std::size_t x, std::size_t y) const
{
	return _data[y * _width + x];
}

std::size_t Map::CountAdjacentTraversables(std::size_t x, std::size_t y) const
{
	std::size_t adjacent = 0;
	if (x != 0)
	{
		if (_data[y * _width + x - 1] == '#' || _data[y * _width + x - 1] == '*')
		{
			adjacent += 1;
		}
	}
	if (x != _width - 1)
	{
		if (_data[y * _width + x + 1] == '#' || _data[y * _width + x + 1] == '*')
		{
			adjacent += 1;
		}
	}
	if (y != 0)
	{
		if (_data[(y - 1) * _width + x] == '#' || _data[(y - 1) * _width + x] == '*')
		{
			adjacent += 1;
		}
	}
	if (y != (_data.Size() / _width) - 1)
	{
		if (_data[(y + 1) * _width + x] == '#' || _data[(y + 1) * _width + x] == '*')
		{
			adjacent += 1;
		}
	}

	return adjacent;
}

List<Map::Position> Map::GetAdjacentTraversables(std::size_t x, std::size_t y) const
{
	List<Position> traversables;
	if (x != 0 && (_data[y * _width + x - 1] == '#' || _data[y * _width + x - 1] == '*'))
	{
		traversables.Add({ x - 1, y });
	}
	if (x != _width - 1 && (_data[y * _width + x + 1] == '#' || _data[y * _width + x + 1] == '*'))
	{
		traversables.Add({ x + 1, y });
	}
	if (y != 0 && (_data[(y - 1) * _width + x] == '#' || _data[(y - 1) * _width + x] == '*'))
	{
		traversables.Add({ x, y - 1 });
	}
	if (y != (_data.Size() / _width) - 1 && (_data[(y + 1) * _width + x] == '#' || _data[(y + 1) * _width + x] == '*'))
	{
		traversables.Add({ x, y + 1 });
	}

	return traversables;
}

void Map::GetAdjacentTraversables(std::size_t x, std::size_t y, List<Map::Position>& list) const
{
	if (x != 0 && (_data[y * _width + x - 1] == '#' || _data[y * _width + x - 1] == '*'))
	{
		list.Add({ x - 1, y });
	}
	if (x != _width - 1 && (_data[y * _width + x + 1] == '#' || _data[y * _width + x + 1] == '*'))
	{
		list.Add({ x + 1, y });
	}
	if (y != 0 && (_data[(y - 1) * _width + x] == '#' || _data[(y - 1) * _width + x] == '*'))
	{
		list.Add({ x, y - 1 });
	}
	if (y != (_data.Size() / _width) - 1 && (_data[(y + 1) * _width + x] == '#' || _data[(y + 1) * _width + x] == '*'))
	{
		list.Add({ x, y + 1 });
	}
}

bool Map::IsBetweenLetters(std::size_t x, std::size_t y) const
{
	if (x == 0 || x == _width - 1)
	{
		return false;
	}

	return Char::IsAlnum(_data[y * _width + x - 1]) && Char::IsAlnum(_data[y * _width + x + 1]);
}

bool Map::IsAdjacentToCity(std::size_t x, std::size_t y) const
{
	if (x >= _width || y >= _data.Size() / _width)
	{
		throw;
	}

	return (
		(x != 0 && y != 0 && _data[(y - 1) * _width + x - 1] == '*') ||
		(x != 0 && y != (_data.Size() / _width) - 1 && _data[(y + 1) * _width + x - 1] == '*') ||
		(x != _width - 1 && y != 0 && _data[(y - 1) * _width + x + 1] == '*') ||
		(x != _width - 1 && y != (_data.Size() / _width) - 1 && _data[(y + 1) * _width + x + 1] == '*') ||
		(x != 0 && _data[y * _width + x - 1] == '*') ||
		(x != _width - 1 && _data[y * _width + x + 1] == '*') ||
		(y != 0 && _data[(y - 1) * _width + x] == '*') ||
		(y != (_data.Size() / _width) - 1 && _data[(y + 1) * _width + x] == '*')
		);
}

Map::Position Map::GetAdjacentCity(std::size_t x, std::size_t y) const
{
	if (x >= _width || y >= _data.Size() / _width)
	{
		throw;
	}

	if (x != 0 && y != 0 && _data[(y - 1) * _width + x - 1] == '*')
	{
		return { x - 1, y - 1 };
	}
	else if (x != 0 && y != (_data.Size() / _width) - 1 && _data[(y + 1) * _width + x - 1] == '*')
	{
		return { x - 1, y + 1 };
	}
	else if (x != _width - 1 && y != 0 && _data[(y - 1) * _width + x + 1] == '*')
	{
		return { x + 1, y - 1 };
	}
	else if (x != _width - 1 && y != (_data.Size() / _width) - 1 && _data[(y + 1) * _width + x + 1] == '*')
	{
		return { x + 1, y + 1 };
	}
	else if (x != 0 && _data[y * _width + x - 1] == '*')
	{
		return { x - 1, y };
	}
	else if (x != _width - 1 && _data[y * _width + x + 1] == '*')
	{
		return { x + 1, y };
	}
	else if (y != 0 && _data[(y - 1) * _width + x] == '*')
	{
		return { x, y - 1 };
	}
	else if (y != (_data.Size() / _width) - 1 && _data[(y + 1) * _width + x] == '*')
	{
		return { x, y + 1 };
	}
	else
	{
		throw;
	}
}

String Map::GetName(std::size_t x, std::size_t y) const
{
	if (!(Char::IsAlnum(_data[y * _width + x])))
	{
		return String();
	}

	while (x > 0 && Char::IsAlnum(_data[y * _width + x - 1]))
	{
		x--;
	}
	std::size_t startX = x;
	while (x < _width && Char::IsAlnum(_data[y * _width + x]))
	{
		x++;
	}
	std::size_t endX = x;
	// this code makes 2 copies of the name instead of 1
	// can be optimized if required
	UniquePtr<char[]> buffer(new char[endX - startX + 1] {});
	for (std::size_t i = 0; i < endX - startX; i++)
	{
		buffer[i] = _data[y * _width + startX + i];
	}
	return String(buffer.GetPtr());
}

void Map::ClearName(std::size_t x, std::size_t y)
{
	if (!(Char::IsAlnum(_data[y * _width + x])))
	{
		return;
	}

	while (x > 0 && Char::IsAlnum(_data[y * _width + x - 1]))
	{
		x--;
	}
	while (x < _width && Char::IsAlnum(_data[y * _width + x]))
	{
		_data[y * _width + x] = '.';
		x++;
	}
}

bool operator==(const Map::Position& lhs, const Map::Position& rhs)
{
	return lhs.first == rhs.first && lhs.second == rhs.second;
}

bool operator!=(const Map::Position& lhs, const Map::Position& rhs)
{
	return !(lhs == rhs);
}
