#include <iostream>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>

#if FT == 1
	#include "../includes/vector.hpp"
	#include "../includes/set.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <vector>
	#include <set>
	#define FT 0
	using namespace std;
#endif

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

template < typename T >
void	displayData(const T& container, std::ostream& stream, typename T::size_type eltsPerLine)
{
	typename T::const_iterator	it;
	typename T::const_iterator	end;
	typename T::size_type		eltCount;

	if (eltsPerLine > container.size())
		eltsPerLine = container.size();
	it = container.begin();
	end = container.end();
	stream << "*****content*****" << '\n';
	eltCount = 0;
	while (it != end)
	{
		if (eltCount == eltsPerLine)
		{
			eltCount = 0;
			stream << '\n';
		}
		if (eltCount)
			stream << " | ";
		stream << *it;
		it++;
		++eltCount;
	}
	stream << "\n\n";
}

template < typename T >
void	displayInfo(const T& container, const char* name, std::ostream& stream, typename T::size_type eltsPerLine = 1)
{
	stream << "----------> " << name << " <---------\n";
	stream << "size " << " : " << container.size() << '\n';
	stream << "capacity " << " : " << container.capacity() << '\n';
	stream << "max_size " << " : " << container.max_size() << "\n";
	if (eltsPerLine)
		displayData(container, stream, eltsPerLine);
	else
		stream << '\n';
}

