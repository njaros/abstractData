#ifndef BASKET_HPP_ABSTRACTDATA_BY_LE_N
#define BASKET_HPP_ABSTRACTDATA_BY_LE_N

# include "vector.hpp"
# include "utility.hpp"
# include "algorithm.hpp"

namespace ft
{
	/**
	 *  @brief A container I created for containers with limited size,
	 *  with simulation of object droppable from the container, falling
	 *  down in a vector Ground, which can be shared between other flat_baskets.
	 * 
	 *  @ingroup sequences
	 * 
	 * 
	 *  @tparam T	Type of objects.
	 *  @tparam Alloc	Allocator type, defaults to allocator<T>
	 * 
	 *  @details
	 *  Complexity of insertion and deletion are the same as vector because this container
	 *  uses vector<T> an underlyings containers.
	 *  This container is intended for simulations of limited sized "baskets".
	*/
	template <class T, class Alloc = std::allocator< T > >
	class flat_basket
	{

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

		/**
		 *  @brief	Creates a %flat_basket with no element, with a capacity fixed to 8, and his
		 *  self ground management.
		 * 
		 *  @param	alloc	An allocator object
		*/
		flat_basket(const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(0), _basketCapacity(8), _selfGround(false)
		{
			_basket.reserve(_basketCapacity);
			_groundControl();
		}

		/**
		 *  @brief	Creates a %flat_basket with no element, with a capacity fixed to n, and his
		 *  self ground management.
		 * 
		 *  @param  n	capacity of the basket
		 *  @param	alloc	An allocator object
		*/
		flat_basket(size_type n, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(0), _basketCapacity(n), _selfGround(false)
		{
			_basket.reserve(_basketCapacity);
			_groundControl();
		}

		/**
		 *  @brief	Creates a %flat_basket with no element, with a capacity fixed to 8, and a
		 *  given ground pointed by groundPtr, not managed with this container.
		 * 
		 *  @param  groundPtr	pointer to a shared vector<T>
		 *  @param	alloc	An allocator object
		*/
		flat_basket(ground_type* groundPtr, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(groundPtr), _basketCapacity(8), _selfGround(false)
		{
			_basket.reserve(_basketCapacity);
			_groundControl();
		}
		
		/**
		 *  @brief	Creates a %flat_basket with n elements of val, with a capacity fixed to 8, and a
		 *  self managed ground.
		 * 
		 *  @param  n	number of objects inserted to the basket
		 *  @param	val	object to copy
		 *  @param	alloc	An allocator object
		 * 
		 *  @exception  if n > capacity, it abort with ft::length_error exception
		*/
		flat_basket(size_type n, const value_type& val, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(0), _basketCapacity(8), _selfGround(false)
		{
			if (n > max_size())
				throw(ft::length_error("ft::flat_basket::constructor : too much elements."));
			_basket.reserve(_basketCapacity);
			_groundControl();
			_basket.insert(begin(), n, val);
		}

		/**
		 *  @brief	Creates a %flat_basket with n elements of val, with a given capacity, and a
		 *  given ground pointed by ground, not managed with this container.
		 *
		 *  @param  n	number of objects inserted to the basket
		 *  @param	val	object to copy
		 *  @param	basketCapacity	capacity of the basket
		 *  @param  groundPtr	pointer to a shared vector<T>
		 *  @param	alloc	An allocator object
		 * 
		 *  @exception  if n > capacity, it abort with ft::length_error exception
		*/
		flat_basket(size_type n, const value_type& val, size_type basketCapacity, ground_type* ground, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(ground), _basketCapacity(basketCapacity), _selfGround(false)
		{
			if (n > max_size())
				throw(ft::length_error("ft::flat_basket::constructor : too much elements."));
			_basket.reserve(_basketCapacity);
			_groundControl();
			_basket.insert(begin(), n, val);
		}

		/**
		 *  @brief	Creates a %flat_basket with no element, with a given capacity, and a
		 *  given ground pointed by ground, not managed with this container. This is the
		 *  most logical constructor to use.
		 * 
		 *  @param	basketCapacity	capacity of the basket
		 *  @param  ground	pointer to a shared vector<T>
		 *  @param	alloc	An allocator object
		*/
		flat_basket(size_type basketCapacity, ground_type* ground, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(ground), _basketCapacity(basketCapacity), _selfGround(false)
		{
			_basket.reserve(_basketCapacity);
			_groundControl();
		}

		/**
		 *  @brief	Creates a %flat_basket with n elements of val, with a capacity fixed to 8, and a
		 *  given ground pointed by ground, not managed with this container.
		 * 
		 *  @param  n	number of objects inserted to the basket
		 *  @param	val	object to copy
		 *  @param  ground	pointer to a shared vector<T>
		 *  @param	alloc	An allocator object
		 * 
		 *  @exception  if n > capacity, it abort with ft::length_error exception
		*/
		flat_basket(size_type n, const value_type& val, ground_type* ground, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(ground), _basketCapacity(8), _selfGround(false)
		{
			if (n > max_size())
				throw(ft::length_error("ft::flat_basket::constructor : too much elements."));
			_basket.reserve(_basketCapacity);
			_groundControl();
			_basket.insert(begin(), n, val);
		}

		/**
		 *  @brief	Creates a %flat_basket with n elements of val, with a capacity fixed to 8, and a
		 *  given ground pointed by ground, not managed with this container.
		 * 
		 *  @param  n	number of objects inserted to the basket
		 *  @param	val	object to copy
		 *  @param  basketCapacity	capacity of the basket
		 *  @param	alloc	An allocator object
		 * 
		 *  @exception  if n > capacity, it abort with ft::length_error exception
		*/
		flat_basket(size_type n, const value_type& val, size_type basketCapacity, const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(0), _basketCapacity(basketCapacity), _selfGround(false)
		{
			if (n > max_size())
				throw(ft::length_error("ft::flat_basket::constructor : too much elements."));
			_basket.reserve(_basketCapacity);
			_groundControl();
			_basket.insert(begin(), n, val);
		}

		/**
		 *  @brief	Creates %flat_basket which is a copy of another %flat_basket. They will share
		 *  the same ground, but this container will not manage the ground after construction.
		 * 
		 *  @param	o	flat_basket to copy
		*/
		flat_basket(const flat_basket& o)
			: _alloc(o._alloc), _allocGround(o._allocGround), _basket(o._basket), _ground(o._ground), _basketCapacity(o._basketCapacity), _selfGround(false) {}

		/**
		 *  @brief	Creates a %flat_basket with copy element in range (first, last]. basketCapacity is 8
		 *  by default, and ground is self by default.
		 * 
		 *	@tparam	InputIterator	Ether an iterator or a pointer
		 * 
		 *  @param	first	element pointing to the first object to copy
		 *  @param	last	element pointing just after the last element to copy
		 *  @param	basketCapacity	capacity of the basket (8 by default)
		 *  @param	ground	pointer to a shared vector<T>
		 *  @param	alloc	An allocator object
		 *
		 *  @exception if ft::distance(first, last) > capacity, an exception ft::length_error is thrown.
		 *  @exception if ft::distance(first, last) > max_size(), an exception ft::length_error is thrown.
		*/
		template < class InputIterator >
		flat_basket(InputIterator first, InputIterator last, size_type basketCapacity = 8, ground_type* ground = 0, const allocator_type& alloc = allocator_type(),
			typename ft::enable_if< !is_integral< InputIterator >::value >::type* = 0)
			: _alloc(alloc), _allocGround(_AllocGround_type()), _basket(alloc), _ground(ground), _basketCapacity(basketCapacity)
		{
			difference_type dist;

			dist = ft::distance(first, last);
			if (dist < 0)
				throw(ft::length_error("ft::flat_basket::constructor : iterator first must be less than iterator last."));
			if ((size_type)dist > max_size())
				throw(ft::length_error("ft::flat_basket::constructor : too much elements."));
			_basket.reserve(_basketCapacity);
			_groundControl();
			while (first != last)
				push_back(*first++);
		}

		/**
		 * Destructor will call the ground destructor only if he used to manage
		 * him, it can be a problem only if the user uses badly the manage_ground(bool)
		*/
		~flat_basket()
		{
			if (_selfGround)
			{
				_allocGround.destroy(_ground);
				_allocGround.deallocate(_ground, 1);
			}
		}

		/**
		 *  @brief %flat_basket assignment operator.
		*/
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

		/**
		 *  @brief	return a non_const iterator pointing to the first element in the %flat_basket.
		*/
		iterator				begin() { return _basket.begin(); }
		/**
		 *  @brief	return a const iterator pointing to the first element in the %flat_basket.
		*/
		const_iterator			begin()		const { return _basket.begin(); }
		/**
		 *  @brief	return a non_const iterator pointing to the after last element in the %flat_basket.
		*/
		iterator				end() { return _basket.end(); }
		/**
		 *  @brief	return a const iterator pointing to the after last element in the %flat_basket.
		*/
		const_iterator			end()		const { return _basket.end(); }
		/**
		 *  @brief	return a non_const reverse iterator pointing to the last element in the %flat_basket.
		*/
		reverse_iterator		rbegin() { return _basket.rbegin(); }
		/**
		 *  @brief	return a const reverse iterator pointing to the last element in the %flat_basket.
		*/
		const_reverse_iterator	rbegin()	const { return _basket.rbegin(); }
		/**
		 *  @brief	return a non_const reverse iterator pointing to the before first element in the %flat_basket.
		*/
		reverse_iterator		rend() { return _basket.rend(); }
		/**
		 *  @brief	return a const reverse iterator pointing to the before first element in the %flat_basket.
		*/
		const_reverse_iterator	rend()		const { return _basket.rend(); }

		/**
		 *  @brief	return true if the iterator is in the basket of this container. 
		 * 
		 * 	That is usefull to know is an object was dropped to the ground or not.
		 * 
		 *  @param	iterator	An iterator
		*/
		bool	in_basket(iterator position) const { return position >= begin() && position < end(); }

		//CAPACITY

		/**
		 *  @brief  return true if the basket is empty.
		 * 
		 *  Note that the ground doesn't infer.
		*/
		bool		empty()		const { return _basket.empty(); }
		
		/**
		 *  return true is the basket is full
		 * 
		 *  If true, a next inserted element will fall to the ground.
		*/
		bool		full()		const { return _basket.size() == _basketCapacity; }
		
		/**
		 *  @brief  return the number of objects in the basket.
		*/
		size_type	size()		const { return _basket.size(); }

		/**
		 *  return the capacity of the basket
		 * 
		 *  Note, this value can be set thanks of enlarge_your_basket or reduce_your_basket
		 *  because only magic can change a size of a basket.
		*/
		size_type	capacity()	const { return _basketCapacity; }

		/**
		 *  @brief	same as capacity
		*/
		size_type	max_size()	const { return _basketCapacity; }

		/**
		 *  @brief	return the number of objects the basket can take before it's fulled.
		*/
		size_type	size_left()	const { return _basketCapacity - _basket.size(); }
		
		/**
		 *  @brief	augment the capacity of the basket
		 * 
		 *  @param	n	if n > capacity(), effectively set the capacity to n
		*/
		void		enlarge_your_basket(size_type n)
		{ 
			if (n > _basketCapacity)
				_basketCapacity = n;
		}

		/**
		 *  @brief	diminish the capacity of the basket
		 * 
		 *  @param	n	if n < capacity(), effectively set the capacity to n
		 * 
		 *  @details
		 *  Lasts elements which can't be content will be dropped to the ground.
		*/
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

		/**
		 *  @brief  resize the basket
		 * 
		 *  @param	n	resize wanted
		 *  @param	val	value of object inserted if resize > size() 
		 * 
		 *  @details
		 *  Note that if n > capacity(), the resize lenght will be reduced to the capacity of the bucket.
		*/
		void		resize(size_type n, value_type val = value_type()) { _basket.resize(ft::min(n, _basketCapacity), val); }

		//ELEMENT ACCESS

		/**
		 *  @brief  return a non const reference to the n'th element of the basket
		 * 
		 *  @param	n	the n'th reference to return
		 * 
		 *  @details
		 *  if n is out of basket bounds, it causes an undefined behavior
		*/
		reference	operator[](size_type n) { return _basket[n]; }

		/**
		 *  @brief  return a const reference to the n'th element of the basket
		 * 
		 *  @param	n	the n'th reference to return
		 * 
		 *  @details
		 *  if n is out of basket bounds, it causes an undefined behavior
		*/
		const_reference operator[](size_type n)	const { return _basket[n]; }

		/**
		 *  @brief  return a reference to the n'th element of the basket
		 * 
		 *  @param	n	the n'th reference to return
		 * 
		 *  @exception	if n is out of basket bound, an ft::out_of_bound exception is thrown.
		*/
		reference at(size_type n) { return _basket.at(n); }

		/**
		 *  @brief  return a const_reference to the n'th element of the basket
		 * 
		 *  @param	n	the n'th reference to return
		 * 
		 *  @exception	if n is out of basket bound, an ft::out_of_bound exception is thrown.
		*/
		const_reference at(size_type n)			const { return _basket.at(n); }

		/**
		 *  @brief	return a reference of the first element to the basket
		 * 
		 *  @details
		 *  if the basket is empty, it causes undefined behavior.
		*/
		reference	front() { return _basket.front(); }

		/**
		 *  @brief	return a const reference of the first element to the basket
		 * 
		 *  @details
		 *  if the basket is empty, it causes undefined behavior.
		*/
		const_reference	front()					const { return _basket.front(); }

		/**
		 *  @brief  return a reference of the last element to the basket
		 * 
		 *  @details
		 *  if the basket is empty, it causes undefined behavior.
		*/
		reference	back() { return _basket.back(); }

		/**
		 *  @brief	return a const reference of the last element to the basket
		 * 
		 * 	@details
		 *  If the basket is empty, it causes undefined behavior.
		*/
		const_reference	back()					const { return _basket.back(); }

		//GROUND MANAGEMENT

		/**
		 *  @brief  return a pointer of the ground.
		*/
		ground_type*		ground() { return _ground; }

		/**
		 *  @brief  return a const_pointer of the ground.
		*/
		const ground_type*	ground()	const { return _ground; }

		/**
		 *  @brief  set a new pointer of a ground.
		 * 
		 *  @param  groundPtr	a pointer to a ground
		 * 
		 *  @details
		 *  If the previous ground is self managed, he'll be destroy and deallocate
		 *  If the groundPtr is the same as the previous ground, nothing happends
		 *  If the groundPtr is null, the previous ground is replaced by a new managed ground,
		 *  and previous ground is destroy and deallocate is he's sefl managed.
		*/
		void				ground(ground_type* groundptr) { _groundControl(groundptr); }
		
		/**
		 *  @brief  return true if the ground of this instance is managed by this instance.
		*/
		bool				manage_ground() const { return _selfGround; }

		/**
		 *  @brief  set the management of the current ground.
		 * 
		 *  @details
		 *  Bad use will causes leaks or double free
		*/
		void				manage_ground(bool aJhon) { _selfGround = aJhon; }

		//BASKET / GROUND INTERACTIONS

		/**
		 *  @brief  drop the object pointed by the iterator on the ground
		 * 
		 *  @param  position  iterator pointing to the object to drop
		 * 
		 *  @return An iterator pointing to the element dropped to the ground
		 * 
		 *  @details
		 *  The object is copied to the ground then erased of the basket
		*/
		iterator	drop(iterator position)
		{
			iterator	groundIterator;

			groundIterator = _ground->insert(_ground->end(), *position);
			_basket.erase(position);
			return groundIterator;
		}

		/**
		 *  @brief  drop a range of objects
		 * 
		 *  @param  first 	iterator pointing to the first object to drop
		 *  @param  last	iterator pointing after the last object to drop
		 * 
		 *  @return An iterator pointing to the first element dropped to the ground
		 * 
		 *  @details
		 *  The objects are copied to the ground then erased of the basket
		*/
		iterator	drop(iterator first, iterator last)
		{
			iterator	groundIterator;

			groundIterator = _ground->insert(_ground->end(), first, last);
			_basket.erase(first, last);
			return groundIterator;
		}

		/**
		 *  @brief  drop all the basket content to thr ground
		 * 
		 *  @return An iterator pointing to the first element dropped to the ground
		 * 
		 * 	@details
		 *  The objects are copied to the ground then erased of the basket
		*/
		ft::pair<iterator, size_type>	drop()
		{
			ft::pair<iterator, size_type> ret;
			
			ret.second = _basket.size();
			ret.first = _ground->insert(_ground->end(), begin(), end());
			clear();
			return ret;
		}

		/**
		 *  @brief  drop the last element of the basket
		 * 
		 * 	@details
		 *  The object is copied to the ground then erased of the basket
		 *  If the basket is empty, it causes undefined behaviour
		*/
		void	drop_back()
		{
			if (!empty())
				drop(--end());
		}

		/**
		 *  @brief  try to pick the elements on the ground between first and last to the basket
		 * 
		 *  @param  basketPosition	iterator where the elements will be inserted
		 *  @param  first	iterator pointing to the first element to insert
		 *  @param  last	iterator pointing after the last element to insert
		 * 
		 *  @return  the numbers of element succesfully inserted to the basket
		 * 
		 *  @exception  if first > last, a ft::length_error exception is thrown
		 * 
		 *  
		 *  @details
		 *  If the basket is full, nothing happends
		 *  If the number of element to insert is > than size_left(), then size_left()
		 *  elements will be inserted
		 *  Elements are copied to the basket, then erased to the ground
		*/
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

		/**
		 *  @brief  try to pick an element on the ground
		 * 
		 *  @param  basketPosition	iterator where the elements will be inserted
		 *  @param  groundPosition	iterator pointing to the element to insert
		 *  
		 *  @return  the numbers of element succesfully inserted to the basket
		 * 
		 *  @exception  if first > last, a ft::length_error exception is thrown
		 * 
		 *  @details  If the basket is full, nothing happends
		 *  Elements are copied to the basket, then erased to the ground
		*/
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

		/**
		 *  @brief  try to pick the last elements on the ground
		 * 
		 *  @return  the numbers of element succesfully inserted to the baset
		 * 
		 *  @details  If the basket is full, nothing happends
		 *  Elements are copied to the basket, then erased to the ground
		*/
		size_type	pick_up()
		{
			if (!_ground->empty)
				return pick_up(end(), --_ground->end());
			return 0;
		}

		/**
		 *  @brief  pick elements on the ground until the basket is full or the ground is empty
		 * 
		 * 	@param  basketPosition  iterator pointing the element which will go after inserted elements.
		 *  @param  groundPosition  iterator pointing to the first element to insert until the end of the ground.
		 *  @return  the numbers of element succesfully inserted to the baset
		 * 
		 *  @details  If the basket is full, nothing happends
		 *  Elements are copied to the basket, then erased to the ground
		*/
		size_type	fill(iterator basketPosition, iterator groundPosition)
		{
			return pick_up(basketPosition, groundPosition, _ground->end());
		}

		/**
		 *  @brief  pick elements on the ground until the basket is full or the ground is empty
		 * 
		 *  @return  the numbers of element succesfully inserted to the basket.
		 * 
		 *  @details  The element are inserted at the iterator end() of the basket
		 *  The range to insert is the possibles lasts element of the ground
		 *  If the basket is full, nothing happends
		 *  Elements are copied to the basket, then erased to the ground
		*/
		size_type	fill()
		{
			size_type	sizeToPick;

			sizeToPick = ft::min(_ground->size(), size_left());
			return fill(end(), _ground->end() - sizeToPick);
		}

		/**
		 *  @brief  pour_out all the content of the basket in another basket.
		 * 
		 *  If the size_left() of the another basket is smaller than the content of this basket,
		 *  the surpluses are dropped to the ground of the another basket.
		 * 
		 *  @param  o  a reference to another basket.
		 * 
		 *  @return  the number of element succesfully inserted to the other basket.
		 * 
		*/
		size_type	pour_out(flat_basket& o)
		{
			if (&o == this)
				return 0;
			size_type	before = o.size();
			o.insert(o.end(), begin(), end());
			clear();

			return o.size() - before;
		}

		//MODIFIERS

		/**
		 *  @brief  clear the basket underlying container
		 * 
		 *  This will not clear the ground.
		*/
		void	clear() { _basket.clear(); }

		/**
		 *  @brief  insert an element to the basket or drop it to the ground
		 * 
		 *  @param  position  iterator pointing to the element which will go after the
		 *                    inserted element
		 *  @param  val   the value to copy
		 * 
		 *  @return  an iterator pointing to the inserted element. 
		 * 
		 *  @details
		 *  The returned iterator can be a basket iterator or a ground iterator if the basket was full.
		 *  
		*/
		iterator insert(iterator position, const value_type& val)
		{
			if (!full())
				return _basket.insert(position, val);
			return _ground->insert(_ground->end(), val);
		}

		/**
		 *  @brief  insert n elements to the basket and drop the surpluses to the ground.
		 * 
		 *  @param  position  Iterator pointing to the element which will go after the
		 *                    inserted element
		 *  @param  n     The number of elements to copy.
		 *  @param  val   The value to copy.
		 * 
		 *  @return  an iterator pointing to the inserted element. 
		 * 
		 *  @details
		 *  The returned iterator can be a basket iterator or a ground iterator if the basket was full.
		 *  
		*/
		void insert(iterator position, size_type n, const value_type& val)
		{
			size_type	left = size_left();
			if (n <= left)
				_basket.insert(position, n, val);
			else
			{
				_basket.insert(position, left, val);
				_ground->insert(_ground->end(), n - left, val);
			}
		}

		/**
		 *  @brief  insert a range of elements to the basket and drop the surpluses to the ground.
		 * 
		 *  @param  position  iterator pointing to the element which will go after the
		 *                    inserted element
		 *  @param  first   InputIterator pointing to the first object to copy
		 *  @param  last    InputIterator pointing after the last iterator to copy
		 *  @return  an iterator pointing to the inserted element
		 * 
		 *  @exception  if first > last, a ft::length_error exception is thrown.
		 * 
		 *  @details
		 *  The returned iterator can be a basket iterator or a ground iterator if the basket was full. 
		*/
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

		/**
		 *  @brief  try to insert an element at the end of the bucket.
		 * 
		 *  @param  val  The element to copy. 
		 * 
		 *  @details
		 *  If the bucket is full, the element is copied at the end of the ground.
		*/
		void	push_back(const value_type& val)
		{
			insert(end(), val);
		}

		/**
		 *  @brief  insert an element and drop the last element is the bucket is full.
		 * 
		 *  @param  position  Iterator pointing element which will go after the inserted element.
		 *  @param  val  The value to copy. 
		 * 
		 *  @details
		 *  If the bucket is full, the element is still inserted but
		 *  the last element is dropped to the ground.
		*/
		iterator insert_force(iterator position, const value_type& val)
		{
			if (full())
			{
				_ground->insert(_ground->end(), _basket.back());
				_basket.pop_back();
			}
			return _basket.insert(position, val);
		}

		/**
		 *  @brief  insert n elements and drop the lasts elements is the bucket has not enought space.
		 * 
		 *  @param  position  Iterator pointing element which will go after the inserted element.
		 *  @param  n  The number of elements to insert.
		 *  @param  val  The value to copy.
		 * 
		 *  @details
		 *  If the bucket doesn,t have enought space, the elements are still inserted but
		 *  the lasts elements is dropped to the ground. The surpluses of inserted element fall to the ground too.
		*/
		iterator insert_force(iterator position, size_type n, const value_type& val)
		{
			size_type	ctxSize;
			size_type	nbToPop;
			size_type	nbToInsert;
			ctxSize = end() - position;

			if (ctxSize && n > size_left())
			{
				nbToPop = ft::min(ctxSize, n - size_left());
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

		/**
		 *  @brief  insert a range of elements and drop the lasts elements is the bucket has not enought space.
		 * 
		 *  @param  position  Iterator pointing element which will go after the inserted element.
		 *  @param  first  InputIterator pointing to the first element to insert.
		 *  @param  last  InputIterator pointing after the last element to insert.
		 * 
		 *  @exception  if first > last, a ft::length_error exception is thrown
		 * 
		 *  @details
		 *  If the bucket doesn,t have enought space, the elements are still inserted but
		 *  the lasts elements is dropped to the ground. The surpluses of inserted element fall to the ground too.
		*/
		template < class InputIterator >
		typename enable_if<!is_integral< InputIterator >::value, iterator>::type
			insert_force(iterator position, InputIterator first, InputIterator last)
		{
			difference_type dist;
			size_type		nbToInsert;
			size_type		nbToPop;
			size_type		ctxSize;
			InputIterator	basketFirst;

			dist = ft::distance(first, last);
			if (dist < 0)
				throw(ft::length_error("ft::flat_basket::insert : iterator first must be less than iterator last."));

			ctxSize = end() - position;
			basketFirst = first;
			if (ctxSize && dist > size_left())
			{
				nbToPop = ft::min(ctxSize, dist - size_left());
				while (nbToPop--)
				{
					_ground->insert(_ground->end(), _basket.back());
					_basket.pop_back();
				}
				nbToInsert = size_left();
				while (nbToInsert--)
					++first;
			}
			else
				first = last;
			position = _basket.insert(position, basketFirst, first);
			if (first != last)
				position = _ground->insert(_ground->end(), first, last);
			return position;
		}

		/**
		 *  @brief  if basket is full, drop the last element, then insert at the end of the basket.
		 * 
		 *  @param  val  the element to copy;
		*/
		void	push_back_force(const value_type& val)
		{
			if (full() && _basketCapacity)
				pop_back();
			push_back(val);
		}

		/**
		 *  @brief  clear basket and insert n elements.
		 * 
		 *  @param  n  Number of elements to assign.
		 *  @param  val  Element to copy.
		*/
		void	assign(size_type n, value_type& val)
		{
			clear();
			insert(begin(), n, val);
		}

		/**
		 *  @brief  clear basket and insert a range of elements.
		 * 
		 *  @param  first InputIterator pointing to the first element to assign.
		 *  @param  last  InputIterator pointing after the last element to assign.
		 * 
		 *  @exception  if first > last, a ft::length_error exception is thrown.
		*/
		template < class InputIterator >
		typename enable_if< !is_integral< InputIterator >::value >::type
			assign(InputIterator first, InputIterator last)
		{
			clear();
			insert(begin(), first, last);
		}

		/**
		 *  @brief  erase element pointing by position
		 * 
		 *  @param  position  iterator pointing the element to erase
		*/
		iterator	erase(iterator position) { return _basket.erase(position); }
		
		/**
		 *  @brief  erase a range of elements
		 * 
		 *  @param  first  InputIterator pointing to the first element to erase.
		 *  @param  last  InputIterator pointing after the last element to erase
		*/
		iterator	erase(iterator first, iterator last) { return _basket.erase(first, last); }

		/**
		 *  @brief  erase the last element if it exists
		*/
		void	pop_back()
		{
			if (!empty())
				erase(--end());
		}

		/**
		 *  @brief  swap datas with another flat_basket
		 * 
		 *  @param  o  An another flat_basket with same element and allocator types.
		 * 
		 *  @details
		 *  Exchange elements between two flat_baskets in constant time.
		 *  Note that the selfGround management boolean is swapped too.
		*/
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

		/**
		 *  @brief  swap ground with another flat_basket
		 * 
		 *  @param  o  An another flat_basket with same element and allocator types.
		 * 
		 *  @details
		 *  Exchange grounds between two flat_baskets in constant time.
		 *  The basket isn't swapped here.
		 *  Note that the selfGround management boolean is swapped too.
		*/
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

		/**
		 *  @brief  swap basket with another flat_basket
		 * 
		 *  @param  o  An another flat_basket with same element and allocator types.
		 * 
		 *  @details
		 *  Exchange basket between two flat_baskets in constant time.
		 *  Note that the ground isn't swapped here.
		*/
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

	/**
	 *  @brief  flat_basket equality relation
	 *  @param  lhs a flat_basket
	 *  @param  rhs a flat_basket of the same type as lhs
	 *  @return  True if lhs is equal to rhs
	 * 
	 *  @details
	 *  This is an equivalence relation. It is linear in the size of the
	 *  flat_baskets (and not the grounds). The elements must have an == operator
	 * 
	 *  Note that only the baskets are compared, and not the grounds.
	*/
	template < class T, class Alloc >
	bool operator==(const flat_basket<T, Alloc>& lhs, const flat_basket<T, Alloc>& rhs)
	{
		return lhs.size() == rhs.size() && ft::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	// Based on operator==
	template < class T, class Alloc >
	bool operator!=(const flat_basket<T, Alloc>& lhs, const flat_basket<T, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	/**
	 *  @brief  flat_basket ordering relation
	 *  @param  lhs a flat_basket
	 *  @param  rhs a flat_basket of the same type as lhs
	 *  @return  True if lhs is lexicographically less than rhs
	 * 
	 *  @details
	 *  This is a total ordering relation. It is linear in the size of the
	 *  flat_baskets (and not the grounds). The elements must be comparable
	 *  with @c <.
	 * 
	 *  Note that only the baskets are compared, and not the grounds.
	*/
	template < class T, class Alloc >
	bool operator< (const flat_basket<T, Alloc>& lhs, const flat_basket<T, Alloc>& rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	//Based on operator <
	template < class T, class Alloc >
	bool operator<=(const flat_basket<T, Alloc>& lhs, const flat_basket<T, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	//Based on operator <
	template < class T, class Alloc >
	bool operator> (const flat_basket<T, Alloc>& lhs, const flat_basket<T, Alloc>& rhs)
	{
		return rhs < lhs;
	}

	//Based on operator <
	template < class T, class Alloc >
	bool operator>=(const flat_basket<T, Alloc>& lhs, const flat_basket<T, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	//Based on operator <
	template < class T, class Alloc >
	void	swap(flat_basket<T, Alloc>& lhs, flat_basket<T, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}

#endif