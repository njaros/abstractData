#ifndef LIST_HPP_ABSTRACT_DATA_BY_LE_N
#define LIST_HPP_ABSTRACT_DATA_BY_LE_N

# include <memory>
# include "iterator.hpp"
# include "reverse_iterator.hpp"
# include "functional.hpp"
# include "algorithm.hpp"

namespace ft
{
	template< class T, class Alloc = std::allocator< T > >
	class list {

	public:

		typedef 			Alloc	allocator_type;
		typedef typename	Alloc::value_type	value_type;
		typedef typename	Alloc::reference reference;
		typedef typename	Alloc::const_reference	const_reference;
		typedef typename	Alloc::pointer pointer;
		typedef typename	Alloc::const_pointer	const_pointer;
		typedef typename	Alloc::difference_type	difference_type;
		typedef typename	Alloc::size_type	size_type;

		struct Node
		{
			Node*	p; //previous
			Node*	n; //next
			pointer	content;
		};

		class MyCIt;
		class MyIt : public ft::iterator<ft::bidirectional_iterator_tag, value_type>
		{
			Node* _n;

		public:

			MyIt() : _n(0) {}
			MyIt(Node* n) : _n(n) {}
			MyIt(const MyIt& o) : _n(o._n) {}
			MyIt(const MyCIt& o) : _n(o.base()) {}
			~MyIt() {}

			virtual Node* base()	const { return _n; }

			MyIt& operator=(const MyIt& o)
			{
				_n = o._n;
				return *this;
			}

			MyIt& operator=(const MyCIt& o)
			{
				_n = o.base();
				return *this;
			}

			bool operator==(const MyIt& o)	const
			{
				return _n == o._n;
			}

			bool operator==(const MyCIt& o)	const
			{
				return _n == o.base();
			}

			bool operator!=(const MyIt& o)	const
			{
				return _n != o._n;
			}

			bool operator!=(const MyCIt& o)	const
			{
				return _n != o.base();
			}

			reference operator*()
			{
				return *(_n->content);
			}

			pointer operator->()
			{
				return _n->content;
			}

			MyIt& operator++()
			{
				_n = _n->n;
				return *this;
			}

			MyIt& operator--()
			{
				_n = _n->p;
				return *this;
			}

			MyIt operator++(int)
			{
				MyIt tmp = *this;
				_n = _n->n;
				return tmp;
			}

			MyIt operator--(int)
			{
				MyIt tmp = *this;
				_n = _n->p;
				return tmp;
			}
		};

		class MyCIt : public ft::iterator<ft::bidirectional_iterator_tag, const value_type>
		{
			Node* _n;

		public:

			MyCIt() : _n(0) {}
			MyCIt(Node* n) : _n(n) {}
			MyCIt(const MyIt& o) : _n(o.base()) {}
			MyCIt(const MyCIt& o) : _n(o._n) {}
			~MyCIt() {}

			virtual Node* base()	const { return _n; }

			MyCIt& operator=(const MyCIt& o)
			{
				_n = o._n;
				return *this;
			}

			MyCIt& operator=(const MyIt& o)
			{
				_n = o.base();
				return *this;
			}

			bool operator==(const MyIt& o)	const
			{
				return _n == o.base();
			}

			bool operator==(const MyCIt& o)	const
			{
				return _n == o._n;
			}

			bool operator!=(const MyIt& o)	const
			{
				return _n != o.base();
			}

			bool operator!=(const MyCIt& o)	const
			{
				return _n != o._n;
			}

			const_reference operator*()	const
			{
				return *(_n->content);
			}

			const_pointer operator->()	const
			{
				return _n->content;
			}

			MyCIt& operator++()
			{
				_n = _n->n;
				return *this;
			}

			MyCIt& operator--()
			{
				_n = _n->p;
				return *this;
			}

			MyCIt operator++(int)
			{
				MyCIt tmp = *this;
				_n = _n->n;
				return tmp;
			}

			MyCIt operator--(int)
			{
				MyCIt tmp = *this;
				_n = _n->p;
				return tmp;
			}
		};

		typedef MyIt	iterator;
		typedef MyCIt	const_iterator;
		typedef ft::reverse_iterator<MyIt>	reverse_iterator;
		typedef ft::reverse_iterator<MyCIt> const_reverse_iterator;

	private:

		typedef typename allocator_type::template rebind<Node>::other	_nodeAllocator_type;

