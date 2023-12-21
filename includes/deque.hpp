#ifndef DEQUE_HPP
# define DEQUE_HPP

# include <memory>
# include <cstddef>

# include "iterator.hpp"
# include "reverse_iterator.hpp"
# include "is_integral.hpp"
# include "enable_if.hpp"
# include "exception.hpp"

namespace ft
{
	template<class T, class Alloc = std::allocator<T> >
	class deque
	{

	public:
		//TYPEDEFS

		typedef T			value_type;
		typedef Alloc		allocator_type;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef ptrdiff_t	difference_type;
		typedef size_t		size_type;

	private:
		//PRIVATE MEMBERS

		pointer			_firstPtr;
		allocator_type	_al;
		size_type		_size;
		size_type		_capacity;
		size_type		_max_size;
		pointer			_trueFirstPtr;

	public:
		//ITERATORS

		class MyConstIterator;
		class MyIterator : public ft::iterator< ft::random_access_iterator_tag, value_type >
		{
		public:

			MyIterator() : _p(0) {}
			explicit MyIterator(pointer ptr) : _p(ptr) {}
			MyIterator(const MyIterator& other) : _p(other._p) {}
			~MyIterator() {}

			bool	operator==(const MyIterator& other)	const
			{
				return (_p == other._p);
			}

			bool	operator==(const MyConstIterator& other)	const
			{
				return (_p == other.base());
			}

			bool	operator!=(const MyIterator& other)	const
			{
				return (_p != other._p);
			}

			bool	operator!=(const MyConstIterator& other)	const
			{
				return (_p != other.base());
			}

			MyIterator& operator=(const MyIterator& other)
			{
				if (other == *this)
					return (*this);
				this->_p = other._p;
				return (*this);
			}

			MyIterator& operator=(const MyConstIterator& other)
			{
				if (other == *this)
					return (*this);
				this->_p = other.base();
				return (*this);
			}

			MyIterator& operator+=(difference_type val)
			{
				this->_p += val;
				return (*this);
			}

			MyIterator& operator-=(difference_type val)
			{
				this->_p -= val;
				return (*this);
			}

			reference	operator*()
			{
				return (*_p);
			}

			pointer	operator->()
			{
				return (_p);
			}

			MyIterator& operator++()
			{
				this->_p += 1;
				return (*this);
			}

			MyIterator	operator++(int)
			{
				MyIterator	tmp(*this);
				this->_p += 1;
				return (tmp);
			}

			MyIterator& operator--()
			{
				this->_p -= 1;
				return (*this);
			}

			MyIterator	operator--(int)
			{
				MyIterator	tmp(*this);
				this->_p -= 1;
				return (tmp);
			}

			MyIterator	operator+(difference_type val)	const
			{
				MyIterator	newIt;

				newIt._p = this->_p + val;
				return (newIt);
			}

			MyIterator	operator-(difference_type val)	const
			{
				MyIterator	newIt;

				newIt._p = this->_p - val;
				return (newIt);
			}

			difference_type operator-(const MyIterator& other)	const
			{
				return (this->_p - other._p);
			}

			difference_type operator-(const MyConstIterator& other)	const
			{
				return (this->_p - other.base());
			}

			bool	operator<(const MyIterator& other)	const
			{
				return (this->_p < other._p);
			}

			bool	operator<(const MyConstIterator& other)	const
			{
				return (this->_p < other.base());
			}

			bool	operator<=(const MyIterator& other)	const
			{
				return (this->_p <= other._p);
			}

			bool	operator<=(const MyConstIterator& other)	const
			{
				return (this->_p <= other.base());
			}

			bool	operator>(const MyIterator& other)	const
			{
				return (this->_p > other._p);
			}

			bool	operator>(const MyConstIterator& other)	const
			{
				return (this->_p > other.base());
			}

			bool	operator>=(const MyIterator& other)	const
			{
				return (this->_p >= other._p);
			}

			bool	operator>=(const MyConstIterator& other)	const
			{
				return (this->_p >= other.base());
			}

			reference	operator[](int val)
			{
				return (*(_p + val));
			}

			virtual pointer base()	const { return (_p); }

			difference_type ptrDiff() const { return (reinterpret_cast<difference_type>(_p)); }

		private:

			value_type* _p;

		};

		class MyConstIterator : public ft::iterator< ft::random_access_iterator_tag, const value_type >
		{
		private:

			value_type* _p;

		public:

