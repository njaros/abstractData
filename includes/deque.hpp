#ifndef DEQUE_HPP
# define DEQUE_HPP

# include <memory>
# include <cstddef>

# include "iterator.hpp"
# include "reverse_iterator.hpp"
# include "type_traits.hpp"
# include "exception.hpp"
# include "vector.hpp"
# include "utility.hpp"

/*
* Documentation sources :
* - https://stackoverflow.com/questions/6292332/what-really-is-a-deque-in-stl
* - https://cplusplus.com/reference/deque/deque/
* idees :
* - la base est un vecteur de chunk = value_type[n] malloqu�es, n dependant du sizeof(value_type)
* - le vecteur contient autant de chunk vide (chunk malloqu� mais non construit) avant et apres l'array de chunk pas vide
* apres chaque agrandissement du vecteur
* - tout element de chunk en dehors de begin() et end() doit etre detruit ou non construit (mais pas desallou�)
* - �tablir une base saine d'allocation et desallocation m�moire de chunk
*/

namespace ft
{

	template <class K>
	struct isChar : public false_type {};

	template <>
	struct isChar<char> : public true_type {};

	template <class K>
	struct is_char : public isChar<K> {};

	template< class T, class Alloc = std::allocator<T> >
	class deque
	{

		//THEY HAVE TO BE FRIENDS

		friend class MyIterator;
		friend class MyConstIterator;

		//PUBLIC TYPEDEFS

	public:

		typedef T								value_type;
		typedef Alloc							allocator_type;
		typedef T&								reference;
		typedef const T&						const_reference;
		typedef typename Alloc::pointer			pointer;
		typedef typename Alloc::const_pointer	const_pointer;
		typedef typename Alloc::difference_type	difference_type;
		typedef typename Alloc::size_type		size_type;

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
		static const size_type			_valueTypeSize = sizeof(value_type);
		static const difference_type	_chunkSize
			= _valueTypeSize <= 1 ? 16
			: _valueTypeSize <= 2 ? 8
			: _valueTypeSize <= 4 ? 4
			: _valueTypeSize <= 8 ? 2
			: 1;
		//_chunkSize is difference_type for signed operations later,
		//so I'll not need to recast it for any signed results.
		//The size depends of how it is worth to copy the element adress unstead of the element himself
		//when the data needs to move.
		_edge							_begin;
		_edge							_end;

	public:

		//ITERATORS

		class MyConstIterator;
		class MyIterator : public ft::iterator< ft::random_access_iterator_tag, value_type, difference_type, pointer, reference >
		{
			friend class ft::deque<value_type>;
			friend class MyConstIterator;

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

			MyIterator& operator=(const MyIterator& o)
			{
				if (o != *this)
				{
					_basePosition = o._basePosition;
					_chunkPosition = o._chunkPosition;
				}
				return *this;
			}

			MyIterator& operator=(const MyConstIterator& o)
			{
				if (o != *this)
				{
					_basePosition = o._basePosition;
					_chunkPosition = o._chunkPosition;
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
				return (*_basePosition)[(int)_chunkPosition];
			}

			pointer		operator->()
			{
				return &(*(*this));
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
				MyIterator	newIt(*this);

				newIt += val;
				return (newIt);
			}

			MyIterator	operator-(difference_type val)	const
			{
				MyIterator	newIt(*this);

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
					(_basePosition == other._basePosition && _chunkPosition < other._chunkPosition));
			}

