#include "../includes/test_elements.hpp"
#include "../includes/exception.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <iostream>

template < typename T >
void	displayHashSet(T& container, const char* name)
{
	typename T::const_local_iterator clit;

	std::cout << name << " content :" << '\n';
	for (size_t i = 0; i < container.bucket_count(); ++i)
	{
		if (container.bucket_size(i))
		{
			std::cout << "bucket [" << i << "] : {";
			clit = container.begin(i);
			while (clit != container.end(i))
			{
				std::cout << "(addr : " << &(*clit) << " | val : " << *clit << ')';
				if (++clit != container.end(i))
					std::cout << ", ";
				else
					std::cout << "}\n";
			}
		}
	}
}

template < typename T >
void	displayHashMap(T& container, const char* name)
{
	typename T::const_local_iterator clit;

	std::cout << name << " content :" << '\n';
	for (size_t i = 0; i < container.bucket_count(); ++i)
	{
		if (container.bucket_size(i))
		{
			std::cout << "bucket [" << i << "] : {";
			clit = container.begin(i);
			while (clit != container.end(i))
			{
				std::cout << "(addr : " << &(*clit) << " | val : " << clit->first << " => " << clit->second << ')';
				if (++clit != container.end(i))
					std::cout << ", ";
				else
					std::cout << "}\n";
			}
		}
	}
}

template < typename T >
void	displayInfo(T& container, const char* name)
{
	std::cout << "size " << name << " : " << container.size() << '\n';
	std::cout << "capacity " << name << " : " << container.capacity() << '\n';
	std::cout << "max_size " << name << " : " << container.max_size() << "\n\n";
}

template < typename T >
void	displayInfoSet(T& container, const char* name)
{
	std::cout << "size " << name << " : " << container.size() << '\n';
	std::cout << "max_size " << name << " : " << container.max_size() << "\n\n";
}

template < typename T >
void	displayInfoMap(T& container, const char* name)
{
	std::cout << "size " << name << " : " << container.size() << '\n';
	std::cout << "max_size " << name << " : " << container.max_size() << "\n\n";
}

template < typename T >
void    displayData(T& container, const char* name)
{
	typename T::iterator it;
	typename T::iterator end;

	it = container.begin();
	end = container.end();
	std::cout << name << " content :" << '\n';
	while (it != end)
	{
		std::cout << *it << '\n';
		it++;
	}
	std::cout << '\n';
}

template < typename T >
void    displayDataMap(T& container, const char* name)
{
	typename T::iterator     it;

	it = container.begin();
	std::cout << name << " content :" << '\n';
	while (it != container.end())
	{
		std::cout << (*it).first << " | " << (*it).second << '\n';
		it++;
	}
	std::cout << '\n';
}

struct Buffer
{
	int idx;
	char buff[4096];
};

class intDebug
{
public:
	int i;

	intDebug() : i(0) {}
	intDebug(int nb) : i(nb) {}
	intDebug(const intDebug& o) : i(o.i) {}
	~intDebug()
	{
		i = -1;
	}

	intDebug& operator=(const intDebug& o)
	{
		i = o.i;
		return *this;
	}
};

class debug
{
public:
	short a;
	short b;

	debug() : a(0) {
		if (!b)
			throw (ft::logic_error("debug::copyConstructor : you construct on an instancied slot, you forgot to destroy"));
		b = 0;
	}
	debug(short nb) : a(nb) {
		if (!b)
			throw (ft::logic_error("debug::copyConstructor : you construct on an instancied slot, you forgot to destroy"));
		b = 0;
	}
	debug(const debug& o)
	{
		if (!b && o.b != 0)
			throw (ft::logic_error("debug::copyConstructor : you try to copy a not constructed object on an instancied slot."));
		if (!b)
			throw (ft::logic_error("debug::copyConstructor : you construct on an instancied slot, you forgot to destroy"));
		if (o.b != 0)
			throw (ft::logic_error("debug::copyConstructor : called with a not constructed debug object"));
		a = o.a;
		b = o.b;
	}
	~debug()
	{
		if (b != 0)
			std::cerr << "debug::Destructor : called for a not constructed object\n";
		a = -1;
		b = -1;
	}

