#pragma once
#include "Utility.hxx"
#include "List.hxx"

template <typename T, typename Comparator = Less<T>>
class PriorityQueue
{
public:
	PriorityQueue() = default;

	explicit PriorityQueue(const Comparator& comparator)
		: _container(), _comparator(comparator)
	{
	}

	explicit PriorityQueue(const List<T>& values)
		: _container(values), _comparator()
	{
		_Heapify();
	}

	explicit PriorityQueue(List<T>&& values)
		: _container(Move(values)), _comparator()
	{
		_Heapify();
	}

	PriorityQueue(const List<T>& values, const Comparator& comparator)
		: _container(values), _comparator(comparator)
	{
		_Heapify();
	}

	PriorityQueue(List<T>&& values, const Comparator& comparator)
		: _container(Move(values)), _comparator(comparator)
	{
		_Heapify();
	}

	~PriorityQueue() = default;

	bool Empty() const
	{
		return _container.Size() == 0;
	}

	const T& Peek() const
	{
		return _container[0];
	}

	void Add(const T& item)
	{
		_container.Add(item);
		_SiftUp(_container.Size() - 1);
	}

	void Add(T&& item)
	{
		_container.Add(Move(item));
		_SiftUp(_container.Size() - 1);
	}

	T Remove()
	{
		if (_container.Size() == 0)
		{
			throw;
		}
		Swap(_container[0], _container[_container.Size() - 1]);
		T removed = _container.Pop();
		_SiftDown(0);
		return removed;
	}

private:
	List<T> _container;
	Comparator _comparator;

	void _Heapify()
	{
		if (_container.Size() == 0 || _container.Size() == 1)
		{
			return;
		}

		for (std::size_t i = (_container.Size() - 2) / 2; i > 0; i--)
		{
			_SiftDown(i);
		}
		_SiftDown(0);
	}

	void _SiftDown(std::size_t start)
	{
		while (start * 2 + 1 < _container.Size())
		{
			std::size_t child = start * 2 + 1;
			if (child + 1 < _container.Size() && _comparator(_container[child + 1], _container[child]))
			{
				child++;
			}
			if (!(_comparator(_container[child], _container[start])))
			{
				return;
			}
			Swap(_container[start], _container[child]);
			start = child;
		}
	}

	void _SiftUp(std::size_t start)
	{
		if (start == 0)
		{
			return;
		}

		std::size_t parent = (start - 1) / 2;
		while (start > 0 && _comparator(_container[start], _container[parent]))
		{
			Swap(_container[start], _container[parent]);
			start = parent;
			parent = (start - 1) / 2;
		}
	}
};
