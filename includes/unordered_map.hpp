#ifndef UNORDERED_MAP_HPP_ABSTRACT_DATA_BY_LE_N
#define UNORDERED_MAP_HPP_ABSTRACT_DATA_BY_LE_N

# include <memory>
# include <cstddef>
# include <cstring>
# include <cmath>
# include "list.hpp"
# include "vector.hpp"
# include "utility.hpp"
# include "reverse_iterator.hpp"
# include "algorithm.hpp"
# include "functional.hpp"

namespace ft
{
	template < class Key, class T, class Hash = ft::hash<Key>, class Pred = ft::equal_to<Key>, class Alloc = std::allocator< ft::pair< const Key, T > > >
	class unordered_map
	{

		friend class _hTIt;
		friend class _hTCIt;

	public:

		typedef Key						key_type;
		typedef T						mapped_type;
		typedef ft::pair<const Key, T>	value_type;
		typedef Alloc					allocator_type;
		typedef typename Alloc::reference		reference;
		typedef typename Alloc::const_reference const_reference;
		typedef typename Alloc::pointer			pointer;
		typedef typename Alloc::const_pointer	const_pointer;
		typedef typename Alloc::size_type		size_type;
		typedef typename Alloc::difference_type	difference_type;
		typedef Pred		key_equal;
		typedef Hash		hasher;

	private :

		typedef ft::list< value_type, Alloc >		_bucket;
		typedef ft::vector< _bucket >				_table;
		typedef typename _table::iterator			_tableIterator;
		typedef typename _table::const_iterator		_tableConstIterator;

	public :

		typedef typename _bucket::iterator			local_iterator;
		typedef typename _bucket::const_iterator	const_local_iterator;

	private:

		allocator_type	_alloc;
		_table			_t;
		hasher			_hasher;
		key_equal		_equal;
		size_type		_size;
		float			_maxLoadFactor;
		size_type		_maxLoad;

	public:

		class _hTCIt;
		class _hTIt : public ft::iterator< ft::forward_iterator_tag, value_type, difference_type, pointer, reference >
		{

			friend class _hTCIt;
			friend class unordered_map;

			_table* _ctx;
			size_type	_tablePos;
			local_iterator	_bucketPos;

			_hTIt(_table* ctx, const size_type& tPos, const local_iterator& bPos) : _ctx(ctx), _tablePos(tPos), _bucketPos(bPos) {}

		public:

			_hTIt() : _ctx(0), _tablePos(0), _bucketPos(local_iterator()) {}
			_hTIt(const _hTIt& o) : _ctx(o._ctx), _tablePos(o._tablePos), _bucketPos(o._bucketPos) {}
			_hTIt& operator=(const _hTIt& o)
			{
				if (&o == this)
					return *this;
				_ctx = o._ctx;
				_tablePos = o._tablePos;
				_bucketPos = o._bucketPos;
				return *this;
			}

			bool operator==(const _hTIt& rhs) const
			{
				return _bucketPos == rhs._bucketPos && _ctx == rhs._ctx && _tablePos == rhs._tablePos;
			}

			bool operator!=(const _hTIt& rhs) const
			{
				return !(*this == rhs);
			}

			bool operator==(const _hTCIt& rhs) const
			{
				return _bucketPos == rhs._bucketPos && _ctx == rhs._ctx && _tablePos == rhs._tablePos;
			}

			bool operator!=(const _hTCIt& rhs) const
			{
				return !(*this == rhs);
			}

			reference operator*()
			{
				return *(_bucketPos);
			}

			const_reference operator*() const
			{
				return *(_bucketPos);
			}

			pointer operator->()
			{
				return _bucketPos.operator->();
			}

			const_pointer operator->() const
			{
				return _bucketPos.operator->();
			}

			_hTIt& operator++()
			{
				if (++_bucketPos == _ctx->at(_tablePos).end())
				{
					while (++_tablePos < _ctx->size())
					{
						if (!(_ctx->at(_tablePos).empty()))
						{
							_bucketPos = _ctx->at(_tablePos).begin();
							return *this;
						}
					}
					*this = _hTIt();
				}
				return *this;
			}

			_hTIt operator++(int)
			{
				_hTIt cpy(*this);

				++(*this);
				return cpy;
			}
		};

		class _hTCIt : public ft::iterator< ft::forward_iterator_tag, const value_type, difference_type, pointer, reference >
		{

			friend class _hTIt;
			friend class unordered_map;

			const _table* _ctx;
			size_type				_tablePos;
			const_local_iterator	_bucketPos;

			_hTCIt(const _table* ctx, const size_type& tPos, const const_local_iterator& bPos) : _ctx(ctx), _tablePos(tPos), _bucketPos(bPos) {}

		public:

			_hTCIt() : _ctx(0), _tablePos(0), _bucketPos(const_local_iterator()) {}
			_hTCIt(const _hTCIt& o) : _ctx(o._ctx), _tablePos(o._tablePos), _bucketPos(o._bucketPos) {}
			_hTCIt(const _hTIt& o) : _ctx(o._ctx), _tablePos(o._tablePos), _bucketPos(o._bucketPos) {}
			_hTCIt& operator=(const _hTCIt& o)
			{
				if (&o == this)
					return *this;
				_ctx = o._ctx;
				_tablePos = o._tablePos;
				_bucketPos = o._bucketPos;
				return *this;
			}
			_hTCIt& operator=(const _hTIt& o)
			{
				_ctx = o._ctx;
				_tablePos = o._tablePos;
				_bucketPos = o._bucketPos;
				return *this;
			}