			MyConstIterator() : _p(0) {}
			explicit MyConstIterator(value_type* ptr) : _p(ptr) {}
			MyConstIterator(const MyConstIterator& other) : _p(other._p) {}
			MyConstIterator(const MyIterator& other) : _p(other.base()) {}
			~MyConstIterator() {}

			bool	operator==(const MyConstIterator& other)	const
			{
				if (_p != other._p)
					return (false);
				return (true);
			}

			bool	operator!=(const MyConstIterator& other)	const
			{
				if (_p != other._p)
					return (true);
				return (false);
			}

			MyConstIterator& operator=(const MyConstIterator& other)
			{
				if (&other == this)
					return (*this);
				this->_p = other._p;
				return (*this);
			}

			MyConstIterator& operator=(const MyIterator& other)
			{
				if (other == *this)
					return (*this);
				this->_p = other.base();
				return (*this);
			}

			MyConstIterator& operator+=(difference_type val)
			{
				this->_p += val;
				return (*this);
			}

			MyConstIterator& operator-=(difference_type val)
			{
				this->_p -= val;
				return (*this);
			}

			const_reference	operator*()
			{
				return (*_p);
			}

			const_pointer	operator->()
			{
				return (_p);
			}

			MyConstIterator& operator++()
			{
				this->_p += 1;
				return (*this);
			}

			MyConstIterator	operator++(int)
			{
				MyConstIterator	tmp(*this);
				this->_p += 1;
				return (tmp);
			}

			MyConstIterator& operator--()
			{
				this->_p -= 1;
				return (*this);
			}

			MyConstIterator	operator--(int)
			{
				MyConstIterator	tmp(*this);
				this->_p -= 1;
				return (tmp);
			}

			MyConstIterator	operator+(difference_type val)	const
			{
				MyConstIterator	newIt;

				newIt._p = this->_p + val;
				return (newIt);
			}

			MyConstIterator	operator-(difference_type val)	const
			{
				MyConstIterator	newIt;

				newIt._p = this->_p - val;
				return (newIt);
			}

			difference_type	operator-(const MyConstIterator& other)	const
			{
				return (this->_p - other._p);
			}

			difference_type	operator-(const MyIterator& other)	const
			{
				return (this->_p - other.base());
			}

			bool	operator<(const MyConstIterator& other)	const
			{
				if (this->_p < other._p)
					return (true);
				return (false);
			}

			bool	operator<=(const MyConstIterator& other)	const
			{
				if (this->_p <= other._p)
					return (true);
				return (false);
			}

			bool	operator>(const MyConstIterator& other)	const
			{
				if (this->_p > other._p)
					return (true);
				return (false);
			}

			bool	operator>=(const MyConstIterator& other)	const
			{
				if (this->_p >= other._p)
					return (true);
				return (false);
			}

			const_reference	operator[](int val)	const
			{
				return (*(_p + val));
			}

			virtual pointer base()	const { return (_p); }

		};
		typedef MyIterator	                                        iterator;
		typedef MyConstIterator										const_iterator;
		typedef ft::reverse_iterator<iterator>                      reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>				const_reverse_iterator;

		//DEQUE TOOLS
	private:

		template < class Size >
		void	fillString(std::string &str, const Size s)	const
		{
			if ((s / 10) > 0)
			{
				fillString(str, s / 10);
				fillString(str, s % 10);
			}
			if ((s / 10) == 0)
				str += s + '0';
		}

		template < class Size >
		std::string	size_tToString(const Size s)	const
		{
			std::string	str;

			fillString(str, s);
			return (str);
		}

		void	_allocate(size_type n, const_reference val)
		{
			if (!_trueFirstPtr)
			{
				_size = n;
				_capacity = 3 * n;
				_trueFirstPtr = _al.allocate(3 * n);
				_firstPtr = _trueFirstPtr + n;
				for (size_type idx = 0; idx < n; ++idx)
					_al.construct(_firstPtr + idx, val);
			}
		}

		void	_prepareFrontMemory(size_type holeSize, size_type holePosition)
		{
			size_type sizeToAdd = _size + 1;
			size_type firstPtrStart = _firstPtr - _trueFirstPtr;
			pointer trueFirstPtrTmp = _trueFirstPtr;
			size_type capacityTmp = _capacity;
			iterator beginTmp = begin();
			iterator endTmp = end();
			size_type idx = 0;

			while (sizeToAdd < holeSize)
				sizeToAdd *= 2;
			_trueFirstPtr = _al.allocate(_capacity + sizeToAdd);
			_capacity += sizeToAdd;
			_size += holeSize;
			_firstPtr = _trueFirstPtr + firstPtrStart + sizeToAdd - holeSize;
			while (beginTmp != endTmp)
			{
				if (idx == holePosition)
					idx += holeSize;
				_al.construct(_firstPtr + idx, *beginTmp);
				_al.destroy(beginTmp.operator->());
				++beginTmp;
				++idx;
			}
			_al.deallocate(trueFirstPtrTmp, capacityTmp);
		}

