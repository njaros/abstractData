#ifndef SET_HPP
# define SET_HPP

# include <memory>
# include <cstddef>
# include <cstring>
# include "functional.hpp"
# include "iterator.hpp"
# include "treeManager.hpp"
# include "utility.hpp"
# include "reverse_iterator.hpp"
# include "algorithm.hpp"

namespace ft {

	template<class Key, class Compare = ft::less<Key>, class Allocator = std::allocator<Key> >
	class set {

	public:

		// TYPEDEFS

		typedef Key									key_type;
		typedef Key									value_type;
		typedef Allocator							allocator_type;
		typedef typename Allocator::size_type		size_type;
		typedef typename Allocator::difference_type	difference_type;
		typedef Compare							 	key_compare;
		typedef Compare							 	value_compare;
		typedef value_type&							reference;
		typedef const value_type&					const_reference;
		typedef typename Allocator::pointer			pointer;
		typedef typename Allocator::const_pointer	const_pointer;

		// ITERATOR CLASS AND HIS TYPEDEFS

		class MyIterator : public ft::iterator<ft::bidirectional_iterator_tag, const value_type, difference_type, const_pointer, const_reference> {

			node<value_type>* _p;

		public:

			MyIterator() : _p(0) {}

			explicit MyIterator(node<value_type>* ptr) : _p(ptr) {}

			MyIterator(const MyIterator& other) : _p(other._p) {}

			~MyIterator() {}

			bool operator==(const MyIterator& other)	const { return (_p == other._p); }

			bool operator!=(const MyIterator& other)	const { return (_p != other._p); }

			MyIterator& operator=(MyIterator const& other) {
				this->_p = other._p;
				return (*this);
			}

			MyIterator& operator=(node<value_type>* ptr) {
				this->_p = ptr;
				return (*this);
			}

			const_reference operator*()	const {
				return (*(_p->content));
			}

			const_pointer operator->()	const {
				return (_p->content);
			}

			MyIterator& operator++() {
				if (isNotLeaf(_p))
					this->_p = getSuccessor(_p);
				else
					_p = _p->left;
				return (*this);
			}

			MyIterator operator++(int) {
				MyIterator temp(*this);
				if (isNotLeaf(_p))
					this->_p = getSuccessor(_p);
				else
					_p = _p->left;
				return (temp);
			}

			MyIterator& operator--() {
				if (isLeaf(_p))
					_p = _p->father;
				else
					_p = getPredecessor(_p);
				return (*this);
			}

			MyIterator operator--(int) {
				MyIterator temp(*this);
				if (isLeaf(_p))
					_p = _p->father;
				else
					_p = getPredecessor(_p);
				return (temp);
			}

			node<value_type>* base()	const { return (_p); }

		};

		typedef MyIterator									iterator;
		typedef MyIterator									const_iterator;
		typedef ft::reverse_iterator<iterator>				reverse_iterator;
		typedef ft::reverse_iterator<iterator>				const_reverse_iterator;

		// CONSTRUCTORS / DESTRUCTOR / OPERATOR=

