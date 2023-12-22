#ifndef DEQUE_HPP
# define DEQUE_HPP

# include <memory>
# include <cstddef>

# include "iterator.hpp"
# include "reverse_iterator.hpp"
# include "is_integral.hpp"
# include "enable_if.hpp"
# include "exception.hpp"
# include "vector.hpp"

/*
* Documentation source : https://stackoverflow.com/questions/6292332/what-really-is-a-deque-in-stl
*/

namespace ft
{
	template<class T, class Alloc = std::allocator<T> >
	class deque
	{

	//THEY USED TO BE FRIENDS

		friend class dequeIterator;
		friend class constDequeIterator;

	//PUBLIC TYPEDEFS

	public:

		typedef T			value_type;
		typedef Alloc		allocator_type;
		typedef T&			reference;
		typedef const T&	const_reference;
		typedef T*			pointer;
		typedef const T*	const_pointer;
		typedef ptrdiff_t	difference_type;
		typedef size_t		size_type;

	//PRIVATE TYPEDEFS

	private:

		typedef ft::vector< pointer >			_base;
		typedef typename _base::size_type		_baseSize;
		typedef typename _base::iterator		_baseIterator;
		typedef typename _base::const_iterator	_baseConstIterator;

	//PRIVATE MEMBERS

		_base				_chunks;
		static const size_t	_chunkSize = 8;

	public:

	//ITERATORS

		class constDequeIterator;
		class dequeIterator : public ft::iterator< ft::bidirectional_iterator_tag, value_type >
		{
			friend ft::deque<value_type>;

		private:

			_baseIterator	_basePosition;
			size_t			_chunkPosition;
			
			dequeIterator(const _baseIterator& b, size_t c) : _basePosition(b), _chunkPosition(c) {}

		public:

			dequeIterator() : _chunkPosition(0) {}
			dequeIterator(const dequeIterator& o) : _basePosition(o._basePosition), _chunkPosition(o._chunkPosition) {}
			~dequeIterator() {}



		};
	};
}

#endif