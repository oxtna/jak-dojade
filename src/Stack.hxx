#pragma once
#include "Utility.hxx"
#include "List.hxx"
#include <stdexcept>

template <typename T>
class Stack
{
public:
	Stack()
		: _container()
	{
	}

	~Stack() = default;

	bool Empty() const
	{
		return _container.Size() == 0;
	}

	const T& Peek() const
	{
		if (Empty())
		{
			throw std::logic_error("Cannot peek at empty stack");
		}

		return _container[_container.Size() - 1];
	}

	void Push(const T& item)
	{
		_container.Add(item);
	}

	void Push(T&& item)
	{
		_container.Add(Move(item));
	}

	T Pop()
	{
		if (Empty())
		{
			throw std::logic_error("Empty stack cannot be popped");
		}

		return _container.Pop();
	}

private:
	List<T> _container;
};
