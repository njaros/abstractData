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
* idees :
* - la base est un vecteur de value_type[8] malloquées
* - le vecteur contient autant de chunk vide (pointer = 0) avant et apres l'array de chunk pas vide
* apres chaque agrandissement du vecteur
* - établir une base saine d'allocation et desallocation mémoire de chunk
*/

namespace ft
{
	template< class T, class Alloc = std::allocator<T> >
	class deque
	{

		//THEY HAVE TO BE FRIENDS

		friend class MyIterator;
		friend class MyConstIterator;

		//PUBLIC TYPEDEFS

	public:

		typedef T			value_type;
		typedef Alloc		allocator_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef ptrdiff_t	difference_type;
		typedef size_t		size_type;

		//PRIVATE TYPEDEFS

	private:

		typedef pointer							_chunk;
		typedef ft::vector< _chunk >			_base;
		typedef typename _base::size_type		_baseSizeType;
		typedef typename _base::iterator		_baseIterator;
		typedef typename _base::const_iterator	_baseConstIterator;
		typedef typename _base::difference_type	_baseDifferenceType;
		typedef ft::pair< _baseSizeType, int >	_edge;

		//PRIVATE MEMBERS

		_base							_chunks;
		Alloc							_alloc;
		size_type						_size;
		static const difference_type	_chunkSize = 8;
		//_chunkSize is difference_type for signed operations later,
		//so I'll not need to recast it for any signed results
		_edge							_begin;
		_edge							_end;

	public:

		//ITERATORS

		class MyConstIterator;
		class MyIterator : public ft::iterator< ft::bidirectional_iterator_tag, value_type >
		{
			friend ft::deque<value_type>;
			friend MyConstIterator;

		private:

			_baseIterator	_basePosition;
			difference_type	_chunkPosition;

			MyIterator(const _baseIterator& b, difference_type c) : _basePosition(b), _chunkPosition(c) {}

			MyIterator& _move(difference_type val)
			{
				_basePosition += val / _chunkSize;
				_chunkPosition += val % _chunkSize;
				if (_chunkPosition < 0)
				{
					--_basePosition;
					_chunkPosition += _chunkSize;
				}
				else if (_chunkPosition >= _chunkSize)
				{
					++_basePosition;
					_chunkPosition -= _chunkSize;
				}
				return *this;
			}

		public:

			MyIterator() : _chunkPosition(0) {}
			MyIterator(const MyIterator& o) : _basePosition(o._basePosition), _chunkPosition(o._chunkPosition) {}
			MyIterator(const MyConstIterator& o) : _basePosition(o._basePosition), _chunkPosition(o._chunkPosition) {}
			~MyIterator() {}

			bool	operator==(const MyIterator& other)	const
			{
				return (_basePosition == other._basePosition && _chunkPosition == other._chunkPosition);
			}

			bool	operator==(const MyConstIterator& other)	const
			{
				return (_basePosition == other._basePosition && _chunkPosition == other._chunkPosition);
			}

			bool	operator!=(const MyIterator& other)	const
			{
				return !(*this == other);
			}

			bool	operator!=(const MyConstIterator& other)	const
			{
				return !(*this == other);
			}

			bool operator=(const MyIterator& o)
			{
				if (o != *this)
				{
					_basePosition = o._basePosition;
					_chunkPosition = o._basePosition;
				}
				return *this;
			}

			bool operator=(const MyConstIterator& o)
			{
				if (o != *this)
				{
					_basePosition = o._basePosition;
					_chunkPosition = o._basePosition;
				}
				return *this;
			}

			MyIterator& operator+=(difference_type val)
			{
				return _move(val);
			}

			MyIterator& operator-=(difference_type val)
			{
				return _move(-val);
			}

			reference	operator*()
			{
				return _basePosition[_chunkPosition];
			}

			pointer		operator->()
			{
				return &_basePosition[_chunkPosition];
			}

			MyIterator& operator++()
			{
				_move(1);
				return (*this);
			}

