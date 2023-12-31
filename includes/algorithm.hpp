#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

namespace ft
{

	//HEAP

	template <class RandomAccessIterator, class Compare>
	void	make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{

	}

	template <class RandomAccessIterator, class Compare>
	void	push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{

	}

	template <class RandomAccessIterator, class Compare>
	void	pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{

	}

	//MIN, MAX

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
