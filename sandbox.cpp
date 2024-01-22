#include "includes/deque.hpp"
#include "includes/vector.hpp"
#include "includes/list.hpp"
#include "includes/map.hpp"
#include "includes/set.hpp"
#include "includes/unordered_map.hpp"
#include "includes/unordered_set.hpp"
#include "includes/stack.hpp"
#include "includes/queue.hpp"
#include "includes/flat_basket.hpp"
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

int main()
{
	std::multimap<std::string, std::string> m1;
	std::multimap<std::string, std::string>::iterator it;
	it = m1.insert(std::make_pair("", ""));
	m1.insert(it, std::make_pair("", ""));

	for (std::multimap<std::string, std::string>::const_reverse_iterator cit = m1.rbegin(); cit != m1.rend(); ++cit)
	{
		std::cout << "pouet " << cit->first << std::endl;
	}
	
	return 0;
}