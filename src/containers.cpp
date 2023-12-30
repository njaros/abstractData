#include "../includes/vector.hpp"
#include "../includes/map.hpp"
#include "../includes/pair.hpp"
#include "../includes/stack.hpp"
#include "../includes/iterator.hpp"
#include "../includes/set.hpp"
#include "../includes/deque.hpp"
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <stack>
#include <windows.h>

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
			throw (ft::logic_error("debug::Destructor : called for a not constructed object"));
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

void	setRandom(unsigned short& s, size_t pouet = 0)
{
	SYSTEMTIME seed;

	if (!s || pouet > s)
	{
		GetSystemTime(&seed);
		s = seed.wMilliseconds;
	}
	else
		s -= pouet;
}

int main()
{
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
	std::cout << "\n\n-----stack tests-----\n\n\n";
	{
		ft::stack<int> test;
		std::cout << "push() and top() test :\n";
		for (int i = 0; i < 100; i++)
		{
			test.push(i + 2);
			if (i >= 90)
				std::cout << test.top() << " | ";
		}
		std::cout << "\npop() and top() test :\n";
		for (int i = 0; i < 10; i++)
		{
			test.pop();
			std::cout << test.top() << " | ";
		}
		std::cout << "\ncrash test (pop() more than size then top())\n";
		for (int i = 0; i < 100; i++)
		{
			test.pop();
		}
		std::cout << "when size = " << test.size() << " top = " << test.top() << '\n';
	}
	std::cout << "\n\n-----vector tests-----\n\n\n";
	{
		ft::vector<int> a(4, 12);
		ft::vector<int> b;
		ft::vector<int> vide(0, 0);
		ft::set<int>    tree;
		vide.pop_back();
		b.pop_back();
		for (int i = 0; i < 1000; ++i) {
			tree.insert(rand() % 1000);
		}
		ft::reverse_iterator<ft::set<int>::iterator> treeRend = tree.rend();
		for (ft::reverse_iterator<ft::set<int>::iterator> it = tree.rbegin(); it != treeRend; ++it) {
			a.insert(a.end(), it, treeRend);
			b.clear();
			b.insert(b.end(), it, treeRend);
			vide.insert(vide.end(), it, treeRend);
		}
		ft::vector<int> c(tree.begin(), tree.end());
		ft::vector<int> d(a.rbegin(), a.rend());
		ft::vector<int> e(tree.rbegin(), tree.rend());
		std::cout << "size tree = " << tree.size() << std::endl;
		std::cout << "size a = " << a.size() << std::endl;
		std::cout << "size b = " << b.size() << std::endl;
		std::cout << "size vide = " << vide.size() << std::endl;
		std::cout << "size c = " << c.size() << std::endl;
		std::cout << "size d = " << d.size() << std::endl;
		std::cout << "size e = " << e.size() << std::endl;

		a.insert(a.begin() + 500, 100, 100);
		b.assign(a.begin() + 500, a.begin() + 600);

		std::cout << b.size() << " " << b[50];
	}*/

	std::cout << "\n\n-----deque tests-----\n\n\n";
	{
		unsigned short random;
		//ft::deque<intDebug> a(4, 13);
		ft::deque<char> b;
		std::deque<char> stdB;
		//ft::deque<intDebug> c(a);
		//ft::vector<std::string> v(7, "pouet");
		//v.push_back("pas pouet");
		//ft::deque<std::string> pouet(v.begin(), v.end());
		//ft::deque<std::string> revPouet(v.rbegin(), v.rend());
		//ft::deque<std::string> revPouetFromPouet(pouet.rbegin(), pouet.rend());

		//displayData(a, "a");

		random = 0;
		setRandom(random);
		for (int i = 1; i < 40; ++i)
		{
			b.insert(b.begin() + (random % (b.size() + 1)), 1, 'A' + i);
			stdB.insert(stdB.begin() + (random % (stdB.size() + 1)), 1, 'A' + i);
			setRandom(random, b.size());
		}
		displayData(b, "b");
		std::deque<char>::const_iterator cmp = stdB.begin();
		for (ft::deque<char>::const_iterator it = b.begin(); it != b.end(); ++it)
		{
			if (*it != *cmp)
			{
				std::cout << "bad order\n";
				break;
			}
			++cmp;
		}
		//displayData(c, "c");
		//displayData(pouet, "pouet");
		//displayData(revPouet, "revPouet");
		//displayData(revPouetFromPouet, "revPouetFromPouet");
	}

	return 0;
}
