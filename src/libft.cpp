#include "../includes/libft.hpp"

namespace ft
{
	size_t strlen(const char* str)
	{
		size_t l = 0;
		while (str[l++]);
		return l;
	}

	void strcpy(char* toFill, const char* toCpy)
	{
		size_t idx = 0;
		while (toCpy[idx])
		{
			toFill[idx] = toCpy[idx];
			idx++;
		}
		toFill[idx] = '\0';
	}
}
