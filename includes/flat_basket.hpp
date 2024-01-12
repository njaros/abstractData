#ifndef BASKET_HPP_ABSTRACTDATA_BY_LE_N
#define BASKET_HPP_ABSTRACTDATA_BY_LE_N

# include "vector.hpp"
# include "utility.hpp"
# include "algorithm.hpp"

namespace ft
{

	template <class T, class Alloc = std::allocator< T > >
	class flat_basket
	{

		//friend class myIterator;

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

		//ITERATORS

		typedef typename basket_type::iterator iterator;
		typedef typename basket_type::const_iterator const_iterator;
		typedef typename basket_type::reverse_iterator reverse_iterator;
		typedef typename basket_type::const_reverse_iterator const_reverse_iterator;

		//PRIVATE MEMBERS

	private:

		typedef std::allocator< ground_type >	_AllocGround_type;

		allocator_type	_alloc;
		_AllocGround_type	_allocGround;
		basket_type		_basket;
		// _ground must never be null, because all insert must be managed in a sequence container like mine : cf https://en.cppreference.com/w/cpp/named_req/SequenceContainer
		ground_type* _ground;
		size_type		_basketCapacity;
		/*
		* Essencial value which allows my container to be a container :
		* if true, _ground is managed by current instance (freed at destructor call)
		* if false, _ground must be freed by the user
		* in constructor, if user set himself a ground, _selfGround is false
		* if user set another _ground when _selfGround is true, current instance must destroy and deallocate his current ground before set the new one,
		* then set _selfGround to false.
		*
		* User can set _setGround to false or true, dependings on what he wants to do. (Then any leaks or double free is user fault !)
		* Good usage should be to never have a _selfGround to true.
		* If user always provides a ground in constructor and he never touch to _selfGround, flat_basket can't leaks.
		*/
		bool	_selfGround;

		void	_groundControl()
		{
			if (!_ground)
			{
				_selfGround = true;
				_ground = _allocGround.allocate(1);
				_allocGround.construct(_ground, basket_type(_alloc));
			}
		}

		void	_groundControl(ground_type* newGround)
		{
			if (_ground != newGround)
			{
				if (_selfGround)
				{
					_allocGround.destroy(_ground);
					_allocGround.deallocate(_ground, 1);
				}
				_ground = newGround;
				_selfGround = false;
				_groundControl();
			}
		}

	public:

		//CONSTRUCTORS / DESTRUCTOR / OPERATOR=

