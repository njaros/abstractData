#ifndef UNORDERED_MAP_HPP_ABSTRACT_DATA_BY_LE_N
#define UNORDERED_MAP_HPP_ABSTRACT_DATA_BY_LE_N

# include <memory>
# include <cstddef>
# include <cstring>
# include "_hashTable.hpp"
# include "utility.hpp"
# include "reverse_iterator.hpp"
# include "algorithm.hpp"

namespace ft
{
	template < class Key, class T, class Hash = ft::hash<Key>, class Pred = ft::equal_to<Key>, class Alloc = std::allocator< pair<const Key,T> > > 
	class unordered_map : public _hashTableMap<Key, T, Hash, Pred, Alloc>
	{

	public:




	};
}

#endif