#ifndef _HASH_TABLE_ABSTRACTDATA_BY_LE_N
# define _HASH_TABLE_ABSTRACTDATA_BY_LE_N

#include <memory>
#include <cstddef>
#include "functional.hpp"
#include "list.hpp"
#include "vector.hpp"
#include "utility.hpp"
#include "iterator.hpp"

namespace ft
{
	/*
	* implem notes :
	* each time size goes > _table.size() => _table.resize(8 * _table.size());
	*
	* each insert places the new element like this : _bucket.push_front(element);
	*
	* each resize involves to rehach each element stored in table.
	* easy way to do it is to create a new table, then rehash all old _table values in it, then swap the tables. problem is that it makes max_size wrong
	* because i need memory for old table and new table to copy and swap.
	* At the moment, I didn't found another efficient way.
	*
	* I don't have to cache hash value, that's hasher rule to be able to do/recover fastly the hash (if hash is complexity O(1) that's cool)
	*
	* Insert with a hint iterator is totally useless because I'll hash the value_type in any case.
	*/
	template < class T, class hash = ft::hash<T>, class alloc = std::allocator< T > >
	class _hashTable
	{

		//THEY HAVE TO BE FRIEND

		friend class _hTIt;
		friend class _htCIt;

	public:

		typedef T			value_type;
		typedef T& reference;
		typedef const		reference const_reference;
		typedef T* pointer;
		typedef const		pointer const_pointer;
		typedef hash		hasher;
		typedef alloc		allocator_type;
		typedef typename	alloc::size_type	size_type;
		typedef typename	alloc::difference_type	difference_type;

	protected:

		typedef ft::list< T, alloc >				_bucket;
		typedef ft::vector< _bucket, alloc >		_table;
		typedef typename _table::size_type			_tableSizeType;
		typedef typename _table::iterator			local_iterator;
		typedef typename _table::const_iterator		const_local_iterator;
		typedef typename _bucket::iterator			_bucketIt;
		typedef typename _bucket::const_iterator	_bucketCIt;

		allocator_type	_alloc;
		_table	_t;
		hasher	_hasher;
		size_type	_size;
		float	_maxLoadFactor;
		size_type	_maxLoad;

		class _hTCIt;
		class _hTIt : public ft::iterator< ft::forward_iterator_tag, T, difference_type, pointer, reference > //_hashTableIterator was too long
		{

			friend _hTCIt;
			friend _hashTable;

			_table* _ctx;
			_tableSizeType	_tablePos;
			_bucketIt		_bucketPos;

			_hTIt(_table* ctx, const _tableSizeType& tPos, const _bucketIt& bPos) : _ctx(ctx), _tablePos(tPos), _bucketPos(bPos) {}

		protected:

			_hTIt() : _ctx(nullptr), _tablePos(_tableIt()), _bucketPos(_bucketIt) {}
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
					_bucketPos = _bucketIt();
				}
				return *this;
			}

			_hTIt operator++(int)
			{
				_hTIt cpy(*this);

				++_hTIt;
				return cpy;
			}
		};

		explicit _hashTable(size_type n = 8, const hasher& h = hasher(), const allocator_type& alloc)
			: _alloc(alloc), _t(n, _bucket(_alloc), _alloc), _hasher(h), _size(n), _maxLoadFactor(1), _maxLoad(n)
		{}

		_hashTable(const _hashTable& o)
			: _alloc(o._alloc), _t(o._t), _hasher(o._hasher), _size(o._size),
			_maxLoadFactor(o._maxLoadFactor), _maxLoad(o._maxLoad)
		{}

		//no need to take care about memory, underlying containers does !
		~_hashTable()
		{}

		void	_rehash(_tableSizeType n)
		{
			size_t		hashValue;
			_table		swapper(n, _bucket(_alloc));
			_bucketIt	bite;
			_bucketIt	biteTmp;

			for (local_iterator lit = _t.begin(); lit != _t.end(); ++lit)
			{
				bite = lit->begin();
				while (bite != lit->end())
				{
					biteTmp = bite;
					++biteTmp;

					hashValue = _hasher(*bite) % n;
					swapper[hashValue].splice(swapper[hashValue].begin(), *lit, bite);

					bite = biteTmp;
				}
			}
			_t.swap(swapper);
			_maxLoad = size_type(float(_table.size()) * _maxLoadFactor);
		}

		_hTIt	_insert(const value_type& val)
		{
			size_t	hashValue;
			_tableSizeType	growth;

			if (++_size >= _maxLoad)
			{
				growth = 8 * _t.size();
				while ((size_type)((float)growth * _maxLoadFactor) <= _size)
					growth *= 8;
				_rehash(growth);
			}

			hashValue = _hasher(val) % _t.size();
			_t[hashValue].push_front(val);

			return _hTIt(&_t, hashValue, _t[hashValue].begin());
		}

		template < class Equal >
		_hTIt	_find(const value_type& val, const Equal& equal)
		{
			size_t hashValue;
			_bucketIt end;

			hashValue = _hasher(val) % _t.size();
			end = _t[hashValue].end();
			for (_bucketIt it = _t[hashValue].begin(); it != end; ++it)
			{
				if (equal(*it, val))
					return (_hTIt(&_t, hashValue, it));
			}
			return (_hTIt(&_t, _t.size(), _bucketIt());
		}

		template < class Equal >
		size_type	_erase(const value_type& val)
		{
			size_t hashValue;
			typename _bucket::size_type	eraseCount;

			hashValue = _hasher(val) % _t.size();
			eraseCount = _t[hashValue].size();
			_t[hashValue].remove(val);

			eraseCount -= _t[hashValue].size();
			_size -= eraseCount;
			return eraseCount;
		}

		_hTIt	_erase(_hTIt position)
		{
			if (position._ctx != &_t)
				throw(ft::out_of_range("ft::_hashTable::_erase : iterator position doesn't belong to this instance."));

			if (position._tablePos >= _t.size())
				throw(ft::out_of_range("ft::_hashTable::_erase : iterator position is out of border."));

			_hTIt toDel = position++;
			_t[toDel._tablePos].erase(toDel._bucketPos);
			--_size;
			return toDel;
		}

		_hTIt	_erase(_hTIt first, _hTIt last)
		{
			while (first != last)
				first = _erase(first);
			return first;
		}

		float	_max_load_factor() const
		{
			return _maxLoadFactor;
		}

		float	_load_factor() const
		{
			return _size / _table.size();
		}

		void	_max_load_factor(float z)
		{
			if (z <= 0)
				throw(ft::invalid_argument("ft::_hashTable::_max_load_factor : _maxLoadFactor can't be set to a negative value"));
			_maxLoadFactor = z;
			_maxLoad = size_type(float(_table.size()) * z);
		}

		void	_reserve(size_type n)
		{
			_tableSizeType	growth;

			if (n >= _maxLoad)
			{
				growth = 8 * _t.size();
				while ((size_type)((float)growth * _maxLoadFactor) <= n)
					growth *= 8;
				_rehash(growth);
			}
		}
	};
}

#endif // !_HASH_TABLE_ABSTRACTDATA_BY_LE_N
