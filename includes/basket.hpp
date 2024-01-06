#ifndef BASKET_HPP_ABSTRACTDATA_BY_LE_N
#define BASKET_HPP_ABSTRACTDATA_BY_LE_N

# include "vector.hpp"

namespace ft
{

	template <class T, class Alloc >
	class basket
	{

		friend class myIterator;

	public:

		typedef	Alloc	allocator_type;
		typedef T	value_type;
		typedef typename Alloc::reference reference;
		typedef typename Alloc::const_reference const_reference;
		typedef typename Alloc::pointer	pointer;
		typedef typename Alloc::const_pointer	const_pointer;
		typedef typename Alloc::difference_type	difference_type;
		typedef typename Alloc::size_type	size_type;
		typedef ft::vector< T, Alloc >	basket_type;
		typedef ft::vector< T, Alloc >	ground_type;
		typedef typename basket_type::iterator basket_iterator;
		typedef typename basket_type::const_iterator basket_const_iterator;
		typedef typename basket_type::reverse_iterator basket_reverse_iterator;
		typedef typename basket_type::const_reverse_iterator basket_const_reverse_iterator;
		typedef typename ground_type::iterator ground_iterator;
		typedef typename ground_type::const_iterator ground_const_iterator;
		typedef typename ground_type::reverse_iterator ground_reverse_iterator;
		typedef typename ground_type::const_reverse_iterator ground_const_reverse_iterator;

		class myConstIterator;
		//this iterator is here just because I have to provide an iterator able to browse all the data in the bucket and on the ground
		class myIterator : public ft::iterator< ft::random_access_iterator_tag, value_type, difference_type, pointer, reference >
		{
			friend basket;
			friend MyConstIterator;

			const basket_type* _b;
			const ground_type* _g;
			pointer _ptr;

			myIterator(const basket_type* b, const ground_type* g, pointer ptr) : _b(b), _g(g), _ptr(ptr) {}

			const_pointer _bptr() const
			{
				return *(*(_b->begin()));
			}

			const_pointer _gptr() const
			{
				return *(*(_g->begin()));
			}

			bool _isBasketSide() const
			{
				return (_ptr >= _bptr() && _ptr < _bptr() + _b->size());
			}

		public:

			myIterator() : _b(0), _g(0), _ptr(0) {}
			myIterator(const myIterator& o) : _b(o._b), _g(o._g), _ptr(o._ptr) {}
			~myIterator() {}
			myIterator& operator=(const myIterator& o)
			{
				if (&o != this)
				{
					_b = o._b;
					_g = o._g;
					_ptr = o._ptr;
				}
				return *this;
			}

			bool operator==(const myIterator& o) const
			{
				return _b == o._b && _g == o._g && _ptr == o._ptr;
			}

			bool operator==(const myConstIterator& o) const
			{
				return _b == o._b && _g == o._g && _ptr == o._ptr;
			}

			bool operator!=(const myIterator& o) const
			{
				return !(*this == o);
			}

			bool operator!=(const myConstIterator& o) const
			{
				return !(*this == o);
			}

			bool operator< (const myIterator& o) const
			{
				if (_isBasketSide() && !o._isBasketSide())
					return true;
				if (!_isBasketSide() && o._isBasketSide())
					return false;
				return _ptr < o._ptr;
			}

			bool operator> (const myIterator& o) const
			{
				return (o < *this);
			}

			bool operator<= (const myIterator& o) const
			{
				return !(*this > o);
			}

			bool operator>= (const myIterator& o) const
			{
				return !(*this < o);
			}

			bool operator< (const myConstIterator& o) const
			{
				if (_isBasketSide() && !o._isBasketSide())
					return true;
				if (!_isBasketSide() && o._isBasketSide())
					return false;
				return _ptr < o._ptr;
			}

			bool operator> (const myConstIterator& o) const
			{
				return (o < *this);
			}

			bool operator<= (const myConstIterator& o) const
			{
				return !(*this > o);
			}