		void	_prepareBackMemory(size_type holeSize, size_type holePosition)
		{
			size_type sizeToAdd = _size + 1;
			size_type firstPtrStart = _firstPtr - _trueFirstPtr;
			pointer trueFirstPtrTmp = _trueFirstPtr;
			size_type capacityTmp = _capacity;
			iterator beginTmp = begin();
			iterator endTmp = end();
			size_type idx = 0;

			while (sizeToAdd < holeSize)
				sizeToAdd *= 2;
			_trueFirstPtr = _al.allocate(_capacity + sizeToAdd);
			_capacity += sizeToAdd;
			_size += holeSize;
			_firstPtr = _trueFirstPtr + firstPtrStart;
			while (beginTmp != endTmp)
			{
				if (idx == holePosition)
					idx += holeSize;
				_al.construct(_firstPtr + idx, *beginTmp);
				_al.destroy(beginTmp.operator->());
				++beginTmp;
				++idx;
			}
			_al.deallocate(trueFirstPtrTmp, capacityTmp);
		}

		iterator	_moveBlock(iterator start, iterator last, iterator here)
		{
			iterator	doubleFreeSecure;

			if (here == start || start == last)
				return (here);
			if (here < start)
			{
				doubleFreeSecure = start;
				while (start != last)
				{
					if (here >= begin() && here < doubleFreeSecure)
						_al.destroy(here.base());
					_al.construct(here.base(), *start);
					_al.destroy(start.operator->());
					here++;
					start++;
				}
			}
			else
			{
				doubleFreeSecure = last - 1;
				here += last - start - 1;
				while (last-- != start)
				{
					if (here < end() && here > doubleFreeSecure)
						_al.destroy(here.base());
					_al.construct(here.base(), *last);
					_al.destroy(last.operator->());
					--here;
				}
			}
			return (here);
		}

		void	_prepareMemory(size_type side, size_type n)
		{
			if (side <= _size / 2)
			{
				if (_firstPtr - _trueFirstPtr < n)
					_prepareFrontMemory(n, side);
				else
				{
					iterator here(_firstPtr - n);
					_moveBlock(begin(), begin() + side, here);
					_size += n;
					_firstPtr -= n;
				}
			}
			else
			{
				if (_firstPtr - _trueFirstPtr + _size + n > _capacity)
					_prepareBackMemory(n, side);
				else
				{
					iterator here(begin() + side + n);
					_moveBlock(begin() + side, end(), here);
					_size += n;
				}
			}
		}

		void	_insert(iterator position, size_type n, const_reference val)
		{
			ptrdiff_t side = position - begin();
			size_type idx = 0;

			if (side < 0 || side > _size)
				throw (ft::out_of_range("deque::insert : Insert position is out of range"));
			_prepareMemory(side, n);
			while (idx < n)
			{
				_al.construct(_firstPtr + side + idx, val);
				++idx;
			}
		}

		template < class Size >
		void	_fillString(std::string &str, const Size s)	const
		{
			if ((s / 10) > 0)
			{
				fillString(str, s / 10);
				fillString(str, s % 10);
			}
			if ((s / 10) == 0)
				str += s + '0';
		}

		template < class Size >
		std::string	_size_tToString(const Size s)	const
		{
			std::string	str;

			fillString(str, s);
			return (str);
		}

	public:
		//CONSTRUCTORS DESTRUCTOR OPERATOR=

		explicit deque(const allocator_type& alloc = allocator_type()) :
			_firstPtr(0),
			_al(alloc),
			_size(0),
			_capacity(0),
			_max_size(_al.max_size()),
			_trueFirstPtr(0)
		{}

		explicit deque(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type()) :
				_firstPtr(0),
				_al(alloc),
				_size(0),
				_capacity(0),
				_max_size(_al.max_size()),
				_trueFirstPtr(0)
		{
			if (n)
			{
				_allocate(n, val);
			}
		}

