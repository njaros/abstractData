/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaros <njaros@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 14:54:41 by njaros            #+#    #+#             */
/*   Updated: 2022/07/21 17:11:26 by njaros           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef		MAP_HPP
# define	MAP_HPP

# include <memory>
# include <cstddef>
# include <cstring>
# include "functional.hpp"
# include "iterator.hpp"
# include "utility.hpp"
# include "treeManager.hpp"
# include "reverse_iterator.hpp"
# include "algorithm.hpp"

namespace ft {

    template< class Key, class T, class Compare = ft::less<Key>, class Alloc = std::allocator<ft::pair< const Key, T> > >
    class map {

    public :

// TYPEDEFS

        typedef Key                                         key_type;
        typedef T                                           mapped_type;
        typedef ft::pair< const key_type, mapped_type>      value_type;
        typedef Compare                                     key_compare;
        typedef Alloc                                       allocator_type;
        typedef value_type                                  &reference;
        typedef const value_type                            &const_reference;
        typedef value_type                                  *pointer;
        typedef const value_type                            *const_pointer;
        typedef typename Alloc::difference_type             difference_type;
        typedef typename Alloc::size_type                   size_type;

// ITERATOR CLASS AND HIS TYPEDEFS
		class MyConstIterator;
        class MyIterator : public ft::iterator<ft::bidirectional_iterator_tag, value_type> {

        private :

            node<value_type> *_p;

        public :

            MyIterator() : _p(0) {}

            explicit MyIterator(node<value_type> *ptr) : _p(ptr) {}

            MyIterator(const MyIterator &other) : _p(other._p) {}

            ~MyIterator() {}

            bool operator==(const MyIterator &other)	const	{return (_p == other._p);}

			bool operator==(const MyConstIterator &other)	const	{return (_p == other.base());}

            bool operator!=(const MyIterator &other)	const	{return (_p != other._p);}

			bool operator!=(const MyConstIterator &other)	const	{return (_p != other.base());}

            MyIterator &operator=(MyIterator const &other) {
                if (&other == this)
                    return (*this);
                this->_p = other._p;
                return (*this);
            }

            MyIterator &operator=(node<value_type> *ptr) {
                this->_p = ptr;
                return (*this);
            }

			reference 	operator*()	const {
                return (*(_p->content));
            }

            pointer	operator->()	const {
                return (_p->content);
            }

            MyIterator &operator++() {
                if (isNotLeaf(_p))
                    this->_p = getSuccessor(_p);
                else
                    _p = _p->left;
                return (*this);
            }

            MyIterator operator++(int) {
                MyIterator  temp(*this);
                if (isNotLeaf(_p))
                    this->_p = getSuccessor(_p);
                else {
					_p = _p->left;
				}
                return (temp);
            }

            MyIterator &operator--() {
                if (isNotLeaf(_p)) {
					_p = getPredecessor(_p);
				}
                else
                    _p = father(_p);
                return (*this);
            }

            MyIterator operator--(int) {
                MyIterator  temp(*this);
				if (isNotLeaf(_p)) {
					_p = getPredecessor(_p);
				}
				else
					_p = father(_p);
                return (temp);
            }

			node<value_type> *base()	const {return (_p);}

        };

		class MyConstIterator : public ft::iterator<ft::bidirectional_iterator_tag, const value_type> {

		private :

			node<value_type> *_p;

		public :

			MyConstIterator() : _p(0) {}

			MyConstIterator(node<value_type> *ptr) : _p(ptr) {}

			MyConstIterator(const MyConstIterator &other) : _p(other._p) {}

			MyConstIterator(const MyIterator &other) : _p(other.base()) {}

			~MyConstIterator() {}

			bool operator==(const MyConstIterator &other)	const	{return (_p == other._p);}

			bool operator!=(const MyConstIterator &other)	const	{return (_p != other._p);}

			bool operator==(const MyIterator &other)	const	{return (_p == other.base());}

			bool operator!=(const MyIterator &other)	const	{return (_p != other.base());}

			MyConstIterator &operator=(MyConstIterator const &other) {
				if (other == *this)
					return (*this);
				this->_p = other._p;
				return (*this);
			}

			MyConstIterator &operator=(MyIterator const &other) {
				if (other == *this)
					return (*this);
				this->_p = other.base();
				return (*this);
			}

			MyConstIterator &operator=(node<value_type> *ptr) {
				this->_p = ptr;
				return (*this);
			}

			const_reference operator*()	const	{
				return (*(_p->content));
			}

			const_pointer operator->() const {
				return (_p->content);
			}

			MyConstIterator &operator++() {
				if (isNotLeaf(_p))
					this->_p = getSuccessor(_p);
				else
					_p = _p->left;
				return (*this);
			}

			MyConstIterator operator++(int) {
				MyConstIterator  temp(*this);
				if (isNotLeaf(_p))
					this->_p = getSuccessor(_p);
				else
					_p = _p->left;
				return (temp);
			}

			MyConstIterator &operator--() {
				if (isLeaf(_p))
					_p = _p->father;
				else
					_p = getPredecessor(_p);
				return (*this);
			}

			MyConstIterator operator--(int) {
				MyConstIterator  temp(*this);
				if (isLeaf(_p))
					_p = _p->father;
				else
					_p = getPredecessor(_p);
				return (temp);
			}

			node<value_type> *base()	const {return (_p);}
		};

        typedef MyIterator									iterator;
        typedef MyConstIterator								const_iterator;
        typedef ft::reverse_iterator<iterator>				reverse_iterator;
        typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;

// THE CLASS VALUE_COMPARE

        class value_compare {
            friend class map;

        protected :
            Compare comp;
            explicit value_compare (Compare c) : comp(c) {}
			value_compare (const value_compare &other) : comp(other.comp) {}
        public :
            typedef bool        result_type;
            typedef value_type  first_argument_type;
            typedef value_type  second_argument_type;
            bool    operator()(const value_type &x, const value_type &y)    const {
                return comp(x.first, y.first);
            }
        };

// CONSTRUCTORS / DESTRUCTOR / OPERATOR=

        map(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
            : _root(0), _size(0), _alloc(alloc), _compare(comp)
        {
			_leaf = doALeaf(value_type(), _allocNode, _alloc);
		}

        template<class InputIterator>
        map(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
            : _root(0), _size(0), _alloc(alloc), _compare(comp)
        {
			_leaf = doALeaf(value_type(),_allocNode, _alloc);
            while (first != last)
            {
                insert(*first);
                first++;
            }
        }

        map(const map &other)
            : _root(0), _size(0), _alloc(other._alloc), _compare(other._compare)
        {
			_leaf = doALeaf(value_type(),_allocNode, _alloc);
            *this = other;
        }

        ~map()  {
            if (_root)
				recursiveBurnTheTree<value_type, std::allocator<node<value_type> >, allocator_type>(_root, _allocNode, _alloc);
			destroyNode(_leaf, _allocNode, _alloc);
		}

        map &operator=(const map &other) {
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

        iterator                begin()
        {
            if (!_root)
                return (end());
            return (iterator(getMinNode(_root)));
        }
        const_iterator          begin() const
        {
            if (!_root)
                return (end());
            return (const_iterator(getMinNode(_root)));
        }
        iterator                end()
        {
            return (iterator(_leaf));
        }
        const_iterator          end()   const
        {
			return (const_iterator(_leaf));
        }
        reverse_iterator        rbegin() {
            return (reverse_iterator(end()));
        }
        const_reverse_iterator  rbegin() const {
            return (const_reverse_iterator(end()));
        }
        reverse_iterator        rend() {
            return (reverse_iterator(begin()));
        }
        const_reverse_iterator  rend() const {
            return (const_reverse_iterator(begin()));
        }

// CAPACITY

        bool        empty()     const {return (!_root);}
        size_type   size()      const {return (_size);}
        size_type   max_size()  const {return (_maxSizeAlloc.max_size());}

// ELEMENT ACCESS

        mapped_type &at(const key_type &k) {
            _it = find(k);
            if (_it == end())
                throw (ft::out_of_range("this element isn't on the map\n"));
            return (*_it).second;
        }

        const mapped_type &at(const key_type &k) const {
            _it = find(k);
            if (_it == end())
                throw (ft::out_of_range("this element isn't on the map\n"));
            return (*_it).second;
        }

        mapped_type &operator[](const key_type &k) {
            return ((*((this->insert(ft::make_pair(k,mapped_type()))).first)).second);
        }

// MODIFIERS

        ft::pair<iterator, bool>    insert(const_reference data)
        {
            if (!_root) {
                _root = insertNode<value_type, Compare>(&_root,
                                                        newNode<value_type, std::allocator<node<value_type> >, allocator_type>(
                                                                data, _allocNode, _alloc), _leaf,
                                                                _compare).first;
                _size++;
                _it = _root;
                return (ft::make_pair<iterator, bool>(_it, true));
            }
            else {
                _it = find(data.first);
                if (_it == end()) {
                    _insertValue = insertNode<value_type, Compare>(&_root,
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
        ft::pair<iterator, bool>    insert(const ft::pair<key_type, mapped_type> &data)
        {
            const key_type		&k = data.first;
            const mapped_type	&m = data.second;
            return insert(ft::make_pair<const key_type, mapped_type>(k, m));
        }
        iterator                    insert(iterator hint, const value_type &data) {
			if (!_root)
				return iterator(insert(data).first);
            bool    highest = false;
            bool    smallest = false;
            _it = begin();
            if (_compare(data.first, _it->first)) {
                smallest = true;
                hint = _it;
            }
            else if (equalIterator(_it, --end()) && _compare(_it->first, data.first)) {
                highest = true;
                hint = _it;
            }
            else if (_compare(data.first, hint->first)) {
                --hint;
                while (_compare(data.first, hint->first))
                    --hint;
            }
            else if (_compare(hint->first, data.first)) {
                ++hint;
                while (_compare(hint->first, data.first))
                    ++hint;
            }
            if (!_compare(hint->first, data.first) && !_compare(data.first, hint->first))
                return (hint);
            ++_size;
            iterator    inserted(recursiveInsert(hint.base(), newNode<value_type, \
                            std::allocator< node < value_type> >, allocator_type>(data, _allocNode, _alloc), \
                            _compare, smallest, highest).first);
            fixTree(inserted.base(), &_root);
            return (inserted);
        }
        iterator                    insert(iterator hint, const ft::pair<key_type, mapped_type> &data)
        {
            const key_type      &k = data.first;
            const mapped_type   &m = data.second;
            return insert(hint, ft::make_pair<const key_type, mapped_type>(k, m));
        }
        template< class InputIterator >
        void                        insert(InputIterator first, InputIterator last) {
            while (first != last) {
                insert(*first);
                first++;
            }
        }

        void                        erase(iterator position) {
			node<value_type>	*nodeToDel = _findNode(position->first, _root);
			if (nodeToDel) {
				deleteNode<value_type, std::allocator<node<value_type> >, allocator_type>(nodeToDel, &_root, _allocNode,
																						  _alloc);
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
        size_type                   erase(const key_type &k) {
            node<value_type> *n = _findNode(k, _root);

            if (!n)
                return (0);
			else
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
        void                        erase(iterator first, iterator last)
        {
			while (first != last)
            {
                erase(first++);
			}
        }
        void                        swap(map &other) {
            node<value_type>* tempRoot = _root;
            node<value_type>* tempLeaf = _leaf;
            size_type			tempSize = _size;
            allocator_type		tempAlloc = _alloc;
            key_type			tempCompare = _compare;

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
        void                        clear() {
            if (_root) {
				recursiveBurnTheTree<value_type, std::allocator<node<value_type> >, allocator_type>(_root, _allocNode, _alloc);
                _root = 0;
                _size = 0;
				_leaf->father = 0;
				_leaf->left = 0;
            }
        }

// OBSERVERS

        key_compare     key_comp()      const   {return (_compare);}
        value_compare   value_comp()    const   {
            value_compare foo(_compare);
            return (foo);}

// OPERATIONS

        iterator            find(const key_type &k) {
            node<value_type>    *search = _root;
            while (isNotLeaf(search))
            {
                if (_compare(search->content->first, k))
                    search = search->right;
                else if (_compare(k, search->content->first))
                    search = search->left;
				else
					return (iterator(search));
            }
            return (iterator(search));
        }
        const_iterator      find(const key_type &k) const {
			node<value_type>    *search = _root;
            while (isNotLeaf(search))
            {
				if (_compare(search->content->first, k))
					search = search->right;
				else if (_compare(k, search->content->first))
					search = search->left;
				else
					return (const_iterator(search));
            }
            return (const_iterator(search));
        }
        size_type           count(const key_type &k)	const {
            node<value_type>    *search = _root;
            while (isNotLeaf(search))
            {
                if (!_compare(search->content->first, k) && !_compare(k, search->content->first))
                    return (1);
                if (_compare(search->content->first, k))
                    search = search->right;
                else
                    search = search->left;
            }
            return (0);
        }
        iterator            lower_bound(const key_type &k) {
			node<value_type>	*begin = _root;
			if (!begin)
				return (iterator(begin));
			while (isNotLeaf(begin)) {
				if (_compare(begin->content->first, k))
					begin = begin->right;
				else if (isNotLeaf(begin->left) && !_compare(getPredecessor(begin)->content->first, k))
					begin = begin->left;
				else
					break ;
			}
			return (iterator(begin));
        }
        const_iterator      lower_bound(const key_type &k)  const {
			node<value_type>	*begin = _root;
			if (!begin)
				return (const_iterator(begin));
            while (isNotLeaf(begin)) {
				if (_compare(begin->content->first, k))
					begin = begin->right;
				else if (isNotLeaf(begin->left) && !_compare(getPredecessor(begin)->content->first, k))
					begin = begin->left;
				else
					break ;
			}
            return (const_iterator(begin));
        }
        iterator            upper_bound(const key_type &k) {
			node<value_type>	*begin = _root;
			if (!begin)
				return (iterator(begin));
			while (isNotLeaf(begin)) {
				if (!_compare(k, begin->content->first))
					begin = begin->right;
				else if (isNotLeaf(begin->left) && _compare(k, getPredecessor(begin)->content->first))
					begin = begin->left;
				else
					break ;
			}
			return (iterator(begin));
        }
        const_iterator      upper_bound(const key_type &k)  const {
			node<value_type>	*begin = _root;
			if (!begin)
				return (const_iterator(begin));
			while (isNotLeaf(begin)) {
				if (!_compare(k, begin->content->first))
					begin = begin->right;
				else if (isNotLeaf(begin->left) && _compare(k, getPredecessor(begin)->content->first))
					begin = begin->left;
				else
					break ;
			}
			return (const_iterator(begin));
        }
        ft::pair<iterator, iterator>    equal_range(const key_type &k) {
            return (ft::make_pair<iterator, iterator>(lower_bound(k), upper_bound(k)));
        }
		ft::pair<const_iterator, const_iterator>    equal_range(const key_type &k)  const {
            return (ft::make_pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k)));
        }

// ALLOCATOR

        allocator_type  get_allocator() const {return (_alloc);}

// AN ADD ONLY FOR THE PROJECT DEFENSE

        void    display() {displayTree(_root);}

// MY VARIABLES I USED TO BUILD MY MAP

    private :

		typedef typename allocator_type::template rebind<node<value_type> >::other	_nodeAllocator_type;

        node<value_type>                    *_root;
		node<value_type>					*_leaf;
        iterator                            _it;
        ft::pair<node<value_type> *, bool>  _insertValue;
        size_type                           _size;
        allocator_type                      _alloc;
		_nodeAllocator_type 				_allocNode;
        key_compare                         _compare;

		struct maxSizeElt
		{
			maxSizeElt	*ptr1;
			maxSizeElt	*ptr2;
			maxSizeElt	*ptr3;
			value_type	*ptr4;
			value_type	object;
			bool		color;
		};

		std::allocator<maxSizeElt>	_maxSizeAlloc;

        void    _copyTree(node<value_type> **root, node<value_type> *other, node<value_type> *father)    {
            if (isLeaf(other))
                return ;
            *root = newNode(*(other->content), _allocNode, _alloc);
            (*root)->father = father;
            _copyTree(&((*root)->left), other->left, *root);
            _copyTree(&((*root)->right), other->right, *root);
        }
        void    _recursiveDoTheLeafs(node<value_type> *root, node<value_type> *leaf) {
            if (!root->right)
                root->right = leaf;
            else
                _recursiveDoTheLeafs(root->right, leaf);
            if (!root->left)
                root->left = leaf;
            else
                _recursiveDoTheLeafs(root->left, leaf);
        }
        void    _doTheLeafs(node<value_type> *root) {
            if (!root)
                return ;
            _leaf->father = getMaxNodeNoLeaf(root);
            _leaf->left = getMinNodeNoLeaf(root);
            _recursiveDoTheLeafs(root, _leaf);
        }
        node<value_type>    *_findNode(const key_type &k, node<value_type> *n) const
        {
			node<value_type>    *search = n;
			while (isNotLeaf(search))
			{
				if (_compare(search->content->first, k))
					search = search->right;
				else if (_compare(k, search->content->first))
					search = search->left;
				else
					return (search);
			}
			return (NULL);
        }
        bool    equalIterator(iterator &it, iterator &end)
        {
            it = end;
            return (true);
        }
    };

    // NON MEMBERS FUNCTIONS

    template <class Key, class T, class Comp, class Alloc>
    bool operator==(const ft::map<Key, T, Comp, Alloc>& lhs, const ft::map<Key, T, Comp, Alloc>& rhs) {
        if (lhs.size() != rhs.size())
            return false;
        return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
    }

    template <class Key, class T, class Comp, class Alloc>
    bool operator!=(const ft::map<Key, T, Comp, Alloc>& lhs, const ft::map<Key, T, Comp, Alloc>& rhs) {
        return (!(lhs == rhs));
    }

    template <class Key, class T, class Comp, class Alloc>
    bool operator<(const ft::map<Key, T, Comp, Alloc>& lhs, const ft::map<Key, T, Comp, Alloc>& rhs) {
        return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
    }

    template <class Key, class T, class Comp, class Alloc>
    bool operator>(const ft::map<Key, T, Comp, Alloc>& lhs, const ft::map<Key, T, Comp, Alloc>& rhs) {
        return (rhs < lhs);
    }

    template <class Key, class T, class Comp, class Alloc>
    bool operator>=(const ft::map<Key, T, Comp, Alloc>& lhs, const ft::map<Key, T, Comp, Alloc>& rhs) {
        return (!(lhs < rhs));
    }

    template <class Key, class T, class Comp, class Alloc>
    bool operator<=(const ft::map<Key, T, Comp, Alloc>& lhs, const ft::map<Key, T, Comp, Alloc>& rhs) {
        return (!(lhs > rhs));
    }

    template <class Key, class T, class Comp, class Alloc>
    void	swap(ft::map<Key, T, Comp, Alloc>& lhs, ft::map<Key, T, Comp, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }

    template< class Key, class T, class Compare = ft::less<Key>, class Alloc = std::allocator<ft::pair< const Key, T> > >
    class multimap
    {

    public:

        // TYPEDEFS

        typedef Key                                         key_type;
        typedef T                                           mapped_type;
        typedef ft::pair< const key_type, mapped_type>      value_type;
        typedef Compare                                     key_compare;
        typedef Alloc                                       allocator_type;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef typename Alloc::difference_type             difference_type;
        typedef typename Alloc::size_type                   size_type;

        // ITERATOR CLASS AND HIS TYPEDEFS
        class MyConstIterator;
        class MyIterator : public ft::iterator<ft::bidirectional_iterator_tag, value_type> {

        private:

            node<value_type>* _p;

        public:

            MyIterator() : _p(0) {}

            explicit MyIterator(node<value_type>* ptr) : _p(ptr) {}

            MyIterator(const MyIterator& other) : _p(other._p) {}

            ~MyIterator() {}

            bool operator==(const MyIterator& other)	const { return (_p == other._p); }

            bool operator==(const MyConstIterator& other)	const { return (_p == other.base()); }

            bool operator!=(const MyIterator& other)	const { return (_p != other._p); }

            bool operator!=(const MyConstIterator& other)	const { return (_p != other.base()); }

            MyIterator& operator=(MyIterator const& other) {
                if (&other == this)
                    return (*this);
                this->_p = other._p;
                return (*this);
            }

            MyIterator& operator=(node<value_type>* ptr) {
                this->_p = ptr;
                return (*this);
            }

            reference 	operator*()	const {
                return (*(_p->content));
            }

            pointer	operator->()	const {
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
                MyIterator  temp(*this);
                if (isNotLeaf(_p))
                    this->_p = getSuccessor(_p);
                else {
                    _p = _p->left;
                }
                return (temp);
            }

            MyIterator& operator--() {
                if (isNotLeaf(_p)) {
                    _p = getPredecessor(_p);
                }
                else
                    _p = father(_p);
                return (*this);
            }

            MyIterator operator--(int) {
                MyIterator  temp(*this);
                if (isNotLeaf(_p)) {
                    _p = getPredecessor(_p);
                }
                else
                    _p = father(_p);
                return (temp);
            }

            node<value_type>* base()	const { return (_p); }

        };

        class MyConstIterator : public ft::iterator<ft::bidirectional_iterator_tag, const value_type> {

        private:

            node<value_type>* _p;

        public:

            MyConstIterator() : _p(0) {}

            MyConstIterator(node<value_type>* ptr) : _p(ptr) {}

            MyConstIterator(const MyConstIterator& other) : _p(other._p) {}

            MyConstIterator(const MyIterator& other) : _p(other.base()) {}

            ~MyConstIterator() {}

            bool operator==(const MyConstIterator& other)	const { return (_p == other._p); }

            bool operator!=(const MyConstIterator& other)	const { return (_p != other._p); }

            bool operator==(const MyIterator& other)	const { return (_p == other.base()); }

            bool operator!=(const MyIterator& other)	const { return (_p != other.base()); }

            MyConstIterator& operator=(MyConstIterator const& other) {
                if (other == *this)
                    return (*this);
                this->_p = other._p;
                return (*this);
            }

            MyConstIterator& operator=(MyIterator const& other) {
                if (other == *this)
                    return (*this);
                this->_p = other.base();
                return (*this);
            }

            MyConstIterator& operator=(node<value_type>* ptr) {
                this->_p = ptr;
                return (*this);
            }

            const_reference operator*()	const {
                return (*(_p->content));
            }

            const_pointer operator->() const {
                return (_p->content);
            }

            MyConstIterator& operator++() {
                if (isNotLeaf(_p))
                    this->_p = getSuccessor(_p);
                else
                    _p = _p->left;
                return (*this);
            }

            MyConstIterator operator++(int) {
                MyConstIterator  temp(*this);
                if (isNotLeaf(_p))
                    this->_p = getSuccessor(_p);
                else
                    _p = _p->left;
                return (temp);
            }

            MyConstIterator& operator--() {
                if (isLeaf(_p))
                    _p = _p->father;
                else
                    _p = getPredecessor(_p);
                return (*this);
            }

            MyConstIterator operator--(int) {
                MyConstIterator  temp(*this);
                if (isLeaf(_p))
                    _p = _p->father;
                else
                    _p = getPredecessor(_p);
                return (temp);
            }

            node<value_type>* base()	const { return (_p); }
        };

        typedef MyIterator									iterator;
        typedef MyConstIterator								const_iterator;
        typedef ft::reverse_iterator<iterator>				reverse_iterator;
        typedef ft::reverse_iterator<const_iterator>		const_reverse_iterator;

        // THE CLASS VALUE_COMPARE

        class value_compare {
            friend class multimap;

        protected:
            Compare comp;
            explicit value_compare(Compare c) : comp(c) {}
            value_compare(const value_compare& other) : comp(other.comp) {}
        public:
            typedef bool        result_type;
            typedef value_type  first_argument_type;
            typedef value_type  second_argument_type;
            bool    operator()(const value_type& x, const value_type& y)    const {
                return comp(x.first, y.first);
            }
        };

        // CONSTRUCTORS / DESTRUCTOR / OPERATOR=

        multimap(const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
            : _root(0), _size(0), _alloc(alloc), _compare(comp)
        {
            _leaf = doALeaf(value_type(), _allocNode, _alloc);
        }

        template<class InputIterator>
        multimap(InputIterator first, InputIterator last, const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
            : _root(0), _size(0), _alloc(alloc), _compare(comp)
        {
            _leaf = doALeaf(value_type(), _allocNode, _alloc);
            while (first != last)
            {
                insert(*first);
                first++;
            }
        }

        multimap(const multimap& other)
            : _root(0), _size(0), _alloc(other._alloc), _compare(other._compare) {
            _leaf = doALeaf(value_type(), _allocNode, _alloc);
            *this = other;
        }

        ~multimap() {
            if (_root)
                recursiveBurnTheTree<value_type, std::allocator<node<value_type> >, allocator_type>(_root, _allocNode, _alloc);
            destroyNode(_leaf, _allocNode, _alloc);
        }

        multimap& operator=(const multimap& other) {
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

        iterator                begin()
        {
            if (!_root)
                return (end());
            return (iterator(getMinNode(_root)));
        }
        const_iterator          begin() const
        {
            if (!_root)
                return (end());
            return (const_iterator(getMinNode(_root)));
        }
        iterator                end()
        {
            return (iterator(_leaf));
        }
        const_iterator          end()   const
        {
            return (const_iterator(_leaf));
        }
        reverse_iterator        rbegin() {
            return (reverse_iterator(end()));
        }
        const_reverse_iterator  rbegin() const {
            return (const_reverse_iterator(end()));
        }
        reverse_iterator        rend() {
            return (reverse_iterator(begin()));
        }
        const_reverse_iterator  rend() const {
            return (const_reverse_iterator(begin()));
        }

        // CAPACITY

        bool        empty()     const { return (!_root); }
        size_type   size()      const { return (_size); }
        size_type   max_size()  const { return (_maxSizeAlloc.max_size()); }

        // MODIFIERS

        iterator    insert(const_reference data)
        {
            if (!_root) {
                _root = insertNodeMultimap<value_type, Compare>(&_root,
                    newNode<value_type, std::allocator<node<value_type> >, allocator_type>(
                        data, _allocNode, _alloc), _leaf,
                    _compare);
                _size++;
                return iterator(_root);
            }    
            _size++;
            _it = insertNodeMultimap<value_type, Compare>(&_root,
                newNode<value_type, std::allocator<node<value_type> >, allocator_type>(data, _allocNode, _alloc),
                _leaf,_compare);
            _root = getRoot(_root);
            return _it;
        }
        iterator   insert(const ft::pair<key_type, mapped_type>& data)
        {
            const key_type& k = data.first;
            const mapped_type& m = data.second;
            return insert(ft::make_pair<const key_type, mapped_type>(k, m));
        }
        iterator                    insert(iterator hint, const value_type& data) {
            if (!_root)
                return iterator(insert(data));
            bool    highest = false;
            bool    smallest = false;
            _it = begin();
            if (_compare(data.first, _it->first)) {
                smallest = true;
                hint = _it;
            }
            else if (equalIterator(_it, --end()) && _compare(_it->first, data.first)) {
                highest = true;
                hint = _it;
            }
            else if (_compare(data.first, hint->first)) {
                --hint;
                while (_compare(data.first, hint->first))
                    --hint;
            }
            else if (!_compare(data.first, hint->first)) {
                ++hint;
                while (!_compare(data.first, hint->first))
                    ++hint;
            }
            ++_size;
            iterator    inserted(recursiveInsertMultimap(hint.base(),
                newNode<value_type, std::allocator< node < value_type> >, allocator_type>(data, _allocNode, _alloc),
                _compare, smallest, highest));
            fixTree(inserted.base(), &_root);
            return (inserted);
        }
        iterator                    insert(iterator hint, const ft::pair<key_type, mapped_type>& data)
        {
            const key_type& k = data.first;
            const mapped_type& m = data.second;
            return insert(hint, ft::make_pair<const key_type, mapped_type>(k, m));
        }
        template< class InputIterator >
        void                        insert(InputIterator first, InputIterator last) {
            while (first != last) {
                insert(*first);
                first++;
            }
        }

        void                        erase(iterator position) {
            node<value_type>* nodeToDel = _findNode(position->first, _root);
            if (nodeToDel) {
                deleteNode<value_type, std::allocator<node<value_type> >, allocator_type>(nodeToDel, &_root, _allocNode,
                    _alloc);
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
        size_type                   erase(const key_type& k) {
            node<value_type>* n = _findNode(k, _root);
            size_type delCount;

            delCount = 0;
            while (_erase(k))
            {
                ++delCount;
                --_size;
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
        void                        erase(iterator first, iterator last)
        {
            while (first != last)
            {
                erase(first++);
            }
        }
        void                        swap(multimap& other) {
            node<value_type>* tempRoot = _root;
            node<value_type>* tempLeaf = _leaf;
            size_type			tempSize = _size;
            allocator_type		tempAlloc = _alloc;
            key_type			tempCompare = _compare;

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
        void                        clear() {
            if (_root) {
                recursiveBurnTheTree<value_type, std::allocator<node<value_type> >, allocator_type>(_root, _allocNode, _alloc);
                _root = 0;
                _size = 0;
                _leaf->father = 0;
                _leaf->left = 0;
            }
        }

        // OBSERVERS

        key_compare     key_comp()      const { return (_compare); }
        value_compare   value_comp()    const {
            value_compare foo(_compare);
            return (foo);
        }

        // OPERATIONS

        iterator            find(const key_type& k) {
            node<value_type>* search = _root;
            while (isNotLeaf(search))
            {
                if (_compare(search->content->first, k))
                    search = search->right;
                else if (_compare(k, search->content->first))
                    search = search->left;
                else
                    return (iterator(search));
            }
            return (iterator(search));
        }
        const_iterator      find(const key_type& k) const {
            node<value_type>* search = _root;
            while (isNotLeaf(search))
            {
                if (_compare(search->content->first, k))
                    search = search->right;
                else if (_compare(k, search->content->first))
                    search = search->left;
                else
                    return (const_iterator(search));
            }
            return (const_iterator(search));
        }
        size_type           count(const key_type& k)	const {
            node<value_type>* search = _root;
            while (isNotLeaf(search))
            {
                if (!_compare(search->content->first, k) && !_compare(k, search->content->first))
                    return 1 + _count(k, search->right) + _count(k, search->left);
                if (_compare(search->content->first, k))
                    search = search->right;
                else
                    search = search->left;
            }
            return (0);
        }
        iterator            lower_bound(const key_type& k) {
            node<value_type>* begin = _root;
            if (!begin)
                return (iterator(begin));
            while (isNotLeaf(begin)) {
                if (_compare(begin->content->first, k))
                    begin = begin->right;
                else if (isNotLeaf(begin->left) && !_compare(getPredecessor(begin)->content->first, k))
                    begin = begin->left;
                else
                    break;
            }
            return (iterator(begin));
        }
        const_iterator      lower_bound(const key_type& k)  const {
            node<value_type>* begin = _root;
            if (!begin)
                return (const_iterator(begin));
            while (isNotLeaf(begin)) {
                if (_compare(begin->content->first, k))
                    begin = begin->right;
                else if (isNotLeaf(begin->left) && !_compare(getPredecessor(begin)->content->first, k))
                    begin = begin->left;
                else
                    break;
            }
            return (const_iterator(begin));
        }
        iterator            upper_bound(const key_type& k) {
            node<value_type>* begin = _root;
            if (!begin)
                return (iterator(begin));
            while (isNotLeaf(begin)) {
                if (!_compare(k, begin->content->first))
                    begin = begin->right;
                else if (isNotLeaf(begin->left) && _compare(k, getPredecessor(begin)->content->first))
                    begin = begin->left;
                else
                    break;
            }
            return (iterator(begin));
        }
        const_iterator      upper_bound(const key_type& k)  const {
            node<value_type>* begin = _root;
            if (!begin)
                return (const_iterator(begin));
            while (isNotLeaf(begin)) {
                if (!_compare(k, begin->content->first))
                    begin = begin->right;
                else if (isNotLeaf(begin->left) && _compare(k, getPredecessor(begin)->content->first))
                    begin = begin->left;
                else
                    break;
            }
            return (const_iterator(begin));
        }
        ft::pair<iterator, iterator>    equal_range(const key_type& k) {
            return (ft::make_pair<iterator, iterator>(lower_bound(k), upper_bound(k)));
        }
        ft::pair<const_iterator, const_iterator>    equal_range(const key_type& k)  const {
            return (ft::make_pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k)));
        }

        // ALLOCATOR

        allocator_type  get_allocator() const { return (_alloc); }

        // AN ADD ONLY FOR THE PROJECT DEFENSE

        void    display() { displayTree(_root); }

        // MY VARIABLES I USED TO BUILD MY MAP

    private:

        typedef typename allocator_type::template rebind<node<value_type> >::other	_nodeAllocator_type;

        node<value_type>* _root;
        node<value_type>* _leaf;
        iterator                            _it;
        ft::pair<node<value_type>*, bool>  _insertValue;
        size_type                           _size;
        allocator_type                      _alloc;
        _nodeAllocator_type 				_allocNode;
        key_compare                         _compare;

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

        void    _copyTree(node<value_type>** root, node<value_type>* other, node<value_type>* father) {
            if (isLeaf(other))
                return;
            *root = newNode(*(other->content), _allocNode, _alloc);
            (*root)->father = father;
            _copyTree(&((*root)->left), other->left, *root);
            _copyTree(&((*root)->right), other->right, *root);
        }
        void    _recursiveDoTheLeafs(node<value_type>* root, node<value_type>* leaf) {
            if (!root->right)
                root->right = leaf;
            else
                _recursiveDoTheLeafs(root->right, leaf);
            if (!root->left)
                root->left = leaf;
            else
                _recursiveDoTheLeafs(root->left, leaf);
        }
        void    _doTheLeafs(node<value_type>* root) {
            if (!root)
                return;
            _leaf->father = getMaxNodeNoLeaf(root);
            _leaf->left = getMinNodeNoLeaf(root);
            _recursiveDoTheLeafs(root, _leaf);
        }
        node<value_type>* _findNode(const key_type& k, node<value_type>* n) const
        {
            node<value_type>* search = n;
            while (isNotLeaf(search))
            {
                if (_compare(search->content->first, k))
                    search = search->right;
                else if (_compare(k, search->content->first))
                    search = search->left;
                else
                    return (search);
            }
            return (NULL);
        }
        bool    equalIterator(iterator& it, iterator& end)
        {
            it = end;
            return (true);
        }
        bool    _erase(const key_type& k)
        {
            node<value_type>* found = _findNode(k, _root);

            if (!found)
                return 0;
            else
                deleteNode<value_type, std::allocator<node<value_type> >, allocator_type>(found, &_root, _allocNode, _alloc);
            return 1;
        }
        size_type   _count(const key_type& k, node<value_type>* current)	const
        {
            if (isNotLeaf(current) && !_compare(current->content->first, k) && !_compare(k, current->content->first))
                return 1 + _count(k, current->right) + _count(k, current->left);
            return 0;
        }

    };

    // NON MEMBERS FUNCTIONS

    template <class Key, class T, class Comp, class Alloc>
    bool operator==(const multimap<Key, T, Comp, Alloc>& lhs, const multimap<Key, T, Comp, Alloc>& rhs) {
        if (lhs.size() != rhs.size())
            return false;
        return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
    }

    template <class Key, class T, class Comp, class Alloc>
    bool operator!=(const multimap<Key, T, Comp, Alloc>& lhs, const multimap<Key, T, Comp, Alloc>& rhs) {
        return (!(lhs == rhs));
    }

    template <class Key, class T, class Comp, class Alloc>
    bool operator<(const multimap<Key, T, Comp, Alloc>& lhs, const multimap<Key, T, Comp, Alloc>& rhs) {
        return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
    }

    template <class Key, class T, class Comp, class Alloc>
    bool operator>(const multimap<Key, T, Comp, Alloc>& lhs, const multimap<Key, T, Comp, Alloc>& rhs) {
        return (rhs < lhs);
    }

    template <class Key, class T, class Comp, class Alloc>
    bool operator>=(const multimap<Key, T, Comp, Alloc>& lhs, const multimap<Key, T, Comp, Alloc>& rhs) {
        return (!(lhs < rhs));
    }

    template <class Key, class T, class Comp, class Alloc>
    bool operator<=(const multimap<Key, T, Comp, Alloc>& lhs, const multimap<Key, T, Comp, Alloc>& rhs) {
        return (!(lhs > rhs));
    }

    template <class Key, class T, class Comp, class Alloc>
    void	swap(multimap<Key, T, Comp, Alloc>& lhs, multimap<Key, T, Comp, Alloc>& rhs)
    {
        lhs.swap(rhs);
    }
}



#endif