			bool operator==(const _hTCIt& rhs) const
			{
				return _bucketPos == rhs._bucketPos && _ctx == rhs._ctx && _tablePos == rhs._tablePos;
			}

			bool operator!=(const _hTCIt& rhs) const
			{
				return !(*this == rhs);
			}

			bool operator==(const _hTIt& rhs) const
			{
				return _bucketPos == rhs._bucketPos && _ctx == rhs._ctx && _tablePos == rhs._tablePos;
			}

			bool operator!=(const _hTIt& rhs) const
			{
				return !(*this == rhs);
			}

			const_reference operator*() const
			{
				return *(_bucketPos);
			}

			const_pointer operator->() const
			{
				return _bucketPos.operator->();
			}

			_hTCIt& operator++()
			{
				if (++_bucketPos == _ctx->at(_tablePos).end())
				{
					while (++_tablePos < _ctx->size())
					{
						if (!(_ctx->at(_tablePos).empty()))
						{
							_bucketPos = _ctx->at(_tablePos).begin();
							return *this;
						}
					}
					*this = _hTCIt();
				}
				return *this;
			}

			_hTCIt operator++(int)
			{
				_hTCIt cpy(*this);

				++(*this);
				return cpy;
			}
		};

		typedef _hTIt	iterator;
		typedef _hTCIt	const_iterator;

