#include "../includes/test_elements.hpp"

void	_itoaRecu(std::string& str, int nb)
{
	int divided = nb / 10;
	if (divided)
		_itoaRecu(str, divided);
	str += nb % 10 + '0';
}

std::string	itoa(int nb)
{
	std::string str;

	_itoaRecu(str, nb);
	return str;
}