			bool operator>= (const myConstIterator& o) const
			{
				return !(*this < o);
			}

			myIterator& operator+=(difference_type n)
			{
				if (n < 0 && !_isBasketSide())
				{
					if (-n > _ptr - _gptr())
					{
						n += _ptr - _gptr();
						_ptr = _bptr() + _b->size();
					}
				}
				else if (n > 0 && _isBasketSide())
				{
					if (n > _ptr - _bptr())
					{
						n -= (_ptr - _bptr());
						_ptr = _gptr();
					}
				}
				_ptr += n;
				return *this;
			}

			myIterator& operator-=(difference_type n)
			{
				return *this += (-n);
			}

			myIterator operator+(difference_type n)
			{
				myIterator tmp(*this);

				return tmp += n;
			}

			myIterator operator-(difference_type n)
			{
				myIterator tmp(*this);

				return tmp -= n;
			}

			difference_type	operator-(const myIterator& o) const
			{
				if (o._isBasketSide() && !_isBasketSide())
				{
					return _ptr - _gptr() + (o._bptr() + o._b->size() - o._ptr);
				}
				if (_isBasketSide() && !o._isBasketSide())
				{
					return -((_bptr() + _b->size() - _ptr) + (o._ptr - o._gptr()));
				}
				return _ptr - o._ptr;
			}

			difference_type	operator-(const myConstIterator& o) const
			{
				if (o._isBasketSide() && !_isBasketSide())
				{
					return _ptr - _gptr() + (o._bptr() + o._b->size() - o._ptr);
				}
				if (_isBasketSide() && !o._isBasketSide())
				{
					return -((_bptr() + _b->size() - _ptr) + (o._ptr - o._gptr()));
				}
				return _ptr - o._ptr;
			}

			reference operator*() { return *_ptr; };
			const_reference operator*() const { return *_ptr; }
			pointer operator->() { return _ptr; };
			const_pointer operator->() const { return _ptr; }
			reference operator[](difference_type n) { return *(*this + n); }
			const_reference operator[](difference_type n) const { return *(*this + n); }

			myIterator& operator++() { return (*this) += 1; }
			myIterator operator++(int)
			{
				myIterator cpy(*this);

				(*this) += 1;
				return cpy;
			}
			myIterator& operator--() { return (*this) -= 1; }
			myIterator operator--(int)
			{
				myIterator cpy(*this);

				(*this) -= 1;
				return cpy;
			}

		};

		class myConstIterator : public ft::iterator< ft::random_access_iterator_tag, const value_type, difference_type, pointer, reference >
		{
			friend basket;
			friend MyIterator;

			const basket_type*	_b;
			const ground_type*	_g;
			const_pointer		_ptr;

			myConstIterator(const basket_type* b, const ground_type* g, pointer ptr) : _b(b), _g(g), _ptr(ptr) {}

			const_pointer _bptr() const
			{
				return *(*(_b->begin()));
			}

			const_pointer _gptr() const
			{
				return *(*(_g->begin()));
			}

			bool _isBasketSide() const
			{
				return (_ptr >= _bptr() && _ptr < _bptr() + _b->size());
			}

		public:

			myConstIterator() : _b(0), _g(0), _ptr(0) {}
			myConstIterator(const myConstIterator& o) : _b(o._b), _g(o._g), _ptr(o._ptr) {}
			myConstIterator(const myIterator& o) : _b(o._b), _g(o._g), _ptr(o._ptr) {}
			~myConstIterator() {}
			myConstIterator& operator=(const myConstIterator& o)
			{
				if (&o != this)
				{
					_b = o._b;
					_g = o._g;
					_ptr = o._ptr;
				}
				return *this;
			}

			myConstIterator& operator=(const myIterator& o)
			{
				if (&o != this)
				{
					_b = o._b;
					_g = o._g;
					_ptr = o._ptr;
				}
				return *this;
			}

			bool operator==(const myIterator& o) const
			{
				return _b == o._b && _g == o._g && _ptr == o._ptr;
			}

