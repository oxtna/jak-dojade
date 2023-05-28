#pragma once
#include "Utility.hxx"
#include <cstddef>

template <typename T, std::size_t S>
class Array
{
public:
	using Iterator = T*;
	using ConstIterator = const T*;

	Array() = default;

	Array(const Array& rhs)
		: _data()
	{
		for (std::size_t i = 0; i < S; i++)
		{
			_data[i] = rhs._data[i];
		}
	}

	Array(Array&& rhs)
		: _data()
	{
		for (std::size_t i = 0; i < S; i++)
		{
			_data[i] = Move(rhs._data[i]);
		}
	}

	~Array() = default;

	constexpr std::size_t Size() const
	{
		return S;
	}

	const T* Data() const
	{
		return _data;
	}

	T* Data()
	{
		return _data;
	}

	void Fill(const T& value)
	{
		for (std::size_t i = 0; i < S; i++)
		{
			_data[i] = value;
		}
	}

	void Swap(Array& rhs)
	{
		for (std::size_t i = 0; i < S; i++)
		{
			Swap(_data[i], rhs._data[i]);
		}
	}

	Iterator begin()
	{
		return Iterator(_data);
	}

	ConstIterator begin() const
	{
		return ConstIterator(_data);
	}

	Iterator end()
	{
		return Iterator(_data + S);
	}

	ConstIterator end() const
	{
		return ConstIterator(_data + S);
	}

	const T& operator[](std::size_t index) const
	{
		if (index >= S)
		{
			throw;
		}

		return _data[index];
	}

	T& operator[](std::size_t index)
	{
		if (index >= S)
		{
			throw;
		}

		return _data[index];
	}

	Array& operator=(const Array& rhs)
	{
		for (std::size_t i = 0; i < S; i++)
		{
			_data[i] = rhs._data[i];
		}

		return *this;
	}

	Array& operator=(Array&& rhs)
	{
		Swap(rhs);
		return *this;
	}

private:
	T _data[S];
};