			MyIterator	operator++(int)
			{
				MyIterator	tmp(*this);
				_move(1);
				return (tmp);
			}

			MyIterator& operator--()
			{
				_move(-1);
				return (*this);
			}

			MyIterator	operator--(int)
			{
				MyIterator	tmp(*this);
				_move(-1);
				return (tmp);
			}

			MyIterator	operator+(difference_type val)	const
			{
				MyIterator	newIt;

				newIt += val;
				return (newIt);
			}

			MyIterator	operator-(difference_type val)	const
			{
				MyIterator	newIt;

				newIt -= val;
				return (newIt);
			}

			difference_type operator-(const MyIterator& other)	const
			{
				return (_chunkSize * (_basePosition - other._basePosition) + (_chunkPosition - other._chunkPosition));
			}

			difference_type operator-(const MyConstIterator& other)	const
			{
				return (_chunkSize * (_basePosition - other._basePosition) + (_chunkPosition - other._chunkPosition));
			}

			bool	operator<(const MyIterator& other)	const
			{
				return (_basePosition < other._basePosition ||
					(_basePosition == other._basePosition && _chunkPosition < other._basePosition));
			}

			bool	operator<(const MyConstIterator& other)	const
			{
				return (_basePosition < other._basePosition ||
					(_basePosition == other._basePosition && _chunkPosition < other._basePosition));
			}

			bool	operator<=(const MyIterator& other)	const
			{
				return !(other < *this);
			}

			bool	operator<=(const MyConstIterator& other)	const
			{
				return !(other < *this);
			}

			bool	operator>(const MyIterator& other)	const
			{
				return (other < *this);
			}

			bool	operator>(const MyConstIterator& other)	const
			{
				return (other < *this);
			}

			bool	operator>=(const MyIterator& other)	const
			{
				return !(*this < other);
			}

			bool	operator>=(const MyConstIterator& other)	const
			{
				return !(*this < other);
			}

			reference	operator[](int val)
			{
				return *(*this + val);
			}
		};

		class MyConstIterator : public ft::iterator< ft::bidirectional_iterator_tag, const value_type >
		{
			friend ft::deque<value_type>;
			friend MyIterator;

		private:

			_baseIterator	_basePosition;
			difference_type	_chunkPosition;

			MyConstIterator(const _baseIterator& b, difference_type c) : _basePosition(b), _chunkPosition(c) {}

			MyConstIterator& _move(difference_type val)
			{
				_basePosition += val / _chunkSize;
				_chunkPosition += val % _chunkSize;
				if (_chunkPosition < 0)
				{
					--_basePosition;
					_chunkPosition += _chunkSize;
				}
				else if (_chunkPosition >= _chunkSize)
				{
					++_basePosition;
					_chunkPosition -= _chunkSize;
				}
				return *this;
			}

		public:

			MyConstIterator() : _chunkPosition(0) {}
			MyConstIterator(const MyConstIterator& o) : _basePosition(o._basePosition), _chunkPosition(o._chunkPosition) {}
			MyConstIterator(const MyIterator& o) : _basePosition(o._basePosition), _chunkPosition(o._chunkPosition) {}
			~MyConstIterator() {}

			bool	operator==(const MyIterator& other)	const
			{
				return (_basePosition == other._basePosition && _chunkPosition == other._chunkPosition);
			}

			bool	operator==(const MyConstIterator& other)	const
			{
				return (_basePosition == other._basePosition && _chunkPosition == other._chunkPosition);
			}

			bool	operator!=(const MyIterator& other)	const
			{
				return !(*this == other);
			}

			bool	operator!=(const MyConstIterator& other)	const
			{
				return !(*this == other);
			}

			bool operator=(const MyIterator& o)
			{
				if (o != *this)
				{
					_basePosition = o._basePosition;
					_chunkPosition = o._basePosition;
				}
				return *this;
			}

