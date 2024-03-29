#ifndef ALGORITHM_HPP
# define ALGORITHM_HPP

namespace ft
{

	//HEAP

	template <typename RandomAccessIterator, typename Compare>
	void	_tasser(RandomAccessIterator first, RandomAccessIterator last, Compare comp, typename RandomAccessIterator::difference_type idx)
	{
		typename RandomAccessIterator::difference_type	current;
		typename RandomAccessIterator::difference_type	childL;
		typename RandomAccessIterator::difference_type	childR;
		typename RandomAccessIterator::difference_type	end;
		typename RandomAccessIterator::value_type	tmp;

		current = idx;
		end = last - first;
		childL = (current * 2) + 1;
		childR = (current * 2) + 2;
		while (childL < end)
		{
			if (!comp(*(first + current), *(first + childL)))
			{
				if (childR >= end || !comp(*(first + current), *(first + childR)))
					return;
				tmp = *(first + current);
				*(first + current) = *(first + childR);
				*(first + childR) = tmp;
				current = childR;
			}
			else
			{
				if (childR < end && comp(*(first + childL), *(first + childR)))
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
		typename RandomAccessIterator::difference_type	size;

		if (first > last)
			throw(ft::logic_error("algorithm::push_heap : iterator first must be lower than last"));
		size = last - first;
		for (typename RandomAccessIterator::difference_type i = size / 2; i >= 0; --i)
			_tasser(first, last, comp, i);
	}

	template <typename RandomAccessIterator, typename Compare>
	void	push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		typename RandomAccessIterator::difference_type	current;
		typename RandomAccessIterator::difference_type	parent;
		typename RandomAccessIterator::value_type	tmp;

		if (first >= last)
			throw(ft::logic_error("algorithm::push_heap : iterator first must be strictly lower than last"));

		current = last - first - 1;
		if (current % 2)
			parent = (current - 1) / 2;
		else
			parent = (current - 2) / 2;
		while (current && comp(*(first + parent), *(first + current)))
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

	// COMPARISONS

	template < class InputIterator1, class InputIterator2 >
	bool	equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		while (first1 != last1)
		{
			if (*first1 != *first2)
				return false;
			++first1;
			++first2;
		}
		return true;
	}

	template < class InputIterator1, class InputIterator2, class BinaryPredicate >
	bool	equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate pred)
	{
		while (first1 != last1)
		{
			if (!pred(*first1, *first2))
				return (false);
			++first1;
			++first2;
		}
		return (true);
	}

	template < class InputIterator1, class InputIterator2 >
	bool	lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || *first2 < *first1)
				return (false);
			else if (*first1 < *first2)
				return (true);
			first1++;
			first2++;
		}
		return (first2 != last2);
	}

	template < class InputIterator1, class InputIterator2, class Compare >
	bool	lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || comp(*first2, *first1))
				return (false);
			else if (comp(*first1, *first2))
				return (true);
			first1++;
			first2++;
		}
		return (first2 != last2);
	}


}
#endif // !ALGORITHM_HPP