		/*
		* because of sequence container rules imposed to have a [begin, last) distance equal to the number of elements in constructors parameter
		* I am forced to throw an exception when the user insert more than the basket capacity (only in constructors)
		*/
		flat_basket(const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(0), _basketCapacity(8), _selfGround(false)
		{
			_basket.reserve(_basketCapacity);
			_groundControl();
		}

		flat_basket(size_type n, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(0), _basketCapacity(n), _selfGround(false)
		{
			_basket.reserve(_basketCapacity);
			_groundControl();
		}

		flat_basket(ground_type* groundPtr, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(groundPtr), _basketCapacity(8), _selfGround(false)
		{
			_basket.reserve(_basketCapacity);
			_groundControl();
		}
		
		flat_basket(size_type n, const value_type& val, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(0), _basketCapacity(8), _selfGround(false)
		{
			if (n > max_size())
				throw(ft::length_error("ft::flat_basket::constructor : too much elements."));
			_basket.reserve(_basketCapacity);
			_groundControl();
			_basket.insert(begin(), n, val);
		}

		flat_basket(size_type n, const value_type& val, size_type basketCapacity, ground_type* ground, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(ground), _basketCapacity(basketCapacity), _selfGround(false)
		{
			if (n > max_size())
				throw(ft::length_error("ft::flat_basket::constructor : too much elements."));
			_basket.reserve(_basketCapacity);
			_groundControl();
			_basket.insert(begin(), n, val);
		}

		//Main constructor to use !
		flat_basket(size_type basketCapacity, ground_type* ground, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(ground), _basketCapacity(basketCapacity), _selfGround(false)
		{
			_basket.reserve(_basketCapacity);
			_groundControl();
		}

		flat_basket(size_type n, const value_type& val, ground_type* ground, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(ground), _basketCapacity(8), _selfGround(false)
		{
			if (n > max_size())
				throw(ft::length_error("ft::flat_basket::constructor : too much elements."));
			_basket.reserve(_basketCapacity);
			_groundControl();
			_basket.insert(begin(), n, val);
		}

		flat_basket(size_type n, const value_type& val, size_type basketCapacity, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(0), _basketCapacity(basketCapacity), _selfGround(false)
		{
			if (n > max_size())
				throw(ft::length_error("ft::flat_basket::constructor : too much elements."));
			_basket.reserve(_basketCapacity);
			_groundControl();
			_basket.insert(begin(), n, val);
		}

		flat_basket(const flat_basket& o)
			: _alloc(o._alloc), _allocGround(o._allocGround), _basket(o._basket), _ground(o._ground), _basketCapacity(o._basketCapacity), _selfGround(false) {}

		template < class InputIterator, typename ft::enable_if< !is_integral< InputIterator >::value >::type >
		flat_basket(InputIterator first, InputIterator last, size_type basketCapacity = 8, ground_type* ground = 0, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(ground), _basketCapacity(basketCapacity)
		{
			difference_type dist;

			dist = ft::distance(first, last);
			if (dist < 0)
				throw(ft::length_error("ft::flat_basket::constructor : iterator first must be less than iterator last."));
			if (dist > max_size())
				throw(ft::length_error("ft::flat_basket::constructor : too much elements."));
			_basket.reserve(_basketCapacity);
			_groundControl();
			while (first != last)
				push_back(*first++);
		}

		~flat_basket()
		{
			if (_selfGround)
			{
				_allocGround.destroy(_ground);
				_allocGround.deallocate(_ground, 1);
			}
		}

		flat_basket& operator=(const flat_basket& o)
		{
			if (&o != this)
			{
				_basket = o._basket;
				_groundControl(o._ground);
				_basketCapacity = o._basketCapacity;
			}
			return *this;
		}

		//ITERATORS ACCESSORS

		iterator				begin() { return _basket.begin(); }
		const_iterator			begin()		const { return _basket.begin(); }
		iterator				end() { return _basket.end(); }
		const_iterator			end()		const { return _basket.end(); }
		reverse_iterator		rbegin() { return _basket.rbegin(); }
		const_reverse_iterator	rbegin()	const { return _basket.rbegin(); }
		reverse_iterator		rend() { return _basket.rend(); }
		const_reverse_iterator	rend()		const { return _basket.rend(); }

		bool	in_basket(iterator position) const { return position >= begin() && position < end(); }

		//CAPACITY

		bool		empty()		const { return _basket.empty(); }
		bool		full()		const { return _basket.size() == _basketCapacity; }
		size_type	size()		const { return _basket.size(); }
		size_type	capacity()	const { return _basketCapacity; }
		size_type	max_size()	const { return _basketCapacity; }
		size_type	size_left()	const { return _basketCapacity - _basket.size(); }
		void		enlarge_your_basket(size_type n)
		{ 
			if (n > _basketCapacity)
				_basketCapacity = n;
		}
		void		reduce_your_basket(size_type n)
		{
			if (n < _basketCapacity)
			{
				_basketCapacity = n;
				while (_basket.size() > _basketCapacity)
				{
					_ground->insert(_ground->end(), _basket.back());
					_basket.pop_back();
				}
			}
		}
		void		resize(size_type n, value_type val = value_type()) { _basket.resize(ft::min(n, _basketCapacity), val); }

		//ELEMENT ACCESS

		reference	operator[](size_type n) { return _basket[n]; }
		const_reference operator[](size_type n)	const { return _basket[n]; }
		reference at(size_type n) { return _basket.at(n); }
		const_reference at(size_type n)			const { return _basket.at(n); }
		reference	front() { return _basket.front(); }
		const_reference	front()					const { return _basket.front(); }
		reference	back() { return _basket.back(); }
		const_reference	back()					const { return _basket.back(); }

		//GROUND MANAGEMENT

		ground_type*		ground() { return _ground; }
		const ground_type*	ground()	const { return _ground; }
		void				ground(ground_type* groundptr) { _groundControl(groundptr); }
		bool				manage_ground() const { return _selfGround; }
		void				manage_ground(bool aJhon) { _selfGround = aJhon; }

		//BASKET / GROUND INTERACTIONS

		iterator	drop(iterator position)
		{
			iterator	groundIterator;

			groundIterator = _ground->insert(_ground->end(), *position);
			_basket.erase(position);
			return groundIterator;
		}

		iterator	drop(iterator first, iterator last)
		{
			iterator	groundIterator;

			groundIterator = _ground->insert(_ground->end(), first, last);
			_basket.erase(first, last);
			return groundIterator;
		}

		ft::pair<iterator, size_type>	drop()
		{
			ft::pair<iterator, size_type> ret;
			
			ret.second = _basket.size();
			ret.first = _ground->insert(_ground->end(), begin(), end());
			clear();
			return ret;
		}

		void	drop_back()
		{
			if (!empty())
				drop(--end());
		}

		size_type	pick_up(iterator basketPosition, iterator first, iterator last)
		{
			difference_type	dist;
			size_type	count;

			count = 0;
			dist = ft::distance(first, last);
			if (dist < 0)
				throw(ft::length_error("ft::flat_basket::pick_up : iterator first must be lesser than last."));

			count = ft::min(size_left(), (size_type)dist);
			_basket.insert(basketPosition, first, first + count);
			_ground->erase(first, first + count);
			return count;
		}

		size_type	pick_up(iterator basketPosition, iterator groundPosition)
		{
			if (!full())
			{
				insert(basketPosition, *groundPosition);
				_ground->erase(groundPosition);
				return 1;
			}
			return 0;
		}

		size_type	pick_up()
		{
			if (!_ground->empty)
				return pick_up(end(), --_ground->end());
			return 0;
		}

		size_type	fill(iterator basketPosition, iterator groundPosition)
		{
			return pick_up(basketPosition, groundPosition, _ground->end());
		}

		size_type	fill()
		{
			size_type	sizeToPick;

			sizeToPick = ft::min(_ground->size(), size_left());
			return fill(end(), _ground->end() - sizeToPick);
		}

		size_type	pour_out(flat_basket& o)
		{
			size_type	before = o.size();
			o.insert(o.end(), begin(), end());
			clear();

			return o.size() - before;
		}

		//MODIFIERS

		void	clear() { _basket.clear(); }

		iterator insert(iterator position, const value_type& val)
		{
			if (!full())
				return _basket.insert(position, val);
			return _ground->insert(_ground->end(), val);
		}

		iterator insert(iterator position, size_type n, const value_type& val)
		{
			if (n >= _alloc.max_size())
				throw(ft::length_error("ft::flat_basket::insert : too much element."));
			if (n <= size_left())
				return _basket.insert(position, n, val);
			_basket.insert(position, size_left(), val);
			return _ground->insert(_ground->end(), n - size_left(), val);
		}

		template < class InputIterator >
		typename ft::enable_if<!is_integral<InputIterator>::value, iterator>::type
			insert(iterator position, InputIterator first, InputIterator last)
		{
			difference_type dist;
			difference_type	basketInsertLen;
			InputIterator	basketFirst;

			dist = ft::distance(first, last);
			if (dist < 0)
				throw(ft::length_error("ft::flat_basket::insert : iterator first must be less than iterator last."));

			basketFirst = first;
			basketInsertLen = ft::min(size_left(), (size_type)dist);
			while (basketInsertLen--)
				++first;
			position = _basket.insert(position, basketFirst, first);
			if (first != last)
				position = _ground->insert(_ground->end(), first, last);
			return position;
		}

		void	push_back(const value_type& val)
		{
			insert(end(), val);
		}

		iterator insert_force(iterator position, const value_type& val)
		{
			if (full())
			{
				_ground->insert(_ground->end(), _basket.back());
				_basket.pop_back();
			}
			return _basket.insert(position, val);
		}

		iterator insert_force(iterator position, size_type n, const value_type& val)
		{
			size_type	nbToPop;
			size_type	nbToInsert;

			if (n > size_left())
			{
				nbToPop = ft::min(size_left() + end() - position, n - size_left());
				while (nbToPop--)
				{
					_ground->insert(_ground->end(), _basket.back());
					_basket.pop_back();
				}
				nbToInsert = size_left();
				_basket.insert(position, nbToInsert, val);
				return _ground->insert(_ground->end(), n - nbToInsert, val);
			}
			return _basket.insert(position, n, val);
		}

		template < class InputIterator >
		typename enable_if<!is_integral< InputIterator >::value, iterator>::type
			insert_force(iterator position, InputIterator first, InputIterator last)
		{
			difference_type dist;
			size_type		nbToInsert;
			size_type		nbToPop;
			InputIterator	basketFirst;

			dist = ft::distance(first, last);
			if (dist < 0)
				throw(ft::length_error("ft::flat_basket::insert : iterator first must be less than iterator last."));

			basketFirst = first;
			if (dist > size_left())
			{
				nbToPop = ft::min(size_left() + end() - position, dist - size_left());
				while (nbToPop--)
				{
					_ground->insert(_ground->end(), _basket.back());
					_basket.pop_back();
				}
				nbToInsert = size_left();
				while (nbToInsert--)
					++first;
			}
			position = insert(position, basketFirst, first);
			if (first != last)
				position = _ground->insert(_ground->end(), first, last);
			return position;
		}

		void	push_back_force(const value_type& val)
		{
			insert_force(end(), val);
		}

		void	assign(size_type n, value_type& val)
		{
			clear();
			insert(begin(), n, val);
		}

		template < class InputIterator >
		typename enable_if< !is_integral< InputIterator >::value >::type
			assign(InputIterator first, InputIterator last)
		{
			clear();
			insert(begin(), first, last);
		}

		iterator	erase(iterator position) { return _basket.erase(position); }
		iterator	erase(iterator first, iterator last) { return _basket.erase(first, last); }

		void	pop_back()
		{
			if (!empty())
				erase(--end());
		}

		void	swap(flat_basket& o)
		{
			allocator_type		allocTmp = _alloc;
			_AllocGround_type	allocGroundTmp = _allocGround;
			ground_type*		groundTmp = _ground;
			size_type			basketCapacityTmp = _basketCapacity;
			bool				selfGroundTmp = _selfGround;

			_basket.swap(o._basket);
			_alloc = o._alloc;
			_allocGround = o._allocGround;
			_ground = o._ground;
			_basketCapacity = o._basketCapacity;
			_selfGround = o._selfGroound;

			o._alloc = allocTmp;
			o._allocGround = allocGroundTmp;
			o._ground = groundTmp;
			o._basketCapacity = basketCapacityTmp;
			o._selfGround = selfGroundTmp;
		}

		void	swap_ground(flat_basket& o)
		{
			ground_type*	groundTmp = _ground;
			_AllocGround_type	allocGroundTmp = _allocGround;
			bool			selfGroundTmp = _selfGround;

			_ground = o._ground;
			_allocGround = o._allocGround;
			_selfGround = o._selfGround;

			o._ground = groundTmp;
			o._allocGround = allocGroundTmp;
			o._selfGround = selfGroundTmp;
		}

		void	swap_basket(flat_basket& o)
		{
			allocator_type		allocTmp = _alloc;
			size_type			basketCapacityTmp = _basketCapacity;

			_basket.swap(o._basket);
			_alloc = o._alloc;
			_basketCapacity = o._basketCapacity;

			o._alloc = allocTmp;
			o._basketCapacity = basketCapacityTmp;
		}
	};

	template < class T, class Alloc >
	bool operator==(const flat_basket<T, Alloc>& lhs, const flat_basket<T, Alloc>& rhs)
	{
		return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template < class T, class Alloc >
	bool operator!=(const flat_basket<T, Alloc>& lhs, const flat_basket<T, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template < class T, class Alloc >
	bool operator< (const flat_basket<T, Alloc>& lhs, const flat_basket<T, Alloc>& rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template < class T, class Alloc >
	bool operator<=(const flat_basket<T, Alloc>& lhs, const flat_basket<T, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template < class T, class Alloc >
	bool operator> (const flat_basket<T, Alloc>& lhs, const flat_basket<T, Alloc>& rhs)
	{
		return rhs < lhs;
	}

	template < class T, class Alloc >
	bool operator>=(const flat_basket<T, Alloc>& lhs, const flat_basket<T, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	template < class T, class Alloc >
	void	swap(flat_basket<T, Alloc>& lhs, flat_basket<T, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}

#endif