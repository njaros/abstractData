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

		explicit priority_queue(const Compare& comp = Compare(), const Container& ctnr = Container()) : _pred(comp), _base(ctnr) 
		{
			ft::make_heap(_base.begin(), _base.end(), _pred);
		}
		template <class InputIterator>
		priority_queue(InputIterator first, InputIterator last, const Compare& comp = Compare(), const Container& ctnr = Container())
			: _pred(comp), _base(ctnr)
		{
			while (first != last)
				_base.push_back(*(first++));
			ft::make_heap(_base.begin(), _base.end(), _pred);
		}

		bool				empty()	const { return _base.empty(); }

		size_type			size()	const { return _base.size(); }

		const value_type	top()	const { return _base.front(); }

		void				push(const value_type& val)
		{
			_base.push_back(val);
			ft::push_heap(_base.begin(), _base.end(), _pred);
		}

		void				pop()
		{
			ft::pop_heap(_base.begin(), _base.end(), _pred);
			_base.pop_back();
		}
	};
}

#endif