			bool operator=(const MyConstIterator& o)
			{
				if (o != *this)
				{
					_basePosition = o._basePosition;
					_chunkPosition = o._basePosition;
				}
				return *this;
			}

			MyIterator& operator+=(difference_type val)
			{
				return _move(val);
			}

			MyIterator& operator-=(difference_type val)
			{
				return _move(-val);
			}

			const_reference	operator*()
			{
				return _basePosition[_chunkPosition];
			}

			const_pointer		operator->()
			{
				return &_basePosition[_chunkPosition];
			}

			MyConstIterator& operator++()
			{
				_move(1);
				return (*this);
			}

			MyConstIterator	operator++(int)
			{
				MyConstIterator	tmp(*this);
				_move(1);
				return (tmp);
			}

			MyConstIterator& operator--()
			{
				_move(-1);
				return (*this);
			}

			MyConstIterator	operator--(int)
			{
				MyConstIterator	tmp(*this);
				_move(-1);
				return (tmp);
			}

			MyConstIterator	operator+(difference_type val)	const
			{
				MyConstIterator	newIt;

				newIt += val;
				return (newIt);
			}

			MyConstIterator	operator-(difference_type val)	const
			{
				MyConstIterator	newIt;

				newIt -= val;
				return (newIt);
			}

			difference_type operator-(const MyIterator& other)	const
			{
				return (_chunkSize * (_basePosition - other._basePosition) + (_chunkPosition - other._chunkPosition));
			}

			difference_type operator-(const MyConstIterator& other)	const
			{
				return (_chunkSize * (_basePosition - other._basePosition) + (_chunkPosition - other._chunkPosition));
			}

			bool	operator<(const MyIterator& other)	const
			{
				return (_basePosition < other._basePosition ||
					(_basePosition == other._basePosition && _chunkPosition < other._basePosition));
			}

			bool	operator<(const MyConstIterator& other)	const
			{
				return (_basePosition < other._basePosition ||
					(_basePosition == other._basePosition && _chunkPosition < other._basePosition));
			}

			bool	operator<=(const MyIterator& other)	const
			{
				return !(other < *this);
			}

			bool	operator<=(const MyConstIterator& other)	const
			{
				return !(other < *this);
			}

			bool	operator>(const MyIterator& other)	const
			{
				return (other < *this);
			}

			bool	operator>(const MyConstIterator& other)	const
			{
				return (other < *this);
			}

			bool	operator>=(const MyIterator& other)	const
			{
				return !(*this < other);
			}

			bool	operator>=(const MyConstIterator& other)	const
			{
				return !(*this < other);
			}

			const_reference	operator[](int val)
			{
				return *(*this + val);
			}
		};

		typedef MyIterator								iterator;
		typedef MyConstIterator							const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

	private:

		//CHUNK HANDLER FUNCTIONS

		_chunk _getNewChunk()
		{
			_chunk toRet;

			toRet = _alloc.allocate(_chunkSize);
			if (!toRet)
				throw(ft::bad_alloc("ft::deque : failed to allocate a chunk."));
			return toRet;
		}

		void	_constructDataChunk(_chunk, size_t pos, const value_type& data = value_type())
		{
			_alloc.construct(_chunk + pos, data);
		}

		void	_destroyDataChunk(_chunk, size_t start, size_t lenght)
		{
			while (lenght--)
				_alloc.destroy(_chunk + start++);
		}

		void	_deleteChunk(_chunk& chunk)
		{
			_alloc.deallocate(chunk, _chunkSize);
			chunk = 0;
		}

		void	_moveChunkLeft(iterator position, _baseSizeType holeChunkSize)
		{

		}

		void	_moveDataLeft(iterator position, size_type holeSize) //insert BEFORE position
		{
			size_type chunkNeed;

			if (holeSize)
			{
				chunkNeed = holeSize / _chunkSize;
				if (holeSize % _chunkSize)
					++chunkNeed;
				else
				{
					_moveChunkLeft(position, chunkNeed);
					return;
				}
			}
		}

