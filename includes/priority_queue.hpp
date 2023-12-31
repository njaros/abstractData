#ifndef PRIORITY_QUEUE_HPP_ABSTRACT_DATA_BY_LE_N
#define PRIORITY_QUEUE_HPP_ABSTRACT_DATA_BY_LE_N

#include "vector.hpp"
#include "functional.hpp"
#include "algorithm.hpp"

/*
* j'ai choisi le tri par insertion, c'est selon moi le meilleur dans ce contexte du fait qu'on insere les element un par un.
*/

namespace ft
{
	template <class T, class Container = vector<T>, class Compare = less<typename Container::value_type> >
	class priority_queue
	{

	public :

		typedef				T						value_type;
		typedef				Container				container_type;
		typedef typename	Container::size_type	size_type;

	protected :

		Compare		_pred;
		Container	_base;

	public :

		explicit priority_queue(const Compare& comp = Compare(), const Container& ctnr = Container()) : _pred(comp), _base(Container())
		{
			_sort(ctnr);
		}
		template <class InputIterator>
		priority_queue(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Container& ctnr = Container())
			: _pred(comp), _base(Container())
		{
			for (typename Container::const_iterator cit = ctnr.begin(); cit != ctnr.end(); ++cit)
				push(*cit);
			while (first != last)
				push(*(first++));
		}

		bool	empty()	const { return _base.empty(); }

		void	push(const value_type& val)
		{
			typename Container::iterator it = _base.end() - 1;

			while (it != _base.begin() && !_pred(val, *it))
				--it;
			_base.insert(it, val);
		}

		void	pop()
		{
			
		}
	};
}

#endif