	debug& operator=(const debug& o)
	{
		if (b != 0)
			throw (ft::logic_error("debug::operator= : not constructed"));
		if (o.b != 0)
			throw (ft::logic_error("debug::operator= : other not constructed"));
		a = o.a;
		b = o.b;
		return *this;
	}

	bool operator!=(const debug& o) const
	{
		return a != o.a;
	}

};

class heavyDebug
{
public:
	int i;
	int a;
	int b;
	int c;

	heavyDebug() : i(0), a(0), b(0), c(0) {}
	heavyDebug(int nb) : i(nb), a(0), b(0), c(0) {}
	heavyDebug(const heavyDebug& o) : i(o.i), a(0), b(0), c(0) {}
	~heavyDebug()
	{
		i = -1;
	}

	heavyDebug& operator=(const heavyDebug& o)
	{
		i = o.i;
		return *this;
	}

	bool operator!=(const heavyDebug& o) const
	{
		return i != o.i;
	}
};

class littleDebug
{
public:
	char i;

	littleDebug() : i(0) {}
	littleDebug(int nb) : i(nb) {}
	littleDebug(const littleDebug& o) : i(o.i) {}
	~littleDebug()
	{
		i = -1;
	}

	littleDebug& operator=(const littleDebug& o)
	{
		i = o.i;
		return *this;
	}

	bool operator!=(const littleDebug& o) const
	{
		return i != o.i;
	}
};

std::ostream& operator<<(std::ostream& o, const intDebug& i)
{
	o << i.i;
	return o;
}

std::ostream& operator<<(std::ostream& o, const heavyDebug& i)
{
	o << i.i;
	return o;
}

std::ostream& operator<<(std::ostream& o, const littleDebug& i)
{
	o << (int)i.i;
	return o;
}

std::ostream& operator<<(std::ostream& o, const debug& i)
{
	o << i.a;
	return o;
}

std::string filePath(const std::string& ctx, const std::string& container)
{
	std::string ret(ctx);
	ret += '/';
	ret += container;
	ret += '/';
	return ret;
}

std::string dirName(const std::string& current, const std::string& ctx, std::string container = "")
{
	std::string ret(current);
	ret += ctx;
	if (!container.empty())
	{
		ret += '/';
		ret += container;
	}
	return ret;
}

