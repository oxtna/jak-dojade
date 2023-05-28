#pragma once

template <typename T1, typename T2, typename T3>
struct Triple
{
	T1 first;
	T2 second;
	T3 third;

	Triple() = default;

	Triple(const T1& first, const T2& second, const T3& third)
		: first(first), second(second), third(third)
	{
	}

	~Triple() = default;
};
