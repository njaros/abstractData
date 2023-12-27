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
* Documentation sources :
* - https://stackoverflow.com/questions/6292332/what-really-is-a-deque-in-stl
* - https://cplusplus.com/reference/deque/deque/
* idees :
* - la base est un vecteur de chunk = value_type[n] malloquées, n dependant du sizeof(value_type)
* - le vecteur contient autant de chunk vide (chunk malloqué mais non construit) avant et apres l'array de chunk pas vide
* apres chaque agrandissement du vecteur
* - tout element de chunk en dehors de begin() et end() doit etre detruit ou non construit (mais pas desalloué)
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

		class MyConstIterator : public ft::iterator< ft::bidirectional_iterator_tag, const value_type >
		{
			friend ft::deque<value_type>;
			friend MyIterator;

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

		_edge& _edgeSub(_edge& e, difference_type d)
		{
			return _edgeAdd(e, -d);
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

		void	_printMemory(const char* str = 0) const
		{
			std::cout << std::endl << "DEBUG---- start of printMemory ----DEBUG";
			if (str)
				std::cout << std::endl << str;
			std::cout << std::endl << "vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv" << std::endl;
			for (int i = 0; i != _chunks.size(); ++i)
			{
				printf("_chunks[%d] = %x", i, _chunks[i]);
				std::cout << " : vals = (";
				for (int j = 0; j < _chunkSize; ++j)
				{
					std::cout << _chunks[i][j];
					if (j != _chunkSize - 1)
						std::cout << ", ";
					else
						std::cout << ")" << std::endl;
				}
			}
			std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^";
			if (str)
				std::cout << std::endl << str;
			std::cout << std::endl << "DEBUG---- end of printMemory -----DEBUG" << std::endl << std::endl;
		}

		//CHUNK HANDLER FUNCTIONS

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

		std::pair<iterator, iterator>	_manageBase(_baseSizeType holeSize, _edge& edgeHole)
		{
			_baseSizeType	endAlone = _end != _begin && _end.second == 0 ? 1 : 0;
			_baseSizeType	sizeNeed = holeSize + (_end.first - _begin.first) + !endAlone;

			if (_chunks.capacity() >= 3 * sizeNeed)
				return _reorderBase(holeSize, edgeHole, sizeNeed);
			return _reallocBase(holeSize, edgeHole, sizeNeed);
		}

		std::pair<iterator, iterator>	_reorderBase(_baseSizeType holeSize, _edge& edgeHole, _baseSizeType sizeNeed)
		{
			/*
			* 0     1      2     3     4     5     6     7     8     9     10
			|~~oo||oooo||ovooo||~~~~||~~~~||~~~~||~~~~||~~~~||~~~~||~~~~||~~~~|
			   ^begin           ^end
			o = element construit entre [begin et end[ (end non compris)
			~ = element non construit ou detruit exterieur a ]begin et end] (end compris)
			v : n elements a inserer a la position edgeHole
			pour inserer ses n elements, il faut holesize block disponibles

			ici on a 11 bloc
			le bloc 0 contient le begin
			le bloc 3 contient le end
			on veut inserer holesize bloc vide |~~~~| quelque part entre begin et end
			on veut autant de bloc vide avant le begin qu apres le end apres le rangement
			on ne veut jamais allouer de la memoire, ni en liberer
			on ne veut pas toucher l'interieur des blocs sauf celui dans lequel on insere :

			|ovooo| => |o~~~| holesize x |~~~~||~ooo|

			on doit calculer le nbr de bloc utiliser avant insertion :
			- end.first - begin.first si end.second == 0
			- end.first - begin.first + 1 sinon
			puis ajouter le nbr de bloc a inserer holeSize et on obtient le sizeNeed

			determiner le sens de copie pour eviter le chevauchement :
			pas de chevauchement possible dans tout les cas dans ce contexte.

			position de debut de copie : _chunks.size() / 2 - sizeNeed / 2
			admettons holeSize vaut 1
			_chunks.size() / 2 - sizeNeed / 2 = 5 - 2 = 3 on commencera la copie au bloc 3
			  0     1     2     3     4     5     6     7     8     9     10
			|~~~~||~~~~||~~~~||~~oo||oooo||o~~~||~~~~||~ooo||~~~~||~~~~||~~~~|
								 ^begin                      ^end
			*/
			_baseSizeType	idx = _begin.first;
			_chunk			tmp;
			_baseSizeType	copyStart = (_chunks.size() / 2) - (sizeNeed / 2);
			int				distHoleFromBegin = edgeHole.first - _begin.first;
			int				constructStartL = edgeHole.first == _begin.first ? _begin.second : 0;
			int				constructEndR = edgeHole.first == _end.first ? _end.second : _chunkSize;
			_edge			nextBegin = ft::make_pair(copyStart, _begin.second);
			_edge			nextEnd;
			std::pair<iterator, iterator>		toReturn;

			nextEnd.first = nextBegin.first + sizeNeed;
			nextEnd.second = _end.second;

			if (_begin == _end)
			{
				_end = nextEnd;
				_begin = nextBegin;
				toReturn.first = iterator(_chunks.begin() + _begin.first, _begin.second);
				toReturn.second = iterator(_chunks.begin() + _end.first, _end.second);
				return toReturn;
			}

			while (idx != edgeHole.first)
				_swapChunk(copyStart++, idx++);
			/*
			* ici on va devoir construire sur un bloc vide et detruire sur un bloc construit
			* cas possibles :
			*
			* Cas ou edgeHole.first != begin.first != end.first :
			*
			*    a                           a
			* |voooo| => holesize x |~~~~||oooo| pas de construction / destruction et on insere avant
			*    a
			* |oooov| => cas impossible
			*    a          b                             a
			* |ovooo| => |o~~~| (holesize - 1) x |~~~~||~ooo| il est moins couteux d inserer avant a et de construire 1 seul element dans b : construire entre 0 et edgeHole.second
			*    a          a                             b
			* |ooovo| => |ooo~| (holesize - 1) x |~~~~||~~~o| inserer apres a et construire 1 seul element dans b : construire entre edgeHole.second et _chunkSize
			*    a
			* |oovoo| => l'un des deux precedents choix, sans preference
			*
			* Autres cas :
			*
			*    a                           a
			* |~~~vo| => holesize x |~~~~||~~~o| si edgeHole == begin on insert avant a
			*    a          b                             a
			* |~ovoo| => |~o~~| (holesize - 1) x |~~~~||~~oo| insertion dans le bloc de begin. construire entre begin.second et edgeHole.second
			*    a          a                             b
			* |~oovo| => |~oo~| (holesize - 1) x |~~~~||~~~o| insertion dans le bloc de begin. meme comportement que le cas lambda
			*    a          b                             a
			* |ovoo~| => |o~~~| (holesize - 1) x |~~~~||~oo~| insertion dans le bloc de end. meme comportement que le cas lambda
			*    a          a
			* |ooov~| => |ooo~| holesize x |~~~~| insertion dans le bloc de end. edgeHole == end => il ne faut rien faire
			*    a          b                             a
			* |~ovo~| => |~o~~| (holesize - 1) x |~~~~||~~o~| faire
			*    a          a
			* |v~~~~| => |~~~~| holesize x |~~~~| insertion dans le bloc de end. edgeHole == end => il ne faut rien faire
			*      a             a                                      b
			* |ooooovoo~| => |ooooo~~~| (holesize - 1) x |~~~~~~~~||~~~~~oo~| construire entre edgeHole.second et end.second
			*/
			if (!edgeHole.second || edgeHole == _begin)
			{
				copyStart += holeSize;
				_swapChunk(idx++, copyStart++);
			}
			else if (edgeHole == _end)
			{
				if (_end.second)
					_swapChunk(idx, copyStart);
				_end = nextEnd;
				_begin = nextBegin;
				toReturn.first = iterator(_chunks.begin() + (_begin.first + distHoleFromBegin), edgeHole.second);
				toReturn.second = iterator(_chunks.begin() + _end.first, _end.second);
				return toReturn;
			}
			else if (edgeHole.second <= _chunkSize / 2)
			{
				for (int i = constructStartL; i < edgeHole.second; ++i)
					_constructDataChunk(_chunks[copyStart], i, _chunks[idx][i]);
				_destroyDataChunk(_chunks[idx], constructStartL, edgeHole.second);
				copyStart += holeSize;
				_swapChunk(idx++, copyStart++);
			}
			else
			{
				tmp = _chunks[copyStart];
				_chunks[copyStart] = _chunks[idx];
				_chunks[idx] = tmp;
				tmp = _chunks[copyStart];
				copyStart += holeSize;
				for (int i = edgeHole.second; i < constructEndR; ++i)
					_constructDataChunk(_chunks[copyStart], i, tmp[i]);
				_destroyDataChunk(tmp, edgeHole.second, constructEndR - edgeHole.second);
				++idx;
				++copyStart;
			}
			while (idx <= _end.second)
				_swapChunk(idx++, copyStart++);
			_end = nextEnd;
			_begin = nextBegin;
			toReturn.first = iterator(_chunks.begin() + (_begin.first + distHoleFromBegin), edgeHole.second);
			nextEnd.first = _begin.first + distHoleFromBegin + holeSize;
			nextEnd.second = edgeHole.second;
			_edgeAdd(nextEnd, 1);
			toReturn.second = iterator(_chunks.begin() + nextEnd.first, nextEnd.second);
			return toReturn;
		}

		std::pair<iterator, iterator>	_reallocBase(_baseSizeType holeSize, _edge& edgeHole, _baseSizeType sizeNeed)
		{
			_baseSizeType	endAlone = _end.second == 0 && _end != _begin ? 1 : 0;
			_baseSizeType	distHoleFromBegin = edgeHole.first - _begin.first;
			_baseSizeType	preSpace = _begin.first;
			_baseSizeType	afterSpace = _chunks.size() - _end.first - endAlone;
			_baseSizeType	chunksBegin = 0;
			_baseSizeType	chunksEnd = _end.first + 1;
			_base			newBase(3 * sizeNeed, nullptr);
			_baseSizeType	idx = 0;
			std::pair<iterator, iterator>		toReturn;

			std::cout << "_begin = (" << _begin.first << ", " << _begin.second << ")\n";
			std::cout << "_end = (" << _end.first << ", " << _end.second << ")\n";

			_printMemory("Before reallocBase");
			while (idx < newBase.size())
			{
				if (idx < sizeNeed || idx >= 2 * sizeNeed)
				{
					if (idx < sizeNeed && idx >= sizeNeed - preSpace) {
						printf("idx = %u | chunksBegin = %u | chunksBeginAddr = %x\n", idx, chunksBegin, _chunks[chunksBegin]);
						newBase[idx] = _chunks[chunksBegin++];
					}
					else if (idx > 2 * sizeNeed && idx < (2 * sizeNeed) + afterSpace)
					{
						printf("idx = %u | chunksEnd = %u | chunksEndAddr = %x\n", idx, chunksEnd, _chunks[chunksEnd]);
						newBase[idx] = _chunks[chunksEnd++];
					}
					else
					{
						newBase[idx] = _getNewChunk();
					}
				}
				else
				{
					if (idx < edgeHole.first + sizeNeed)
						newBase[idx] = _chunks[idx - sizeNeed];
					else if (idx > edgeHole.first + sizeNeed + holeSize)
					{
						printf("idx = %u | sizeNeed = %u | holeSize = %u | newBase[idx] = %x | _chunks[idx - sizeNeed - holeSize] = %x\n", idx, sizeNeed,
							holeSize, newBase[idx], _chunks[idx - sizeNeed - holeSize]);
						newBase[idx] = _chunks[idx - sizeNeed - holeSize];
					}
					else
					{
						if (idx == edgeHole.first + sizeNeed)
						{
							newBase[idx] = _getNewChunk();
							for (int i = 0; i < edgeHole.second; ++i)
								_constructDataChunk(newBase[idx], i, _chunks[edgeHole.first][i]);
						}
						else if (idx == edgeHole.first + sizeNeed + holeSize)
						{
							newBase[idx] = _chunks[edgeHole.first];
							_destroyDataChunk(newBase[idx], 0, edgeHole.second);
						}
						else
							newBase[idx] = _getNewChunk();
					}
				}
				++idx;
			}
			newBase.swap(_chunks);
			toReturn.first = iterator(_chunks.begin() + sizeNeed + edgeHole.first, edgeHole.second);
			toReturn.second = iterator(_chunks.begin() + sizeNeed + edgeHole.first + holeSize, edgeHole.second);
			_edge debugFirst = _edgeCastFromIterator(toReturn.first);
			_edge debugSecond = _edgeCastFromIterator(toReturn.second);
			_begin.first = sizeNeed;
			_end.first = sizeNeed + holeSize;
			_end.second = 0;
			_edgeAdd(_end, _size);
			edgeHole.first += sizeNeed + distHoleFromBegin;
			_printMemory("After reallocBase");
			return toReturn;
		}

		void	_moveChunkLeft(iterator& position, _edge& edgeHole, _baseSizeType holeChunkSize)
		{
			_baseSizeType	chunkAvailable;
			_baseSizeType	idx;
			_chunk			tmp;

			chunkAvailable = _begin.first;
			if (_chunks.empty() || chunkAvailable < holeChunkSize)
			{
				position = _manageBase(holeChunkSize, edgeHole).first;
				return;
			}
			idx = chunkAvailable - holeChunkSize;
			_printMemory("BEFORE : _chunks adresses : ");
			while (idx < edgeHole.first - holeChunkSize)
			{
				printf("idx = %u | holeSize = %u | _chunks[idx] = %x | _chunks[idx + holeChunkSize] = %x\n", idx, holeChunkSize, _chunks[idx], _chunks[idx + holeChunkSize]);
				tmp = _chunks[idx];
				_chunks[idx] = _chunks[idx + holeChunkSize];
				_chunks[idx + holeChunkSize] = tmp;
				++idx;
			}
			for (int i = edgeHole.second - 1; i >= 0 && !(edgeHole.first == _begin.first && i < _begin.second); --i)
				_constructDataChunk(_chunks[idx], i, _chunks[idx + holeChunkSize][i]);
			_destroyDataChunk(_chunks[idx + holeChunkSize], 0, edgeHole.second);
			_begin.first -= holeChunkSize;
			position -= (_chunkSize * holeChunkSize);
			edgeHole.first -= holeChunkSize;
			_printMemory("AFTER : _chunks adresses : ");
		}

		void	_moveDataLeft(iterator& position, size_type holeSize) //insert hole BEFORE position
		{
			_baseSizeType	chunkNeed;
			difference_type	modulo;
			_edge			edgeHole;

			if (holeSize)
			{
				edgeHole = _edgeCastFromIterator(position);
				chunkNeed = holeSize / _chunkSize;
				modulo = holeSize % _chunkSize;
				if (modulo > _begin.second)
				{
					++chunkNeed;
					modulo -= _chunkSize;
				}
				_moveChunkLeft(position, edgeHole, chunkNeed);
				_moveModulo(position, _begin, edgeHole, -modulo);
				_begin.second -= modulo;
			}
		}

		void	_moveChunkRight(iterator& position, _edge& edgeHole, _baseSizeType holeChunkSize)
		{
			_baseSizeType	chunkAvailable;
			_baseSizeType	idx;
			_chunk			tmp;

			chunkAvailable = _chunks.size() - _end.first;
			if (_chunks.empty() || chunkAvailable < holeChunkSize)
			{
				position = _manageBase(holeChunkSize, edgeHole).second;
				edgeHole.first += holeChunkSize;
				return;
			}
			_end.first += holeChunkSize;
			idx = _end.first;
			while (idx >= edgeHole.first + holeChunkSize)
			{
				tmp = _chunks[idx];
				_chunks[idx] = _chunks[idx - holeChunkSize];
				_chunks[idx - holeChunkSize] = tmp;
				--idx;
			}
			for (int i = 0; i < edgeHole.second; ++i)
				_constructDataChunk(_chunks[idx + 1 - holeChunkSize], i, _chunks[idx + 1][i]);
			_destroyDataChunk(_chunks[idx + 1], 0, edgeHole.second);
			position += (_chunkSize * holeChunkSize);
		}

		void	_moveModulo(iterator& position, _edge start, _edge end, difference_type modulo)
		{
			_edge	here;

			if (modulo > 0)
			{
				_printMemory("Before MoveModulo with modulo > 0");
				here = end;
				_edgeAdd(here, modulo - 1);
				while (end != start)
				{
					_edgeSub(end, 1);
					_constructDataChunk(_chunks[here.first], here.second, _chunks[end.first][end.second]);
					_destroyDataChunk(_chunks[end.first], end.second, 1);
					_edgeSub(here, 1);
				}
				_printMemory("After MoveModulo with modulo > 0");
			}
			else if (modulo < 0)
			{
				_printMemory("Before MoveModulo with modulo < 0");
				here = start;
				_edgeSub(here, modulo);
				while (start != end)
				{
					_constructDataChunk(_chunks[here.first], here.second, _chunks[start.first][start.second]);
					_destroyDataChunk(_chunks[start.first], start.second, 1);
					_edgeAdd(start, 1);
					_edgeAdd(here, 1);
				}
				_printMemory("After MoveModulo with modulo < 0");
			}
			position += modulo;
		}

		void	_moveDataRight(iterator& position, size_type holeSize) //insert hole BEFORE position
		{
			_baseSizeType	chunkNeed;
			difference_type	modulo;
			_edge			edgeHole;

			if (holeSize)
			{
				edgeHole = _edgeCastFromIterator(position);
				chunkNeed = holeSize / _chunkSize;
				modulo = holeSize % _chunkSize;
				if (modulo + _end.second > _chunkSize)
				{
					++chunkNeed;
					modulo -= _chunkSize;
				}
				_moveChunkRight(position, edgeHole, chunkNeed);
				_moveModulo(position, edgeHole, _end, modulo);
				_end.second += modulo;
			}
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
				while (_size-- > n)
					_alloc.destroy(&(*(--it)));
			}
			else if (n > _size)
				insert(end(), n - _size, val);
		}

		//ELEMENT ACCESS

		//MODIFIERS

		void	insert(iterator it, size_type n, const value_type& val)
		{
			if (it < begin() || it > end())
				throw(ft::out_of_range("ft::deque::insert : iterator is out of range"));

			std::cout << "_chunkSize = " << _chunkSize << std::endl;
			if (_isRightSide(it))
			{
				_moveDataRight(it, n);
				_size += n;
				while (n--)
					_alloc.construct(&(*(--it)), val);
			}
			else
			{
				_moveDataLeft(it, n);
				_size += n;
				while (n--)
					_alloc.construct(&(*(it++)), val);
			}
		}

	};
}

#endif