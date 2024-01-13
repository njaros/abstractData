#include "../includes/test_elements.hpp"

void	_itoaRecu(std::string& str, long long nb)
{
	long long divided = nb / 10;
	if (divided)
		_itoaRecu(str, divided);
	str += nb % 10 + '0';
}

std::string	itoa(long long nb)
{
	std::string str;

	_itoaRecu(str, nb);
	return str;
}

long long pow(long long a, long long b)
{
	long long ret = 1;

	if (b < 0)
		return 0;
	
	while (b--)
		ret *= a;
	return ret;
}