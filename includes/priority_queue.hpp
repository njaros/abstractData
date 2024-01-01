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

		Compare		comp;
		Container	c;

	public :

		explicit priority_queue(const Compare& pred = Compare(), const Container& ctnr = Container()) : comp(pred), c(ctnr) 
		{
			ft::make_heap(c.begin(), c.end(), comp);
		}
		template <class InputIterator>
		priority_queue(InputIterator first, InputIterator last, const Compare& pred = Compare(), const Container& ctnr = Container())
			: comp(pred), c(ctnr)
		{
			while (first != last)
				c.push_back(*(first++));
			ft::make_heap(c.begin(), c.end(), comp);
		}

		bool				empty()	const { return c.empty(); }

		size_type			size()	const { return c.size(); }

		const value_type	top()	const { return c.front(); }

		void				push(const value_type& val)
		{
			c.push_back(val);
			ft::push_heap(c.begin(), c.end(), comp);
		}

		void				pop()
		{
			ft::pop_heap(c.begin(), c.end(), comp);
			c.pop_back();
		}
	};
}

#endif