		set(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(0), _size(0), _alloc(alloc), _compare(comp)
		{
			_leaf = doALeaf(value_type(), _allocNode, _alloc);
		}

		template < class InputIterator >
		set(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(0), _size(0), _alloc(alloc), _compare(comp)
		{
			_leaf = doALeaf(value_type(), _allocNode, _alloc);
			while (first != last) {
				insert(*first);
				first++;
			}
		}

		set(const set& other)
			: _root(0), _size(0), _alloc(other._alloc), _compare(other._compare)
		{
			_leaf = doALeaf(value_type(), _allocNode, _alloc);
			*this = other;
		}

		~set() {
			if (_root)
				recursiveBurnTheTree<value_type, std::allocator<node<value_type> >, allocator_type>(_root, _allocNode, _alloc);
			destroyNode(_leaf, _allocNode, _alloc);
		}

		set& operator=(const set& other) {
			if (&other == this)
				return (*this);
			_size = other._size;
			_compare = other._compare;
			if (_root) {
				recursiveBurnTheTree<value_type, std::allocator<node<value_type> >, allocator_type>(_root, _allocNode, _alloc);
				_root = 0;
			}
			if (other._root) {
				_copyTree(&_root, other._root, 0);
				_doTheLeafs(_root);
			}
			return (*this);
		}

		// ITERATOR FUNCTIONS

		iterator				begin()
		{
			if (!_root)
				return (end());
			return (iterator(getMinNode(_root)));
		}
		const_iterator		  begin() const
		{
			if (!_root)
				return (end());
			return (const_iterator(getMinNode(_root)));
		}
		iterator				end()
		{
			return (iterator(_leaf));
		}
		const_iterator		  end()   const
		{
			return (const_iterator(_leaf));
		}
		reverse_iterator		rbegin() {
			return (reverse_iterator(end()));
		}
		const_reverse_iterator  rbegin() const {
			return (const_reverse_iterator(end()));
		}
		reverse_iterator		rend() {
			return (reverse_iterator(begin()));
		}
		const_reverse_iterator  rend() const {
			return (const_reverse_iterator(begin()));
		}

		// CAPACITY

		bool		empty()	 const { return (!_root); }
		size_type   size()	  const { return (_size); }
		size_type   max_size()  const { return (_maxSizeAlloc.max_size()); }

		// MODIFIERS

		ft::pair<iterator, bool>	insert(const_reference data)
		{
			if (!_root) {
				_root = insertNodeSet<value_type, Compare>(&_root,
					newNode<value_type, std::allocator<node<value_type> >, allocator_type>(
						data, _allocNode, _alloc), _leaf,
					_compare).first;
				_size++;
				_it = _root;
				return (ft::make_pair<iterator, bool>(_it, true));
			}
			else {
				_it = find(data);
				if (_it == end()) {
					_insertValue = insertNodeSet<value_type, Compare>(&_root,
						newNode<value_type, std::allocator<node<value_type> >, allocator_type>(
							data, _allocNode, _alloc), _leaf,
						_compare);
					_it = _insertValue.first;
					_root = getRoot(_root);
				}
				else
					return (ft::make_pair<iterator, bool>(_it, false));
				_size++;
				return (ft::make_pair<iterator, bool>(_it, _insertValue.second));
			}
		}

		iterator					insert(iterator hint, const value_type& data) {
			if (!_root || hint == end())
				return iterator(insert(data).first);
			bool	highest = false;
			bool	smallest = false;
			_it = begin();
			if (_compare(data, *_it)) {
				smallest = true;
				hint = _it;
			}
			else if (equalIterator(_it, --end()) && _compare(*_it, data)) {
				highest = true;
				hint = _it;
			}
			else if (_compare(data, *hint))
			{
				if (!_compare(*(getPredecessor(hint.base())->content), data))
					return (insert(data).first);
				else if (hint.base()->father == getPredecessor(hint.base()))
					--hint;
			}
			else if (_compare(*hint, data))
			{
				if (!_compare(data, *(getSuccessor(hint.base())->content)))
					return (insert(data).first);
				else if (hint.base()->father == getSuccessor(hint.base()))
					++hint;
			}
			else
				return (hint);
			++_size;
			iterator	inserted(recursiveInsertSet(hint.base(), newNode<value_type, \
				std::allocator< node < value_type> >, allocator_type>(data, _allocNode, _alloc), \
				_compare, smallest, highest).first);
			fixTree(inserted.base(), &_root);
			_root = getRoot(_root);
			return (inserted);
		}

		template< class InputIterator >
		void						insert(InputIterator first, InputIterator last) {
			while (first != last) {
				insert(*first);
				first++;
			}
		}

		void						erase(iterator position) {
			node<value_type>* nodeToDel = _findNode(*position, _root);
			if (nodeToDel) {
				deleteNode<value_type, std::allocator<node<value_type> >, allocator_type>(
					nodeToDel, &_root, _allocNode, _alloc);
				if (!--_size) {
					_root = 0;
					_leaf->left = 0;
					_leaf->right = 0;
				}
				else {
					_leaf->father = getMaxNode(_root);
					_leaf->left = getMinNode(_root);
				}
			}
		}
		size_type				   erase(const key_type& k) {
			node<value_type>* n = _findNode(k, _root);

			if (!n)
				return (0);
			deleteNode<value_type, std::allocator<node<value_type> >, allocator_type>(n, &_root, _allocNode, _alloc);
			if (!--_size) {
				_root = 0;
				_leaf->left = 0;
				_leaf->father = 0;
			}
			else
			{
				_leaf->father = getMaxNode(_root);
				_leaf->left = getMinNode(_root);
			}
			return (1);
		}
		void						erase(iterator first, iterator last)
		{
			while (first != last)
			{
				erase(first++);
			}
		}
		void						swap(set& other) {
			node<value_type>* tempRoot = _root;
			node<value_type>* tempLeaf = _leaf;
			size_type			tempSize = _size;
			allocator_type		tempAlloc = _alloc;
			key_compare			tempCompare = _compare;

			_root = other._root;
			_size = other._size;
			_leaf = other._leaf;
			_alloc = other._alloc;
			_compare = other._compare;

			other._root = tempRoot;
			other._size = tempSize;
			other._leaf = tempLeaf;
			other._alloc = tempAlloc;
			other._compare = tempCompare;
		}
		void						clear() {
			if (_root) {
				recursiveBurnTheTree<value_type, std::allocator<node<value_type> >, allocator_type>(_root, _allocNode, _alloc);
				_root = 0;
				_size = 0;
				_leaf->father = 0;
				_leaf->left = 0;
			}
		}

		// OBSERVERS

		key_compare	 key_comp()	  const { return (_compare); }
		value_compare   value_comp()	const { return (_compare); }

		// OPERATIONS

		iterator			find(const key_type& k) {
			node<value_type>* search = _root;

			if (!search)
				return end();
			while (isNotLeaf(search))
			{
				if (!_compare(*(search->content), k) && !_compare(k, *(search->content)))
					return (iterator(search));
				if (_compare(*(search->content), k))
					search = search->right;
				else
					search = search->left;
			}
			return (iterator(search));
		}
		const_iterator	  find(const key_type& k) const {
			node<value_type>* search = _root;

			if (!search)
				return end();
			while (isNotLeaf(search))
			{
				if (!_compare(*(search->content), k) && !_compare(k, *(search->content)))
					return (const_iterator(search));
				if (_compare(*(search->content), k))
					search = search->right;
				else
					search = search->left;
			}
			return (const_iterator(search));
		}
		size_type		   count(const key_type& k)	const {
			node<value_type>* search = _root;

			if (!search)
				return 0;
			while (isNotLeaf(search))
			{
				if (!_compare(*(search->content), k) && !_compare(k, *(search->content)))
					return (1);
				if (_compare(*(search->content), k))
					search = search->right;
				else
					search = search->left;
			}
			return (0);
		}
		iterator			lower_bound(const key_type& k) {
			node<value_type>* begin = _root;

			if (!begin)
				return end();
			while (isNotLeaf(begin)) {
				if (_compare(*(begin->content), k))
					begin = begin->right;
				else if (isNotLeaf(begin->left) && !_compare(*(getPredecessor(begin)->content), k))
					begin = begin->left;
				else
					break;
			}
			return (iterator(begin));
		}
		const_iterator	  lower_bound(const key_type& k)  const {
			node<value_type>* begin = _root;

			if (!begin)
				return end();
			while (isNotLeaf(begin)) {
				if (_compare(*(begin->content), k))
					begin = begin->right;
				else if (isNotLeaf(begin->left) && !_compare(*(getPredecessor(begin)->content), k))
					begin = begin->left;
				else
					break;
			}
			return (const_iterator(begin));
		}
		iterator			upper_bound(const key_type& k) {
			node<value_type>* begin = _root;

			if (!begin)
				return end();
			while (isNotLeaf(begin)) {
				if (!_compare(k, *(begin->content)))
					begin = begin->right;
				else if (isNotLeaf(begin->left) && _compare(k, *(getPredecessor(begin)->content)))
					begin = begin->left;
				else
					break;
			}
			return (iterator(begin));
		}
		const_iterator	  upper_bound(const key_type& k)  const {
			node<value_type>* begin = _root;

			if (!begin)
				return end();
			while (isNotLeaf(begin)) {
				if (!_compare(k, *(begin->content)))
					begin = begin->right;
				else if (isNotLeaf(begin->left) && _compare(k, *(getPredecessor(begin)->content)))
					begin = begin->left;
				else
					break;
			}
			return (const_iterator(begin));
		}
		ft::pair<iterator, iterator>	equal_range(const key_type& k) {
			return (ft::make_pair<iterator, iterator>(lower_bound(k), upper_bound(k)));
		}
		ft::pair<const_iterator, const_iterator>	equal_range(const key_type& k)  const {
			return (ft::make_pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k)));
		}