int main()
{
	std::string mainPath;
	std::string ctxPath;
	std::ofstream exceptionFile;

	mainPath = get_current_dir_name();
	mainPath += '/';
	if (FT == 1)
		ctxPath = "ft_logs";
	else
		ctxPath = "std_logs";
	
	mkdir(dirName(mainPath, ctxPath).c_str(), 0777);
	exceptionFile.open((ctxPath + "/exceptions.log").c_str());

	mkdir(dirName(mainPath, ctxPath, "vector").c_str(), 0777);
	vector_tests(filePath(ctxPath, "vector"), exceptionFile);

	mkdir(dirName(mainPath, ctxPath, "deque").c_str(), 0777);
	deque_tests(filePath(ctxPath, "deque"), exceptionFile);

	mkdir(dirName(mainPath, ctxPath, "stack").c_str(), 0777);
	stack_tests(filePath(ctxPath, "stack"));

	mkdir(dirName(mainPath, ctxPath, "queue").c_str(), 0777);
	queue_tests(filePath(ctxPath, "queue"));

	mkdir(dirName(mainPath, ctxPath, "priority_queue").c_str(), 0777);
	priority_queue_tests(filePath(ctxPath, "priority_queue"));

	mkdir(dirName(mainPath, ctxPath, "map").c_str(), 0777);
	map_tests(filePath(ctxPath, "map"), exceptionFile);

	mkdir(dirName(mainPath, ctxPath, "multimap").c_str(), 0777);
	multimap_tests(filePath(ctxPath, "multimap"), exceptionFile);

	mkdir(dirName(mainPath, ctxPath, "set").c_str(), 0777);
	set_tests(filePath(ctxPath, "set"));

	mkdir(dirName(mainPath, ctxPath, "multiset").c_str(), 0777);
	multiset_tests(filePath(ctxPath, "multiset"));
	/*
	std::cout << "------exceptions tests------\n\n\n";
	{
		ft::vector<int> lolo(2);
		try {
			lolo.at(3) = 4;
		}
		catch (ft::exception& ex)
		{
			std::cout << ex.what() << std::endl;
		}
	}

	std::cout << "\n\n------map tests------\n\n\n";
	{
		srand(12);
		ft::map<char, int> test;
		for (int i = 0; i < 25; i++) {
			int access = (rand() % 26) + 'a';
			test[access];
		}
		for (ft::map<char, int>::iterator it = test.begin(); it != test.end(); it++)
			std::cout << it->first << " | " << it->second << " ; ";
		ft::map<char, int> cpy = test;
		std::cout << std::endl;
		for (ft::map<char, int>::iterator it = cpy.begin(); it != cpy.end(); it++)
			std::cout << it->first << " | ";
		std::cout << std::endl;
		cpy.clear();
		cpy.insert(test.rbegin(), test.rend());
		for (ft::map<char, int>::iterator it = cpy.begin(); it != cpy.end(); it++)
			std::cout << it->first << " | ";
		std::cout << std::endl;
		for (int i = 0; i < 8; ++i) {
			std::cout << "\n\n affichage actuel arbre : \n";
			test.display();
			int delIt = rand() % (8 - i);
			ft::map<char, int>::iterator itt = test.begin();
			while (delIt--)
				++itt;
			std::cout << "\nelement delete : " << itt->first << std::endl;
			test.erase(itt);
			std::cout << "elements restants : \n";
			for (ft::map<char, int>::iterator it = test.begin(); it != test.end(); it++)
				std::cout << it->first << " | " << it->second << " ; ";
		}
		ft::map<int, int> hint;
		for (int i = 0; i < 10; ++i) { hint.insert(ft::make_pair(i, i)); }
		hint.insert(hint.begin(), ft::make_pair(15, 15));
		displayDataMap(hint, "hint");

		std::cout << "before non member function swap : \n\n";
		displayDataMap(test, "test");
		displayDataMap(cpy, "cpy");
		displayInfoMap(test, "test");
		displayInfoMap(cpy, "cpy");
		swap(cpy, test);
		std::cout << "after non member function swap : \n\n";
		displayDataMap(test, "test");
		displayDataMap(cpy, "cpy");
		displayInfoMap(test, "test");
		displayInfoMap(cpy, "cpy");
	}
	std::cout << "\n\n-----set tests-----\n\n\n";
	{
		std::string tab[] = { "hello", "haha", "hello", "eat", "cat", "dog", "chicken" };
		ft::vector<std::string> vec;
		for (int i = 0; i < 7; ++i) {
			vec.push_back(tab[i]);
		}
		std::cout << "construct set with vector iterators\n";
		ft::set<std::string> set1(vec.begin(), vec.end());
		displayData(set1, "set");
		ft::set<std::string> set2;
		set2.insert("pouet");
		set2.insert("pouet");
		set2.insert("pouet");
		set2.insert("pouet2");
		set2.insert("pouet");
		set2.insert("");
		set2.insert("  some spaces");

		std::cout << "before non member function swap : \n";
		displayData(set1, "set1");
		displayData(set2, "set2");
		displayInfoSet(set1, "set1");
		displayInfoSet(set2, "set2");
		swap(set1, set2);
		std::cout << "after non member function swap : \n";
		displayData(set1, "set1");
		displayData(set2, "set2");
		displayInfoSet(set1, "set1");
		displayInfoSet(set2, "set2");

		ft::set<int>	tree;
		for (int i = 0; i < 20; ++i)
		{
			tree.insert(rand() % 15);
		}
		std::cout << "display of a tree with 20 random inserted ints in range [0, 14] : \n";
		tree.display();
	}

		std::cout << "======Priority_queue tests========\n\n";
		{
			int myInts[] = { 5, 77, 1, 1, 1, 1, 45, 1000, 0 };
			int myInts2[] = { 12, -1, -2, -1, 458, -4, 75, 45, 22, 4, -45, 1 };
			ft::vector<int> b;
			ft::vector<int> c(4, 4);
			ft::set<int>	s;
			for (int i : myInts)
				b.insert(b.end(), i);
			displayData(b, "b");
			ft::priority_queue<int> p(b.begin(), b.end());
			for (int i : myInts2)
			{
				std::cout << "current top is " << p.top() << '\n';
				std::cout << "i insert " << i << '\n';
				p.push(i);
				std::cout << "now top is " << p.top() << '\n';
			}
			while (!p.empty())
			{
				std::cout << p.top() << '\n';
				p.pop();
			}
		}
		
	std::cout << "============list tests=========\n\n";
	{
		int n[] = { 9, 5, 1, 12, 0, 421, 74, -45, 79 };
		int n2[] = { 1, 3, 5, 6, 8, 6, 11 };
		ft::list<int> l(n, n + 9);
		ft::list<int> l2(n2, n2 + 6);
		l.sort();
		displayData(l, "l");
		l.merge(l2);
		displayData(l, "l");
		l.sort();
		displayData(l, "l");
		ft::list<int> l3(l.begin(), l.end());
		l3.reverse();
		displayData(l3, "l3");

		ft::list<int> l4(12, 7);
		displayData(l4, "l4");
	}

	std::cout << "=========multimap tests=========\n\n";
	{
		ft::multimap<int, char> test;
		ft::multimap<int, char>::iterator itLow;
		ft::multimap<int, char>::iterator itUp;

		test.insert(ft::make_pair(4, 'a'));
		test.insert(ft::make_pair(4, 'b'));
		test.insert(ft::make_pair(4, 'c'));
		test.insert(ft::make_pair(4, 'd'));
		test.insert(ft::make_pair(4, 'e'));
		displayDataMap(test, "test");

		test.insert(ft::make_pair(5, 'a'));
		test.insert(ft::make_pair(5, 'b'));
		test.insert(ft::make_pair(5, 'c'));
		test.insert(ft::make_pair(5, 'd'));
		test.insert(ft::make_pair(5, 'e'));

		test.insert(ft::make_pair(6, '6'));
		test.insert(ft::make_pair(6, '7'));
		test.insert(ft::make_pair(6, '8'));


		itLow = test.upper_bound(4);
		itUp = test.upper_bound(5);

		while (itLow != itUp)
		{
			std::cout << itLow->first << " => " << itLow->second << '\n';
			++itLow;
		}
		std::vector<int> d;
		displayDataMap(test, "test");
	}
	std::cout << "=========unordered_map tests=========\n\n";
	{
		std::unordered_set<int> pouet;
		int count;

		for (int i = 0; i < 63; i++)
		{
			//std::cout << "size : " << pouet.size() << " | number of buckets : " << pouet.bucket_count() << '\n';
			pouet.insert(i);
		}
		std::unordered_set<int>::local_iterator lcit = pouet.begin(1);

		displayHashSet(pouet, "pouet");
		pouet.max_load_factor(0.3);
		displayHashSet(pouet, "pouet after MLF = 0.5");
		pouet.insert(63);
		displayHashSet(pouet, "pouet after MLF = 0.5  and 1 insert");
		ft::unordered_map<int, int> pouet;
		displayHashMap(pouet, "pouet");
		*/
	//flat_basket_tests();
	return 0;
}
