#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

namespace ft
{
	template<class T>
	T& max(T& a, T& b)
	{
		return (a < b) ? b : a;
	}

	template<class T>
	T& min(T& a, T& b)
	{
		return (b < a) ? b : a;
	}

	template<class T>
	const T& max(const T& a, const T& b)
	{
		return (a < b) ? b : a;
	}

	template<class T>
	const T& min(const T& a, const T& b)
	{
		return (b < a) ? b : a;
	}
}

#endif // !ALGORITHM_HPP
