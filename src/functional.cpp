#include "../includes/functional.hpp"

size_t	_do_hash_pls(const unsigned char* first, size_t len)
{	
	size_t h;
	
	h = ft::FNV_offset_basis;
	for (size_t i = 0; i < len; ++i)
	{
		h ^= first[i];
		h *= ft::FNV_prime;
	}
	return h;
}