		explicit unordered_map(size_type n = 8, const hasher& h = hasher(), const key_equal& eql = key_equal(), const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _t(n, _bucket(_alloc)), _hasher(h), _equal(eql), _size(0), _maxLoadFactor(1), _maxLoad(n)
		{}

		unordered_map(const unordered_map& o)
			: _alloc(o._alloc), _t(o._t), _hasher(o._hasher), _equal(o._equal), _size(o._size),
			_maxLoadFactor(o._maxLoadFactor), _maxLoad(o._maxLoad)
		{}

		unordered_map(const unordered_map& o, const allocator_type& alloc)
			: _alloc(alloc), _t(o.bucket_count(), _bucket(_alloc)), _hasher(o._hasher), _equal(o._equal), _size(0),
			_maxLoadFactor(o._maxLoadFactor), _maxLoad(o._maxLoad)
		{
			for (const_iterator cit = o.begin(); cit != o.end(); ++cit)
				_insert(*cit);
		}

		template< class InputIterator >
		unordered_map(InputIterator first, InputIterator last, size_type n = 8, const hasher& h = hasher(), const key_equal& eq = key_equal(), const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _t(n, _bucket(_alloc)), _hasher(h), _equal(eq), _size(0), _maxLoadFactor(1), _maxLoad(n)
		{
			while (first != last)
				insert(*first++);
		}
		//no need to take care about memory, underlying containers does !
		~unordered_map()
		{}

		unordered_map& operator=(const unordered_map& o)
		{
			if (&o != this)
			{
				_t = o._t;
				_hasher = o._hasher;
				_equal = o._equal;
				_size = o._size;
				_maxLoadFactor = o._maxLoadFactor;
				_maxLoad = o._maxLoad;
			}

			return *this;
		}

		//ITERATOR ACCESS
		//end() is default constructor because hash iterator can't go backward, so I don't care about end() values
		iterator	end() { return iterator(); }
		const_iterator end()	const { return const_iterator(); }

		iterator	begin()
		{
			if (!_size)
				return end();
			for (size_type idx = 0; idx < _t.size(); ++idx)
			{
				if (!_t[idx].empty())
					return iterator(&_t, idx, _t[idx].begin());
			}
			//TO PREVENT COMPILATOR WARNING
			return end();
		}

		const_iterator	begin()	const
		{
			if (!_size)
				return end();
			for (size_type idx = 0; idx < _t.size(); ++idx)
			{
				if (!_t[idx].empty())
					return const_iterator(&_t, idx, _t[idx].begin());
			}
			//TO PREVENT COMPILATOR WARNING
			return end();
		}

		local_iterator			begin(size_type idx) { return _t.at(idx).begin(); }
		const_local_iterator	begin(size_type idx)	const { return _t.at(idx).begin(); }
		local_iterator			end(size_type idx) { return _t.at(idx).end(); }
		const_local_iterator	end(size_type idx)		const { return _t.at(idx).end(); }

	private:

		void	_rehash(size_type n)
		{
			size_t			hashValue;
			_table			swapper(n, _bucket(_alloc));
			local_iterator	lit;
			local_iterator	litTmp;

			for (_tableIterator tit = _t.begin(); tit != _t.end(); ++tit)
			{
				lit = tit->begin();
				while (lit != tit->end())
				{
					litTmp = lit;
					++litTmp;

					hashValue = _hasher(lit->first) % n;
					swapper[hashValue].splice(swapper[hashValue].begin(), *tit, lit);

					lit = litTmp;
				}
			}
			_t.swap(swapper);
			_maxLoad = size_type(float(_t.size()) * _maxLoadFactor);
		}

		iterator	_insert(const value_type& val)
		{
			size_t	hashValue;
			size_type	growth;

			if (++_size > _maxLoad)
			{
				growth = 8 * _t.size();
				while ((size_type)((float)growth * _maxLoadFactor) <= _size)
					growth *= 8;
				_rehash(growth);
			}

			hashValue = _hasher(val.first) % _t.size();
			_t[hashValue].push_front(val);

			return iterator(&_t, hashValue, _t[hashValue].begin());
		}

		// Hint is a bucket iterator with a key equal to the key of val (only for multimap)
		// I have to ensure that _size < _maxLoad
		// if hint was given by user, i have to ensure val.first equal_to hint->first
		// if one of two previous condition is wrong, I do normal insert
		iterator	_insert(iterator hint, const value_type& val)
		{
			++_size;
			_t[hint._tablePos].insert(hint._bucketPos, val);
			return --hint;
		}

	public:
		
		size_type	size()	const
		{
			return _size;
		}

		bool	empty()	const
		{
			return _size == 0;
		}

		size_type	max_size()	const
		{
			return _alloc.max_size();
		}

		iterator	find(const key_type& key)
		{
			size_t hashValue;
			local_iterator endIt;

			hashValue = _hasher(key) % _t.size();
			endIt = _t[hashValue].end();
			for (local_iterator it = _t[hashValue].begin(); it != endIt; ++it)
			{
				if (_equal(it->first, key))
					return (iterator(&_t, hashValue, it));
			}
			return end();
		}

		const_iterator	find(const key_type& key) const
		{
			size_t hashValue;
			const_local_iterator endIt;

			hashValue = _hasher(key) % _t.size();
			endIt = _t[hashValue].end();
			for (const_local_iterator it = _t[hashValue].begin(); it != endIt; ++it)
			{
				if (_equal(it->first, key))
					return (const_iterator(&_t, hashValue, it));
			}
			return end();
		}

		size_type	count(const key_type& key) const
		{
			size_t			hashValue;

			hashValue = _hasher(key) % _t.size();
			for (const_local_iterator clit = _t[hashValue].begin(); clit != _t[hashValue].end(); ++clit)
			{
				if (_equal(key, clit->first))
					return 1;
			}
			return 0;
		}

		ft::pair<iterator, iterator>	equal_range(const key_type& k)
		{
			ft::pair<iterator, iterator> ret(end(), end());
			size_t	hashValue;

			hashValue = _hasher(k) % _t.size();
			for (local_iterator lit = _t[hashValue].begin(); lit != _t[hashValue].end(); ++lit)
			{
				if (_equal(k, lit->first))
				{
					ret.first = iterator(&_t, hashValue, lit++);
					ret.second = ret.first;
					while (ret.second != end() && _equal(k, ret.second->first))
						++ret.second;
					return ret;
				}
			}
			return ret;
		}

		ft::pair<const_iterator, const_iterator>	equal_range(const key_type& k)	const
		{
			ft::pair<const_iterator, const_iterator> ret((const_iterator() ), (const_iterator() ) );
			size_t	hashValue;

			hashValue = _hasher(k) % _t.size();
			for (const_local_iterator clit = _t[hashValue].begin(); clit != _t[hashValue].end(); ++clit)
			{
				if (_equal(k, clit->first))
				{
					ret.first = const_iterator(&_t, hashValue, clit++);
					ret.second = ret.first;
					while (ret.second != end() && _equal(k, ret.second->first))
						++ret.second;
					return ret;
				}
			}
			return ret;
		}

		//ELEMENT ACCESS

		mapped_type& operator[](const key_type& k)
		{
			return (insert(ft::make_pair(k, mapped_type())).first)->second;
		}

		mapped_type& at(const key_type& k)
		{
			iterator found = find(k);
			if (found == end())
				throw(ft::out_of_range("ft::unordered_map::at : this element isn't mapped."));
			return found->second;
		}

		const mapped_type& at(const key_type& k) const
		{
			const_iterator found = find(k);
			if (found == end())
				throw(ft::out_of_range("ft::unordered_map::at : this element isn't mapped."));
			return found->second;
		}

		//INSERTS
		
		ft::pair<iterator, bool> insert(const value_type& val)
		{
			iterator found = find(val.first);
			bool	inserted = false;

			if (found == end())
			{
				found = _insert(val);
				inserted = true;
			}
			return ft::make_pair(found, inserted);
		}

		iterator	insert(iterator hint, const value_type& val)
		{
			(void) hint;
			return insert(val).first;
		}

		template <class InputIterator>
		void	insert(InputIterator first, InputIterator last)
		{
			while (first != last)
				insert(*first++);
		}

		size_type	erase(const key_type& key)
		{
			size_t			hashValue;

			hashValue = _hasher(key) % _t.size();
			for (local_iterator lit = _t[hashValue].begin(); lit != _t[hashValue].end(); ++lit)
			{
				if (_equal(lit->first, key))
				{
					--_size;
					_t[hashValue].erase(lit);
					return 1;
				}
			}
			return 0;
		}

		iterator	erase(iterator position)
		{
			if (position._ctx != &_t)
				throw(ft::out_of_range("ft::unordered_map::erase : iterator position doesn't belong to this instance."));

			if (position._tablePos >= _t.size())
				throw(ft::out_of_range("ft::unordered_map::erase : iterator position is out of border."));

			iterator toDel = position++;
			_t[toDel._tablePos].erase(toDel._bucketPos);
			--_size;
			return position;
		}

		iterator	erase(iterator first, iterator last)
		{
			while (first != last)
				first = erase(first);
			return first;
		}

		void	clear()
		{
			_size = 0;
			for (_tableIterator it = _t.begin(); it != _t.end(); ++it)
			{
				it->clear();
			}
		}

		void	swap(unordered_map& o)
		{
			allocator_type	allocTmp;
			size_type		sizeTmp;
			key_equal		equalTmp;
			hasher			hasherTmp;
			float			maxLoadFactorTmp;
			size_type		maxLoadTmp;

			allocTmp = _alloc;
			sizeTmp = _size;
			equalTmp = _equal;
			hasherTmp = _hasher;
			maxLoadFactorTmp = _maxLoadFactor;
			maxLoadTmp = _maxLoad;

			_alloc = o._alloc;
			_size = o._size;
			_equal = o._equal;
			_hasher = o._hasher;
			_maxLoadFactor = o._maxLoadFactor;
			_maxLoad = o._maxLoad;

			o._alloc = allocTmp;
			o._size = sizeTmp;
			o._equal = equalTmp;
			o._hasher = hasherTmp;
			o._maxLoadFactor = maxLoadFactorTmp;
			o._maxLoad = maxLoadTmp;

			_t.swap(o._t);
		}

		float	max_load_factor() const
		{
			return _maxLoadFactor;
		}

		float	load_factor() const
		{
			return static_cast<float>(_size) / _t.size();
		}

		void	max_load_factor(float z)
		{
			if (z <= 0)
				throw(ft::invalid_argument("ft::unordered_map::_max_load_factor : _maxLoadFactor can't be set to a negative value"));
			_maxLoadFactor = z;
			_maxLoad = size_type(float(_t.size()) * z);
		}

		void	rehash(size_type n)
		{
			size_type newhashVal;
			size_type minimum;

			minimum = static_cast<size_type>(ceilf(static_cast<float>(_size) / _maxLoadFactor));
			newhashVal = ft::max(n, minimum);
			if (newhashVal != _t.size())
				_rehash(newhashVal);
		}

		void	reserve(size_type n)
		{
			size_type newHashVal;
			size_type minimum;
			size_type asked;

			minimum = static_cast<size_type>(ceilf(static_cast<float>(_size) / _maxLoadFactor));
			asked = static_cast<size_type>(ceilf(static_cast<float>(n) / _maxLoadFactor));
			newHashVal = ft::max(asked, minimum);
			if (newHashVal != _t.size())
				_rehash(newHashVal);
		}

		size_t bucket(const key_type& key)	const
		{
			return _hasher(key) % _t.size();
		}

		size_type	bucket_count()	const
		{
			return _t.size();
		}

		size_type	bucket_size(size_type idx)	const
		{
			return _t[idx].size();
		}

		size_type	max_bucket_count()	const
		{
			return _t.get_allocator().max_size();
		}

		allocator_type	get_allocator()	const
		{
			return	_alloc;
		}

		key_equal	key_eq()	const
		{
			return _equal;
		}

		hasher	hash_function()	const
		{
			return _hasher;
		}
	};

	//NON MEMBERS RELATIONAL OPERATORS OVERLOADS + SWAP

	template <class Key, class T, class Hash, class Pred, class Alloc>
	bool operator== (const unordered_map<Key, T, Hash, Pred, Alloc>& lhs, const unordered_map<Key, T, Hash, Pred, Alloc>& rhs)
	{
		typename unordered_map<Key, T, Hash, Pred, Alloc>::const_iterator	found;

		if (lhs.size() != rhs.size())
			return false;

		for (typename unordered_map<Key, T, Hash, Pred, Alloc>::const_iterator cit = lhs.begin(); cit != lhs.end(); ++cit)
		{
			found = rhs.find(cit->first);
			if (found == rhs.end() || (cit->second != found->second))
				return false;
		}

		return true;
	}

	template <class Key, class T, class Hash, class Pred, class Alloc>
	bool operator!= (const unordered_map<Key, T, Hash, Pred, Alloc>& lhs, const unordered_map<Key, T, Hash, Pred, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class Key, class T, class Hash, class Pred, class Alloc>
	void	swap(unordered_map<Key, T, Hash, Pred, Alloc>& lhs, unordered_map<Key, T, Hash, Pred, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}

	template < class Key, class T, class Hash = ft::hash<Key>, class Pred = ft::equal_to<Key>, class Alloc = std::allocator< ft::pair< const Key, T > > >
	class unordered_multimap
	{

		friend class _hTIt;
		friend class _hTCIt;

	public:

		typedef Key						key_type;
		typedef T						mapped_type;
		typedef ft::pair<const Key, T>	value_type;
		typedef Alloc					allocator_type;
		typedef typename Alloc::reference		reference;
		typedef typename Alloc::const_reference const_reference;
		typedef typename Alloc::pointer			pointer;
		typedef typename Alloc::const_pointer	const_pointer;
		typedef typename Alloc::size_type		size_type;
		typedef typename Alloc::difference_type	difference_type;
		typedef Pred		key_equal;
		typedef Hash		hasher;

	private:

		typedef ft::list< value_type, Alloc >		_bucket;
		typedef ft::vector< _bucket >				_table;
		typedef typename _table::iterator			_tableIterator;
		typedef typename _table::const_iterator		_tableConstIterator;

	public:

		typedef typename _bucket::iterator			local_iterator;
		typedef typename _bucket::const_iterator	const_local_iterator;

	private:

		allocator_type	_alloc;
		_table			_t;
		hasher			_hasher;
		key_equal		_equal;
		size_type		_size;
		float			_maxLoadFactor;
		size_type		_maxLoad;

		struct _erase_if_cond : public ft::unary_function<value_type, bool>
		{
			const key_type& cmp;
			key_equal		keq;

			_erase_if_cond(const key_type& k, const key_equal& eq) : cmp(k), keq(eq) {};

			bool operator()(const value_type& toEraseIf) const
			{
				return keq(cmp, toEraseIf.first);
			}
		};

	public:

		class _hTCIt;
		class _hTIt : public ft::iterator< ft::forward_iterator_tag, value_type, difference_type, pointer, reference >
		{

			friend class _hTCIt;
			friend class unordered_multimap;

			_table* _ctx;
			size_type	_tablePos;
			local_iterator	_bucketPos;

			_hTIt(_table* ctx, const size_type& tPos, const local_iterator& bPos) : _ctx(ctx), _tablePos(tPos), _bucketPos(bPos) {}

		public:

			_hTIt() : _ctx(0), _tablePos(0), _bucketPos(local_iterator()) {}
			_hTIt(const _hTIt& o) : _ctx(o._ctx), _tablePos(o._tablePos), _bucketPos(o._bucketPos) {}
			_hTIt& operator=(const _hTIt& o)
			{
				if (&o == this)
					return *this;
				_ctx = o._ctx;
				_tablePos = o._tablePos;
				_bucketPos = o._bucketPos;
				return *this;
			}

			bool operator==(const _hTIt& rhs) const
			{
				return _bucketPos == rhs._bucketPos && _ctx == rhs._ctx && _tablePos == rhs._tablePos;
			}

			bool operator!=(const _hTIt& rhs) const
			{
				return !(*this == rhs);
			}

			bool operator==(const _hTCIt& rhs) const
			{
				return _bucketPos == rhs._bucketPos && _ctx == rhs._ctx && _tablePos == rhs._tablePos;
			}

			bool operator!=(const _hTCIt& rhs) const
			{
				return !(*this == rhs);
			}

			reference operator*()
			{
				return *(_bucketPos);
			}

			const_reference operator*() const
			{
				return *(_bucketPos);
			}

			pointer operator->()
			{
				return _bucketPos.operator->();
			}

			const_pointer operator->() const
			{
				return _bucketPos.operator->();
			}

			_hTIt& operator++()
			{
				if (++_bucketPos == _ctx->at(_tablePos).end())
				{
					while (++_tablePos < _ctx->size())
					{
						if (!(_ctx->at(_tablePos).empty()))
						{
							_bucketPos = _ctx->at(_tablePos).begin();
							return *this;
						}
					}
					_ctx = 0;
					_tablePos = 0;
					_bucketPos = local_iterator();
				}
				return *this;
			}

			_hTIt operator++(int)
			{
				_hTIt cpy(*this);

				++(*this);
				return cpy;
			}
		};

		class _hTCIt : public ft::iterator< ft::forward_iterator_tag, const value_type, difference_type, pointer, reference >
		{

			friend class _hTIt;
			friend class unordered_multimap;

			const _table* _ctx;
			size_type				_tablePos;
			const_local_iterator		_bucketPos;

			_hTCIt(const _table* ctx, const size_type& tPos, const const_local_iterator& bPos) : _ctx(ctx), _tablePos(tPos), _bucketPos(bPos) {}

		public:

			_hTCIt() : _ctx(0), _tablePos(0), _bucketPos(const_local_iterator()) {}
			_hTCIt(const _hTCIt& o) : _ctx(o._ctx), _tablePos(o._tablePos), _bucketPos(o._bucketPos) {}
			_hTCIt(const _hTIt& o) : _ctx(o._ctx), _tablePos(o._tablePos), _bucketPos(o._bucketPos) {}
			_hTCIt& operator=(const _hTCIt& o)
			{
				if (&o == this)
					return *this;
				_ctx = o._ctx;
				_tablePos = o._tablePos;
				_bucketPos = o._bucketPos;
				return *this;
			}
			_hTCIt& operator=(const _hTIt& o)
			{
				_ctx = o._ctx;
				_tablePos = o._tablePos;
				_bucketPos = o._bucketPos;
				return *this;
			}

			bool operator==(const _hTCIt& rhs) const
			{
				return _bucketPos == rhs._bucketPos && _ctx == rhs._ctx && _tablePos == rhs._tablePos;
			}

			bool operator!=(const _hTCIt& rhs) const
			{
				return !(*this == rhs);
			}

			bool operator==(const _hTIt& rhs) const
			{
				return _bucketPos == rhs._bucketPos && _ctx == rhs._ctx && _tablePos == rhs._tablePos;
			}

			bool operator!=(const _hTIt& rhs) const
			{
				return !(*this == rhs);
			}

			const_reference operator*() const
			{
				return *(_bucketPos);
			}

			const_pointer operator->() const
			{
				return _bucketPos.operator->();
			}

			_hTCIt& operator++()
			{
				if (++_bucketPos == _ctx->at(_tablePos).end())
				{
					while (++_tablePos < _ctx->size())
					{
						if (!(_ctx->at(_tablePos).empty()))
						{
							_bucketPos = _ctx->at(_tablePos).begin();
							return *this;
						}
					}
					_ctx = 0;
					_tablePos = 0;
					_bucketPos = const_local_iterator();
				}
				return *this;
			}

			_hTCIt operator++(int)
			{
				_hTCIt cpy(*this);

				++(*this);
				return cpy;
			}
		};

		typedef _hTIt	iterator;
		typedef _hTCIt	const_iterator;

		explicit unordered_multimap(size_type n = 8, const hasher& h = hasher(), const key_equal& eql = key_equal(), const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _t(n, _bucket(_alloc)), _hasher(h), _equal(eql), _size(0), _maxLoadFactor(1), _maxLoad(n)
		{}

		unordered_multimap(const unordered_multimap& o)
			: _alloc(o._alloc), _t(o._t), _hasher(o._hasher), _equal(o._equal), _size(o._size),
			_maxLoadFactor(o._maxLoadFactor), _maxLoad(o._maxLoad)
		{}

		unordered_multimap(const unordered_multimap& o, const allocator_type& alloc)
			: _alloc(alloc), _t(o.bucket_count(), _bucket(_alloc)), _hasher(o._hasher), _equal(o._equal), _size(0),
			_maxLoadFactor(o._maxLoadFactor), _maxLoad(o._maxLoad)
		{
			for (const_iterator cit = o.begin(); cit != o.end(); ++cit)
				_insert(*cit);
		}

		template< class InputIterator >
		unordered_multimap(InputIterator first, InputIterator last, size_type n = 8, const hasher& h = hasher(), const key_equal& eq = key_equal(), const allocator_type& alloc = allocator_type())
			: _alloc(alloc), _t(n, _bucket(_alloc)), _hasher(h), _equal(eq), _size(0), _maxLoadFactor(1), _maxLoad(n)
		{
			while (first != last)
				_insert(*first++);
		}
		//no need to take care about memory, underlying containers does !
		~unordered_multimap()
		{}

		unordered_multimap& operator=(const unordered_multimap& o)
		{
			if (&o != this)
			{
				_t = o._t;
				_hasher = o._hasher;
				_equal = o._equal;
				_size = o._size;
				_maxLoadFactor = o._maxLoadFactor;
				_maxLoad = o._maxLoad;
			}

			return *this;
		}

		//ITERATOR ACCESS
		//end() is default constructor because hash iterator can't go backward, so I don't care about end() values
		iterator	end() { return iterator(); }
		const_iterator end()	const { return const_iterator(); }

		iterator	begin()
		{
			if (!_size)
				return end();
			for (size_type idx = 0; idx < _t.size(); ++idx)
			{
				if (!_t[idx].empty())
					return iterator(&_t, idx, _t[idx].begin());
			}
			//TO PREVENT COMPILATOR WARNING
			return end();
		}

		const_iterator	begin()	const
		{
			if (!_size)
				return end();
			for (size_type idx = 0; idx < _t.size(); ++idx)
			{
				if (!_t[idx].empty())
					return const_iterator(&_t, idx, _t[idx].begin());
			}
			//TO PREVENT COMPILATOR WARNING
			return end();
		}

		local_iterator			begin(size_type idx) { return _t.at(idx).begin(); }
		const_local_iterator	begin(size_type idx)	const { return _t.at(idx).begin(); }
		local_iterator			end(size_type idx) { return _t.at(idx).end(); }
		const_local_iterator	end(size_type idx)		const { return _t.at(idx).end(); }

	private:

		void	_rehash(size_type n)
		{
			size_t			hashValue;
			_table			swapper(n, _bucket(_alloc));
			local_iterator	lit;
			local_iterator	litTmp;

			for (_tableIterator tit = _t.begin(); tit != _t.end(); ++tit)
			{
				lit = tit->begin();
				while (lit != tit->end())
				{
					litTmp = lit;
					++litTmp;

					hashValue = _hasher(lit->first) % n;
					swapper[hashValue].splice(swapper[hashValue].begin(), *tit, lit);

					lit = litTmp;
				}
			}
			_t.swap(swapper);
			_maxLoad = size_type(float(_t.size()) * _maxLoadFactor);
		}

		iterator	_insert(const value_type& val)
		{
			size_t	hashValue;
			size_type	growth;

			if (++_size > _maxLoad)
			{
				growth = 8 * _t.size();
				while ((size_type)((float)growth * _maxLoadFactor) <= _size)
					growth *= 8;
				_rehash(growth);
			}

			hashValue = _hasher(val.first) % _t.size();
			_t[hashValue].push_front(val);

			return iterator(&_t, hashValue, _t[hashValue].begin());
		}

		// Hint is a bucket iterator with a key equal to the key of val (only for multimap)
		// if hint was given by user, i have to ensure val.first equal_to hint->first
		// if one of two previous condition is wrong, I do normal insert
		iterator	_insert(iterator hint, const value_type& val)
		{
			local_iterator inserted;

			++_size;
			inserted = _t[hint._tablePos].insert(hint._bucketPos, val);
			return iterator(hint._ctx, hint._tablePos, inserted);
		}

	public:

		size_type	size()	const
		{
			return _size;
		}

		bool	empty()	const
		{
			return _size == 0;
		}

		size_type	max_size()	const
		{
			return _alloc.max_size();
		}

		iterator	find(const key_type& key)
		{
			size_t hashValue;
			local_iterator endIt;

			hashValue = _hasher(key) % _t.size();
			endIt = _t[hashValue].end();
			for (local_iterator it = _t[hashValue].begin(); it != endIt; ++it)
			{
				if (_equal(it->first, key))
					return (iterator(&_t, hashValue, it));
			}
			return end();
		}

		const_iterator	find(const key_type& key) const
		{
			size_t hashValue;
			const_local_iterator endIt;

			hashValue = _hasher(key) % _t.size();
			endIt = _t[hashValue].end();
			for (const_local_iterator it = _t[hashValue].begin(); it != endIt; ++it)
			{
				if (_equal(it->first, key))
					return (const_iterator(&_t, hashValue, it));
			}
			return end();
		}

		size_type	count(const key_type& key) const
		{
			size_t			hashValue;
			size_type		count;

			count = 0;
			hashValue = _hasher(key) % _t.size();
			for (const_local_iterator clit = _t[hashValue].begin(); clit != _t[hashValue].end(); ++clit)
				count += _equal(key, clit->first);

			return count;
		}

		ft::pair<iterator, iterator>	equal_range(const key_type& k)
		{
			ft::pair<iterator, iterator> ret(end(), end());
			size_t	hashValue;

			hashValue = _hasher(k) % _t.size();
			for (local_iterator lit = _t[hashValue].begin(); lit != _t[hashValue].end(); ++lit)
			{
				if (_equal(k, lit->first))
				{
					ret.first = iterator(&_t, hashValue, lit++);
					ret.second = ret.first;
					while (ret.second != end() && _equal(k, ret.second->first))
						++ret.second;
					return ret;
				}
			}
			return ret;
		}

		ft::pair<const_iterator, const_iterator>	equal_range(const key_type& k)	const
		{
			ft::pair<const_iterator, const_iterator> ret( (const_iterator() ), (const_iterator() ) );
			size_t	hashValue;

			hashValue = _hasher(k) % _t.size();
			for (const_local_iterator clit = _t[hashValue].begin(); clit != _t[hashValue].end(); ++clit)
			{
				if (_equal(k, clit->first))
				{
					ret.first = const_iterator(&_t, hashValue, clit++);
					ret.second = ret.first;
					while (ret.second != end() && _equal(k, ret.second->first))
						++ret.second;
					return ret;
				}
			}
			return ret;
		}
		
		//INSERTS

		iterator	insert(const value_type& val)
		{
			iterator found = find(val.first);

			if (found != end())
				return _insert(found, val);
			
			return _insert(val);
		}

		iterator	insert(iterator hint, const value_type& val)
		{
			if (hint != end() && _equal(hint->first, val.first))
				return _insert(hint, val);
			return _insert(val);
		}

		template <class InputIterator>
		void	insert(InputIterator first, InputIterator last)
		{
			while (first != last)
				insert(*first++);
		}

		size_type	erase(const key_type& key)
		{
			size_t			hashValue;
			size_type		eraseCount;
			_erase_if_cond	pred(key, _equal);

			hashValue = _hasher(key) % _t.size();
			eraseCount = _t[hashValue].size();
			_t[hashValue].remove_if(pred);

			eraseCount -= _t[hashValue].size();
			_size -= eraseCount;
			return eraseCount;
		}

		iterator	erase(iterator position)
		{
			if (position._ctx != &_t)
				throw(ft::out_of_range("ft::unordered_multimap::erase : iterator position doesn't belong to this instance."));

			if (position._tablePos >= _t.size())
				throw(ft::out_of_range("ft::unordered_multimap::erase : iterator position is out of border."));

			iterator toDel = position++;
			_t[toDel._tablePos].erase(toDel._bucketPos);
			--_size;
			return position;
		}

		iterator	erase(iterator first, iterator last)
		{
			while (first != last)
				first = erase(first);
			return first;
		}

		void	clear()
		{
			_size = 0;
			for (_tableIterator it = _t.begin(); it != _t.end(); ++it)
			{
				it->clear();
			}
		}

		void	swap(unordered_multimap& o)
		{
			allocator_type	allocTmp;
			size_type		sizeTmp;
			key_equal		equalTmp;
			hasher			hasherTmp;
			float			maxLoadFactorTmp;
			size_type		maxLoadTmp;

			allocTmp = _alloc;
			sizeTmp = _size;
			equalTmp = _equal;
			hasherTmp = _hasher;
			maxLoadFactorTmp = _maxLoadFactor;
			maxLoadTmp = _maxLoad;

			_alloc = o._alloc;
			_size = o._size;
			_equal = o._equal;
			_hasher = o._hasher;
			_maxLoadFactor = o._maxLoadFactor;
			_maxLoad = o._maxLoad;

			o._alloc = allocTmp;
			o._size = sizeTmp;
			o._equal = equalTmp;
			o._hasher = hasherTmp;
			o._maxLoadFactor = maxLoadFactorTmp;
			o._maxLoad = maxLoadTmp;

			_t.swap(o._t);
		}

		float	max_load_factor() const
		{
			return _maxLoadFactor;
		}

		float	load_factor() const
		{
			return static_cast<float>(_size) / _t.size();
		}

		void	max_load_factor(float z)
		{
			if (z <= 0)
				throw(ft::invalid_argument("ft::unordered_multimap::_max_load_factor : _maxLoadFactor can't be set to a negative value"));
			_maxLoadFactor = z;
			_maxLoad = size_type(float(_t.size()) * z);
		}

		void	rehash(size_type n)
		{
			size_type newhashVal;
			size_type minimum;

			minimum = static_cast<size_type>(static_cast<float>(_size) / _maxLoadFactor);
			newhashVal = ft::max(n, minimum);
			if (newhashVal != _t.size())
				_rehash(newhashVal);
		}

		void	reserve(size_type n)
		{
			size_type newHashVal;
			size_type minimum;
			size_type asked;

			minimum = static_cast<size_type>(ceilf(static_cast<float>(_size) / _maxLoadFactor));
			asked = static_cast<size_type>(ceilf(static_cast<float>(n) / _maxLoadFactor));
			newHashVal = ft::max(asked, minimum);
			if (newHashVal != _t.size())
				_rehash(newHashVal);
		}

		size_t bucket(const key_type& key)	const
		{
			return _hasher(key) % _t.size();
		}

		size_type	bucket_count()	const
		{
			return _t.size();
		}

		size_type	bucket_size(size_type idx)	const
		{
			return _t[idx].size();
		}

		size_type	max_bucket_count()	const
		{
			return _t.get_allocator().max_size();
		}

		allocator_type	get_allocator()	const
		{
			return	_alloc;
		}

		key_equal	key_eq()	const
		{
			return _equal;
		}

		hasher	hash_function()	const
		{
			return _hasher;
		}
	};

	//NON MEMBERS RELATIONAL OPERATORS OVERLOADS + SWAP

	template <class Key, class T, class Hash, class Pred, class Alloc>
	bool operator== (const unordered_multimap<Key, T, Hash, Pred, Alloc>& lhs, const unordered_multimap<Key, T, Hash, Pred, Alloc>& rhs)
	{
		typedef typename unordered_multimap<Key, T, Hash, Pred, Alloc>::const_iterator	Ite;
		typedef ft::pair<Ite, Ite> Range;

		Range rangeLhs;
		Range rangeRhs;
		ft::list< const T* > lhsL;
		ft::list< const T* > rhsL;
		bool returnFalse;
		Ite ite;

		if (lhs.size() != rhs.size())
			return false;

		ite = lhs.begin();

		while (ite != lhs.end())
		{
			rangeLhs = lhs.equal_range(ite->first);
			rangeRhs = rhs.equal_range(ite->first);
			// Because I don't know if mapped_type dispose of a operator<, I only can compare with operator==, if it exists.
			// Good way to do that without including other librarie is to collect all addresses of each mapped values I have
			// to compare in two lists, one for lhs, one for rhs.
			// Then I erase each address on rhs which has his pair in lhs
			// If one element doesn't have pair I return false;
			while (rangeLhs.first != rangeLhs.second)
			{
				lhsL.push_back(&(rangeLhs.first->second));
				if (rangeRhs.first == rangeRhs.second)
					return false;
				rhsL.push_back(&(rangeRhs.first->second));
				++rangeLhs.first;
				++rangeRhs.first;
			}
			if (lhsL.size() != rhsL.size())
				return false;
			while (!lhsL.empty())
			{
				returnFalse = true;
				for (typename ft::list< const T* >::iterator it = rhsL.begin(); it != rhsL.end(); ++it)
				{
					if (*(lhsL.back()) == *(*it))
					{
						returnFalse = false;
						lhsL.pop_back();
						rhsL.erase(it);
						break;
					}
				}
				if (returnFalse)
					return false;
			}

			ite = rangeLhs.second;
		}

		return true;
	}

	template <class Key, class T, class Hash, class Pred, class Alloc>
	bool operator!= (const unordered_multimap<Key, T, Hash, Pred, Alloc>& lhs, const unordered_multimap<Key, T, Hash, Pred, Alloc>& rhs)
	{
		return !(lhs == rhs);
	}

	template <class Key, class T, class Hash, class Pred, class Alloc>
	void	swap(unordered_multimap<Key, T, Hash, Pred, Alloc>& lhs, unordered_multimap<Key, T, Hash, Pred, Alloc>& rhs)
	{
		lhs.swap(rhs);
	}
}

#endif