		template<class InputIterator>
		deque(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
			typename ft::enable_if< !ft::is_integral< InputIterator >::value >::type* = 0) :
				_firstPtr(0),
				_al(alloc),
				_size(0),
				_capacity(0),
				_max_size(_al.max_size()),
				_trueFirstPtr(0)
		{
			if (first != last)
			{
				difference_type dist = ft::distance<InputIterator>(first, last);
				size_type idx = 0;
				if (dist < 0)
					throw (ft::length_error("deque::deque(random_access_iterator first, random_access_iterator last) : First must be > last"));
				_size = dist;
				_capacity = 3 * _size;
				_trueFirstPtr = _al.allocate(3 * _size);
				_firstPtr = _trueFirstPtr + _size;
				while (first != last)
				{
					_al.construct(_firstPtr + idx, *first);
					++idx;
					++first;
				}
			}
		}
	
		deque(const deque& o) :
			_al(o._al),
			_size(o._size),
			_capacity(o._size),
			_max_size(_al.max_size())
		{
			if (!_size)
			{
				_trueFirstPtr = 0;
				_firstPtr = 0;
			}
			else
			{
				size_type idx = 0;
				_trueFirstPtr = _al.allocate(_size);
				_firstPtr = _trueFirstPtr;
				for (const_iterator cit = o.begin(); cit != o.end(); ++cit)
				{
					_al.construct(_firstPtr + idx, *cit);
					++idx;
				}
			}
		}

		~deque()
		{
			for (iterator it = begin(); it != end(); ++it)
				_al.destroy(it.operator->());
			_al.deallocate(_trueFirstPtr, _capacity);
		}

		deque& operator=(const deque& o)
		{
			if (*this != o)
				assign(o.begin(), o.end());
			return *this;
		}

		//ITERATOR ACCESSORS

		iterator				begin()				{ return iterator(_firstPtr); }
		iterator				end()				{ return iterator(&_firstPtr[_size]); }
		reverse_iterator		rbegin()			{ return reverse_iterator(end()); }
		reverse_iterator		rend()				{ return reverse_iterator(begin()); }
		const_iterator			begin()		const	{ return const_iterator(_firstPtr); }
		const_iterator			end()		const	{ return const_iterator(&_firstPtr[_size]); }
		const_reverse_iterator	rbegin()	const	{ return const_reverse_iterator(end()); }
		const_reverse_iterator	rend()		const	{ return const_reverse_iterator(begin()); }

		//INSERTS
		
		iterator insert(iterator position, const_reference val)
		{
			_insert(position, 1, val);
		}

		void insert(iterator position, size_type n, const_reference val)
		{
			_insert(position, n, val);
		}

		template<class InputIterator>
		void insert(iterator position, InputIterator first, InputIterator last,\
			typename ft::enable_if< !ft::is_integral< InputIterator >::value >::type* = 0)
		{
			ptrdiff_t side = position - begin();
			size_type idx;

			if (side < 0 || side > _size)
				throw (ft::out_of_range("deque::insert : Insert position is out of range"));
			if (first != last)
			{
				difference_type dist = ft::distance<InputIterator>(first, last);
				idx = 0;
				if (dist < 0)
					throw (ft::length_error("deque::insert : Inverted InputIterators"));
				_prepareMemory(side, dist);
				while (first != last)
				{
					_al.construct(_firstPtr + side + idx, *first);
					++first;
					++idx;
				}
			}
		}

		//ERASES

		iterator	erase(iterator position)	{ return erase(position, position + 1); }

		iterator	erase(iterator first, iterator last)
		{
			iterator	it;
			size_type	eraseLen;
			if (first < begin() || last > end())
				throw ft::out_of_range("deque::erase : one or both iterators are out of range");
			if (first > last)
				throw ft::length_error("deque::erase : inverted iterators");
			eraseLen = last - first;
			if (first - begin() < end() - last)
			{
				it = _moveBlock(begin(), first, last - first + begin());
				if (eraseLen > first - begin())
				{
					for (iterator ite = first; ite < last - first + begin(); ++ite)
						_al.destroy(it.operator->());
				}
				_firstPtr += eraseLen;
			}
			else
			{
				it = _moveBlock(last, end(), first);
				if (eraseLen > end() - last)
				{
					for (iterator ite = end() - eraseLen; ite < end() - last; ++ite)
						_al.destroy(ite->operator->());
				}
			}
			_size -= eraseLen;
			return it;
		}

		//CLEAR

		void	clear()	{ resize(0); }

		//ASSIGNS

