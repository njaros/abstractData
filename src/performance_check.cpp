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

	{
		std::cout << "\n====MAP====\n\n";
		std::map<int, int> stdm;
		ft::map<int, int> ftm;
		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; ++i)
			stdm[i] = i;
		displayTime(t, 0, "insert 10000000 ints");

		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; ++i)
			ftm[i] = i;
		displayTime(t, 1, "insert 10000000 ints");

		gettimeofday(&t, 0);
		for (int i = 2500000; i < 7500000; ++i)
			stdm.erase(i);
		displayTime(t, 0, "erase 5000000 ints");

		gettimeofday(&t, 0);
		for (int i = 2500000; i < 7500000; ++i)
			ftm.erase(i);
		displayTime(t, 1, "erase 5000000 ints");
	}

	{
		std::cout << "\n====VECTOR====\n\n";
		std::vector<std::string> stdv;
		ft::vector<std::string> ftv;

		gettimeofday(&t, 0);
		for (int i = 0; i < 500000; ++i)
			stdv.push_back("hello world");
		displayTime(t, 0, "vector push_back 500 000 \"hello world\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 500000; ++i)
			ftv.push_back("hello world");
		displayTime(t, 1, "vector push_back 500 000 \"hello world\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 500; ++i)
			stdv.insert(stdv.begin(), "hello world");
		displayTime(t, 0, "vector insert begin 500 \"hello world\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 500; ++i)
			ftv.insert(ftv.begin(), "hello world");
		displayTime(t, 1, "vector insert begin 500 \"hello world\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 500; ++i)
			stdv.erase(stdv.begin() + 250250 - (i / 2));
		displayTime(t, 0, "vector erase in middle 500 \"hello world\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 500; ++i)
			ftv.erase(ftv.begin() + 250250 - (i / 2));
		displayTime(t, 1, "vector erase in middle 500 \"hello world\"");
	}

	{
		std::cout << "\n====DEQUE====\n\n";
		std::deque<std::string> stdd;
		ft::deque<std::string> ftd;

		gettimeofday(&t, 0);
		for (int i = 0; i < 5000000; ++i)
			stdd.push_back("hello world");
		displayTime(t, 0, "push_back 5 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 5000000; ++i)
			ftd.push_back("hello world");
		displayTime(t, 1, "push_back 5 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 5000000; ++i)
			stdd.push_front("hello world");
		displayTime(t, 0, "push_front 5 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 5000000; ++i)
			ftd.push_front("hello world");
		displayTime(t, 1, "push_front 5 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 5000000; ++i)
			stdd.pop_back();
		displayTime(t, 0, "pop_back 5 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 5000000; ++i)
			ftd.pop_back();
		displayTime(t, 1, "pop_back 5 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 5000000; ++i)
			stdd.pop_front();
		displayTime(t, 0, "pop_front 5 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 5000000; ++i)
			ftd.pop_front();
		displayTime(t, 1, "pop_front 5 000 000 \"hello worlds\"");
	}

	{
		std::cout << "\n====LIST====\n\n";
		std::list<std::string> stdl;
		ft::list<std::string> ftl;
		std::list<std::string>::iterator stdIt;
		ft::list<std::string>::iterator ftIt;

		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; ++i)
			stdl.push_back("hello world");
		displayTime(t, 0, "push_back 10 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; ++i)
			ftl.push_back("hello world");
		displayTime(t, 1, "push_back 10 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; ++i)
			stdl.push_front("hello world");
		displayTime(t, 0, "push_front 10 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; ++i)
			ftl.push_front("hello world");
		displayTime(t, 1, "push_front 10 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		stdIt = stdl.begin();
		for (int i = 1; i < 5000000; ++i)
			++stdIt;
		for (int i = 0; i < 10000000; ++i)
			stdl.insert(++stdIt ,"hello world");
		displayTime(t, 0, "insert at middle 10 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		ftIt = ftl.begin();
		for (int i = 1; i < 5000000; ++i)
			++ftIt;
		for (int i = 0; i < 10000000; ++i)
			ftIt = ftl.insert(++ftIt ,"hello world");
		displayTime(t, 1, "insert at middle 10 000 000 \"hello worlds\"");

		gettimeofday(&t, 0);
		stdl.sort();
		displayTime(t, 0, "sort 30 000 000 elements");
		
		gettimeofday(&t, 0);
		ftl.sort();
		displayTime(t, 1, "sort 30 000 000 elements");
		
	}

	{
		std::cout << "\n====PRIORITY_QUEUE====\n\n";
		std::priority_queue<int> stdp;
		ft::priority_queue<int> ftp;
		int top;

		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; ++i)
		{
			stdp.push(i);
			top = stdp.top();
			stdp.push(-i);
			top = stdp.top();
		}
		displayTime(t, 0, "push 20 000 000 elements");

		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; ++i)
		{
			ftp.push(i);
			top = ftp.top();
			ftp.push(-i);
			top = ftp.top();
		}
		displayTime(t, 1, "push 20 000 000 elements");

		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; ++i)
		{
			stdp.pop();
			top = stdp.top();
		}
		displayTime(t, 0, "pop 10 000 000 times");

		gettimeofday(&t, 0);
		for (int i = 0; i < 10000000; ++i)
		{
			ftp.pop();
			top = ftp.top();
		}
		displayTime(t, 1, "pop 10 000 000 times");

	}

	return 0;
}