		// ALLOCATOR

		allocator_type  get_allocator() const { return (_alloc); }

		// AN ADD ONLY FOR THE PROJECT DEFENSE

		void display() { displayTreeSet(_root); }

	private:

		typedef typename allocator_type::template rebind<node<value_type> >::other	_nodeAllocator_type;

		node<value_type>* _root;
		node<value_type>* _leaf;
		size_type						   _size;
		iterator							_it;
		allocator_type					  _alloc;
		_nodeAllocator_type					_allocNode;
		key_compare						 _compare;
		ft::pair< node<value_type>*, bool > _insertValue;

		struct maxSizeElt
		{
			maxSizeElt* ptr1;
			maxSizeElt* ptr2;
			maxSizeElt* ptr3;
			value_type* ptr4;
			value_type	object;
			bool		color;
		};

		std::allocator<maxSizeElt>	_maxSizeAlloc;

		void	_copyTree(node<value_type>** root, node<value_type>* other, node<value_type>* father) {
			if (isLeaf(other))
				return;
			*root = newNode(*(other->content), _allocNode, _alloc);
			(*root)->color = other->color; 
			(*root)->father = father;
			_copyTree(&((*root)->left), other->left, *root);
			_copyTree(&((*root)->right), other->right, *root);
		}
		void	_recursiveDoTheLeafs(node<value_type>* root, node<value_type>* leaf) {
			if (!root->right)
				root->right = leaf;
			else
				_recursiveDoTheLeafs(root->right, leaf);
			if (!root->left)
				root->left = leaf;
			else
				_recursiveDoTheLeafs(root->left, leaf);
		}
		void	_doTheLeafs(node<value_type>* root) {
			if (!root)
				return;
			_leaf->father = getMaxNodeNoLeaf(root);
			_leaf->left = getMinNodeNoLeaf(root);
			_recursiveDoTheLeafs(root, _leaf);
		}
		node<value_type>* _findNode(const key_type& k, node<value_type>* n) const
		{
			node<value_type>* search = n;
			if (!search)
				return NULL;
			while (isNotLeaf(search))
			{
				if (_compare(*(search->content), k))
					search = search->right;
				else if (_compare(k, *(search->content)))
					search = search->left;
				else
					return (search);
			}
			return (NULL);
		}
		bool	equalIterator(iterator& it, iterator& end)
		{
			it = end;
			return (true);
		}
	};

