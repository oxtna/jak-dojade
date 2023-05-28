#pragma once
#include "Utility.hxx"
#include <cstddef>
#include <cstring>
#include <stdexcept>

template <typename T>
class List
{
public:
	using Iterator = T*;
	using ConstIterator = const T*;

	List()
		: _size(0), _capacity(1), _data(new T[_capacity]{})
	{
	}

	explicit List(std::size_t size)
		: _size(size),
		_capacity(size),
		_data(nullptr)
	{
		if (_capacity > 0)
		{
			_data = new T[_capacity]{};
		}
	}

	List(const List& rhs)
		: _size(rhs._size),
		_capacity(rhs._capacity),
		_data(nullptr)
	{
		if (_size > _capacity)
		{
			throw std::logic_error("Invalid List state: size cannot be bigger than capacity");
		}

		if (_capacity > 0)
		{
			_data = new T[_capacity]{};
		}
		else
		{
			return;
		}
		
		for (std::size_t i = 0; i < _size; i++)
		{
			_data[i] = rhs._data[i];
		}
	}

	List(List&& rhs) noexcept
		: _size(rhs._size),
		_capacity(rhs._capacity),
		_data(rhs._data)
	{
		rhs._size = 0;
		rhs._capacity = 0;
		rhs._data = nullptr;
	}

	~List()
	{
		delete[] _data;
	}

	std::size_t Size() const
	{
		return _size;
	}

	std::size_t Capacity() const
	{
		return _capacity;
	}

	const T* Data() const
	{
		return _data;
	}

	T* Data()
	{
		return _data;
	}

	void Add(const T& item)
	{
		if (_size == _capacity)
		{
			Reserve(_capacity * _RESIZE_FACTOR);
		}

		_data[_size] = item;
		_size++;
	}

	void Add(T&& item)
	{
		if (_size == _capacity)
		{
			Reserve(_capacity * _RESIZE_FACTOR);
		}

		_data[_size] = Move(item);
		_size++;
	}

	T Pop()
	{
		if (_size == 0)
		{
			throw std::logic_error("Empty list cannot be popped");
		}

		_size--;
		return T(Move(_data[_size]));
	}

	void Remove(std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is out of range");
		}

		_size--;
		for (std::size_t i = index; i < _size; i++)
		{
			_data[i] = Move(_data[i + 1]);
		}
	}
	
	void Reset()
	{
		_size = 0;
	}

	// do not use this if T is not a scalar type
	// this method does not call any constructors
	void Zero()
	{
		std::memset(_data, 0, _capacity * sizeof(T));
	}

	void Reserve(std::size_t capacity)
	{
		if (capacity == _capacity)
		{
			return;
		}
		if (capacity == 0)
		{
			_capacity = 0;
			_size = 0;
			delete[] _data;
			_data = nullptr;
			return;
		}

		T* tmp = new T[capacity]{};
		std::size_t itemCount = capacity < _size ? capacity : _size;
		for (std::size_t i = 0; i < itemCount; i++)
		{
			tmp[i] = Move(_data[i]);
		}
		delete[] _data;
		_size = itemCount;
		_capacity = capacity;
		_data = tmp;
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
		return Iterator(_data + _size);
	}

	ConstIterator end() const
	{
		return ConstIterator(_data + _size);
	}

	const T& operator[](std::size_t index) const
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is out of range");
		}

		return _data[index];
	}

	T& operator[](std::size_t index)
	{
		if (index >= _size)
		{
			throw std::out_of_range("Index is out of range");
		}

		return _data[index];
	}

	List& operator=(const List& rhs)
	{
		List tmp = rhs;
		*this = Move(tmp);
		return *this;
	}

	List& operator=(List&& rhs)
	{
		Swap(_size, rhs._size);
		Swap(_capacity, rhs._capacity);
		Swap(_data, rhs._data);
		return *this;
	}

private:
	std::size_t _size;
	std::size_t _capacity;
	T* _data;

	static constexpr std::size_t _RESIZE_FACTOR = 2;
};
