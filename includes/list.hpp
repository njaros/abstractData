#ifndef LIST_HPP_ABSTRACT_DATA_BY_LE_N
#define LIST_HPP_ABSTRACT_DATA_BY_LE_N

namespace ft
{
	template< class T, class Alloc >
	class list {

	public:

		typedef	T	value_type;
		typedef Alloc	allocator_type;
		typedef T& reference;
		typedef const reference	const_reference;
		typedef T* pointer;
		typedef const pointer	const_pointer;
		typedef typename ft::iterator_traits<allocator_type>::difference_type	difference_type;
		typedef typename Alloc::size_type	size_type;

		struct Node
		{
			Node* p; //previous
			Node* n; //next
			T*	content;
		};

		class MyCIt;
		class MyIt : ft::iterator<ft::bidirectional_iterator_tag, value_type>
		{
			Node*	_n;

		public:

			MyIt() : _n(nullptr) {}
			MyIt(Node* n) : _n(n) {}
			MyIt(const MyIt& o) : _n(o._n) {}
			MyIt(const MyCIt& o) : _n(o._n) {}
			~MyIt() {}

			MyIt& operator=(const MyIt& o)
			{
				_n = o._n;
				return *this;
			}

			MyIt& operator=(const MyCIt& o)
			{
				_n = o._n;
				return *this;
			}

			bool operator==(const MyIt& o)
			{
				return _n == o._n;
			}

			bool operator==(const MyCIt& o)
			{
				return _n == o._n;
			}

			bool operator!=(const MyIt& o)
			{
				return _n != o._n;
			}

			bool operator!=(const MyCIt& o)
			{
				return _n != o._n;
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
				return &this;
			}

			MyIt& operator--()
			{
				_n = _n->p;
				return &this;
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

		class MyCIt : ft::iterator<ft::bidirectional_iterator_tag, const value_type>
		{
			Node* _n;

		public:

			MyCIt() : _n(nullptr) {}
			MyCIt(Node* n) : _n(n) {}
			MyCIt(const MyIt& o) : _n(o._n) {}
			MyCIt(const MyCIt& o) : _n(o._n) {}
			~MyCIt() {}

			MyCIt& operator=(const MyIt& o)
			{
				_n = o._n;
				return *this;
			}

			MyCIt& operator=(const MyCIt& o)
			{
				_n = o._n;
				return *this;
			}

			bool operator==(const MyIt& o)
			{
				return _n == o._n;
			}

			bool operator==(const MyCIt& o)
			{
				return _n == o._n;
			}

			bool operator!=(const MyIt& o)
			{
				return _n != o._n;
			}

			bool operator!=(const MyCIt& o)
			{
				return _n != o._n;
			}

			const_reference operator*()
			{
				return *(_n->content);
			}

			const_pointer operator->()
			{
				return _n->content;
			}

			MyCIt& operator++()
			{
				_n = _n->n;
				return &this;
			}

			MyCIt& operator--()
			{
				_n = _n->p;
				return &this;
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

		Node*				_end;
		size_type			_size;
		allocator_type		_alloc;
		_nodeAllocator_type	_allocN;

		//UTILITIES

		Node*	_getNewNullNode()
		{
			Node* nullNode;
			
			nullNode = _allocN.alloc(1);
			nullNode->n = nullptr;
			nullNode->p = nullptr;
			nullNode->content = nullptr;
			return nullNode;
		}

		Node* _getNewNode(Node* parent, Node* next, const value_type& val = value_type())
		{
			Node* newNode;
			
			newNode = _allocN.alloc(1);
			newNode->n = next;
			newNode->p = parent;
			newNode->content = _alloc.alloc(1);
			_alloc.construct(newNode->content, val);
			return newNode;
		}

	public:

		explicit list(const allocator_type& alloc = allocator_type()) : _end(_getNewNullNode()), _size(0), _alloc(alloc) {}
		list(const list& o) : _end(_getNewNullNode), _size(o._size), _alloc(o._alloc)
		{
			Node* newNode;
			Node* first;

			first = _end;
			for (typename ft::list<value_type>::const_iterator cit = o.begin(); cit != o.end(); ++cit)
			{
				newNode = _getNewNode(first, nullptr, *cit);
				first->n = newNode;
				first = newNode;
			}
			first->n = _end;
		}
		explicit list(size_type n, const value_type& val, const allocator_type& alloc = allocator_type())
			: _end(_getNewNullNode()), _size(n), _alloc(alloc)
		{
			Node* newNode;
			Node* first;

			first = _end;
			while (n--)
			{
				newNode = _getNewNode(first, nullptr, val);
				first->n = newNode;
				first = newNode;
			}
			first->n = _end;
		}
		template < typename InputIterator >
		list(InputIterator first, InputIterator last,
			typename ft::enable_if< !ft::isIntegral< InputIterator >::value >::type* = 0)
		{
			difference_type	dist;
			Node*			newNode;
			Node*			firstNode;

			if (ft::is_pointer<value_type, InputIterator>::value)
				dist = last - first;
			else
				dist = ft::distance(first, last);
			if (dist < 0)
				throw (ft::length_error("list::rangeConstructor: Input last must be greater then first"));

			_size = dist;
			firstNode = _end;
			while (first != last)
			{
				newNode = _getNewNode(firstNode, nullptr, *first);
				++first;
				firstNode->n = newNode;
				firstNode = newNode;
			}
			firstNode->n = _end;
		}
		
	};
}

#endif