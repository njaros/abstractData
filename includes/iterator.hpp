#ifndef ITERATOR_HPP
# define ITERATOR_HPP

#include <cstddef>
#include <iterator>
#include <iostream>
#include "exception.hpp"
#include "type_traits.hpp"

namespace ft
{

	typedef std::input_iterator_tag			input_iterator_tag;
	typedef std::output_iterator_tag		output_iterator_tag;
	typedef std::forward_iterator_tag		forward_iterator_tag;
	typedef std::bidirectional_iterator_tag	bidirectional_iterator_tag;
	typedef std::random_access_iterator_tag	random_access_iterator_tag;

	template <class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T&>
	struct iterator
	{
		typedef T			value_type;
		typedef Distance	difference_type;
		typedef Pointer		pointer;
		typedef Reference	reference;
		typedef Category	iterator_category;
	};

	template < class Iterator >
	class iterator_traits
	{
	public:

		typedef typename Iterator::difference_type      difference_type;
		typedef typename Iterator::value_type           value_type;
		typedef typename Iterator::pointer              pointer;
		typedef typename Iterator::reference            reference;
		typedef typename Iterator::iterator_category    iterator_category;

	};

	template < class T >
	class iterator_traits<T*>
	{
	public:

		typedef ptrdiff_t                       difference_type;
		typedef T                               value_type;
		typedef T* pointer;
		typedef T& reference;
		typedef ft::random_access_iterator_tag  iterator_category;

	};

	template < class T >
	class iterator_traits<const T*>
	{
	public:

		typedef ptrdiff_t                       difference_type;
		typedef T                               value_type;
		typedef const T* pointer;
		typedef const T& reference;
		typedef ft::random_access_iterator_tag  iterator_category;

	};

	template < typename >
	struct isRandomAccessIterator : public false_type {};

	template <>
	struct isRandomAccessIterator<ft::random_access_iterator_tag> : public true_type {};

	template < typename T >
	struct is_RandomAccessIterator : public isRandomAccessIterator<T> {};

	template<class InputIterator>
	typename ft::iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last,
		typename ft::enable_if<is_RandomAccessIterator< typename InputIterator::iterator_category >::value >::type* = 0)
	{
		return last - first;
	}

	template<class InputIterator>
	typename ft::iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last,
		typename ft::enable_if<!is_RandomAccessIterator< typename InputIterator::iterator_category >::value >::type* = 0)
	{
		typedef typename InputIterator::difference_type diff;
		diff res = 0;
		while (first != last)
		{
			++first;
			++res;
			if (res <= 0)
				throw(ft::length_error("ft::iterator::distance : memory size overlap"));
		}
		return res;
	}

	template< typename pointer >
	typename long distance(pointer first, pointer last,
		typename ft::enable_if< ft::is_pointer< pointer >::value >::type* = 0)
	{
		return last - first;
	}
}

#endif