		template <class InputIterator> 
		void	assign(InputIterator first, InputIterator last,\
			typename ft::enable_if< !ft::is_integral< InputIterator >::value >::type* = 0)
		{
			difference_type	dist = ft::distance<InputIterator>(first, last); 
			difference_type memDif;

			resize(0);
			if (dist < 0)
				throw ft::length_error("deque::assign : inverted iterators");
			memDif = _capacity - (_firstPtr - _trueFirstPtr) - dist;
			if (memDif < 0)
				_prepareBackMemory(-memDif, 0);
			while (first != last)
			{
				_al.construct(_firstPtr + _size, *first);
				++_size;
				++first;
			}
		}

		void	assign(size_type n, const value_type& val)
		{
			difference_type memDif;

			resize(0);
			if (n > _max_size)
				throw ft::length_error("deque::assign : n can't be > than max_size()");
			memDif = _capacity - (_firstPtr - _trueFirstPtr) - n;
			if (memDif < 0)
				_prepareBackMemory(-memDif, 0);
			while (_size < n)
			{
				_al.construct(_firstPtr + _size, val);
				++_size;
			}
		}

		//PUSH POP

		void	push_back(value_type& val)	{ insert(end(), val); }
		void	push_front(value_type& val)	{ insert(begin(), val); }
		void	pop_front()
		{
			if (_size)
			{
				_al.destroy(_firstPtr);
				if (!--_size)
					_firstPtr = _trueFirstPtr + _capacity / 2;
				else
					++_firstPtr;
			}
		}
		void	pop_back()
		{
			if (_size)
			{
				_al.destroy(&_firstPtr[_size - 1]);
				if (!--_size)
					_firstPtr = _trueFirstPtr + _capacity / 2;
			}
		}

		//SWAP

		void	swap(deque& other)
		{
			pointer			_firstPtrTmp		= _firstPtr;
			allocator_type	_alTmp				= _al;
			size_type		_sizeTmp			= _size;
			size_type		_capacityTmp		= _capacity;
			size_type		_max_sizeTmp		= _max_size;
			pointer			_trueFirstPtrTmp	= _trueFirstPtr;

			_firstPtr			= other._firstPtr;
			_al					= other._al;
			_size				= other._size;
			_capacity			= other._capacity;
			_max_size			= other._max_size;
			_trueFirstPtr		= other._trueFirstPtr;

			other._firstPtr		= _firstPtrTmp;
			other._al			= _alTmp;
			other._size			= _sizeTmp;
			other._capacity		= _capacityTmp;
			other._max_size		= _max_sizeTmp;
			other._trueFirstPtr	= _trueFirstPtrTmp;
		}

		//ELEMENT ACCESS

		reference		operator[](size_type n)			{ return _firstPtr[n]; }
		const_reference	operator[](size_type n)	const	{ return _firstPtr[n]; }
		reference		at(size_type n)
		{
			if (n >= _size)
			{
				std::string	err = "deque::at: n (which is ";
				err += size_tToString(n);
				err += ") >= this->size() (which is ";
				err += size_tToString(_size);
				err += ")";
				throw ft::out_of_range(err);
			}
			return (_firstPtr[n]);
		}
		const_reference	at(size_type n) const
		{
			if (n >= _size)
			{
				std::string	err = "deque::at: n (which is ";
				err += size_tToString(n);
				err += ") >= this->size() (which is ";
				err += size_tToString(_size);
				err += ")";
				throw ft::out_of_range(err);
			}
			return (_firstPtr[n]);
		}

		reference		front()			{ return _firstPtr[0]; }
		reference		back()			{ return _firstPtr[_size - 1]; }
		const_reference	front() const	{ return _firstPtr[0]; }
		const_reference	back()	const	{ return _firstPtr[_size - 1]; }

		//CAPACITY

		bool		empty()		const	{ return !_size; }
		size_type	size()		const	{ return _size; }
		size_type	max_size()	const	{ return _max_size; }

		void	resize(size_type n, value_type val = value_type())
		{
			if (n == _size)
				return ;
			if (n < _size)
			{
				while (_size > n)
				{
					_size--;
					_al.destroy(&_firstPtr[_size]);
				}
				if (!_size)
					_firstPtr = _trueFirstPtr + (_size / 2);
			}
			else
			{
				if (n > _capacity - (_firstPtr - _trueFirstPtr))
                    _prepareBackMemory(n - _size, _size);
				pointer ptr = &_firstPtr[_size];
				while (_size < n)
				{
					_al.construct(ptr, val);
					ptr++;
					_size++;
				}
			}
		}


	};


}

#endif