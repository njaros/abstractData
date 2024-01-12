#ifndef QUEUE_HPP_ABSTRACT_DATA_BY_LE_N
#define QUEUE_HPP_ABSTRACT_DATA_BY_LE_N

#include "deque.hpp"
#include "vector.hpp"
#include "functional.hpp"
#include "algorithm.hpp"

namespace ft
{
	template< class T, class Container = ft::deque< T > >
	class queue
	{
		//No choice to be friendly with non members overloads to acceed to the underlying container
		template <class TN, class ContainerN>
		friend bool operator==(const queue<TN, ContainerN>& lhs, const queue<TN, ContainerN>& rhs);
		template <class TN, class ContainerN>
		friend bool operator!=(const queue<TN, ContainerN>& lhs, const queue<TN, ContainerN>& rhs);
		template <class TN, class ContainerN>
		friend bool operator>(const queue<TN, ContainerN>& lhs, const queue<TN, ContainerN>& rhs);
		template <class TN, class ContainerN>
		friend bool operator>=(const queue<TN, ContainerN>& lhs, const queue<TN, ContainerN>& rhs);
		template <class TN, class ContainerN>
		friend bool operator<(const queue<TN, ContainerN>& lhs, const queue<TN, ContainerN>& rhs);
		template <class TN, class ContainerN>
		friend bool operator<=(const queue<TN, ContainerN>& lsh, const queue<TN, ContainerN>& rhs);

	public:

		typedef				T							value_type;
		typedef				Container					container_type;
		typedef typename	container_type::size_type	size_type;

	protected:

		container_type		_base;

	public:

		explicit queue(const container_type& ctnr = container_type()) : _base(ctnr) {}

		bool				empty()				const	{ return _base.empty(); }
		size_type			size()				const	{ return _base.size(); }
		value_type&			front()						{ return _base.front(); }
		const value_type&	front()				const	{ return _base.front(); }
		value_type&			back()						{ return _base.back(); }
		const value_type&	back()				const	{ return _base.back(); }
		void				push(const value_type& val)	{ _base.push_back(val); }
		void				pop()						{ _base.pop_front(); }
	};

	template <class T, class Container>  bool operator==(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
	{
		return (lhs._base == rhs._base);
	}

	template <class T, class Container>  bool operator!=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
	{
		return (lhs._base != rhs._base);
	}

	template <class T, class Container>  bool operator<(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
	{
		return (lhs._base < rhs._base);
	}

	template <class T, class Container>  bool operator<=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
	{
		return (lhs._base <= rhs._base);
	}

	template <class T, class Container>  bool operator>(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
	{
		return (lhs._base > rhs._base);
	}

	template <class T, class Container>  bool operator>=(const queue<T, Container>& lhs, const queue<T, Container>& rhs)
	{
		return (lhs._base >= rhs._base);
	}

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