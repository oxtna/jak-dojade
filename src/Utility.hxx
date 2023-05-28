#pragma once
#include <cstddef>

template <typename T>
struct _RemoveReference
{
	typedef T type;
};

template <typename T>
struct _RemoveReference<T&>
{
	typedef T type;
};

template <typename T>
struct _RemoveReference<T&&>
{
	typedef T type;
};

template <typename T>
using RemoveReference = typename _RemoveReference<T>::type;

template <typename T>
RemoveReference<T>&& Move(T&& value) noexcept
{
	return static_cast<RemoveReference<T>&&>(value);
}

template <typename T>
void Swap(T& lhs, T& rhs)
{
	T tmp = Move(lhs);
	lhs = Move(rhs);
	rhs = Move(tmp);
}

template <typename T>
struct Less
{
	constexpr bool operator()(const T& lhs, const T& rhs) const
	{
		return lhs < rhs;
	}
};

template <typename T>
struct Greater
{
	constexpr bool operator()(const T& lhs, const T& rhs) const
	{
		return lhs > rhs;
	}
};

template <typename T>
class UniquePtr
{
public:
	UniquePtr()
		: _ptr(nullptr)
	{
	}

	explicit UniquePtr(T* ptr)
		: _ptr(ptr)
	{
	}

	UniquePtr(const UniquePtr& rhs) = delete;

	UniquePtr(UniquePtr&& rhs)
		: _ptr(rhs._ptr)
	{
		rhs._ptr = nullptr;
	}

	~UniquePtr()
	{
		delete _ptr;
	}

	T* GetPtr() const
	{
		return _ptr;
	}

	UniquePtr& operator=(const UniquePtr& rhs) = delete;

	UniquePtr& operator=(UniquePtr&& rhs)
	{
		Swap(_ptr, rhs._ptr);
		return *this;
	}

	T* operator->()
	{
		return _ptr;
	}

	T& operator*()
	{
		return *_ptr;
	}

private:
	T* _ptr;
};

template <typename T>
class UniquePtr<T[]>
{
public:
	UniquePtr()
		: _ptr(nullptr)
	{
	}

	explicit UniquePtr(T* ptr)
		: _ptr(ptr)
	{
	}

	UniquePtr(const UniquePtr& rhs) = delete;

	UniquePtr(UniquePtr&& rhs)
		: _ptr(rhs._ptr)
	{
		rhs._ptr = nullptr;
	}

	~UniquePtr()
	{
		delete[] _ptr;
	}

	const T* GetPtr() const
	{
		return _ptr;
	}

	UniquePtr& operator=(const UniquePtr& rhs) = delete;

	UniquePtr& operator=(UniquePtr&& rhs)
	{
		Swap(_ptr, rhs._ptr);
		return *this;
	}

	T& operator[](std::size_t index)
	{
		return _ptr[index];
	}

private:
	T* _ptr;
};