void	vector_tests(const std::string& currentPath)
{
	std::string fileName;
	set<std::string> toolForRangeFill;
	std::ofstream outfile;
	
	toolForRangeFill.insert("Jhon");
	toolForRangeFill.insert("Clem");
	toolForRangeFill.insert("");
	toolForRangeFill.insert("Alexi");
	toolForRangeFill.insert("Alexis");
	toolForRangeFill.insert("Diane");

	const set<std::string> constRange(toolForRangeFill);
	
	//CONSTRUCTOR ASSERTS
	{
		fileName = currentPath + "constructors.log";
		outfile.open(fileName.c_str(), std::ios_base::out);


		vector<std::string> empty;
		displayInfo(empty, "default constructor", outfile);

		const vector<std::string> filled(12, "canard");
		displayInfo(filled, "filled constructor", outfile, 5);

		const vector<std::string> ranged(toolForRangeFill.begin(), toolForRangeFill.end());
		displayInfo(ranged, "range constructor with another container type", outfile);

		vector<std::string> ranged2(constRange.begin(), constRange.end());
		displayInfo(ranged2, "range constructor with a const another container type", outfile);

		vector<std::string> ranged3(filled.begin(), filled.end());
		displayInfo(ranged3, "range constructor with a const vector", outfile, 5);

		vector<std::string> copy(ranged);
		displayInfo(copy, "copy constructor with a const vector", outfile);

		copy = filled;
		displayInfo(copy, "operator =", outfile, 5);

		vector<int> vInt1(4, 4);
		displayInfo(vInt1, "fill constructor on vector of int", outfile, 4);
		vector<int> vInt2(vInt1.begin(), vInt1.end());
		displayInfo(vInt2, "range constructor on vector of int", outfile, 4);
		
		outfile << "construtor with too much elements\nEXCEPT : ";
		try
		{
			vector<int> big(std::allocator<int>().max_size() + 1, 1);
			displayInfo(big, "fill constructor with max_size + 1 elements", outfile, 0);
		}
		catch(length_error& e)
		{
			outfile << e.what() << "\n\n";
		}

		try
		{
			vector<int> lessBig(15000000, 1);
			displayInfo(lessBig, "fill constructor with 15000000 elements", outfile, 0);
		}
		catch(std::exception& e)
		{
			outfile << e.what() << '\n';
			std::cerr << "an abort during memory allocation causes memory issues, you have to relog to catch back lost memory.\n";
			exit(errno);
		}
		outfile.close();
	}

	//INSERT ASSERTS

	{	
		fileName = currentPath + "insert.log";
		outfile.open(fileName.c_str(), std::ios_base::out);

		const vector<std::string>	empty;
		vector<std::string>			v1;

		v1.insert(v1.begin(), "begin");
		v1.insert(v1.begin() + 1, "begin + 1");
		v1.insert(v1.end(), "end");
		v1.insert(v1.end() - 1, "end - 1");

		displayInfo(v1, "one element insert", outfile);

		vector<std::string>	v2;

		v2.insert(v2.begin(), empty.begin(), empty.end());
		v2.insert(v2.begin(), v1.begin(), v1.begin());

		displayInfo(v2, "insert nothing in empty vector", outfile);

		v1.insert(v1.begin() + 1, 15, "fillElt");
		displayInfo(v1, "fill insert some elements", outfile, 6);

		outfile << "try inserting too much elements\nEXCEPT : ";
		try
		{
			v2.insert(v2.begin(), v2.max_size() + 1, "too much elt");
		}
		catch(length_error& e)
		{
			outfile << e.what() << "\n\n";
		}

		v2.insert(v2.begin(), 0, "saucisse");
		displayInfo(v2, "insert nothing on nothing container", outfile, 0);

		v2.insert(v2.begin(), 15, "poule");
		displayInfo(v2, "fill insert on empty vector", outfile, 5);

		v2.insert(v2.end() - v2.size() / 2, toolForRangeFill.begin(), toolForRangeFill.end());
		displayInfo(v2, "range insert with another container type", outfile, 5);

		vector<int> vInt1;
		vector<int> vInt2(50, 5);

		vInt1.insert(vInt1.begin(), vInt2.begin() + vInt2.size() / 4, vInt2.end() - vInt2.size() / 4);
		displayInfo(vInt1, "range insert with int vectors", outfile, 10);
		outfile.close();
	}

	// CLEAR

	{
		fileName = currentPath + "clear.log";
		outfile.open(fileName.c_str(), std::ios_base::out);

		vector<std::string> empty;
		vector<std::string> filled(30000, "hello");

		empty.clear();
		filled.clear();

		displayInfo(empty, "clear an empty vector", outfile);
		displayInfo(filled, "clear a big filled vector", outfile);

		outfile.close();
	}

	// ASSIGN

	{
		fileName = currentPath + "assign.log";
		outfile.open(fileName.c_str(), std::ios_base::out);

		vector<std::string>	v1(14, "Vim a vie");
		vector<std::string> empty;
		vector<std::string> v2;

		vector<int> vInt1;
		vector<int> vInt2(8, 8);
		
		v2.assign(v1.begin(), v1.end());
		displayInfo(v2, "range assign an empty not allocated vector", outfile, 4);
		v1.assign(empty.begin(), empty.end());
		displayInfo(v1, "range assign a filled vector with nothing", outfile);

		v2.assign(4, "pouet");
		displayInfo(v2, "fill assign a filled vector", outfile, 4);

		v2.assign(0, "pouet");
		displayInfo(v2, "fill assign nothing a filled vector", outfile, 4);

		v2.assign(4, "pouet");
		displayInfo(v2, "fill assign an empty vector", outfile, 4);

		empty.assign(4, "pouet");
		displayInfo(empty, "fill assign an empty not allocated vector", outfile, 4);

		outfile << "try assign too much elements\nEXCEPT : ";
		try
		{
			empty.assign(empty.max_size() + 1, "lol");
		}
		catch(length_error& e)
		{
			outfile << e.what() << "\n\n";
		}
		
		displayInfo(empty, "check a filled vector after a length_error exception on assign", outfile, 4);

		vInt1.assign(12, 7);
		displayInfo(vInt1, "fill assign on vector of ints", outfile, 6);

		vInt1.assign(vInt2.begin(), vInt2.end());
		displayInfo(vInt1, "range assign on vector of ints", outfile, 4);

		outfile.close();
	}

	//PUSH_BACK

	{
		fileName = currentPath + "push_back.log";
		outfile.open(fileName.c_str(), std::ios_base::out);

		vector<std::string> v;
		v.push_back("coucou");

		displayInfo(v, "push_back 1 time on empty vector", outfile, 10);

		for (int i = 0; i < 20; ++i)
			v.push_back(itoa(i));
		displayInfo(v, "push_back 20 times", outfile, 10);

		outfile.close();
	}

	//ERASE

	{
		fileName = currentPath + "erase.log";
		outfile.open(fileName.c_str(), std::ios_base::out);

		vector<std::string>	v;

		v.erase(v.begin(), v.end());
		displayInfo(v, "range erase begin to end on empty vector", outfile, 10);

		for (int i = 0; i < 20; ++i)
			v.push_back(itoa(i));

		v.erase(v.begin() + v.size() / 3);
		displayInfo(v, "erase on middle iterator", outfile, 10);

		v.erase(v.begin());
		displayInfo(v, "erase on begin() iterator", outfile, 10);

		v.erase(v.end() - 1);
		displayInfo(v, "erase on end() - 1 iterator", outfile, 10);

		v.erase(v.begin() + 7, v.begin() + 11);
		displayInfo(v, "range erase", outfile, 7);

		v.erase(v.begin(), v.end());
		displayInfo(v, "range erase all data", outfile);

		outfile.close();
	}

	//POP_BACK

	{
		fileName = currentPath + "pop_back.log";
		outfile.open(fileName.c_str(), std::ios_base::out);

		vector<std::string> v;

		for (int i = 0; i < 20; ++i)
			v.push_back(itoa(i));

		v.pop_back();
		displayInfo(v, "pop_back() one time a filled vector", outfile, 10);

		for (int i = 0; i < 5; ++i)
			v.pop_back();
		
		displayInfo(v, "pop_back() 5 times a filled vector", outfile, 10);
		outfile.close();
	}

	//ACCESS

	{
		fileName = currentPath + "access.log";

		outfile.open(fileName.c_str(), std::ios_base::out);
		vector<std::string> v_str;
		for (int i = 0; i < 20; ++i)
			v_str.push_back(itoa(i));

		const vector<std::string> const_v_str(v_str);
		std::string& str = v_str[7];
		outfile << "with [], take a non const reference from a vector : " << str << '\n';
		const std::string& const_str2 = v_str[0];
		outfile << "with [], take a const reference from a vector : " << const_str2 << '\n';
		const std::string& const_str3 = const_v_str[0];
		outfile << "with [], take a const reference from a const vector : " << const_str3 << '\n';
		
		std::string& str2 = v_str.at(12);
		outfile << "with at(), take a non const reference from a vector : " << str2 << '\n';
		const std::string& const_str4 = v_str.at(3);
		outfile << "with at(), take a const reference from a vector : " << const_str4 << '\n';
		const std::string& const_str5 = const_v_str.at(3);
		outfile << "with at(), take a const reference from a const vector : " << const_str5 << '\n';

		v_str[4] = "I was 4";
		displayInfo(v_str, "change the 4th element with [] operator", outfile, 10);

		v_str.at(18) = "I was 18";
		displayInfo(v_str, "change the 18th element with at()", outfile, 10);

		outfile << "provoque an exception with an out of range element with at().\nEXCEPT : ";
		try
		{
			v_str.at(25);
		}
		catch(out_of_range& e)
		{
			outfile << e.what() << "\n\n";
		}
		
		outfile.close();
	}

	//ITERATOR

	{
		typedef vector<int>::iterator Tt;
		typedef vector<int>::const_iterator Cit;

		fileName = currentPath + "iterator.log";
		outfile.open(fileName.c_str());

		vector<std::string> v;
		
		for (int i = 0; i < 20; ++i)
		{
			v.push_back(itoa(i));
		}

		const vector<int> const_v(v);
		Cit cit = const_v.begin();
		outfile << "const_iterator tests\n";
		outfile << *cit << " | size = " << cit->size() << '\n';
		outfile << *++cit << " must be equal to " << *cit++ << '\n';
		outfile << *(cit + 2) << " | " << *(2 + cit) << '\n';
		outfile << *--cit << " must be equal to " << *cit-- << '\n';
		outfile << *(cit - 2) << '\n';
	}
}