		Node*	_end;
		size_type			_size;
		allocator_type		_alloc;
		_nodeAllocator_type	_allocN;

		//JUST TO DEFINE MAX_SIZE

		struct _maxSizeElt
		{
			Node* a;
			Node* b;
			value_type c;
			pointer d;
		};

		std::allocator<_maxSizeElt>	_allocMaxSize;

		//UTILITIES

		Node* _getNewNullNode()
		{
			Node* nullNode;

			nullNode = _allocN.allocate(1);
			nullNode->n = nullNode;
			nullNode->p = nullNode;
			nullNode->content = 0;
			return nullNode;
		}

		Node* _getNewNode(Node* parent, Node* next, const value_type& val = value_type())
		{
			Node* newNode;

			newNode = _allocN.allocate(1);
			newNode->n = next;
			newNode->p = parent;
			newNode->content = _alloc.allocate(1);
			_alloc.construct(newNode->content, val);
			return newNode;
		}

		void	_deleteNode(Node* toDel)
		{
			_alloc.destroy(toDel->content);
			_alloc.deallocate(toDel->content, 1);
			_allocN.deallocate(toDel, 1);
		}

		void	_swapNodeContent(Node* a, Node* b)
		{
			pointer tmp;

			tmp = a->content;
			a->content = b->content;
			b->content = tmp;
		}

		template < class Compare >
		Node* _mergeSelf(Node* first, Node* middle, Node* last, Compare comp)
		{
			/*
			* I assume that the list is sorted in range [first, middle[ and in range [middle, last[
			*/
			Node* nextFirst;
			Node* tmp;
			Node* lastFirstElt;
			//Because the two part-lists are sorted, I can already define my nextFirst
			if (!comp(*(first->content), *(middle->content)))
				nextFirst = middle;
			else
				nextFirst = first;

			lastFirstElt = middle->p;
			while (middle != last)
			{
				tmp = middle;
				//[oaooo][boooo] on avance a tant que b n'est pas inserable, ie b > a
				// ->
				while (comp(*(first->content), *(middle->content)))
					first = first->n;
				if (first == middle)
					return nextFirst;
				//ici je decompte tout les element qui suivent b inserable avant a, ie tant que b->n <= a, on avance b
				while (middle->n != last && !comp(*(first->content), *(middle->n->content)))
					middle = middle->n;
				//je colle les bouts comme il faut et j'incremente middle
				middle->n->p = lastFirstElt;
				lastFirstElt->n = middle->n;
				tmp->p = first->p;
				first->p->n = tmp;
				middle->n = first;
				first->p = middle;
				middle = lastFirstElt->n;
			}
			return nextFirst;
		}

		template < class Compare >
		Node* _sort(Node*& first, size_type size, Compare comp)
		{
			Node* middle;
			Node* last;

			if (!size)
				return first;
			if (size == 1)
				return first->n;

			middle = _sort(first, size / 2, comp);
			last = _sort(middle, size - (size / 2), comp);
			first = _mergeSelf(first, middle, last, comp);
			return last;
		}

		template < class Compare >
		bool	_isSorted(const list& l, Compare comp)
		{
			const_iterator current;
			const_iterator previous;

			current = l.begin();
			previous = l.begin();
			++current;
			while (current != l.end())
			{
				if (comp(*(current++), *(previous++)))
					return false;
			}
			return true;
		}

	public:

		//CONSTRUCTORS DESTRUCTOR OPERATOR=