			bool	operator<(const MyConstIterator& other)	const
			{
				return (_basePosition < other._basePosition ||
					(_basePosition == other._basePosition && _chunkPosition < other._chunkPosition));
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

		class MyConstIterator : public ft::iterator< ft::random_access_iterator_tag, const value_type, difference_type, const_pointer, const_reference >
		{
			friend class ft::deque<value_type>;
			friend class MyIterator;

		private:

			_baseConstIterator	_basePosition;
			difference_type	_chunkPosition;

			MyConstIterator(const _baseConstIterator& b, difference_type c) : _basePosition(b), _chunkPosition(c) {}

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

			bool operator!=(const MyConstIterator& other)	const
			{
				return !(*this == other);
			}

			MyConstIterator& operator=(const MyIterator& o)
			{
				if (o != *this)
				{
					_basePosition = o._basePosition;
					_chunkPosition = o._chunkPosition;
				}
				return *this;
			}

			MyConstIterator& operator=(const MyConstIterator& o)
			{
				if (o != *this)
				{
					_basePosition = o._basePosition;
					_chunkPosition = o._chunkPosition;
				}
				return *this;
			}

			MyConstIterator& operator+=(difference_type val)
			{
				return _move(val);
			}

			MyConstIterator& operator-=(difference_type val)
			{
				return _move(-val);
			}

			const_reference	operator*()
			{
				return (*_basePosition)[_chunkPosition];
			}

			const_pointer		operator->()
			{
				return &(*(*this));
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
				MyConstIterator	newIt(*this);

				newIt += val;
				return (newIt);
			}

			MyConstIterator	operator-(difference_type val)	const
			{
				MyConstIterator	newIt(*this);

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
					(_basePosition == other._basePosition && _chunkPosition < other._chunkPosition));
			}

			bool	operator<(const MyConstIterator& other)	const
			{
				return (_basePosition < other._basePosition ||
					(_basePosition == other._basePosition && _chunkPosition < other._chunkPosition));
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

		//UTILITIES

		_edge& _edgeAdd(_edge& e, difference_type d)
		{
			e.first += d / _chunkSize;
			e.second += d % _chunkSize;
			if (e.second < 0)
			{
				--e.first;
				e.second += _chunkSize;
			}
			else if (e.second >= _chunkSize)
			{
				++e.first;
				e.second -= _chunkSize;
			}
			return e;
		}

		_edge& _edgeAdd(_edge& lhs, const _edge& rhs)
		{
			lhs.first += rhs.first;
			lhs.second += rhs.second;
			if (lhs.second >= _chunkSize)
			{
				++lhs.first;
				lhs.second -= _chunkSize;
			}
			return lhs;
		}

		_edge& _edgeSub(_edge& e, difference_type d)
		{
			return _edgeAdd(e, -d);
		}

		_edge& _edgeSub(_edge& lhs, const _edge& rhs)
		{
			lhs.first -= rhs.first;
			lhs.second -= rhs.second;
			if (lhs.second < 0)
			{
				--lhs.first;
				lhs.second += _chunkSize;
			}
			return lhs;
		}

		bool	_isRightSide(const_iterator it) const
		{
			return ((it - begin()) >= (end() - it));
		}

		bool	_isLeftSide(const_iterator it) const
		{
			return !_isRightSide(it);
		}

		_edge	_edgeCastFromIterator(iterator it) const
		{
			difference_type	dist;
			_edge			result;

			dist = it - begin();
			result = _begin;
			result.first += dist / _chunkSize;
			result.second += dist % _chunkSize;
			if (result.second >= _chunkSize)
			{
				++result.first;
				result.second -= _chunkSize;
			}
			return result;
		}

		//DEBUG UTILITIES

		template <class K>
		typename enable_if<is_char<K>::value, void>::type
			_printVal(K elt) const
			{
				std::cout << (int) elt;
			}
		
		template <class K>
		typename enable_if<!is_char<K>::value, void>::type
			_printVal(const K& elt) const
			{
				std::cout << elt;
			}

		void	_printMemory(const char* str = 0) const
		{
			std::cout << std::endl << "DEBUG---- start of printMemory ----DEBUG";
			if (str)
				std::cout << std::endl << str;
			std::cout << std::endl << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
			std::cout << "_size is " << _size << '\n';
			std::cout << "_begin is " << "(" << _begin.first << " , " << _begin.second << ")\n";
			std::cout << " _end  is " << "(" << _end.first << " , " << _end.second << ")\n";
			for (size_type i = 0; i < _chunks.size(); ++i)
			{
				std::hex(std::cout);
				std::cout << "_chunks[" << i << "] = " << (ptrdiff_t)_chunks[i];
				std::dec(std::cout);
				std::cout << " : vals = (";
				for (int j = 0; j < _chunkSize; ++j)
				{
					_printVal(_chunks[i][j]);
					if (j != _chunkSize - 1)
						std::cout << ", ";
					else
						std::cout << ")\n";
				}
			}
			std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
			if (str)
				std::cout << std::endl << str;
			std::cout << std::endl << "DEBUG---- end of printMemory -----DEBUG" << std::endl << std::endl;
		}

		//CHUNK HANDLER FUNCTIONS / MEMORY MANAGEMENT

		_chunk _getNewChunk()
		{
			_chunk toRet;

			toRet = _alloc.allocate(_chunkSize);
			if (!toRet)
				throw(ft::bad_alloc("ft::deque : failed to allocate a chunk."));
			return toRet;
		}

		void	_constructDataChunk(_chunk chunk, size_t pos, const value_type& data = value_type())
		{
			_alloc.construct(chunk + pos, data);
		}

		void	_destroyDataChunk(_chunk chunk, size_t start, size_t lenght)
		{
			while (lenght--)
				_alloc.destroy(chunk + start++);
		}

		void	_deleteChunk(_chunk chunk)
		{
			_alloc.deallocate(chunk, _chunkSize);
		}

		void	_swapChunk(_baseSizeType idxA, _baseSizeType idxB)
		{
			_chunk tmp = _chunks[idxA];
			_chunks[idxA] = _chunks[idxB];
			_chunks[idxB] = tmp;
		}

		void	_clearBase(_base& base)
		{
			int loop = 0;
			for (_baseIterator it = base.begin(); it != base.end(); ++it)
			{
				_deleteChunk(*it);
				loop++;
			}
			base.clear();
		}

		void	_reorderBaseRight(_baseSizeType lenToAdd, _baseSizeType& insertPos)
		{
			_baseSizeType	distBetweenBeginAndEnd;
			_baseSizeType	distBetweenBeginAndInsertPos;
			_baseSizeType	newBeginFirst;
			_baseSizeType	destIdx;
			bool			countEnd;

			countEnd = _end.second != 0;
			distBetweenBeginAndEnd = _end.first - _begin.first;
			distBetweenBeginAndInsertPos = insertPos - _begin.first;
			newBeginFirst = distBetweenBeginAndEnd + countEnd + lenToAdd;
			destIdx = newBeginFirst;

			for (_baseSizeType srcIdx = _begin.first; srcIdx < _end.first; ++srcIdx)
				_swapChunk(destIdx++, srcIdx);
			if (_end.second)
				_swapChunk(destIdx, _end.first);

			_begin.first = newBeginFirst;
			_end.first = newBeginFirst + distBetweenBeginAndEnd;
			insertPos = newBeginFirst + distBetweenBeginAndInsertPos;
		}

		void	_reallocBaseRight(_baseSizeType lenToAdd, _baseSizeType& insertPos)
		{
			_baseSizeType	idxNew;
			_baseSizeType	newLen;
			_baseSizeType	distBetweenBeginAndEnd;
			_baseSizeType	distBetweenBeginAndInsertPos;
			_baseSizeType	copyStart;
			_base			newBase;
			bool			countEnd;

			countEnd = _end.second != 0;
			distBetweenBeginAndEnd = _end.first - _begin.first;
			distBetweenBeginAndInsertPos = insertPos - _begin.first;
			newLen = distBetweenBeginAndEnd + countEnd + lenToAdd;
			if (_begin.first > newLen)
				newLen = _begin.first;
			copyStart = newLen - _begin.first;
			newBase.assign(3 * newLen, 0);
			idxNew = 0;

			while (idxNew < copyStart)
				newBase[idxNew++] = _getNewChunk();
			for (typename _base::const_iterator cit = _chunks.begin(); cit != _chunks.end(); ++cit)
				newBase[idxNew++] = *cit;
			while (idxNew < newBase.size())
				newBase[idxNew++] = _getNewChunk();
			
			newBase.swap(_chunks);
			_begin.first = newLen;
			_end.first = newLen + distBetweenBeginAndEnd;
			insertPos = newLen + distBetweenBeginAndInsertPos;
		}

		void	_moveDataRight(const _edge& position, size_type holeSize)
		{
			_edge	src = _end;
			_edge	dest = _end;

			_edgeSub(src, 1);
			_edgeAdd(dest, holeSize - 1);
			while (src >= position)
			{
				_alloc.construct(_chunks[dest.first] + dest.second, *(_chunks[src.first] + src.second));
				_alloc.destroy(_chunks[src.first] + src.second);
				_edgeSub(dest, 1);
				_edgeSub(src, 1);
			}

			_edgeAdd(_end, holeSize);
		}

		void	_moveChunksRight(const _edge& position, _baseSizeType jump)
		{
			int		copyStart(0);
			int		copyEnd(_chunkSize);
			bool	needToCopy(false);
			bool	copyWay(false);
			_baseSizeType	dest;
			_baseSizeType	src;

			/*
			* copyWay :
			* if false :
			*        a             cpy                                     a
			*   [ooovooooo] => [ooo~~~~~] + (jump - 1) * [~~~~~~~~] + [~~~ooooo]
			* if true :
			*        a              a                                     cpy
			*   [ooooovooo] => [ooooo~~~] + (jump - 1) * [~~~~~~~~] + [~~~~~ooo]
			*/

			// For cases I need to copy value_types, I set indexes to copy as less as possible datas
			if (position.second)
			{
				needToCopy = true;
				if (position.first == _end.first)
					copyEnd = _end.second;
				if (position.first == _begin.first)
					copyStart = _begin.second;
				if (position.second >= (copyEnd - copyStart) / 2)
					copyWay = true;
			}

			// Here I set the empty chunks I'll swap with chunks that I have to right shift
			src = _end.first - 1;
			dest = _end.first + jump - 1;
			if (_end.second)
			{
				++src;
				++dest;
			}
			// I have 3 possibles cases which each needs a proper algorythm
			// case 1 : needToCopy == false
			if (!copyWay)
			{
				while (src >= position.first)
					_swapChunk(src--, dest--);
				if (needToCopy)
				{
					/* here we are
					*       cpy                                    a
					*	[~~~~~~~~] + (jump - 1) * [~~~~~~~~] + [oooooooo]
					* that we want
					*       cpy                                    a
					*   [ooo~~~~~] + (jump - 1) * [~~~~~~~~] + [~~~ooooo]
					* steps :
					* 1 - find cpy and a indexes
					* 2 - do the constructs and destroys
					* I'll reuse dest for cpy and dest for a
					*/
					dest = position.first;
					src = dest + jump;
					for (int i = copyStart; i < position.second; ++i)
						_constructDataChunk(_chunks[dest], i, _chunks[src][i]);
					_destroyDataChunk(_chunks[src], copyStart, position.second - copyStart);
				}
			}
			else
			{
				/*
				*        a              a                                     cpy
				*   [ooooovooo] => [ooooo~~~] + (jump - 1) * [~~~~~~~~] + [~~~~~ooo]
				*/
				while (src > position.first)
					_swapChunk(src--, dest--);
				src = position.first;
				dest = src + jump;
				for (int i = position.second; i < copyEnd; ++i)
					_constructDataChunk(_chunks[dest], i, _chunks[src][i]);
				_destroyDataChunk(_chunks[src], position.second, copyEnd - position.second);
			}
			_end.first += jump;
		}

		void	_moveRight(_edge& position, size_type holeSize)
		{
			_baseSizeType	chunkNeed;
			size_type		available;

			if (holeSize)
			{
				chunkNeed = holeSize / _chunkSize;
				available = _chunkSize * (_chunks.size() - _end.first) - _end.second;
				if (available < holeSize)
				{
					if (_chunks.capacity() > 2 && _chunks.capacity() * _chunkSize > 3 * (_size + holeSize))
						_reorderBaseRight(chunkNeed + 1, position.first); //Only repositionning the chunks, _begin and _end
					else
						_reallocBaseRight(chunkNeed + 1, position.first); //Allocate bigger _base, copy _chunks, repositionning _begin and _end
				}
				if (position == _end)
				{
					_edgeAdd(_end, holeSize);
					return;
				}
				if (holeSize % _chunkSize)
					_moveDataRight(position, holeSize);
				else
					_moveChunksRight(position, chunkNeed);
			}
		}

		void	_reallocBaseLeft(_baseSizeType lenToAdd, _baseSizeType& insertPos)
		{
			_baseSizeType	idxNew;
			_baseSizeType	newLen;
			_baseSizeType	distBetweenBeginAndEnd;
			_baseSizeType	distBetweenBeginAndInsertPos;
			_baseSizeType	copyStart;
			_base			newBase;
			bool			countEnd;

			countEnd = _end.second != 0;
			distBetweenBeginAndEnd = _end.first - _begin.first;
			distBetweenBeginAndInsertPos = insertPos - _begin.first;
			newLen = distBetweenBeginAndEnd + countEnd + lenToAdd;
			if (_chunks.size() - _end.first >= newLen)
				newLen = _chunks.size() - _end.first;
			copyStart = newLen - _begin.first + lenToAdd;
			newBase.assign(3 * newLen, 0);
			idxNew = 0;

			while (idxNew < copyStart)
				newBase[idxNew++] = _getNewChunk();
			for (typename _base::const_iterator cit = _chunks.begin(); cit != _chunks.end(); ++cit)
				newBase[idxNew++] = *cit;
			while (idxNew < newBase.size())
				newBase[idxNew++] = _getNewChunk();

			newBase.swap(_chunks);
			_begin.first = newLen + lenToAdd;
			_end.first = _begin.first + distBetweenBeginAndEnd;
			insertPos = _begin.first + distBetweenBeginAndInsertPos;
		}

		void	_reorderBaseLeft(_baseSizeType lenToAdd, _baseSizeType& insertPos)
		{
			_baseSizeType	distBetweenBeginAndEnd;
			_baseSizeType	distBetweenBeginAndInsertPos;
			_baseSizeType	newBeginFirst;
			_baseSizeType	srcIdx;
			_baseSizeType	destIdx;
			bool			countEnd;

			//_printMemory("before");

			countEnd = _end.second != 0;
			distBetweenBeginAndEnd = _end.first - _begin.first;
			distBetweenBeginAndInsertPos = insertPos - _begin.first;
			newBeginFirst = distBetweenBeginAndEnd + countEnd + (2 * lenToAdd);
			destIdx = newBeginFirst + distBetweenBeginAndEnd - !countEnd;

			/*std::cout << "newBeginFirst " << newBeginFirst << " | "
			<< "distBetweenBeginAndEnd " <<  distBetweenBeginAndEnd << " | "
			<< "distBetweenBeginAndInsertPos " << distBetweenBeginAndInsertPos << " | "
			<< "destIdx " << destIdx << " | "
			<< "countEnd " << countEnd << '\n';*/
			if (_end.second)
				_swapChunk(destIdx--, _end.first);
			//_printMemory("before crash");
			srcIdx = _end.first - 1 + countEnd;
			while (srcIdx > _begin.first)
			{
				_swapChunk(destIdx--, srcIdx);
				--srcIdx;
			}
			_swapChunk(destIdx, srcIdx);

			_begin.first = newBeginFirst;
			_end.first = newBeginFirst + distBetweenBeginAndEnd;
			insertPos = newBeginFirst + distBetweenBeginAndInsertPos;
			//_printMemory("after");
		}

		void	_moveDataLeft(_edge& position, size_type holeSize)
		{
			_edge	src = _begin;
			_edge	dest = _begin;

			_edgeSub(dest, holeSize);
			while (src < position)
			{
				_alloc.construct(_chunks[dest.first] + dest.second, *(_chunks[src.first] + src.second));
				_alloc.destroy(_chunks[src.first] + src.second);
				_edgeAdd(dest, 1);
				_edgeAdd(src, 1);
			}
			_edgeSub(_begin, holeSize);
			_edgeSub(position, holeSize);
		}

		void	_moveChunksLeft(_edge& position, _baseSizeType jump)
		{
			int		copyStart(0);
			int		copyEnd(_chunkSize);
			bool	needToCopy(false);
			bool	copyWay(false);
			_baseSizeType	dest;
			_baseSizeType	src;

			/*
			* copyWay :
			* if false :
			*       cpy                                     a             a
			*   [ooo~~~~~] + (jump - 1) * [~~~~~~~~] + [~~~ooooo] <= [ooovooooo]
			* if true :
			*        a                                     cpy            a
			*   [ooooo~~~] + (jump - 1) * [~~~~~~~~] + [~~~~~ooo] <= [ooooovooo]
			*/

			// For cases I need to copy value_types, I set indexes to copy as less as possible datas
			if (position.second)
			{
				needToCopy = true;
				if (position.first == _end.first)
					copyEnd = _end.second;
				if (position.first == _begin.first)
					copyStart = _begin.second;
				if (position.second > (copyEnd - copyStart) / 2)
					copyWay = true;
			}

			// Here I set the empty chunks I'll swap with chunks that I have to left shift
			src = _begin.first;
			dest = _begin.first - jump;
			// I have 3 possibles cases which each needs a proper algorythm
			// case 1 : needToCopy == false
			if (!copyWay)
			{
				while (src < position.first)
					_swapChunk(src++, dest++);
				if (needToCopy)
				{
					/* here we are
					*       cpy                                     a
					*	[~~~~~~~~] + (jump - 1) * [~~~~~~~~] + [ooovooooo]
					* that we want
					*       cpy                                     a             a
					*   [ooo~~~~~] + (jump - 1) * [~~~~~~~~] + [~~~ooooo] <= [ooovooooo]
					* steps :
					* 1 - find cpy and a indexes
					* 2 - do the constructs and destroys
					* I'll reuse dest for cpy and dest for a
					*/
					src = position.first;
					dest = src - jump;
					for (int i = copyStart; i < position.second; ++i)
						_constructDataChunk(_chunks[dest], i, _chunks[src][i]);
					_destroyDataChunk(_chunks[src], copyStart, position.second - copyStart);
				}
			}
			else
			{
				while (src <= position.first)
					_swapChunk(src++, dest++);
				dest = position.first;
				src = dest - jump;
				for (int i = position.second; i < copyEnd; ++i)
					_constructDataChunk(_chunks[dest], i, _chunks[src][i]);
				_destroyDataChunk(_chunks[src], position.second, copyEnd - position.second);
			}
			_begin.first -= jump;
			position.first -= jump;
		}

		void	_moveLeft(_edge& position, size_type holeSize)
		{
			_baseSizeType	chunkNeed;
			size_type		available;

			if (holeSize)
			{
				chunkNeed = holeSize / _chunkSize;
				available = _chunkSize * (_begin.first) + _begin.second;
				if (available < holeSize)
				{
					if (_chunks.capacity() > 2 && _chunks.capacity() * _chunkSize > 3 * (_size + holeSize))
						_reorderBaseLeft(chunkNeed + 1, position.first); //Only repositionning the chunks, _begin and _end
					else
						_reallocBaseLeft(chunkNeed + 1, position.first); //Allocate bigger _base, copy _chunks, repositionning _begin and _end
				}
				if (position == _begin)
				{
					_edgeSub(_begin, holeSize);
					_edgeSub(position, holeSize);
					return;
				}
				if (holeSize % _chunkSize)
					_moveDataLeft(position, holeSize);
				else
					_moveChunksLeft(position, chunkNeed);
			}
		}

		void	_joinDatasRightToLeft(_edge first, difference_type range)
		{
			/*
			* [left] [~~~~] [right]
			*          <======
			* [left] [right]
			*/
			_edge	src;

			src = first;
			_edgeAdd(src, range);
			while (src < _end)
			{
				_constructDataChunk(_chunks[first.first], first.second, _chunks[src.first][src.second]);
				_destroyDataChunk(_chunks[src.first], src.second, 1);
				_edgeAdd(first, 1);
				_edgeAdd(src, 1);
			}
		}

		void	_joinChunksRightToLeft(_edge first, _edge last)
		{
			bool	haveToJoin(false);
			bool	joinWay(false);
			int		copyStart(0);
			int		copyEnd(_chunkSize);
			/*
			* [left] [~~~~] [right]
			*          <======
			* [left] [right]
			* 
			*  leftside     first          last   rightside
			* [oooooooo][oo~~~~~~][...][~~oooooo][oooooooo]
			* 
			* 
			* steps:
			* if last == end() just return
			* if first.second, set the indexes to join the chunk first and last to one later
			*/

			if (last == _end)
				return;

			if (first.second)
			{
				haveToJoin = true;
				if (first.first == _begin.first)
					copyStart = _begin.second;
				if (last.first == _end.first)
					copyEnd = _end.second;
				if (first.second > (copyEnd - copyStart) / 2)
					joinWay = true;
			}
			if (!joinWay)
			{
				if (haveToJoin)
				{
					//  first           last
					//[ooo~~~~~][...][~~~ooooo]
					//  last            ?
					//[oooooooo][...][????????]
					for (int i = copyStart; i < last.second; ++i)
						_constructDataChunk(_chunks[last.first], i, _chunks[first.first][i]);
					_destroyDataChunk(_chunks[first.first], copyStart, last.second - copyStart);
				}
				while (last.first < _end.first)
				{
					_swapChunk(first.first, last.first);
					++first.first;
					++last.first;
				}
				if (_end.second)
					_swapChunk(first.first, last.first);
			}
			else
			{
				//  first           last
				//[ooooo~~~][...][~~~~~ooo]
				//  first           ?
				//[oooooooo][...][????????]
				for (int i = last.second; i < copyEnd; ++i)
					_constructDataChunk(_chunks[first.first], i, _chunks[last.first][i]);
				_destroyDataChunk(_chunks[last.first], last.second, copyEnd - last.second);
				if (last.first == _end.first)
					return;
				++first.first;
				++last.first;
				while (last.first < _end.first)
				{
					_swapChunk(first.first, last.first);
					++first.first;
					++last.first;
				}
				if (_end.second)
					_swapChunk(first.first, last.first);
			}
		}

		void	_joinRightToLeft(_edge first, _edge last, difference_type range)
		{
			if (range % _chunkSize)
				_joinDatasRightToLeft(first, range);
			else
				_joinChunksRightToLeft(first, last);
			_edgeSub(_end, range);
		}

		void	_joinDatasLeftToRight(_edge first, _edge last)
		{
			/*
			* [left] [~~~~] [right]
			*    =======>
			*        [left] [right]
			*/
			_edge	src;
			_edge	dest;

			if (first == _begin)
				return;
			src = first;
			dest = last;
			_edgeSub(src, 1);
			_edgeSub(dest, 1);
			while (src > _begin)
			{
				_constructDataChunk(_chunks[dest.first], dest.second, _chunks[src.first][src.second]);
				_destroyDataChunk(_chunks[src.first], src.second, 1);
				_edgeSub(dest, 1);
				_edgeSub(src, 1);
			}
			_constructDataChunk(_chunks[dest.first], dest.second, _chunks[src.first][src.second]);
			_destroyDataChunk(_chunks[src.first], src.second, 1);
		}

		void	_joinChunksLeftToRight(_edge first, _edge last)
		{
			bool	haveToJoin(false);
			bool	joinWay(false);
			int		copyStart(0);
			int		copyEnd(_chunkSize);
			/*
			* [left] [~~~~] [right]
			*    =======>
			*        [left] [right]
			*
			*  leftside     first          last   rightside
			* [oooooooo][oo~~~~~~][...][~~oooooo][oooooooo]
			*
			*
			* steps:
			* if first == begin() just return
			* if first.second, set the indexes to join the chunk first and last to one later
			*/

			if (first == _begin)
				return;

			if (first.second)
			{
				haveToJoin = true;
				if (first.first == _begin.first)
					copyStart = _begin.second;
				if (last.first == _end.first)
					copyEnd = _end.second;
				if (first.second < (copyEnd - copyStart) / 2)
					joinWay = true;
			}
			if (!joinWay)
			{
				if (haveToJoin)
				{
					//  first           last
					//[ooo~~~~~][...][~~~ooooo]
					//  first           last
					//[~~~~~~~~][...][oooooooo]
					for (int i = copyStart; i < last.second; ++i)
						_constructDataChunk(_chunks[last.first], i, _chunks[first.first][i]);
					_destroyDataChunk(_chunks[first.first], copyStart, last.second - copyStart);
				}
				--first.first;
				--last.first;
				while (first.first >= _begin.first)
				{
					_swapChunk(first.first, last.first);
					--first.first;
					--last.first;
				}
			}
			else
			{
				//  first           last
				//[ooooo~~~][...][~~~~~ooo]
				//  first           last
				//[oooooooo][...][~~~~~~~~]
				for (int i = last.second; i < copyEnd; ++i)
					_constructDataChunk(_chunks[first.first], i, _chunks[last.first][i]);
				_destroyDataChunk(_chunks[last.first], last.second, copyEnd - last.second);
				while (first.first >= _begin.first)
				{
					_swapChunk(first.first, last.first);
					--first.first;
					--last.first;
				}
			}
		}

		void	_joinLeftToRight(_edge first, _edge last, difference_type range)
		{
			if (range % _chunkSize)
				_joinDatasLeftToRight(first, last);
			else
				_joinChunksLeftToRight(first, last);
			_edgeAdd(_begin, range);
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

			if (n > _alloc.max_size())
				throw(ft::length_error("ft::deque::constructor (fill) : too much elements."));
			if (_size)
			{
				size = _size / _chunkSize;
				if (_size % _chunkSize)
					++size;
				_chunks.assign(size, 0);
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
				_chunks.assign(size, 0);
				for (const_iterator cit = o.begin(); cit != o.end(); ++cit)
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
				_chunks.assign(baseSize, 0);
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

		deque&	operator=(const deque& o)
		{
			if (&o == this)
				return *this;
			assign(o.begin(), o.end());
			return *this;
		}

		//DESTRUCTOR

		~deque()
		{
			for (iterator it = begin(); it != end(); ++it)
			{
				_alloc.destroy(&(*it));
			}
			_clearBase(_chunks);
		}

		//BEGIN END ITERATORS REVERSE_ITERATORS

		iterator	begin() {
			return iterator(_chunks.begin() + _begin.first, _begin.second);
		}
		const_iterator begin() const {
			return const_iterator(_chunks.begin() + _begin.first, _begin.second);
		}
		iterator	end() {
			return iterator(_chunks.begin() + _end.first, _end.second);
		}
		const_iterator	end() const {
			return const_iterator(_chunks.begin() + _end.first, _end.second);
		}
		reverse_iterator		rbegin() { return reverse_iterator(end()); }
		const_reverse_iterator	rbegin() const { return const_reverse_iterator(end()); }
		reverse_iterator		rend() { return reverse_iterator(begin()); }
		const_reverse_iterator	rend() const { return const_reverse_iterator(begin()); }

		//CAPACITY

		size_type	size() const { return _size; }
		bool		empty() const { return !_size; }
		size_type	max_size() const { return _alloc.max_size(); }
		void		resize(size_type n, const value_type& val = value_type())
		{
			iterator it;

			if (n < _size)
			{
				it = end();
				_edgeSub(_end, (_size - n));
				while (_size > n)
				{
					--_size;
					_alloc.destroy(&(*(--it)));
				}
			}
			else if (n > _size)
				insert(end(), n - _size, val);
		}

		//ELEMENT ACCESS

		reference		front()							{ return _chunks[_begin.first][_begin.second]; }
		const_reference	front()	const					{ return _chunks[_begin.first][_begin.second]; }
		reference		back()							{ return *(end() - 1); }
		const_reference	back()	const					{ return *(end() - 1); }
		reference		operator[](size_type n)			{ return *(begin() + n); }
		const_reference	operator[](size_type n)	const	{ return *(begin() + n); }
		reference		at(size_type n)
		{
			if (n >= _size)
				throw(ft::out_of_range("this element is out of range"));
			return *(begin() + n);
		}
		const_reference		at(size_type n)	const
		{
			if (n >= _size)
				throw(ft::out_of_range("this element is out of range"));
			return *(begin() + n);
		}
		//MODIFIERS

		void	insert(iterator position, size_type n, const value_type& val)
		{
			_edge	ePosition;

			if (position < begin() || position > end())
				throw(ft::out_of_range("ft::deque::insert : iterator is out of range"));

			if (!n)
				return;

			ePosition = _edgeCastFromIterator(position);
			/*std::string printmsg = "Before insert Insertpos = (";
			printmsg += std::to_string(ePosition.first);
			printmsg += ", ";
			printmsg += std::to_string(ePosition.second);
			printmsg += ") , n = ";
			printmsg += std::to_string(n);
			_printMemory(printmsg.c_str());*/
			if (_isRightSide(position))
				_moveRight(ePosition, n);
			else
				_moveLeft(ePosition, n);
			_size += n;
			while (n--)
			{
				_alloc.construct(_chunks[ePosition.first] + ePosition.second, val);
				_edgeAdd(ePosition, 1);
			}
		}

		iterator	insert(iterator position, const value_type& val)
		{
			_edge	ePosition;

			if (position < begin() || position > end())
				throw(ft::out_of_range("ft::deque::insert : iterator is out of range"));

			ePosition = _edgeCastFromIterator(position);
			if (_isRightSide(position))
				_moveRight(ePosition, 1);
			else
				_moveLeft(ePosition, 1);
			++_size;
			_alloc.construct(_chunks[ePosition.first] + ePosition.second, val);
			return iterator(_chunks.begin() + ePosition.first, ePosition.second);
		}

		template <class InputIterator>
		void	insert(iterator position, InputIterator first, InputIterator last,
			typename enable_if<!ft::is_integral< InputIterator >::value >::type* = 0)
		{
			_edge ePosition;
			difference_type	dist;

			if (position < begin() || position > end())
				throw(ft::out_of_range("ft::deque::insert : iterator is out of range."));

			dist = ft::distance<InputIterator>(first, last);
			if (dist < 0)
				throw(ft::length_error("ft::deque::insert : inputIterator \"first\" can't be higher than \"last\"."));
			if (!dist)
				return;

			ePosition = _edgeCastFromIterator(position);
			if (_isRightSide(position))
				_moveRight(ePosition, dist);
			else
				_moveLeft(ePosition, dist);
			_size += dist;
			while (first != last)
			{
				_alloc.construct(_chunks[ePosition.first] + ePosition.second, *first);
				++first;
				_edgeAdd(ePosition, 1);
			}
		}

		iterator	erase(iterator first, iterator last)
		{
			_edge			ePositionFirst;
			_edge			ePositionLast;
			difference_type	rangeDeleted;

			if (first < begin() || last > end() || last < first)
				throw(ft::out_of_range("ft::deque::erase : bad iterator ranges"));
			
			rangeDeleted = last - first;
			if (!rangeDeleted)
				return first;

			ePositionFirst = _edgeCastFromIterator(first);
			ePositionLast = _edgeCastFromIterator(last);
			for (iterator it = first; it != last; ++it)
				_alloc.destroy(&(*(it)));

			if (first - begin() < end() - last)
			{
				_joinLeftToRight(ePositionFirst, ePositionLast, rangeDeleted);
				first += rangeDeleted;
			}
			else
				_joinRightToLeft(ePositionFirst, ePositionLast, rangeDeleted);

			_size -= rangeDeleted;
			if (!_size)
			{
				_begin.first = _chunks.size() / 2;
				_begin.second = _chunks.size() % 2 ? _chunkSize / 2 : 0;
				_end = _begin;
				first = iterator(_chunks.begin() + _end.first, _end.second);
			}
			return first;
		}

		iterator	erase(iterator position)
		{
			return erase(position, position + 1);
		}

		void clear()
		{
			erase(begin(), end());
		}

		template < class InputIterator >
		void		assign(InputIterator first, InputIterator last, \
			typename ft::enable_if< !ft::is_integral< InputIterator >::value >::type* = 0)
		{
			clear();
			insert(end(), first, last);
		}

		void	assign(size_type n, const value_type& val)
		{
			clear();
			insert(end(), n, val);
		}

		void	push_back(const value_type& val)
		{
			insert(end(), val);
		}

		void	push_front(const value_type& val)
		{
			insert(begin(), val);
		}

		void	pop_back()
		{
			--_size;
			_edgeSub(_end, 1);
			_alloc.destroy(&(_chunks[_end.first][_end.second]));
		}

		void	pop_front()
		{
			_alloc.destroy(&(_chunks[_begin.first][_begin.second]));
			--_size;
			_edgeAdd(_begin, 1);
		}

		void	swap(deque& other)
		{
			_edge		beginTmp;
			_edge		endTmp;
			size_type	sizeTmp;

			beginTmp = _begin;
			endTmp = _end;
			sizeTmp = _size;

			_begin = other._begin;
			_end = other._end;
			_size = other._size;

			other._begin = beginTmp;
			other._end = endTmp;
			other._size = sizeTmp;

			_chunks.swap(other._chunks);
		}

		//ALLOCATOR

		allocator_type	get_allocator()	const { return _alloc; }
	};

	//NON_MEMBER OVERLOADS

	template <class T, class Alloc>  bool operator==(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
	{
		if (lhs.size() != rhs.size())
			return false;
		return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <class T, class Alloc>  bool operator!=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class T, class Alloc>  bool operator<(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, class Alloc>  bool operator>(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
	{
		return (rhs < lhs);
	}

	template <class T, class Alloc>  bool operator<=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}


	template <class T, class Alloc>  bool operator>=(const deque<T, Alloc>& lhs, const deque<T, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	template <class T, class Alloc>  void swap(deque<T, Alloc>& x, deque<T, Alloc>& y) { x.swap(y); }

}

#endif