		void	_moveDataRight(iterator position, size_type holeSize) //insert BEFORE position
		{

		}

	public:

		//CONSTRUCTORS

		deque(const allocator_type& alloc = allocator_type()) :
			_alloc(alloc),
			_size(0),
			_begin(ft::make_pair(0, 0)),
			_end(_begin)
		{}
		deque(size_type n, const value_type& val, const allocator_type& alloc = allocator_type()) :
			_alloc(alloc),
			_size(n),
			_begin(ft::make_pair(0, 0)),
			_end(_begin)
		{
			_baseSizeType	size;

			if (_size)
			{
				size = _size / _chunkSize;
				if (_size % _chunkSize)
					++size;
				_chunks.assign(size, nullptr);
				while (n--)
				{
					if (_end.second == 0)
						_chunks[_end.first] = _getNewChunk();
					_constructDataChunk(_chunks[_end.first], _end.second, val);
					++_end.second;
					if (_end.second == _chunkSize)
					{
						++_end.first;
						_end.second = 0;
					}
				}
			}

		}
		deque(const deque& o) :
			_alloc(o._alloc),
			_size(o._size),
			_begin(ft::make_pair(0, 0)),
			_end(_begin)
		{
			_baseSizeType	size;

			if (_size)
			{
				size = _size / _chunkSize;
				if (_size % _chunkSize)
					++size;
				_chunks.assign(size, nullptr);
				for (const_iterator cit = o._begin(); cit != o._end(); ++cit)
				{
					if (_end.second == 0)
						_chunks[_end.first] = _getNewChunk();
					_constructDataChunk(_chunks[_end.first], _end.second, *cit);
					++_end.second;
					if (_end.second == _chunkSize)
					{
						++_end.first;
						_end.second = 0;
					}
				}
			}
		}
		template< class InputIterator >
		deque(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
			typename enable_if<!ft::is_integral< InputIterator >::value >::type* = 0) :
			_alloc(alloc),
			_size(0),
			_begin(ft::make_pair(0, 0)),
			_end(_begin)
		{
			difference_type dist;
			_baseSizeType	baseSize;

			if (first != last)
			{
				dist = ft::distance<InputIterator>(first, last);
				if (dist < 0)
					throw (ft::length_error("deque::deque(random_access_iterator first, random_access_iterator last) : first must be < last"));
				_size = dist;
				baseSize = _size / _chunkSize;
				if (_size % _chunkSize)
					++baseSize;
				_chunks.assign(baseSize, nullptr);
				while (first != last)
				{
					if (_end.second == 0)
						_chunks[_end.first] = _getNewChunk();
					_constructDataChunk(_chunks[_end.first], _end.second, *first);
					++_end.second;
					if (_end.second == _chunkSize)
					{
						++_end.first;
						_end.second = 0;
					}
					++first;
				}
			}

		}
		
		//DESTRUCTOR

		~deque()
		{
			for (iterator it = begin(); it != end(); ++it)
			{
				_alloc.destroy(&(*it));
			}
			for (_baseIterator it = _chunks.begin(); it != _chunks.end(); ++it)
			{
				_deleteChunk(&(*it));
			}
		}
		
		//BEGIN END ITERATORS REVERSE_ITERATORS

		iterator	begin() {
			return iterator(_base.begin() + _begin.first, _begin.second);
		}
		const_iterator begin() {
			return const_iterator(_base.begin() + _begin.first, _begin.second);
		}
		iterator	end() {
			return iterator(_base.begin() + _end.first, _end.second);
		}
		const_iterator	end() {
			return const_iterator(_base.begin() + _end.first, _end.second);
		}
		reverse_iterator		rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator	rbegin() { return const_reverse_iterator(end()); }
		reverse_iterator		rend() { return reverse_iterator(begin()); }
		const_reverse_iterator	rend() { return const_reverse_iterator(begin()); }
	};
}

#endif