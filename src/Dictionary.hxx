#pragma once
#include "Utility.hxx"
#include "List.hxx"
#include "String.h"
#include <stdexcept>

template <typename T>
class Dictionary
{
private:
	class Bucket
	{
	public:
		Bucket(const T& data, const String& key, UniquePtr<Bucket>&& next)
			: _data(data), _key(key), _next(Move(next)) {}

		Bucket(T&& data, const String& key, UniquePtr<Bucket>&& next)
			: _data(Move(data)), _key(key), _next(Move(next)) {}

		~Bucket() = default;

		const String& GetKey() const
		{
			return _key;
		}

		T& GetData()
		{
			return _data;
		}

		Bucket* GetNext() const
		{
			return _next.GetPtr();
		}
		
	private:
		T _data;
		String _key;
		UniquePtr<Bucket> _next;
	};

public:
	Dictionary()
		: _container(_CONTAINER_SIZE) {}

	~Dictionary() = default;

	void Add(const String& key, const T& value)
	{
		std::size_t index = _Hash(key) % _CONTAINER_SIZE;
		if (_container[index].GetPtr() == nullptr)
		{
			_container[index] = UniquePtr<Bucket>(new Bucket(value, key, UniquePtr<Bucket>()));
		}
		else
		{
			_container[index] = UniquePtr<Bucket>(new Bucket(value, key, Move(_container[index])));
		}
	}
	
	void Add(const String& key, T&& value)
	{
		std::size_t index = _Hash(key) % _CONTAINER_SIZE;
		if (_container[index].GetPtr() == nullptr)
		{
			_container[index] = UniquePtr<Bucket>(new Bucket(Move(value), key, UniquePtr<Bucket>()));
		}
		else
		{
			_container[index] = UniquePtr<Bucket>(new Bucket(Move(value), key, Move(_container[index])));
		}
	}

	T& Get(const String& key)
	{
		std::size_t index = _Hash(key) % _CONTAINER_SIZE;
		Bucket* current = _container[index].GetPtr();
		while (current != nullptr && current->GetKey() != key)
		{
			current = current->GetNext();
		}
		if (current == nullptr)
		{
			throw std::out_of_range("Dictionary has no entry for this key");
		}

		return current->GetData();
	}

private:
	static constexpr std::size_t _CONTAINER_SIZE = 36 * 36 * 36 * 36;
	List<UniquePtr<Bucket>> _container;

	unsigned int _Hash(const String& key)
	{
		unsigned int hash = 5381;
		for (std::size_t i = 0; i < key.GetLength(); i++)
		{
			hash = ((hash << 5) + hash) + key[i];
		}

		return hash;
	}
};
