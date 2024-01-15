#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <string>
#include "../includes/test_elements.hpp"

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


void	vector_tests(const std::string& currentPath, std::ostream& except)
{
	std::string fileName;
	set<std::string> toolForRangeFill;
	std::ofstream outfile;
	
	std::boolalpha(outfile);
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
		
		try
		{
			vector<int> big(std::allocator<int>().max_size() + 1, 1);
			displayInfo(big, "fill constructor with max_size + 1 elements", outfile, 0);
		}
		catch(length_error& e)
		{
			except << "VECTOR : construtor with too much elements\n" << e.what() << '\n';
		}

		vector<int> lessBig(15000000, 1);
		displayInfo(lessBig, "fill constructor with 15000000 elements", outfile, 0);

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

		try
		{
			v2.insert(v2.begin(), v2.max_size() + 1, "too much elt");
		}
		catch(length_error& e)
		{
			except << "VECTOR: try inserting too much elements\n" << e.what() << '\n';
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
			except << "VECTOR : try assign too much elements\n" << e.what() << '\n';
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
			v.push_back(myItoa(i));
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
			v.push_back(myItoa(i));

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
			v.push_back(myItoa(i));

		v.pop_back();
		displayInfo(v, "pop_back() one time a filled vector", outfile, 10);

		for (int i = 0; i < 5; ++i)
			v.pop_back();
		
		displayInfo(v, "pop_back() 5 times a filled vector", outfile, 10);
		outfile.close();
	}

	//CAPACITY

	{
		fileName = currentPath + "capacity.log";
		outfile.open(fileName.c_str());

		vector<int> e1;
		vector<int> e2;
		vector<int> e3;
		vector<int> e4;
		const vector<int> ec;
		const vector<int> c(12, 12);
		vector<int> filled(c);

		outfile << "non modifiers methods\n\n";
		outfile << "empty of empty vector is " << e1.empty() << '\n';
		outfile << "empty of empty const vector is " << ec.empty() << '\n';
		outfile << "empty of filled const vector is " << c.empty() << '\n';
		outfile << "empty of filled vector is " << filled.empty() << '\n';

		outfile << "size of empty vector is " << e1.size() << '\n';
		outfile << "size of empty const vector is " << ec.size() << '\n';
		outfile << "size of filled const vector is " << c.size() << '\n';
		outfile << "size of filled vector is " << filled.size() << '\n';

		outfile << "max_size of empty vector is " << e1.max_size() << '\n';
		outfile << "max_size of empty const vector is " << ec.max_size() << '\n';
		outfile << "max_size of filled const vector is " << c.max_size() << '\n';
		outfile << "max_size of filled vector is " << filled.max_size() << '\n';

		outfile << "capacity of empty vector is " << e1.capacity() << '\n';
		outfile << "capacity of empty const vector is " << ec.capacity() << '\n';
		outfile << "capacity of filled const vector is " << c.capacity() << '\n';
		outfile << "capacity of filled vector is " << filled.capacity() << '\n';

		outfile << "\nmodifier methods\n";
		outfile << "\nempty vector with 0 capacity tests\n\n";
		displayInfo(e1, "before reserve", outfile);
		e1.reserve(450);
		displayInfo(e1, "after reserve", outfile);
		e2.resize(30);
		displayInfo(e2, "after resise with default value_type", outfile, 10);
		e3.resize(30, 3);
		displayInfo(e3, "after resise with value_type 3", outfile, 10);
		e4.reserve(0);
		displayInfo(e4, "after reserve 0", outfile, 10);
		e4.resize(0, 4);
		displayInfo(e4, "after resise 0", outfile, 10);
		e4.resize(0);
		displayInfo(e4, "after resise 0 with default value_type", outfile, 10);

		outfile << "empty vector with some capacity tests\n\n";
		e1.reserve(550);
		displayInfo(e1, "after reserve more", outfile);
		e1.reserve(450);
		displayInfo(e1, "after reserve less", outfile);
		e1.resize(30);
		displayInfo(e1, "after resise with default value_type", outfile, 10);
		e4.reserve(450);
		e4.resize(30, 3);
		displayInfo(e4, "after resise with value_type 3", outfile, 10);

		outfile << "vector with some elements tests\n\n";
		e3.reserve(50);
		displayInfo(e3, "after reserve more", outfile, 10);
		e3.reserve(20);
		displayInfo(e3, "after reserve less", outfile, 10);
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

	//ACCESS

	{
		fileName = currentPath + "access.log";

		outfile.open(fileName.c_str(), std::ios_base::out);
		vector<std::string> v_str;
		for (int i = 0; i < 20; ++i)
			v_str.push_back(myItoa(i));

		const vector<std::string> const_v_str(v_str);

		outfile << "front, back, const, non const : \n\n";
		outfile << v_str.front() << " | " << v_str.back() << '\n';
		v_str.front() = "lol";
		v_str.back() = "mdr";
		outfile << v_str.front() << " | " << v_str.back() << '\n';
		outfile << const_v_str.front() << " | " << const_v_str.back() << '\n';
		outfile << "\n[] and at() tests : \n\n";
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
		outfile << "with at(), take a const reference from a const vector : " << const_str5 << "\n\n";

		v_str[4] = "I was 4";
		displayInfo(v_str, "change the 4th element with [] operator", outfile, 10);

		v_str.at(18) = "I was 18";
		displayInfo(v_str, "change the 18th element with at()", outfile, 10);

		try
		{
			v_str.at(25);
		}
		catch(out_of_range& e)
		{
			except << "VECTOR : provoque an exception with an out of range element with at().\n" << e.what() << '\n';
		}
		
		outfile.close();
	}

	//ITERATOR

	{
		fileName = currentPath + "iterator.log";
		outfile.open(fileName.c_str());
		randomIteratorTests<vector<std::string> >(outfile);
		ReverseRandomIteratorTests<vector<std::string> >(outfile);
		outfile.close();
	}

	//SWAP

	{
		fileName = currentPath + "swap.log";
		outfile.open(fileName.c_str());
		vector<std::string> a(12, "hello");
		vector<std::string> b(1, "pouet");
		displayInfo(a, "a before swap", outfile, 6);
		displayInfo(b, "b before swap", outfile);

		a.swap(b);
		displayInfo(a, "a after swap member method", outfile);
		displayInfo(b, "b after swap member method", outfile, 6);

		swap(a, b);
		displayInfo(a, "a after swap non member overload", outfile, 6);
		displayInfo(b, "b after swap non member overload", outfile);
		outfile.close();
	}

	//GET_ALLOCATOR

	{
		typedef dummyAllocator<char> Dumb;
		typedef vector<char, Dumb> V;
		fileName = currentPath + "get_allocator.log";
		outfile.open(fileName.c_str());

		char adr = 'o';
		char otherchar = 'u';
		dummyAllocator<char> dumb(&adr);
		V v(dumb);
		Dumb cpy = v.get_allocator();
		outfile << *cpy.adresse(otherchar) << " must be equal to " << *dumb.adresse(otherchar);
		outfile.close();
	}

	//RELATIONAL OPERATORS

	{
		typedef vector<std::string> V;
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