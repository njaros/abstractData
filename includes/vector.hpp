/* ************************************************************************** */
/*                                                                            */
/*                                                       :::      ::::::::    */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaros <njaros@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 14:54:34 by njaros            #+#    #+#             */
/*   Updated: 2022/07/22 16:05:10 by njaros           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef vector_HPP
# define vector_HPP

# include "type_traits.hpp"
# include "iterator.hpp"
# include "reverse_iterator.hpp"
# include "exception.hpp"

# include <memory>
# include <new>
# include <cstddef>
# include <cstring>
# include <iostream>
# include <cstdlib>

namespace ft
{

	template < typename T, class Alloc = std::allocator<T> >

	class vector
	{

	public:

		// TYPEDEFS

		typedef T													value_type;
		typedef Alloc												allocator_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef typename Alloc::difference_type						difference_type;
		typedef typename Alloc::size_type							size_type;

		// CLASS ITERATOR AND HIS TYPEDEFS

		class MyConstIterator;
		class MyIterator : public ft::iterator< ft::random_access_iterator_tag, value_type >
		{
		public:

			MyIterator() : _p(0) {}
			explicit MyIterator(value_type* ptr) : _p(ptr) {}
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
				value_type* toReturn;

				toReturn = _p + val;
				return (*toReturn);
			}

			virtual pointer base()	const { return (_p); }

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
				value_type* toReturn;

				toReturn = _p + val;
				return (*toReturn);
			}

			virtual pointer base()	const { return (_p); }

		};
		typedef MyIterator	                                        iterator;
		typedef MyConstIterator										const_iterator;
		typedef ft::reverse_iterator<iterator>                      reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>				const_reverse_iterator;

		// CONSTRUCTORS / DESTRUCTOR

		vector(const allocator_type& alloc = allocator_type()) :
			_alloc(alloc),
			_firstPtr(0),
			_size(0),
			_capacity(0)
		{}

		vector(size_type n, value_type val, const allocator_type& alloc = allocator_type()) :
			_alloc(alloc),
			_firstPtr(0),
			_size(n),
			_capacity(n)
		{
			size_type	idx;
			if (n > _alloc.max_size())
				throw(ft::length_error("cannot create ft::vector larger than max_size()"));
			
			_firstPtr = _alloc.allocate(n, 0);
			_ptr = _firstPtr;
			idx = 0;
			while (idx++ < _size)
			{
				_alloc.construct(_ptr, val);
				_ptr++;
			}
		}

		explicit vector(size_type n, const allocator_type& alloc = allocator_type()) : //C++11 constructor but he's so useful for testing, he deserves to be here.
			_alloc(alloc),
			_size(n),
			_capacity(n)
		{
			size_type   s = 0;

			if (n > _alloc.max_size())
				throw (ft::length_error("cannot create ft::vector larger than max_size()"));
			_firstPtr = _alloc.allocate(n, 0);
			while (s < n)
			{
				_alloc.construct(&_firstPtr[s], value_type());
				s++;
			}
		}

		template < class InputIterator >
		vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
			typename ft::enable_if< !ft::is_integral< InputIterator >::value >::type* = 0)
			: _alloc(alloc)
		{
			_size = ft::distance<InputIterator>(first, last);
			if (_size > _alloc.max_size())
				throw (ft::length_error("cannot create ft::vector larger than max_size()"));
			_capacity = _size;
			_firstPtr = _alloc.allocate(_size, 0);
			_ptr = _firstPtr;
			while (first != last)
			{
				_alloc.construct(_ptr, *first);
				first++;
				_ptr++;
			}
		}

		vector(const vector& other) :
			_alloc(other._alloc),
			_size(other._size),
			_capacity(other._size)
		{
			if (!other._firstPtr)
				_firstPtr = 0;
			else {
				_firstPtr = _alloc.allocate(other._size, 0);
				const_iterator it = other.begin();
				_ptr = _firstPtr;
				while (it != other.end()) {
					_alloc.construct(_ptr, *it);
					_ptr++;
					it++;
				}
			}
		}

		~vector()
		{
			for (_it = begin(); _it != end(); _it++)
				_alloc.destroy(_it.operator->());
			_alloc.deallocate(_firstPtr, _capacity);
		}


		// ITERATOR FUNCTIONS

		iterator	begin()
		{
			MyIterator	begin(_firstPtr);

			return (begin);
		}

		const_iterator  begin()	const
		{
			const_iterator  begin(_firstPtr);

			return (begin);
		}

		iterator	end()
		{
			MyIterator	end(&_firstPtr[_size]);

			return (end);
		}

		const_iterator  end() const
		{
			const_iterator  end(&_firstPtr[_size]);

			return (end);
		}

		reverse_iterator    rbegin() {
			return (reverse_iterator(end()));
		}

		const_reverse_iterator  rbegin() const {
			return (const_reverse_iterator(end()));
		}

		reverse_iterator    rend() {
			return (reverse_iterator(begin()));
		}

		const_reverse_iterator  rend() const {
			return (const_reverse_iterator(begin()));
		}

		// CAPACITY

		size_type	size()		const { return (_size); }
		size_type	max_size()	const { return (_alloc.max_size()); }
		size_type	capacity()	const { return (_capacity); }
		bool		empty()		const { return (_size == 0); }
		void		resize(size_type n)
		{
			if (n == _size)
				return;
			if (n < _size)
			{
				while (_size > n)
					_alloc.destroy(&_firstPtr[--_size]);
			}
			else
			{
				if (n > _capacity)
					reserve(n);
				while (_size < n)
				{
					_alloc.construct(&_firstPtr[_size], value_type());
					_size++;
				}
			}
		}
		void		resize(size_type n, value_type val)
		{
			if (n == _size)
				return;
			if (n < _size)
			{
				while (_size > n)
				{
					_size--;
					_alloc.destroy(&_firstPtr[_size]);
				}
			}
			else
			{
				if (n > _capacity)
					reserve(n);
				_ptr = &_firstPtr[_size];
				while (_size < n)
				{
					_alloc.construct(_ptr, val);
					_ptr++;
					_size++;
				}
			}
		}
		void		reserve(size_type n)
		{
			if (n > _alloc.max_size())
				throw(ft::length_error("cannot reserve larger than max_size()"));
			if (n > _capacity)
			{
				if (!_firstPtr)
					_firstPtr = _alloc.allocate(n, 0);
				else
				{
					pointer tempPtr = _firstPtr;
					iterator tempBegin = begin();
					iterator tempLast = end();
					_firstPtr = _alloc.allocate(n, 0);
					_it = begin();
					while (tempBegin != tempLast) {
						_alloc.construct(_it++.operator->(), *tempBegin);
						_alloc.destroy(tempBegin.operator->());
						++tempBegin;
					}
					_alloc.deallocate(tempPtr, _capacity);
				}
				_capacity = n;
			}
		}

		// ELEMENT ACCESS

		reference		operator[](size_type idx) { return (this->_firstPtr[idx]); }
		const_reference	operator[](size_type idx) const { return (this->_firstPtr[idx]); }
		reference		at(size_type n)
		{
			if (n >= _size)
			{
				std::string	err = "vector::_M_range_check: __n (which is ";
				err += size_tToString(n);
				err += ") >= this->size() (which is ";
				err += size_tToString(_size);
				err += ")";

				throw ft::out_of_range(err);
			}
			return (_firstPtr[n]);
		}

		const_reference 		at(size_type n) const
		{
			if (n >= _size)
			{
				std::string	err = "vector::_M_range_check: __n (which is ";
				err += size_tToString(n);
				err += ") >= this->size() (which is ";
				err += size_tToString(_size);
				err += ")";
				throw ft::out_of_range(err);
			}
			return (_firstPtr[n]);
		}
		reference		front() { return (*_firstPtr); }
		const_reference	front()	const { return (*_firstPtr); }
		reference		back() { return (_firstPtr[_size - 1]); }
		const_reference	back()	const { return (_firstPtr[_size - 1]); }
		pointer			data() { return (_firstPtr); }
		const_pointer	data()	const { return (_firstPtr); }

		// MODIFIERS

		vector& operator=(vector const& other)
		{
			assign(other.begin(), other.end());
			return (*this);
		}
		template < class InputIterator >
		void		assign(InputIterator first, InputIterator last, \
			typename ft::enable_if< !ft::is_integral< InputIterator >::value >::type* = 0)
		{
			typename ft::iterator_traits<InputIterator>::difference_type s;
			_it = begin();
			while (_it != end())
				_alloc.destroy(_it++.operator->());
			_size = 0;
			s = ft::distance(first, last);
			if (!s)
				return;
			reserve(s);
			_size = s;
			_it = begin();
			while (first != last)
				_alloc.construct(_it++.operator->(), *first++);
		}
		void		assign(size_type n, const value_type& val)
		{
			if (n > _alloc.max_size())
				throw(ft::length_error("cannot create ft::vector larger than max_size()"));
			_it = begin();
			while (_it != end())
				_alloc.destroy(_it++.operator->());
			_size = 0;
			reserve(n);
			_size = n;
			_it = begin();
			while (_it != end())
				_alloc.construct(_it++.operator->(), val);
		}

		void		push_back(const value_type& val)
		{
			checkCapacityAvailable(1);
			_alloc.construct(&_firstPtr[_size], val);
			_size += 1;
		}
		void		pop_back()
		{
			if (_size) {
				_size -= 1;
				_alloc.destroy(&_firstPtr[_size]);
			}
		}
		iterator		insert(iterator position, const value_type& val)
		{
			_ptr = position.base();
			if (!_ptr && !_firstPtr)
				this->push_back(val);
			else
			{
				if (_size + 1 > _capacity) {
					size_type diff = position - begin();
					checkCapacityAvailable(1);
					position = begin() + diff;
					_ptr = position.base();
				}
				if (position != end())
				{
					copyBlock(position, this->end(), position + 1);
					_alloc.destroy(_ptr);
				}
				_alloc.construct(_ptr, val);
				_size += 1;
			}
			return (position);
		}
		iterator		insert(iterator position, size_type n, const value_type& val)
		{
			if (!n)
				return (position);
			if (!position.operator->() && !_firstPtr)
				this->assign(n, val);
			else {
				if (_size + n > _capacity) {
					typename ft::iterator_traits<iterator>::difference_type diff = ft::distance<iterator>(begin(),
						position);
					checkCapacityAvailable(n);
					position = begin() + diff;
				}
				copyBlock(position, this->end(), position + n);
				for (size_type i = 0; i < n; ++i) {
					if (position < end())
						_alloc.destroy(position.base());
					_alloc.construct(position.base(), val);
					position++;
				}
				_size += n;
			}
			return (end() - n);
		}
		template < class InputIterator >
		iterator		insert(iterator position, InputIterator first, InputIterator last, \
			typename ft::enable_if< !ft::is_integral< InputIterator >::value >::type* = 0)
		{
			typename ft::iterator_traits<InputIterator>::difference_type addSize = ft::distance<InputIterator>(first, last);
			if (!addSize)
				return position;
			if (!position.operator->() && !_firstPtr) {
				reserve(addSize);
				_it = begin();
				while (first != last)
					_alloc.construct(_it++.operator->(), *first++);
				_size = _capacity;
			}
			else
			{
				if (_size + addSize > _capacity) {
					typename ft::iterator_traits<iterator>::difference_type diff = ft::distance<iterator>(begin(), position);
					checkCapacityAvailable(addSize);
					position = begin() + diff;
				}
				copyBlock(position, this->end(), position + addSize);
				while (first != last)
				{
					if (position < end())
						_alloc.destroy(position.operator->());
					_alloc.construct(position.operator->(), *first);
					first++;
					position++;
				}
				_size += addSize;
			}
			return (end() - addSize);
		}
		iterator	erase(iterator position)
		{
			_alloc.destroy(position.base());
			copyBlock(position + 1, end(), position);
			_size -= 1;
			if (position != end())
				_alloc.destroy(end().base());
			return (position);
		}

		iterator	erase(iterator first, iterator last) {
			size_type delSize;

			if (last == first)
				return (last);
			if (last > first)
			{
				delSize = last - first;
				_it = first;
				while (_it != last) {
					_alloc.destroy(&(*_it));
					_it++;
				}
				copyBlock(last, this->end(), first);
				_it = this->end() - 1;
				_size -= delSize;
				if (first != this->end()) {
					while (delSize--)
						_alloc.destroy(&(*_it--));
				}
				return (first);
			}
			else
			{
				throw (ft::length_error("iterator first must be > to iterator last"));
			}
		}
		void		swap(vector& other)
		{
			allocator_type allocTmp = _alloc;
			pointer	ptrTmp = _firstPtr;
			size_type	sizeTmp = _size;
			size_type	capacityTmp = _capacity;

			_alloc = other._alloc;
			_firstPtr = other._firstPtr;
			_size = other._size;
			_capacity = other._capacity;

			other._alloc = allocTmp;
			other._firstPtr = ptrTmp;
			other._size = sizeTmp;
			other._capacity = capacityTmp;
		}
		void        clear() { resize(0); }

		// ALLOCATOR

		allocator_type	get_allocator()	const
		{
			allocator_type	copy(_alloc);

			return (copy);
		}

	private:

		allocator_type	_alloc;
		pointer			_firstPtr;
		pointer			_ptr;
		MyIterator		_it;
		size_type		_size;
		size_type		_capacity;
		void			checkCapacityAvailable(size_type n)
		{
			if (n > _alloc.max_size())
				throw(ft::length_error("cannot reserve larger than max_size()"));
			if (!_firstPtr && n)
			{
				_firstPtr = _alloc.allocate(n, 0);
				_capacity = n;
			}
			else if (_size + n > _capacity)
			{
				size_t tempCapacity = _capacity;
				pointer tempPtr = _firstPtr;
				iterator tempBegin = begin();
				iterator tempLast = end();
				if (!_capacity)
					_capacity += 1;
				while (_size + n > _capacity)
					_capacity *= 2;
				_firstPtr = _alloc.allocate(_capacity, 0);
				iterator first(_firstPtr);
				while (tempBegin != tempLast) {
					_alloc.construct(first++.operator->(), *tempBegin);
					_alloc.destroy(tempBegin.operator->());
					++tempBegin;
				}
				_alloc.deallocate(tempPtr, tempCapacity);
			}
		}
		MyIterator		copyBlock(MyIterator start, MyIterator last, MyIterator here)
		{
			if (here == start || start == last)
				return (here);
			if (here < start)
			{
				while (start != last)
				{
					_alloc.destroy(here.base());
					_alloc.construct(here.base(), *start);
					here++;
					start++;
				}
			}
			else
			{
				here += last - start - 1;
				while (last-- != start)
				{
					if (here < end())
						_alloc.destroy(here.base());
					_alloc.construct(here.base(), *last);
					here--;
				}
			}
			return (here);
		}

		void	fillString(std::string& str, size_t s)	const
		{
			if ((s / 10) > 0)
			{
				fillString(str, s / 10);
				fillString(str, s % 10);
			}
			if ((s / 10) == 0)
				str += (char)s + '0';
		}

		std::string	size_tToString(size_t s)	const
		{
			std::string	str;

			fillString(str, s);
			return (str);
		}

	};

	// NON MEMBERS OPERATIONS

	template < class T, class Alloc >
	bool	operator==(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs)
	{
		typename ft::vector<T>::iterator itL;
		typename ft::vector<T>::iterator itR;

		if (lhs.size() != rhs.size())
			return (false);
		itL = lhs.begin();
		itR = rhs.begin();
		while (itL != lhs.end())
		{
			if (*itL != *itR)
				return (false);
			itL++;
			itR++;
		}
		return (true);
	}

	template < class T, class Alloc >
	bool	operator!=(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs) { return (!(lhs == rhs)); }

	template < class T, class Alloc >
	bool	operator<(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs)
	{
		typename ft::vector<T>::const_iterator itL;
		typename ft::vector<T>::const_iterator itR;

		itL = lhs.begin();
		itR = rhs.begin();
		while (itL != lhs.end() && itR != rhs.end())
		{
			if (*itL != *itR)
				return (*itL < *itR);
			itR++;
			itL++;
		}
		return (lhs.size() < rhs.size());
	}

	template < class T, class Alloc >
	bool	operator>=(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs) { return (!(lhs < rhs)); }

	template < class T, class Alloc >
	bool	operator>(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs)
	{
		typename ft::vector<T>::const_iterator itL;
		typename ft::vector<T>::const_iterator itR;

		itL = lhs.begin();
		itR = rhs.begin();
		while (itL != lhs.end() && itR != rhs.end())
		{
			if (*itL != *itR)
				return (*itL > *itR);
			itR++;
			itL++;
		}
		return (lhs.size() > rhs.size());
	}

	template < class T, class Alloc >
	bool	operator<=(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs) { return (!(lhs > rhs)); }

	template < class T, class Alloc >
	void	swap(ft::vector<T, Alloc>& x, ft::vector<T, Alloc>& y)
	{
		x.swap(y);
	}

}

#endif