		explicit list(const allocator_type& alloc = allocator_type()) : _end(_getNewNullNode()), _size(0), _alloc(alloc) {}
		list(const list& o) : _end(_getNewNullNode()), _size(o._size), _alloc(o._alloc)
		{
			Node* newNode;
			Node* first;

			first = _end;
			for (typename ft::list<value_type>::const_iterator cit = o.begin(); cit != o.end(); ++cit)
			{
				newNode = _getNewNode(first, 0, *cit);
				first->n = newNode;
				first = newNode;
			}
			first->n = _end;
			_end->p = first;
		}
		explicit list(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
			: _end(_getNewNullNode()), _size(n), _alloc(alloc)
		{
			Node* newNode;
			Node* first;

			if (n >= _allocMaxSize.max_size())
				throw (ft::length_error("ft::list::fillConstructor : they're is too much elements"));
			first = _end;
			while (n--)
			{
				newNode = _getNewNode(first, 0, val);
				first->n = newNode;
				first = newNode;
			}
			first->n = _end;
			_end->p = first;
		}
		template < typename InputIterator >
		list(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
			typename ft::enable_if< !ft::isIntegral< InputIterator >::value >::type* = 0)
			: _end(_getNewNullNode()), _size(0), _alloc(alloc)
		{
			difference_type	dist;
			Node* newNode;
			Node* firstNode;

			dist = ft::distance(first, last);
			if (dist < 0)
				throw (ft::length_error("ft::list::rangeConstructor: Input last must be greater then first"));
			_size = dist;
			firstNode = _end;
			while (first != last)
			{
				newNode = _getNewNode(firstNode, 0, *first);
				++first;
				firstNode->n = newNode;
				firstNode = newNode;
			}
			firstNode->n = _end;
			_end->p = firstNode;
		}
		~list()
		{
			clear();
			_allocN.deallocate(_end, 1);
		}
		list& operator=(const list& other)
		{
			Node* tmp;
			Node* first;

			if (&other == this)
				return *this;

			if (other.empty())
			{
				clear();
				return *this;
			}

			first = _end;
			for (const_iterator cit = other.begin(); cit != other.end(); ++cit)
			{
				if (first->n && first->n != _end)
				{
					first = first->n;
					_alloc.destroy(first->content);
					_alloc.construct(first->content, *cit);
				}
				else
				{
					tmp = _getNewNode(first, 0, *cit);
					first->n = tmp;
					first = tmp;
				}
			}
			if (first->n)
			{
				while (first->n != _end)
				{
					tmp = first->n;
					first->n = first->n->n;
					_deleteNode(tmp);
				}
			}
			else
				first->n = _end;
			_end->p = first;
			_size = other._size;
			return *this;
		}

		//ITERATORS

		iterator				begin() { return iterator(_end->n); }
		iterator				end() { return iterator(_end); }
		const_iterator			begin()		const { return const_iterator(_end->n); }
		const_iterator			end()		const { return const_iterator(_end); }
		reverse_iterator		rbegin() { return reverse_iterator(end()); }
		reverse_iterator		rend() { return reverse_iterator(begin()); }
		const_reverse_iterator	rbegin()	const { return const_reverse_iterator(end()); }
		const_reverse_iterator	rend()		const { return const_reverse_iterator(begin()); }

		//CAPACITY

		bool		empty()		const { return !_size; }
		size_type	size()		const { return _size; }
		size_type	max_size()	const { return _allocMaxSize.max_size(); }

		//ELEMENTS ACCESS

		reference		front() { return *(_end->n->content); }
		const_reference	front()	const { return *(_end->n->content); }
		reference		back() { return *(_end->p->content); }
		const_reference	back()	const { return *(_end->p->content); }

		//MODIFIERS

		template < class InputIterator >
		void	assign(InputIterator first, InputIterator last,
			typename ft::enable_if< !ft::isIntegral< InputIterator >::value >::type* = 0)
		{
			Node* tmp;
			Node* firstNode;
			difference_type dist;

			dist = ft::distance(first, last);
			if (dist < 0)
				throw (ft::length_error("ft::list::assign : last input must be greater than first input"));
			_size = (size_type)dist;
			if (!_size)
			{
				clear();
				return;
			}
			firstNode = _end;
			while (first != last)
			{
				if (firstNode->n && firstNode->n != _end)
				{
					firstNode = firstNode->n;
					_alloc.destroy(firstNode->content);
					_alloc.construct(firstNode->content, *first);
				}
				else
				{
					tmp = _getNewNode(firstNode, 0, *first);
					firstNode->n = tmp;
					firstNode = tmp;
				}
				++first;
			}
			if (firstNode->n)
			{
				while (firstNode->n != _end)
				{
					tmp = firstNode->n;
					firstNode->n = firstNode->n->n;
					_deleteNode(tmp);
				}
			}
			else
				firstNode->n = _end;
			_end->p = firstNode;
		}
		void	assign(size_type n, const value_type& val = value_type())
		{
			Node* tmp;
			Node* firstNode;

			if (n >= _allocMaxSize.max_size())
				throw (ft::length_error("ft::list::assign : they're is too much elements"));

			_size = n;
			if (!_size)
			{
				clear();
				return;
			}
			firstNode = _end;
			while (n--)
			{
				if (firstNode->n && firstNode->n != _end)
				{
					firstNode = firstNode->n;
					_alloc.destroy(firstNode->content);
					_alloc.construct(firstNode->content, val);
				}
				else
				{
					tmp = _getNewNode(firstNode, 0, val);
					firstNode->n = tmp;
					firstNode = tmp;
				}
			}
			if (firstNode->n)
			{
				while (firstNode->n != _end)
				{
					tmp = firstNode->n;
					firstNode->n = firstNode->n->n;
					_deleteNode(tmp);
				}
			}
			else
				firstNode->n = _end;
			_end->p = firstNode;
		}

		void	clear()
		{
			Node* tmp;

			while (_end->n != _end)
			{
				tmp = _end->n;
				_end->n = _end->n->n;
				_deleteNode(tmp);
			}
			_end->p = _end;
			_size = 0;
		}

		iterator	insert(iterator position, const value_type& val)
		{
			Node* newNode;

			newNode = _getNewNode(position.base()->p, position.base(), val);
			position.base()->p->n = newNode;
			position.base()->p = newNode;
			++_size;
			return iterator(newNode);
		}
		void	insert(iterator position, size_type n, const value_type& val)
		{
			if (n + _size >= _allocMaxSize.max_size())
				throw (ft::length_error("ft::list::insert : too much element to insert."));

			while (n--)
				insert(position, val);
		}
		template < class InputIterator >
		void	insert(iterator position, InputIterator first, InputIterator last,
			typename ft::enable_if< !ft::isIntegral< InputIterator >::value >::type* = 0)
		{
			difference_type	dist;

			dist = ft::distance(first, last);
			if (dist < 0)
				throw (ft::length_error("ft::list::insert : last input must be greater than first input"));
			if (_size + dist >= _allocMaxSize.max_size())
				throw (ft::length_error("ft::list::insert : too much element to insert."));

			while (first != last)
			{
				insert(position, *first);
				++first;
			}
		}

		iterator	erase(iterator position)
		{
			Node* toDel;
			Node* toReturn;

			if (position == end())
				return position;
			toDel = position.base();
			toDel->p->n = toDel->n;
			toDel->n->p = toDel->p;
			toReturn = toDel->n;
			_deleteNode(toDel);
			--_size;
			return iterator(toReturn);
		}
		iterator	erase(iterator first, iterator last) //naive implementation
		{
			while (first != last)
				erase(first++);
			return last;
		}

		void	pop_back()
		{
			if (_size)
				erase(--end());
		}

		void	pop_front()
		{
			if (_size)
				erase(begin());
		}

		void	push_back(const value_type& val)
		{
			insert(end(), val);
		}

		void	push_front(const value_type& val)
		{
			insert(begin(), val);
		}

		void	resize(size_type n, const value_type& val = value_type())
		{
			if (n >= _allocMaxSize.max_size())
				throw (ft::length_error("ft::list::resize : size too high."));
			if (n < _size)
			{
				while (n < _size)
					pop_back();
			}
			else if (n > _size)
				insert(end(), n - _size, val);
		}

		void	swap(list& o)
		{
			Node* nodeTmp;
			size_type		sizeTmp;
			allocator_type	allocTmp;

			nodeTmp = _end;
			sizeTmp = _size;
			allocTmp = _alloc;

			_end = o._end;
			_size = o._size;
			_alloc = o._alloc;

			o._end = nodeTmp;
			o._size = sizeTmp;
			o._alloc = allocTmp;
		}

		//LIST OPERATIONS

		void	splice(iterator position, list& x)
		{
			Node* first;
			Node* last;
			Node* beforeInsert;
			Node* afterInsert;

			if (x.empty())
				return;

			//Extraction from x
			first = x._end->n;
			last = x._end->p;
			_size += x._size;
			x._size = 0;
			x._end->n = x._end;
			x._end->p = x._end;

			//Insertion to self
			beforeInsert = position.base()->p;
			afterInsert = position.base();
			beforeInsert->n = first;
			first->p = beforeInsert;
			afterInsert->p = last;
			last->n = afterInsert;
		}

		void	splice(iterator position, list& x, iterator i)
		{
			Node* toInsert;
			Node* beforeInsert;
			Node* afterInsert;

			//Extraction from x
			--(x._size);
			toInsert = i.base();
			toInsert->p->n = toInsert->n;
			toInsert->n->p = toInsert->p;
			beforeInsert = position.base()->p;
			afterInsert = position.base();

			//Insertion to self
			beforeInsert->n = toInsert;
			toInsert->p = beforeInsert;
			afterInsert->p = toInsert;
			toInsert->n = afterInsert;
			++_size;
		}

		void	splice(iterator position, list& x, iterator start, iterator end)
		{
			Node* first;
			Node* last;
			Node* beforeInsert;
			Node* afterInsert;
			difference_type	dist;

			if (start == end)
				return;

			//Extraction from x
			first = start.base();
			last = end.base()->p;
			dist = ft::distance(start, end);
			x._size -= dist;
			first->p->n = last->n;
			last->n->p = first->p;

			//Insertion to self
			_size += dist;
			beforeInsert = position.base()->p;
			afterInsert = position.base();
			beforeInsert->n = first;
			first->p = beforeInsert;
			afterInsert->p = last;
			last->n = afterInsert;
		}

		void	remove(const value_type& val)
		{
			iterator current;

			current = begin();
			while (current != end())
			{
				if (*current == val)
					current = erase(current);
				else
					++current;
			}
		}

		template < class Predicate >
		void	remove_if(Predicate pred)
		{
			iterator current;

			current = begin();
			while (current != end())
			{
				if (pred(*current))
					current = erase(current);
				else
					++current;
			}
		}

		void	unique()
		{
			iterator current;
			iterator previous;

			current = begin();
			previous = begin();
			++current;
			while (current != end())
			{
				if (*current == *previous)
					current = erase(current);
				else
				{
					++current;
					++previous;
				}
			}
		}

		template < class BinaryPredicate >
		void	unique(BinaryPredicate binary_pred)
		{
			iterator current;
			iterator previous;

			current = begin();
			previous = begin();
			++current;
			while (current != end())
			{
				if (binary_pred(*current, *previous))
					current = erase(current);
				else
				{
					++current;
					++previous;
				}
			}
		}

		template < class Compare >
		void	merge(list& x, Compare comp)
		{
			Node* start;
			Node* selfIdx;
			Node* tmp;

			if (x.empty())
				return;

			if (_isSorted(x, comp))
			{
				start = x._end->n;
				selfIdx = _end->n;
				while (start != x._end)
				{
					while (selfIdx != _end && !comp(*(start->content), *(selfIdx->content)))
						selfIdx = selfIdx->n;
					tmp = start;
					while (start->n != x._end && (selfIdx == _end || !comp(*(selfIdx->content), *(start->n->content))))
						start = start->n;

					selfIdx->p->n = tmp;
					tmp->p = selfIdx->p;
					selfIdx->p = start;

					tmp = start;
					start = start->n;
					tmp->n = selfIdx;
				}
				_size += x._size;
				x._size = 0;
				x._end->n = x._end;
				x._end->p = x._end;
			}
			else
				splice(end(), x); //windows STL behaviour
		}

		void	merge(list& x)
		{
			merge(x, ft::less< value_type >());
		}

		template < class Compare >
		void	sort(Compare comp)
		{
			//recurrence entrypoint
			_sort(_end->n, _size, comp);
		}

		void	sort()
		{
			sort(ft::less< value_type >());
		}

		void	reverse()
		{
			Node* avance;
			Node* recule;

			avance = _end->n;
			recule = _end->p;

			while (avance != recule)
			{
				_swapNodeContent(avance, recule);
				if (avance->n == recule)
					return;
				avance = avance->n;
				recule = recule->p;
			}
		}

		//OBSERVERS

		allocator_type	get_allocator() const
		{
			return _alloc;
		}
	};

	//NON MEMBER OVERLOADS

	template <class T, class Alloc>  bool operator== (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		if (lhs.size() != rhs.size())
			return false;
		return ft::equal(lhs.begin(), lhs.end(), rhs.begin());
	}

	template <class T, class Alloc>  bool operator!= (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class T, class Alloc>  bool operator<  (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class T, class Alloc>  bool operator<= (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return !(rhs < lhs);
	}

	template <class T, class Alloc>  bool operator>  (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return rhs < lhs;
	}

	template <class T, class Alloc>  bool operator>= (const list<T, Alloc>& lhs, const list<T, Alloc>& rhs)
	{
		return !(lhs < rhs);
	}

	template <class T, class Alloc>
	void	swap(list<T, Alloc>& x, list<T, Alloc>& y)
	{
		x.swap(y);
	}

}
#endif