			bool operator==(const myConstIterator& o) const
			{
				return _b == o._b && _g == o._g && _ptr == o._ptr;
			}

			bool operator!=(const myIterator& o) const
			{
				return !(*this == o);
			}

			bool operator!=(const myConstIterator& o) const
			{
				return !(*this == o);
			}

			bool operator< (const myIterator& o) const
			{
				if (_isBasketSide() && !o._isBasketSide())
					return true;
				if (!_isBasketSide() && o._isBasketSide())
					return false;
				return _ptr < o._ptr;
			}

			bool operator> (const myIterator& o) const
			{
				return (o < *this);
			}

			bool operator<= (const myIterator& o) const
			{
				return !(*this > o);
			}

			bool operator>= (const myIterator& o) const
			{
				return !(*this < o);
			}

			bool operator< (const myConstIterator& o) const
			{
				if (_isBasketSide() && !o._isBasketSide())
					return true;
				if (!_isBasketSide() && o._isBasketSide())
					return false;
				return _ptr < o._ptr;
			}

			bool operator> (const myConstIterator& o) const
			{
				return (o < *this);
			}

			bool operator<= (const myConstIterator& o) const
			{
				return !(*this > o);
			}

			bool operator>= (const myConstIterator& o) const
			{
				return !(*this < o);
			}

			myConstIterator& operator+=(difference_type n)
			{
				if (n < 0 && !_isBasketSide())
				{
					if (-n > _ptr - _gptr())
					{
						n += _ptr - _gptr();
						_ptr = _bptr() + _b->size();
					}
				}
				else if (n > 0 && _isBasketSide())
				{
					if (n > _ptr - _bptr())
					{
						n -= (_ptr - _bptr());
						_ptr = _gptr();
					}
				}
				_ptr += n;
				return *this;
			}

			myConstIterator& operator-=(difference_type n)
			{
				return *this += (-n);
			}

			myConstIterator operator+(difference_type n)
			{
				myConstIterator tmp(*this);

				return tmp += n;
			}

			myConstIterator operator-(difference_type n)
			{
				myConstIterator tmp(*this);

				return tmp -= n;
			}

			difference_type	operator-(const myIterator& o) const
			{
				if (o._isBasketSide() && !_isBasketSide())
				{
					return _ptr - _gptr() + (o._bptr() + o._b->size() - o._ptr);
				}
				if (_isBasketSide() && !o._isBasketSide())
				{
					return -((_bptr() + _b->size() - _ptr) + (o._ptr - o._gptr()));
				}
				return _ptr - o._ptr;
			}

			difference_type	operator-(const myConstIterator& o) const
			{
				if (o._isBasketSide() && !_isBasketSide())
				{
					return _ptr - _gptr() + (o._bptr() + o._b->size() - o._ptr);
				}
				if (_isBasketSide() && !o._isBasketSide())
				{
					return -((_bptr() + _b->size() - _ptr) + (o._ptr - o._gptr()));
				}
				return _ptr - o._ptr;
			}

			reference operator*() { return *_ptr; };
			const_reference operator*() const { return *_ptr; }
			pointer operator->() { return _ptr; };
			const_pointer operator->() const { return _ptr; }
			reference operator[](difference_type n) { return *(*this + n); }
			const_reference operator[](difference_type n) const { return *(*this + n); }

			myConstIterator& operator++() { return (*this) += 1; }
			myConstIterator operator++(int)
			{
				myConstIterator cpy(*this);

				(*this) += 1;
				return cpy;
			}
			myConstIterator& operator--() { return (*this) -= 1; }
			myConstIterator operator--(int)
			{
				myConstIterator cpy(*this);

				(*this) -= 1;
				return cpy;
			}

		};

		typedef myIterator								iterator;
		typedef myConstIterator							const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

	private:

		allocator_type	_alloc;
		basket_type		_basket;
		ground_type		_ground;
		size_type		_basketCapacity;

	public:


	};

}
#endif