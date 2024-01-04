#ifndef UNORDERED_MAP_HPP_ABSTRACT_DATA_BY_LE_N
#define UNORDERED_MAP_HPP_ABSTRACT_DATA_BY_LE_N

# include <memory>
# include <cstddef>
# include <cstring>
# include "functional.hpp"
# include "iterator.hpp"
# include "treeManager.hpp"
# include "utility.hpp"
# include "reverse_iterator.hpp"
# include "algorithm.hpp"

namespace ft
{
	template < class Key, class T, class Hash = std::hash<Key>, class Pred = ft::equal_to<Key>, class Alloc = std::allocator< pair<const Key,T> > > 
	class unordered_map
	{

	};
}

#endif