	// NON MEMBERS FUNCTIONS

	template <class T, class Comp, class Alloc>
	bool operator==(const ft::set<T, Comp, Alloc>& lhs, const ft::set<T, Comp, Alloc>& rhs) {
		if (lhs.size() != rhs.size())
			return false;
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class T, class Comp, class Alloc>
	bool operator!=(const ft::set<T, Comp, Alloc>& lhs, const ft::set<T, Comp, Alloc>& rhs) {
		return (!(lhs == rhs));
	}

	template <class T, class Comp, class Alloc>
	bool operator<(const ft::set<T, Comp, Alloc>& lhs, const ft::set<T, Comp, Alloc>& rhs) {
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class T, class Comp, class Alloc>
	bool operator>(const ft::set<T, Comp, Alloc>& lhs, const ft::set<T, Comp, Alloc>& rhs) {
		return (rhs < lhs);
	}

	template <class T, class Comp, class Alloc>
	bool operator>=(const ft::set<T, Comp, Alloc>& lhs, const ft::set<T, Comp, Alloc>& rhs) {
		return (!(lhs < rhs));
	}

	template <class T, class Comp, class Alloc>
	bool operator<=(const ft::set<T, Comp, Alloc>& lhs, const ft::set<T, Comp, Alloc>& rhs) {
		return (!(lhs > rhs));
	}

	template <class T, class Comp, class Alloc>
	void	swap(ft::set<T, Comp, Alloc>& lhs, ft::set<T, Comp, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}

	template<class Key, class Compare = ft::less<Key>, class Allocator = std::allocator<Key> >
	class multiset {

	public:

		// TYPEDEFS

		typedef Allocator							allocator_type;
		typedef Key									key_type;
		typedef Key									value_type;
		typedef typename Allocator::size_type		size_type;
		typedef typename Allocator::difference_type	difference_type;
		typedef Compare								key_compare;
		typedef Compare								value_compare;
		typedef value_type&							reference;
		typedef const value_type&					const_reference;
		typedef typename Allocator::pointer pointer;
		typedef typename Allocator::const_pointer const_pointer;

		// ITERATOR CLASS AND HIS TYPEDEFS

		class MyIterator : public ft::iterator<ft::bidirectional_iterator_tag, const value_type, difference_type, const_pointer, const_reference> {

			node<value_type>* _p;

		public:

			MyIterator() : _p(0) {}

			explicit MyIterator(node<value_type>* ptr) : _p(ptr) {}

			MyIterator(const MyIterator& other) : _p(other._p) {}

			~MyIterator() {}

			bool operator==(const MyIterator& other)	const { return (_p == other._p); }

			bool operator!=(const MyIterator& other)	const { return (_p != other._p); }

			MyIterator& operator=(MyIterator const& other) {
				this->_p = other._p;
				return (*this);
			}

			MyIterator& operator=(node<value_type>* ptr) {
				this->_p = ptr;
				return (*this);
			}

			const_reference operator*()	const {
				return (*(_p->content));
			}

			const_pointer operator->()	const {
				return (_p->content);
			}

			MyIterator& operator++() {
				if (isNotLeaf(_p))
					this->_p = getSuccessor(_p);
				else
					_p = _p->left;
				return (*this);
			}

			MyIterator operator++(int) {
				MyIterator temp(*this);
				if (isNotLeaf(_p))
					this->_p = getSuccessor(_p);
				else
					_p = _p->left;
				return (temp);
			}

			MyIterator& operator--() {
				if (isLeaf(_p))
					_p = _p->father;
				else
					_p = getPredecessor(_p);
				return (*this);
			}

			MyIterator operator--(int) {
				MyIterator temp(*this);
				if (isLeaf(_p))
					_p = _p->father;
				else
					_p = getPredecessor(_p);
				return (temp);
			}

			node<value_type>* base()	const { return (_p); }

		};

		typedef MyIterator									iterator;
		typedef MyIterator									const_iterator;
		typedef ft::reverse_iterator<iterator>				reverse_iterator;
		typedef ft::reverse_iterator<iterator>				const_reverse_iterator;

		// CONSTRUCTORS / DESTRUCTOR / OPERATOR=

		multiset(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(0), _size(0), _alloc(alloc), _compare(comp)
		{
			_leaf = doALeaf(value_type(), _allocNode, _alloc);
		}

		template < class InputIterator >
		multiset(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			: _root(0), _size(0), _alloc(alloc), _compare(comp)
		{
			_leaf = doALeaf(value_type(), _allocNode, _alloc);
			while (first != last) {
				insert(*first);
				first++;
			}
		}

		multiset(const multiset& other)
			: _root(0), _size(0), _alloc(other._alloc), _compare(other._compare)
		{
			_leaf = doALeaf(value_type(), _allocNode, _alloc);
			*this = other;
		}

		~multiset() {
			if (_root)
				recursiveBurnTheTree<value_type, std::allocator<node<value_type> >, allocator_type>(_root, _allocNode, _alloc);
			destroyNode(_leaf, _allocNode, _alloc);
		}

		multiset& operator=(const multiset& other) {
			if (&other == this)
				return (*this);
			_size = other._size;
			_compare = other._compare;
			if (_root) {
				recursiveBurnTheTree<value_type, std::allocator<node<value_type> >, allocator_type>(_root, _allocNode, _alloc);
				_root = 0;
			}
			if (other._root) {
				_copyTree(&_root, other._root, 0);
				_doTheLeafs(_root);
			}
			return (*this);
		}

		// ITERATOR FUNCTIONS

		iterator				begin()
		{
			if (!_root)
				return (end());
			return (iterator(getMinNode(_root)));
		}
		const_iterator		  begin() const
		{
			if (!_root)
				return (end());
			return (const_iterator(getMinNode(_root)));
		}
		iterator				end()
		{
			return (iterator(_leaf));
		}
		const_iterator		  end()   const
		{
			return (const_iterator(_leaf));
		}
		reverse_iterator		rbegin() {
			return (reverse_iterator(end()));
		}
		const_reverse_iterator  rbegin() const {
			return (const_reverse_iterator(end()));
		}
		reverse_iterator		rend() {
			return (reverse_iterator(begin()));
		}
		const_reverse_iterator  rend() const {
			return (const_reverse_iterator(begin()));
		}

		// CAPACITY

		bool		empty()	 const { return (!_root); }
		size_type   size()	  const { return (_size); }
		size_type   max_size()  const { return (_maxSizeAlloc.max_size()); }

		// MODIFIERS

		iterator   insert(const_reference data)
		{
			if (!_root) {
				_root = insertNodeMultiset<value_type, Compare>(&_root,
					newNode<value_type, std::allocator<node<value_type> >, allocator_type>(
						data, _allocNode, _alloc), _leaf,
					_compare);
				_size++;
				return iterator(_root);
			}
			_it = insertNodeMultiset<value_type, Compare>(&_root,
				newNode<value_type, std::allocator<node<value_type> >, allocator_type>(
					data, _allocNode, _alloc), _leaf,
				_compare);
			_root = getRoot(_root);
			_size++;
			return _it;
		}

		iterator					insert(iterator hint, const value_type& data) {
			if (!_root || hint == end())
				return insert(data);
			bool	highest = false;
			bool	smallest = false;
			_it = begin();
			if (_compare(data, *_it)) {
				smallest = true;
			}
			else if (equalIterator(_it, --end()) && _compare(*_it, data)) {
				highest = true;
			}
			if (_compare(data, *hint))
			{
				if (_compare(data, *(getPredecessor(hint.base())->content)))
					hint = iterator(_root);
				else if (hint.base()->father == getPredecessor(hint.base()))
					--hint;
			}
			else if (!_compare(data, *hint))
			{
				if (_compare(*(getSuccessor(hint.base())->content), data))
					hint = iterator(_root);
				else if (hint.base()->father == getSuccessor(hint.base()))
					++hint;
			}
			++_size;
			iterator	inserted(recursiveInsertMultiset(hint.base(), newNode<value_type, \
				std::allocator< node < value_type> >, allocator_type>(data, _allocNode, _alloc), \
				_compare, smallest, highest));
			fixTree(inserted.base(), &_root);
			return (inserted);
		}

		template< class InputIterator >
		void						insert(InputIterator first, InputIterator last) {
			while (first != last) {
				insert(*(first++));
			}
		}

		void						erase(iterator position) {
			if (position == end())
				return;
			deleteNode<value_type, std::allocator<node<value_type> >, allocator_type>(
				position.base(), &_root, _allocNode, _alloc);
			if (!--_size) {
				_root = 0;
				_leaf->left = 0;
				_leaf->right = 0;
			}
			else {
				_leaf->father = getMaxNode(_root);
				_leaf->left = getMinNode(_root);
			}
		}
		size_type				   erase(const key_type& k) {
			size_type	delCount;
			iterator	found;

			delCount = 0;
			found = find(k);
			while (found != end() && !_compare(k, *found) && !_compare(*found, k))
			{
				++delCount;
				erase(found++);
			}
			if (!delCount)
				return delCount;
			if (!_size) {
				_root = 0;
				_leaf->left = 0;
				_leaf->father = 0;
			}
			else
			{
				_leaf->father = getMaxNode(_root);
				_leaf->left = getMinNode(_root);
			}
			return delCount;
		}
		void						erase(iterator first, iterator last)
		{
			while (first != last)
			{
				erase(first++);
			}
		}
		void						swap(multiset& other) {
			node<value_type>*	tempRoot = _root;
			node<value_type>*	tempLeaf = _leaf;
			size_type			tempSize = _size;
			allocator_type		tempAlloc = _alloc;
			key_compare			tempCompare = _compare;

			_root = other._root;
			_size = other._size;
			_leaf = other._leaf;
			_alloc = other._alloc;
			_compare = other._compare;

			other._root = tempRoot;
			other._size = tempSize;
			other._leaf = tempLeaf;
			other._alloc = tempAlloc;
			other._compare = tempCompare;
		}
		void						clear() {
			if (_root) {
				recursiveBurnTheTree<value_type, std::allocator<node<value_type> >, allocator_type>(_root, _allocNode, _alloc);
				_root = 0;
				_size = 0;
				_leaf->father = 0;
				_leaf->left = 0;
			}
		}

		// OBSERVERS

		key_compare	 key_comp()	  const { return (_compare); }
		value_compare   value_comp()	const { return (_compare); }

		// OPERATIONS

		iterator			find(const key_type& k) {
			node<value_type>* search = _root;
			iterator current;
			iterator previous;
			if (!search)
                return end();
			while (isNotLeaf(search))
			{
				if (!_compare(*(search->content), k) && !_compare(k, *(search->content)))
					{
						current = iterator(search);
						previous = current;
						while (current != end() &&
							!_compare(*current, k) &&
							!_compare(k, *current))
							{
								previous = current;
								--current;
							}
						return previous;
					}
				if (_compare(*(search->content), k))
					search = search->right;
				else
					search = search->left;
			}
			return (iterator(search));
		}
		const_iterator	  find(const key_type& k) const {
			node<value_type>* search = _root;
			const_iterator current;
			const_iterator previous;
			if (!search)
                return end();
			while (isNotLeaf(search))
			{
				if (!_compare(*(search->content), k) && !_compare(k, *(search->content)))
					{
						current = const_iterator(search);
						previous = current;
						while (current != end() &&
							!_compare(*current, k) &&
							!_compare(k, *current))
							{
								previous = current;
								--current;
							}
						return previous;
					}
				if (_compare(*(search->content), k))
					search = search->right;
				else
					search = search->left;
			}
			return (const_iterator(search));
		}
		size_type		   count(const key_type& k)	const {
			node<value_type>* search = _root;

			if (!search)
                return 0;
			while (isNotLeaf(search))
			{
				if (!_compare(*(search->content), k) && !_compare(k, *(search->content)))
					return 1 + _countUp(k, ++iterator(search)) + _countDown(k, --iterator(search));
				if (_compare(*(search->content), k))
					search = search->right;
				else
					search = search->left;
			}
			return (0);
		}
		iterator			lower_bound(const key_type& k) {
			node<value_type>* begin = _root;

			if (!begin)
				return end();
			while (isNotLeaf(begin)) {
				if (_compare(*(begin->content), k))
					begin = begin->right;
				else if (isNotLeaf(begin->left) && !_compare(*(getPredecessor(begin)->content), k))
					begin = begin->left;
				else
					break;
			}
			return (iterator(begin));
		}
		const_iterator	  lower_bound(const key_type& k)  const {
			node<value_type>* begin = _root;

			if (!begin)
				return end();
			while (isNotLeaf(begin)) {
				if (_compare(*(begin->content), k))
					begin = begin->right;
				else if (isNotLeaf(begin->left) && !_compare(*(getPredecessor(begin)->content), k))
					begin = begin->left;
				else
					break;
			}
			return (const_iterator(begin));
		}
		iterator			upper_bound(const key_type& k) {
			node<value_type>* begin = _root;

			if (!begin)
				return end();
			while (isNotLeaf(begin)) {
				if (!_compare(k, *(begin->content)))
					begin = begin->right;
				else if (isNotLeaf(begin->left) && _compare(k, *(getPredecessor(begin)->content)))
					begin = begin->left;
				else
					break;
			}
			return (iterator(begin));
		}
		const_iterator	  upper_bound(const key_type& k)  const {
			node<value_type>* begin = _root;

			if (!begin)
				return end();
			while (isNotLeaf(begin)) {
				if (!_compare(k, *(begin->content)))
					begin = begin->right;
				else if (isNotLeaf(begin->left) && _compare(k, *(getPredecessor(begin)->content)))
					begin = begin->left;
				else
					break;
			}
			return (const_iterator(begin));
		}
		ft::pair<iterator, iterator>	equal_range(const key_type& k) {
			return (ft::make_pair<iterator, iterator>(lower_bound(k), upper_bound(k)));
		}
		ft::pair<const_iterator, const_iterator>	equal_range(const key_type& k)  const {
			return (ft::make_pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k)));
		}

		// ALLOCATOR

		allocator_type  get_allocator() const { return (_alloc); }

		// AN ADD ONLY FOR THE PROJECT DEFENSE

		void display() { displayTreeSet(_root); }

	private:

		typedef typename allocator_type::template rebind<node<value_type> >::other	_nodeAllocator_type;

		node<value_type>* _root;
		node<value_type>* _leaf;
		size_type						   _size;
		iterator							_it;
		allocator_type					  _alloc;
		_nodeAllocator_type					_allocNode;
		key_compare						 _compare;
		ft::pair< node<value_type>*, bool > _insertValue;

		struct maxSizeElt
		{
			maxSizeElt* ptr1;
			maxSizeElt* ptr2;
			maxSizeElt* ptr3;
			value_type* ptr4;
			value_type	object;
			bool		color;
		};

		std::allocator<maxSizeElt>	_maxSizeAlloc;

		void	_copyTree(node<value_type>** root, node<value_type>* other, node<value_type>* father) {
			if (isLeaf(other))
				return;
			*root = newNode(*(other->content), _allocNode, _alloc);
			(*root)->father = father;
			(*root)->color = other->color;
			_copyTree(&((*root)->left), other->left, *root);
			_copyTree(&((*root)->right), other->right, *root);
		}
		void	_recursiveDoTheLeafs(node<value_type>* root, node<value_type>* leaf) {
			if (!root->right)
				root->right = leaf;
			else
				_recursiveDoTheLeafs(root->right, leaf);
			if (!root->left)
				root->left = leaf;
			else
				_recursiveDoTheLeafs(root->left, leaf);
		}
		void	_doTheLeafs(node<value_type>* root) {
			if (!root)
				return;
			_leaf->father = getMaxNodeNoLeaf(root);
			_leaf->left = getMinNodeNoLeaf(root);
			_recursiveDoTheLeafs(root, _leaf);
		}
		node<value_type>* _findNode(const key_type& k, node<value_type>* n) const
		{
			node<value_type>* search = n;
			if (!search)
				return NULL;
			while (isNotLeaf(search))
			{
				if (_compare(*(search->content), k))
					search = search->right;
				else if (_compare(k, *(search->content)))
					search = search->left;
				else
					return (search);
			}
			return (NULL);
		}
		bool	equalIterator(iterator& it, iterator& end)
		{
			it = end;
			return (true);
		}
		size_type   _countUp(const key_type& k, iterator current)	const
		{
			if (current != end() && !_compare(*current, k) && !_compare(k, *current))
				return (1 + _countUp(k, ++current));
			return 0;
		}
		size_type   _countDown(const key_type& k, iterator current)	const
		{
			if (current != end() && !_compare(*current, k) && !_compare(k, *current))
				return (1 + _countDown(k, --current));
			return 0;
		}

	};

	// NON MEMBERS FUNCTIONS

	template <class T, class Comp, class Alloc>
	bool operator==(const multiset<T, Comp, Alloc>& lhs, const multiset<T, Comp, Alloc>& rhs) {
		if (lhs.size() != rhs.size())
			return false;
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class T, class Comp, class Alloc>
	bool operator!=(const multiset<T, Comp, Alloc>& lhs, const multiset<T, Comp, Alloc>& rhs) {
		return (!(lhs == rhs));
	}

	template <class T, class Comp, class Alloc>
	bool operator<(const multiset<T, Comp, Alloc>& lhs, const multiset<T, Comp, Alloc>& rhs) {
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class T, class Comp, class Alloc>
	bool operator>(const multiset<T, Comp, Alloc>& lhs, const multiset<T, Comp, Alloc>& rhs) {
		return (rhs < lhs);
	}

	template <class T, class Comp, class Alloc>
	bool operator>=(const multiset<T, Comp, Alloc>& lhs, const multiset<T, Comp, Alloc>& rhs) {
		return (!(lhs < rhs));
	}

	template <class T, class Comp, class Alloc>
	bool operator<=(const multiset<T, Comp, Alloc>& lhs, const multiset<T, Comp, Alloc>& rhs) {
		return (!(lhs > rhs));
	}

	template <class T, class Comp, class Alloc>
	void	swap(multiset<T, Comp, Alloc>& lhs, multiset<T, Comp, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}


#endif