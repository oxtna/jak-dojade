#include "String.h"
#include "Utility.hxx"
#include "Char.h"
#include <stdexcept>
#include <cstdio>

String::String()
	: _length(0),
	_data(nullptr)
{
}

String::String(const char* str)
	: _length(_GetStringLiteralLength(str)),
	_data(_length == 0 ? nullptr : new char[_length + 1] {})
{
	if (_length > 0)
	{
		_CopyStringData(str, _length);
	}
}

String::String(const char* str, std::size_t maxLength)
	: _length(_GetStringLiteralLength(str) > maxLength ? maxLength : _GetStringLiteralLength(str)),
	_data(_length == 0 ? nullptr : new char[_length + 1] {})
{
	if (_length > 0)
	{
		_CopyStringData(str, _length);
	}
}

String::String(const String& rhs)
	: _length(rhs._length),
	_data(_length == 0 ? nullptr : new char[rhs._length + 1] {})
{
	if (_length > 0)
	{
		_CopyStringData(rhs._data, _length);
	}
}

String::String(String&& rhs) noexcept
	: _length(rhs._length),
	_data(rhs._data)
{
	rhs._length = 0;
	rhs._data = nullptr;
}

String::~String()
{
	delete[] _data;
}

std::size_t String::GetLength() const
{
	return _length;
}

const char* String::GetData() const
{
	return _data;
}

String String::GetSubstring(std::size_t start, std::size_t end) const
{
	if (end <= start || start >= _length)
	{
		return String();
	}

	return String(_data + start, end - start);
}

String& String::operator=(const String& rhs)
{
	String tmp = rhs;
	Swap(_length, tmp._length);
	Swap(_data, tmp._data);
	return *this;
}

String& String::operator=(String&& rhs) noexcept
{
	Swap(_length, rhs._length);
	Swap(_data, rhs._data);
	return *this;
}

bool String::operator==(const String& rhs) const
{
	if (_data == nullptr || rhs._data == nullptr)
	{
		return _data == rhs._data;
	}
	if (_length != rhs._length)
	{
		return false;
	}

	const char* thisStr = _data;
	const char* rhsStr = rhs._data;
	while (*thisStr && *rhsStr && *thisStr == *rhsStr)
	{
		thisStr++;
		rhsStr++;
	}

	return *thisStr == *rhsStr;
}

bool String::operator!=(const String& rhs) const
{
	return !(*this == rhs);
}

String String::operator+(const String& rhs) const
{
	String concat;
	concat._length = _length + rhs._length;
	concat._data = new char[concat._length + 1] {};
	concat._CopyStringData(_data, _length);
	concat._CopyStringData(rhs._data, rhs._length, _length);

	return concat;
}

char String::operator[](const std::size_t index) const
{
	if (index >= _length)
	{
		throw std::out_of_range("Index is out of range");
	}

	return _data[index];
}

String String::GetFromStdin()
{
	constexpr std::size_t BUFFER_SIZE = 4096;
	char buffer[BUFFER_SIZE] = {};
	int c = std::getchar();
	while (Char::IsWhitespace(static_cast<char>(c)))
	{
		c = std::getchar();
	}
	if (c == '\0' || c == EOF)
	{
		return String();
	}
	buffer[0] = static_cast<char>(c);
	std::size_t i = 1;
	while (true)
	{
		if (i == BUFFER_SIZE - 1)
		{
			throw std::runtime_error("Out of buffer space");
		}
		
		c = std::getchar();
		if (Char::IsWhitespace(static_cast<char>(c)) || c == '\0')
		{
			break;
		}
		buffer[i] = static_cast<char>(c);
		i++;
	}
	return String(buffer, BUFFER_SIZE - 1);
}

void String::_CopyStringData(const char* str, std::size_t len, std::size_t offset)
{
	std::size_t i = 0;
	while (i < len && str[i])
	{
		_data[i + offset] = str[i];
		i++;
	}
	_data[i + offset] = '\0';
}

std::size_t String::_GetStringLiteralLength(const char* str)
{
	std::size_t length = 0;
	while (*str)
	{
		length++;
		str++;
	}

	return length;
}

std::ostream& operator<<(std::ostream& os, const String& s)
{
	return os << (s._data == nullptr ? "" : s._data);
}

std::istream& operator>>(std::istream& is, String& s)
{
	constexpr std::size_t BUFFER_SIZE = 4096;
	char buffer[BUFFER_SIZE] = {};
	char c = '\0';
	is >> c;
	buffer[0] = c;
	is >> std::noskipws;
	std::size_t i = 1;
	while (true)
	{
		if (i == BUFFER_SIZE - 1)
		{
			throw std::runtime_error("Out of buffer space");
		}
		
		is >> c;
		if (Char::IsWhitespace(c) || c == '\0')
		{
			break;
		}
		buffer[i] = c;
		i++;
	}
	s = String(buffer, BUFFER_SIZE - 1);
	is >> std::skipws;
	return is;
}
