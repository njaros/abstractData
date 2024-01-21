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
	ft::set<std::string> sentence;
	sentence.insert("World");
	sentence.insert("Hello ");

	for (ft::set<std::string>::const_iterator it = sentence.begin(); it != sentence.end(); ++it)
		std::cout << *it;
	std::cout << '\n';
	return 0;
}