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

void deque_tests(const std::string& currentPath, std::ostream& except)
{
	typedef deque<short> S;
	typedef deque<std::string> Strs;

	std::string fileName;
	std::ofstream outfile;
	std::boolalpha(outfile);

	set<std::string> filler;
	set<int> fillerInt;
	for (int i = 0; i < 25; ++i)
	{
		filler.insert(myItoa(i));
		fillerInt.insert(i * i / 2 + i);
	}


	//CONSTRUCTORS

	{
		fileName = currentPath + "constructors.log";
		outfile.open(fileName.c_str());

		Strs e;
		Strs fill(37, "patates");
		const Strs rangeA(filler.begin(), filler.end());
		Strs rangeB(fill.begin(), fill.end());
		Strs cpyA(rangeA);
		const Strs cpyB(e);

		displayV2(e, "empty constructor", outfile);
		displayV2(fill, "fill constructor", outfile, 10);
		displayV2(rangeA, "const range constructor with set iterators", outfile, 10);
		displayV2(rangeB, "range constructor with deque iterators", outfile, 10);
		displayV2(cpyA, "filled copy constructor", outfile, 10);
		displayV2(cpyB, "const empty copy constructor", outfile);

		cpyA = fill;
		displayV2(cpyA, "operator=() normal use ", outfile, 20);

		cpyA = cpyB;
		displayV2(cpyA, "operator=() with empty one", outfile);

		cpyA = cpyB;
		displayV2(cpyA, "same thing", outfile);

		cpyA = rangeA;
		cpyA = rangeA;
		displayV2(cpyA, "2 time operator=() with filled deque", outfile, 20);

		try
		{
			Strs tooMuch(Strs::allocator_type().max_size() + 1, "lol");
		}
		catch(const length_error& e)
		{
			except << "DEQUE : too much elements on fill constructor\n" << e.what() << '\n';
		}

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

		Strs str(3, "pouet");
		str.push_back("banana");

		outfile << str.begin()->length() << '\n';
		outfile << str.rbegin()->length() << '\n';
		outfile << (++str.begin())->length() << '\n';
		outfile << (++str.rbegin())->length() << '\n';
		outfile << (--str.end())->length() << '\n';
		outfile << (--str.rend())->length() << '\n';

		const Strs str2(str);

		outfile << str2.begin()->length() << '\n';
		outfile << str2.rbegin()->length() << '\n';
		outfile << (++str2.begin())->length() << '\n';
		outfile << (++str2.rbegin())->length() << '\n';
		outfile << (--str2.end())->length() << '\n';
		outfile << (--str2.rend())->length() << '\n';

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
		fileName = currentPath + "insert.log";
		outfile.open(fileName.c_str());

		//RANDOM INSERTS - deque inserts can have too many cases of failure, that's why I choose to do those random inserts
		typedef deque<char> D;
		size_t seed = 0;
		D::size_type pos;
		D::size_type len;
		D::size_type debug;
		multiset<D::const_iterator> rangePos;
		const D::size_type maxRangeInsert = 17;

		D d1;
		D d2(5, 6);
		D d3(fillerInt.begin(), ++(fillerInt.begin()));

		for (int c = 0; c < 127; ++c)
		{
			len = getRandom(seed, maxRangeInsert);

			pos = getRandom(seed, d1.size());
			d1.insert(d1.begin() + pos, len, c);
			pos = getRandom(seed, d1.size());
			d1.insert(d1.begin() + pos, c);

			pos = getRandom(seed, d2.size());
			d2.insert(d2.begin() + pos, len, c);
			pos = getRandom(seed, d2.size());
			d2.insert(d2.begin() + pos, c);

			debug = getRandom(seed, d2.size());
			rangePos.insert(d2.begin() + debug);
			debug = getRandom(seed, d2.size());
			rangePos.insert(d2.begin() + debug);
			pos = getRandom(seed, d3.size());
			d3.insert(d3.begin() + pos, *(rangePos.begin()), *(++rangePos.begin()));
			rangePos.clear();
		}

		displayV2(d1, "insert on an initially empty deque", outfile, 0);
		displayDataCastInt(d1, "", outfile, 50);

		displayV2(d2, "insert on an initially filled deque", outfile, 0);
		displayDataCastInt(d2, "", outfile, 50);

		displayV2(d3, "range insert on an initially filled deque", outfile, 0);
		displayDataCastInt(d3, "", outfile, 50);

		//SAME WITH BIGGER DATA WHERE CHUNKSIZE == 1

		size_t seedS = 0;
		Strs::size_type posS;
		Strs::size_type lenS;
		multiset<Strs::const_iterator> rangePosS;
		const Strs::size_type maxRangeInsertS = 2;

		Strs s1;
		Strs s2(5, "fill");
		Strs s3(filler.begin(), ++(filler.begin()));

		try
		{
			for (int c = 0; c < 38; ++c)
			{
				lenS = getRandom(seedS, maxRangeInsertS);

				posS = getRandom(seedS, s1.size());
				s1.insert(s1.begin() + posS, lenS, myItoa(c));
				posS = getRandom(seedS, s1.size());
				s1.insert(s1.begin() + posS, myItoa(c));

				posS = getRandom(seedS, s2.size());
				s2.insert(s2.begin() + posS, lenS, myItoa(c));
				posS = getRandom(seedS, s2.size());
				s2.insert(s2.begin() + posS, myItoa(c));

				rangePosS.insert(s2.begin() + getRandom(seedS, s2.size()));
				rangePosS.insert(s2.begin() + getRandom(seedS, s2.size()));

				posS = getRandom(seedS, s3.size());
				s3.insert(s3.begin() + posS, *(rangePosS.begin()), *(++rangePosS.begin()));
				rangePosS.clear();
			}
		}
		catch(const length_error& e)
		{
			except << "DEQUE : range insert with bad iterators. (rangePosS received two same value_type) \n" << e.what() << '\n';
		}

		displayV2(s1, "insert on an initially empty deque", outfile, 50);

		displayV2(s2, "insert on an initially filled deque", outfile, 50);

		displayV2(s3, "range insert on an initially filled deque", outfile, 50);

		//PARTICULAR INSERTS

		D p1(1, 'a');
		p1.insert(p1.begin(), 'b');

		displayV2(p1, "insert before begin on deque initialized with 1 char", outfile, 2);

		D p2(16, 'a');
		D p3(16, 'a');
		p2.insert(p2.end(), 'b');
		displayV2(p2, "insert at end on deque initialized with one fulled chunk", outfile, 20);

		p3.insert(p3.begin(), 'b');
		displayV2(p3, "insert at begin on deque initialized with one fulled chunk", outfile, 20);
		
		outfile.close();
	}

	//ASSIGN
	
	{
		fileName = currentPath + "assign.log";
		outfile.open(fileName.c_str());

		S s1(150, 42);
		S s2(fillerInt.begin(), fillerInt.end());
		S tmp;

		tmp = s1;
		s1.assign(s2.begin(), s2.end());

		s2.assign(tmp.begin(), tmp.end());

		displayV2(tmp, "assign on an empty one", outfile, 21);

		displayV2(s1, "normal assign", outfile, 15);
		tmp.assign(0, 4);

		displayV2(tmp, "assign nothing", outfile);

		try
		{
			tmp.assign(s1.end(), s1.begin());
		}
		catch(const length_error& e)
		{
			except << "DEQUE : assign : bad use of iterators\n" << e.what() << '\n';
		}

		outfile.close();
	}

	//PUSH_POP

	{
		fileName = currentPath + "push_pop.log";
		outfile.open(fileName.c_str());

		S s1(1, 1);

		for (int i = 0; i < 500000; ++i)
		{
			s1.push_front(2);
			s1.pop_back();
		}

		displayV2(s1, "push_front() + pop_back()", outfile);
		
		for (int i = 0; i < 500000; ++i)
		{
			s1.push_front(2);
		}

		displayV2(s1, "only push_front()", outfile, 0);

		S s2(1, 1);

		for (int i = 0 ; i < 500000; ++i)
		{
			s2.push_back(2);
			s2.pop_back();
		}

		displayV2(s2, "push_back() + pop_back()", outfile);

		for (int i = 0; i < 500000; ++i)
		{
			s2.push_back(2);
		}

		displayV2(s2, "only push_back()", outfile, 0);

		S s3(1, 1);

		for (int i = 0 ; i < 500000; ++i)
		{
			s3.push_back(2);
			s3.pop_front();
		}

		displayV2(s3, "push_back() + pop_front()", outfile);

		S s4(1, 1);

		for (int i = 0 ; i < 500000; ++i)
		{
			s4.push_front(2);
			s4.pop_front();
		}

		displayV2(s4, "push_front() + pop_front()", outfile);

		S e1;

		e1.push_back(1);
		displayV2(e1, "push_back() on empty one", outfile);

		S e2;

		e2.push_front(1);
		displayV2(e2, "push_front() on empty one", outfile);

		outfile.close();
	}

	//ACCESS

	{
		fileName = currentPath + "access.log";
		outfile.open(fileName.c_str());

		S s1;
		for (short s = 0; s < 20; ++s)
		{
			s1.push_back(s);
		}
		
		const S s2(s1);

		outfile << s1[2] << " | " << s2[19] << '\n';

		s1.front() = 55;
		s1.back() = -55;
		outfile << s1.front() << " | " << s1.back() << '\n';
		outfile << s2.front() << " | " << s2.back() << '\n';

		s1.at(4) = 1000;
		s1.at(0) = 88;
		outfile << s1.at(4) << " | " << s1.at(19) << " | " << s1.at(0);
		outfile << s2.at(4) << " | " << s2.at(19) << " | " << s2.at(0);

		try
		{
			s1.at(20);
		}
		catch(const out_of_range& e)
		{
			except << "DEQUE : at() out of range.\n" << e.what() << '\n';
		}
		
		outfile.close();
	}

	//ERASE

	{
		fileName = currentPath + "erase.log";
		outfile.open(fileName.c_str());

		S s1;

		for (short s = 0; s < 32; ++s)
			s1.push_front(s);

		S s2(s1);

		s1.erase(s1.begin() + 5, s1.begin() + 13);
		displayV2(s1, "erase on middle of chunk a size of chunk", outfile, 8);

		s1 = s2;

		s1.erase(s1.begin());
		displayV2(s1, "erase begin", outfile, 8);

		s1.erase(s1.end() - 1);
		displayV2(s1, "erase end - 1", outfile, 8);

		s1 = s2;

		s1.erase(s1.begin() + 4);
		displayV2(s1, "erase somewhere", outfile, 8);

		s1.erase(s1.begin() + 2, s1.end() - 4);
		displayV2(s1, "erasing a lot", outfile, 8);

		s1 = s2;

		s1.erase(s1.begin() + 8, s1.begin() + 16);
		displayV2(s1, "erase on begin on a chunk a size of chunk", outfile, 8);

		s1 = s2;

		s1.erase(s1.begin() + 8, s1.begin() + 24);
		displayV2(s1, "erase on begin on a chunk a size of 2 chunks", outfile, 8);

		s1 = s2;

		s1.erase(s1.begin() + 6, s1.begin() + 22);
		displayV2(s1, "erase on middle on a chunk a size of 2 chunks", outfile, 8);

		while (!s2.empty())
		{
			s2.erase(s2.begin());
		}

		displayV2(s2, "erase begin until empty", outfile);

		outfile.close();
	}

	//CLEAR

	{
		fileName = currentPath + "clear.log";
		outfile.open(fileName.c_str());

		Strs str1(16, "coucou");
		str1.clear();

		S s1(45, 1);
		s1.clear();

		S s2;
		s2.clear();

		displayV2(s2, "clear empty one", outfile);

		displayV2(str1, "clear filled deque of strings", outfile);
		displayV2(s1, "clear filled deque of shorts", outfile);

		str1.insert(str1.begin(), 3, "still working ?");

		displayV2(str1, "test after clear", outfile, 3);

		s1.insert(s1.begin(), 3, 3);

		displayV2(s1, "same for shorts", outfile, 3);

		outfile.close();
	}

	//SWAP

	{
		fileName = currentPath + "swap.log";
		outfile.open(fileName.c_str());

		Strs s1;
		for (int i = 0; i < 50; ++i)
			s1.push_back(myItoa(i));
		Strs s2(4, "pouet");

		s1.swap(s2);
		displayV2(s1, "swap", outfile, 25);
		swap(s1, s2);
		displayV2(s1, "swap", outfile, 25);

		outfile.close();
	}

	//GET_ALLOCATOR

	{
		//not implemented because std::deque doesn't support dummyAllocator
	}

	//RELATIONAL

	{
		typedef deque<std::string> V;
		fileName = currentPath + "relational.log";
		outfile.open(fileName.c_str());
		const V c1(4, "fleur");
		V v4(c1);
		V v1(5, "fleur");
		V v2(5, "fleurs");
		V v3(v1);
		v3.resize(7, "fleur");

		outfile << (c1 == v4) << '|' << (c1 == v1) << '|' << (v1 == v2) << '|' << (v3 == v1) << '|' << (v3 == v2) << '\n';
		outfile << (c1 != v4) << '|' << (c1 != v1) << '|' << (v1 != v2) << '|' << (v3 != v1) << '|' << (v3 != v2) << '\n';
		outfile << (c1 < v4) << '|' << (c1 < v1) << '|' << (v1 < v2) << '|' << (v3 < v1) << '|' << (v3 < v2) << '\n';
		outfile << (c1 <= v4) << '|' << (c1 <= v1) << '|' << (v1 <= v2) << '|' << (v3 <= v1) << '|' << (v3 <= v2) << '\n';
		outfile << (c1 > v4) << '|' << (c1 > v1) << '|' << (v1 > v2) << '|' << (v3 > v1) << '|' << (v3 > v2) << '\n';
		outfile << (c1 >= v4) << '|' << (c1 >= v1) << '|' << (v1 >= v2) << '|' << (v3 >= v1) << '|' << (v3 >= v2) << '\n';

		outfile.close();
	}

}
