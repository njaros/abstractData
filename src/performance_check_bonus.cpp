#include <unordered_map>
#include "../includes/unordered_map.hpp"
#include <iostream>
#include <sys/time.h>

void displayTime(timeval t1, int idFT, const char* testName)
{
	timeval t2;
	gettimeofday(&t2, 0);
	if (idFT == 1)
		std::cout << " FT";
	else
		std::cout << "STD";
	std::cout<< "::" << testName << " time elapsed : " <<
		(t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec) << " usec\n";
}

int main()
{
	timeval t;

	{
		std::cout << "====UNORDERED_MAP====\n\n";
		std::unordered_map<int, int> stdUm;
		ft::unordered_map<int, int> ftUm;

		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; ++i)
		{
			stdUm.insert(std::make_pair(i * i, i));
		}
		displayTime(t, 0, "insert 10 000 000 ints");

		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; ++i)
		{
			ftUm.insert(ft::make_pair(i * i, i));
		}
		displayTime(t, 1, "insert 10 000 000 ints");

		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; i += 2)
		{
			stdUm.find(i);
		}
		displayTime(t, 0, "find 10 000 000 ints");
		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; i += 2)
		{
			stdUm.find(i);
		}
		displayTime(t, 1, "find 10 000 000 ints");
	}

	return 0;
}