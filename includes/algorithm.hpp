#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

namespace ft
{

	//HEAP

	template <typename RandomAccessIterator, typename size_type = typename RandomAccessIterator::size_type, typename Compare>
	void	_tasser(RandomAccessIterator first, RandomAccessIterator last, Compare comp, size_type idx)
	{
		size_type	current;
		size_type	childL;
		size_type	childR;
		size_type	end;
		typename RandomAccessIterator::value_type	tmp;

		current = idx;
		end = last - first;
		childL = (current * 2) + 1;
		childR = (current * 2) + 2;
		while (childL < end)
		{
			if (!comp(*(first + childL), *(first + current)))
			{
				if (childR >= end || !comp(*(first + childR), *(first + current)))
					return;
				tmp = *(first + current);
				*(first + current) = *(first + childR);
				*(first + childR) = tmp;
				current = childR;
			}
			else
			{
				if (childR < end && comp(*(first + childR), *(first + childL)))
				{
					tmp = *(first + current);
					*(first + current) = *(first + childR);
					*(first + childR) = tmp;
					current = childR;
				}
				else
				{
					tmp = *(first + current);
					*(first + current) = *(first + childL);
					*(first + childL) = tmp;
					current = childL;
				}
			}
			childL = (2 * current) + 1;
			childR = (2 * current) + 2;
		}
	}

	template <typename RandomAccessIterator, typename Compare>
	void	make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		typename RandomAccessIterator::size_type	size;
		typename RandomAccessIterator::value_type	tmp;

		if (first > last)
			throw(ft::logic_error("algorithm::push_heap : iterator first must be lower than last"));
		size = last - first;
		for (size_t i = size / 2; i != ~0; --i)
			_tasser(first, last, comp, i);
	}

	template <typename RandomAccessIterator, typename Compare>
	void	push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		typename RandomAccessIterator::size_type	current;
		typename RandomAccessIterator::size_type	parent;
		typename RandomAccessIterator::value_type	tmp;

		if (first >= last)
			throw(ft::logic_error("algorithm::push_heap : iterator first must be strictly lower than last"));

		current = last - first - 1;
		if (current % 2)
			parent = (current - 1) / 2;
		else
			parent = (current - 2) / 2;
		while (current && comp(*(first + current), *(first + parent)))
		{
			tmp = *(first + current);
			*(first + current) = *(first + parent);
			*(first + parent) = tmp;
			current = parent;
			if (current % 2)
				parent = (current - 1) / 2;
			else
				parent = (current - 2) / 2;
		}
	}

	template <typename RandomAccessIterator, typename Compare>
	void	pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		typename RandomAccessIterator::value_type	tmp;

		if (first >= last)
			throw(ft::logic_error("algorithm::push_heap : iterator first must be strictly lower than last"));
		
		tmp = *first;
		*first = *(last - 1);
		*(last - 1) = tmp;
		_tasser(first, last - 1, comp, 0);
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

#include "algorithm.tpp"
#endif // !ALGORITHM_HPP
