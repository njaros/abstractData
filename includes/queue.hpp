#ifndef QUEUE_HPP_ABSTRACT_DATA_BY_LE_N
#define QUEUE_HPP_ABSTRACT_DATA_BY_LE_N

#include "deque.hpp"

namespace ft
{
	template< class T, class Container = ft::deque< T > >
	class queue
	{
		//No choice to be friendly with non members overloads to acceed to the underlying container
		template <class TN, class ContainerN>
		friend bool operator==(const deque<TN, ContainerN>& lhs, const deque<TN, ContainerN>& rhs);
		template <class TN, class ContainerN>
		friend bool operator!=(const deque<TN, ContainerN>& lhs, const deque<TN, ContainerN>& rhs);
		template <class TN, class ContainerN>
		friend bool operator>(const deque<TN, ContainerN>& lhs, const deque<TN, ContainerN>& rhs);
		template <class TN, class ContainerN>
		friend bool operator>=(const deque<TN, ContainerN>& lhs, const deque<TN, ContainerN>& rhs);
		template <class TN, class ContainerN>
		friend bool operator<(const deque<TN, ContainerN>& lhs, const deque<TN, ContainerN>& rhs);
		template <class TN, class ContainerN>
		friend bool operator<=(const deque<TN, ContainerN>& lsh, const deque<TN, ContainerN>& rhs);

		container_type		_base;

	public:
		typedef				T							value_type;
		typedef				Container					container_type;
		typedef typename	container_type::size_type	size_type;

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

}

#endif