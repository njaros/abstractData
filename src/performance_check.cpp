#include "../includes/deque.hpp"
#include "../includes/vector.hpp"
#include "../includes/list.hpp"
#include "../includes/map.hpp"
#include "../includes/set.hpp"
#include "../includes/unordered_map.hpp"
#include "../includes/unordered_set.hpp"
#include "../includes/stack.hpp"
#include "../includes/queue.hpp"
#include <set>
#include <map>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <stack>
#include <queue>
#include <iostream>
#include <sys/time.h>

void displayTime(timeval t1, int idFT, const char* testName)
{
	timeval t2;
	gettimeofday(&t2, 0);
	if (idFT == 1)
		std::cout << "FT";
	else
		std::cout << "STD";
	std::cout<< "::" << testName << " time elapsed : " <<
		(t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec) << "usec\n";
}

int main()
{
	timeval t;
	std::unordered_map<int, int> stdUm;
	ft::unordered_map<int, int> ftUm;
	gettimeofday(&t, 0);
	for (int i = 0; i < 1000000; ++i)
	{
		stdUm[i] = i;
	}
	displayTime(t, 0, "1000000 ints");
	gettimeofday(&t, 0);
	for (int i = 0; i < 1000000; ++i)
	{
		ftUm[i] = i;
	}
	displayTime(t, 1, "1000000 ints");
	return 0;
}