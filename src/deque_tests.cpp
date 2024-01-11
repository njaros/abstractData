#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include "../includes/test_elements.hpp"

#if FT == 1
	#include "../includes/deque.hpp"
	#include "../includes/set.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <deque>
	#include <set>
	#define FT 0
	using namespace std;
#endif

void deque_tests(const std::string& currentPath)
{
	std::string fileName;
	std::ofstream outfile;
	std::boolalpha(outfile);

	set<std::string> filler;
	for (int i = 0; i < 25; ++i)
		filler.insert(itoa(i));

	//CONSTRUCTORS

	{
		typedef deque<std::string> D;
		fileName = currentPath + "constructors.log";
		outfile.open(fileName.c_str());

		D e;
		D fill(37, "patates");
		const D rangeA(filler.begin(), filler.end());
		D rangeB(fill.begin(), fill.end());
		D cpyA(rangeA);
		const D cpyB(e);

		displayV2(e, "empty constructor", outfile);
		displayV2(fill, "fill constructor", outfile, 10);
		displayV2(rangeA, "const range constructor with set iterators", outfile, 10);
		displayV2(rangeB, "range constructor with deque iterators", outfile, 10);
		displayV2(cpyA, "filled copy constructor", outfile, 10);
		displayV2(cpyB, "const empty copy constructor", outfile);

		outfile.close();
	}

	//ITERATORS

	{
		fileName = currentPath + "iterator.log";
		outfile.open(fileName.c_str());
		
		randomIteratorTests<deque<std::string> >(outfile);
		randomIteratorTestsForDeque<deque<char> >(outfile);
		ReverseRandomIteratorTests<deque<std::string> >(outfile);
		ReverseRandomIteratorTestsForDeque<deque<char> >(outfile);

		outfile.close();
	}

	//CAPACITY

	{
		fileName = currentPath + "capacity.log";
		outfile.open(fileName.c_str());

		deque<int> e1;
		deque<int> e2;
		deque<int> e3;
		deque<int> e4;
		const deque<int> ec;
		const deque<int> c(12, 12);
		deque<int> filled(c);

		outfile << "non modifiers methods\n\n";
		outfile << "empty of empty deque is " << e1.empty() << '\n';
		outfile << "empty of empty const deque is " << ec.empty() << '\n';
		outfile << "empty of filled const deque is " << c.empty() << '\n';
		outfile << "empty of filled deque is " << filled.empty() << '\n';

		outfile << "size of empty deque is " << e1.size() << '\n';
		outfile << "size of empty const deque is " << ec.size() << '\n';
		outfile << "size of filled const deque is " << c.size() << '\n';
		outfile << "size of filled deque is " << filled.size() << '\n';

		outfile << "max_size of empty deque is " << e1.max_size() << '\n';
		outfile << "max_size of empty const deque is " << ec.max_size() << '\n';
		outfile << "max_size of filled const deque is " << c.max_size() << '\n';
		outfile << "max_size of filled deque is " << filled.max_size() << '\n';

		outfile << "\nmodifier methods\n";
		outfile << "\nempty deque with 0 capacity tests\n\n";
		e2.resize(30);
		displayV2(e2, "after resise with default value_type", outfile, 10);
		e3.resize(30, 3);
		displayV2(e3, "after resise with value_type 3", outfile, 10);
		e4.resize(0, 4);
		displayV2(e4, "after resise 0", outfile, 10);
		e4.resize(0);
		displayV2(e4, "after resise 0 with default value_type", outfile, 10);

		outfile << "deque with some elements tests\n\n";
		e3.resize(51);
		displayV2(e3, "after resize more than size with defaut value_type", outfile, 21);
		e3.resize(60, 4);
		displayV2(e3, "after resize more than size", outfile, 21);
		e3.resize(35);
		displayV2(e3, "after resize less then size with default value_type", outfile, 20);
		e3.resize(12, 785);
		displayV2(e3, "after resize less then size", outfile, 20);
		e3.resize(0);
		displayV2(e3, "after resize 0", outfile, 20);

		outfile.close();
	}

	//INSERTS

	{
		typedef deque<char> D;
		int seed = 482346873;

		for (char c = '0'; c < 'z'; ++c)
			
	}

}
