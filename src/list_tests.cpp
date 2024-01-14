#include <sys/types.h>
#include <sys/stat.h>
#include "../includes/test_elements.hpp"

#if FT == 1
	#include "../includes/list.hpp"
	#include "../includes/set.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <list>
	#include <set>
	#define FT 0
	using namespace std;
#endif

struct is_odd
{
  bool operator() (const int& value) { return (value%2)==1; }
};

struct is_peer
{
  bool operator() (const int& value) { return (value%2)==0; }
};

struct is_near
{
  bool operator() (double first, double second)
  { return (fabs(first-second)<5.0); }
};

bool same_integral_part (double first, double second)
{ return ( int(first)==int(second) ); }

bool compare_nocase (const std::string& first, const std::string& second)
{
  unsigned int i=0;
  while ( (i<first.length()) && (i<second.length()) )
  {
    if (tolower(first[i])<tolower(second[i])) return true;
    else if (tolower(first[i])>tolower(second[i])) return false;
    ++i;
  }
  return ( first.length() < second.length() );
}

class IntLog
{
	int i;
	std::ostream* outfile;
	
	public:

	IntLog() : i(0), outfile(0) {}
	IntLog(const IntLog& o) : i(o.i), outfile(o.outfile)
	{
		if (outfile)
			*outfile << "copy constructor called\n";
	}
	IntLog(int nb, std::ostream* o = 0) : i(nb), outfile(o) {}
	~IntLog()
	{
		if (outfile)
			*outfile << "destructor called\n";
	}

	IntLog& operator=(const LogInt& o)
	{
		if (&o != this)
		{
			i = o.i;
			outfile = o.outfile;
		}
		if (outfile)
			*outfile << "operator=() called\n";
		return *this;
	}

	void	switchLog(std::ostream* out)
	{
		if (outfile == out)
			outfile = 0;
		else
			outfile = out;
	}

	int	get() const { return i; }
};

std::ostream& operator<<(std::ostream& o, const IntLog& i)
{
	o << i.get();
	return o;
}

bool operator<(const IntLog& lhs, const IntLog& rhs)
{
	return lhs.get() < rhs.get();
}

void	switchLog(list<IntLog>& l, std::ostream& out)
{
	for (list<IntLog>::iterator it = l.begin(); it != l.end(); ++it)
		it->switchLog(&out);
}

void	list_tests(const std::string& currentPath, std::ostream& except)
{
	std::string fileName;
	std::ofstream outfile;

	std::boolalpha(outfile);

	typedef list<std::string> S;
	typedef list<int> I;
	typedef list<double> D;
	typedef list<IntLog> IL;
	typedef set<std::string> SetS;
	typedef set<int> SetI;

	SetS setS;
	SetI setI;

	for (int i = 0; i < 50; ++i)
	{
		setI.insert(i);
		setS.insert(itoa(i));
	}


	//CONSTRUCTORS

	{
		fileName = currentPath + "constructor.log";
		outfile.open(fileName.c_str());

		S le;
		S lf(15);
		S lf2(15, "pouet");
		const S lc(lf2);
		S lc2(lc);
		S lr(++lc.begin(), --lc.begin());		
		S lr2(setS.find("3"), setS.find("17"));

		*(lc2.begin()) = "lc2 deep?";
		*(lr.begin()) = "lr deep?";

		displayV2(le, "default constructor", outfile);
		displayV2(lf, "fill constructors with default value_type", outfile, 5);
		displayV2(lf2, "fill constructors", outfile, 5);
		displayV2(lc, "const copy constructors", outfile, 5);
		displayV2(lc2, "copy constructor with a const list", outfile, 5);
		displayV2(lr, "range constructors with list iterators", outfile, 5);
		displayV2(lr2, "range constructors with set iterators", outfile, 5);

		lf = le;
		displayV2(lf, "operator= filled => empty", outfile);

		le = lf;
		displayV2(lf, "operator= empty => empty", outfile, 5);

		le = lc;
		*(lc.begin()) = "operator= deep ?";
		displayV2(le, "operator= empty => filled", outfile, 5);

		le = lc;
		displayV2(le, "operator= filled => filled", outfile, 5);

		le = lc;
		displayV2(le, "operator= with a const one", outfile, 5);

		try
		{
			S toobig(S::allocator_type().max_size() + 1, "ouch");
		}
		catch(const length_error& e)
		{
			except << "LIST : too many elts in fill constructor.\n" << e.what() << '\n';
		}
		
		outfile.close();
	}

	//ITERATORS

	{
		fileName = currentPath + "iterator.log";
		outfile.open(fileName.c_str());

		S l;
		for (long long i = 0; i < 9; ++i)
			l.push_back(itoa(pow(10, i)));

		bidirectionalIteratorTests(l, outfile);
		bidirectionalReverseIteratorTests(l, outfile);

		const S l2(l);
		S::iterator it;
		S::const_iterator cit;
		
		cit = l2.begin();
		outfile << *cit << '\n';
		++cit;
		outfile << cit++->size() << '\n';
		outfile << *cit->size() << '\n';

		cit = l2.end();
		--cit;
		outfile << *cit << '\n';
		--cit;
		outfile << cit--->size() << '\n'; //don't worry, I never write that thing in usual codes. It must work anyway
		outfile << cit->size() << '\n';

		it = l.begin();

		outfile << *it << '\n';
		++it;
		outfile << it++->size() << '\n';
		outfile << it->size() << '\n';

		it = l.end();
		--it;
		outfile << it->size() << '\n';
		--it;
		outfile << it--->size() << '\n';
		outfile << it->size() << '\n';

		it->second = "pouet";
		outfile << *it << '\n';

		outfile.close();
	}

	//ACCESS

	{
		fileName = currentPath + "access.log";
		outfile.open(fileName.c_str());

		S l(setS.begin(), setS.end());
		const S l2(l);
		const S l3(1, "alone");

		outfile << l2.front() << " | " << l2.back() << '\n';
		outfile << l.front() << " | " << l.back() << '\n';
		outfile << l3.front() << " | " << l3.back() << '\n';

		l.front() = l2.back();
		l.back() = l.front();

		outfile << l2.front() << " | " << l2.back() << '\n';
		outfile << l.front() << " | " << l.back() << '\n';		

		outfile.close();
	}

	//INSERT

	{
		fileName = currentPath + "insert.log";
		outfile.open(fileName.c_str());

		I::iterator it;
		const I lce;
		I le;
		I lf(12, 12);

		it = le.insert(le.begin(), 1);
		outfile << *it << '\n';
		displayV2(le, "insert one at begin() on empty list", outfile);

		le = lce;
		it = le.insert(le.end(), 1);
		outfile << *it << '\n';
		displayV2(le, "insert one at end() on empty list", outfile);

		it = le.begin();
		for (int i = 2; i < 20; ++i)
			it = le.insert(it, i);
		displayV2(le, "insert one by one element at last element inserted position", outfile);

		++(++(++(it)));

		for (int i = 20; i < 40; ++i)
			le.insert(it, i);
		displayV2(le, "insert one by one element at fixed position", outfile);

		le = lce;

		le.insert(le.begin(), 14, 14);
		displayV2(le, "insert some at begin() on empty list", outfile);

		le = lce;

		le.insert(le.end(), 14, 14);
		displayV2(le, "insert some at end() on empty list", outfile);

		le.insert(le.begin(), 4, 4);
		displayV2(le, "insert some at begin() on filled list", outfile);

		le.insert(le.end(), 3, 3);
		displayV2(le, "insert some at end() on filled list", outfile);

		it = le.begin();
		++(++(++(++(++(++it)))));

		le.insert(it, 5, 5);
		displayV2(le, "insert some at somewhere between begin() and end()", outfile);

		le = lce;

		try
		{
			le.insert(le.max_size() + 1, 2);
		}
		catch(const length_error& e)
		{
			except << "LIST : insert too much elements.\n" << e.what() << '\n';
		}
		
		le.insert(le.begin(), lf.begin(), lf.end());
		displayV2(le, "range insert at begin() on empty list", outfile);

		le = lce;

		le.insert(le.end(), lf.begin(), lf.end());
		displayV2(le, "range insert at end() on empty list", outfile);

		it = le.begin();

		++(++(++(++(++(++it)))));

		le.insert(it, setI.begin(), setI.end());
		displayV2(le, "range insert somewhere between begin() and end()", outfile);

		outfile.close();
	}

	//ERASE

	{
		fileName = currentPath + "erase.log";
		outfile.open(fileName.c_str());

		I::iterator it;
		I l(setI.begin(), setI.end());
		const I lc(l);

		l.erase(l.begin());
		displayV2(l, "erase first element", outfile, 5);

		l.erase(l.rbegin());
		displayV2(l, "erase last element", outfile, 5);

		l.erase(++(++(++(l.begin()))));
		displayV2(l, "erase somewhere", outfile, 5);

		it = l.begin();
		++(++(++(++(++(++(++(++(++it))))))));

		while (it != l.end())
			it = l.erase(it);

		displayV2(l, "erase one by one from somewhere into end()", outfile, 3);

		l = lc;

		it = l.begin();
		++(++(++(++(++(++(++(++(++(++(++it)))))))))); //rofl...

		it = l.erase(it, ++it);
		displayV2(l, "range erase one element somewhere", outfile, 5);

		it = l.erase(it, ++(++(++(++it))));
		displayV2(l, "range erase some elements somewhere", outfile, 5);

		l = lc;

		it = erase(++(l.begin()), --(l.end()));
		outfile << *it << '\n';
		outfile << it == l.end() << '\n';
		displayV2(l, "range erase almost all", outfile, 3);

		it = erase(l.begin(), l.end());
		outfile << it == l.end() << '\n';
		displayV2(l, "range erase all", outfile, 3);


		l = lc;
		it = erase(--(l.end()), l.end());
		outfile << it == l.end() << '\n';
		displayV2(l, "range erase only last element", outfile, 5);

		it = erase(l.begin(), l.begin());
		outfile << it == l.end() << '\n';
		outfile << it == l.begin() << '\n';
		displayV2(l, "range erase distance of 0 at begin()", outfile, 5);

		it = erase(l.end(), l.end());
		outfile << it == l.end() << '\n';
		outfile << it == l.begin() << '\n';
		displayV2(l, "range erase distance of 0 at end()", outfile, 5);

		it = l.begin();

		++(++(++(++(++(++(++(++it)))))));
		I::iterator it2(it);
		it = erase(it, it);
		outfile << it == l.end() << '\n';
		outfile << it == l.begin() << '\n';
		outfile << it2 == it << '\n';
		displayV2(l, "range erase distance of 0 somewhere else", outfile, 5);

		it = l.erase(l.begin(), l.end());
		outfile << it == l.end() << '\n';
		outfile << it == l.begin() << '\n';

		it = l.erase(l.begin(), l.end());
		outfile << it == l.end() << '\n';
		outfile << it == l.begin() << '\n';
		displayV2(l, "range erase all on empty list", outfile, 5);

		outfile.close();
	}

	//CLEAR

	{
		fileName = currentPath + "clear.log";
		outfile.open(fileName.c_str());

		I l;

		l.clear()

		displayV2(l, "clear() an empty list", outfile);

		l.insert(12, 13);
		displayV2(l, "reuse a cleared list", outfile);

		l.clear();

		displayV2(l, "clear() a filled list", outfile);

		l.insert(7, 7);
		displayV2(l, "reuse again", outfile);

		outfile.close();
	}

	//ASSIGN

	{
		fileName = currentPath + "assign.log";
		outfile.open(fileName.c_str());

		S l;
		const S l2(setI.begin(), setI.end());

		l.assign(0);
		displayV2(l, "assign test", outfile, 5);
		l.assign(0, 9);
		displayV2(l, "assign test", outfile, 5);
		l.assign(4);
		displayV2(l, "assign test", outfile, 5);
		l.assign(5);
		displayV2(l, "assign test", outfile, 5);
		l.assign(2);
		displayV2(l, "assign test", outfile, 5);
		l.assign(0);
		displayV2(l, "assign test", outfile, 5);
		l.assign(4, 4);
		displayV2(l, "assign test", outfile, 5);
		l.assign(0, 1);
		displayV2(l, "assign test", outfile, 5);
		l.assign(3, 3);
		displayV2(l, "assign test", outfile, 5);

		try
		{
			l.assign(l.max_size() + 1);
		}
		catch(const length_error& e)
		{
			except << "LIST : assign too much elements.\n" << e.what() << '\n';
		}
		
		l.assign(l2.begin(), l2.end());
		displayV2(l, "assign test", outfile, 5);
		l.assign(setI.begin(), setI.end());
		displayV2(l, "assign test", outfile, 5);

		outfile.close();
	}

	//RESIZE

	{
		fileName = currentPath + "resize.log";
		outfile.open(fileName.c_str());

		I l;

		try
		{
			l.resize(l.max_size() + 1);
		}
		catch(const length_error& e)
		{
			except << "LIST : resize too much element.\n" << e.what() << '\n';
		}
		
		l.resize(3);
		displayV2(l, "resize test", outfile, 5);
		l.resize(2);
		displayV2(l, "resize test", outfile, 5);
		l.resize(8);
		displayV2(l, "resize test", outfile, 5);
		l.resize(0);
		displayV2(l, "resize test", outfile, 5);
		l.resize(3, 4);
		displayV2(l, "resize test", outfile, 5);
		l.resize(8, 5);
		displayV2(l, "resize test", outfile, 5);
		l.resize(6, 99);
		displayV2(l, "resize test", outfile, 5);
		l.resize(15, 99);
		displayV2(l, "resize test", outfile, 5);
		l.resize(20, 14);
		displayV2(l, "resize test", outfile, 5);

		outfile.close();
	}

	//PUSH_POP

	{
		fileName = currentPath + "push_pop.log";
		outfile(fileName.c_str());

		S l;

		for (int i = 0; i < 10; ++i)
		{
			l.push_back(itoa(i));
			outfile << l.back() << " | ";
			l.pop_back(-itoa(i));
			outfile << l.front() << '\n';
		}

		while (l.size())
		{
			outfile << l.back() << " | ";
			l.pop_back();
			outfile << l.front() << '\n';
			l.pop_front();
		}

		outfile.close();
	}

	//SWAP

	{
		fileName = currentPath + "swap.log";
		outfile.open(fileName.c_str());
		std::string* ptr;

		S le;

		S lr(setS.begin(), setS.end());

		S l;

		l.push_back("pouet");
		l.push_front("mdr");
		l.push_back("koko");

		ptr = &(*(l.begin()));

		l.swap(lr);

		outfile << "must not be a deep copy, is it a deep copy ? " << ptr == &(*(lr.begin())) << '\n';

		displayV2(l, "l after swap", outfile, 10);
		displayV2(lr, "lr after swap", outfile, 10);

		le.swap(l);

		displayV2(l, "l after swap", outfile, 10);
		displayV2(le, "le after swap", outfile, 10);

		le.swap(l);

		displayV2(l, "l after swap", outfile, 10);
		displayV2(le, "le after swap", outfile, 10);

		swap(l, lr);

		displayV2(l, "l after swap", outfile, 10);
		displayV2(lr, "lr after swap", outfile, 10);

		outfile.close();
	}

	//SPLICE

	{
		fileName = currentPath + "splice.log";
		outfile.open(fileName.c_str());

		IL l;
		IL l2;

		for (int i = 0; i < 20; ++i)
			l.push_back(IntLog(i));
		
		switchLog(l, outfile);

		l.splice(l2);
		displayV2(l, "after splice full", outfile, 5);
		l.splice(l2);
		displayV2(l, "same operation", outfile, 5);

		l2.splice(++(l.begin()));
		displayV2(l2, "l2 after splice one elt", outfile, 5);
		displayV2(l, "l after got spliced", outfile, 5);
		l2.splice(++(l.begin()));
		displayV2(l2, "l2 after splice one elt", outfile, 5);
		displayV2(l, "l after got spliced", outfile, 5);
		l2.splice(++(l.begin()));
		displayV2(l2, "l2 after splice one elt", outfile, 5);
		displayV2(l, "l after got spliced", outfile, 5);

		l2.splice(++(l.begin()), --(l.end()));
		displayV2(l2, "l2 after range splice", outfile, 5);
		displayV2(l, "l after got spliced", outfile, 5);

		l2.splice(l.begin(), l.end());
		displayV2(l2, "l2 after range splice all", outfile, 5);
		displayV2(l, "l after got spliced", outfile, 5);

		l.splice(l2.begin(), l2.end());
		displayV2(l, "l after range splice all", outfile, 5);
		displayV2(l2, "l2 after got spliced", outfile, 5);

		l.splice(l2.begin(), l2.end());
		displayV2(l, "l after range splice all again", outfile, 5);
		displayV2(l2, "l2 after got spliced", outfile, 5);

		l2.splice(l.begin(), l.begin());
		displayV2(l2, "l2 after range splice nothing", outfile, 5);
		displayV2(l, "l after got spliced", outfile, 5);

		l2.splice(l.begin(), ++(l.begin()));
		displayV2(l2, "l2 after range splice 1 elt", outfile, 5);
		displayV2(l, "l after got spliced", outfile, 5);

		l2.splice(--(l.end()), l.end());
		displayV2(l2, "l2 after range splice 1 elt", outfile, 5);
		displayV2(l, "l after got spliced", outfile, 5);

		switchLog(l, outfile);
		switchLog(l2, outfile);

		outfile.close();
	}

	//REMOVE

	{
		fileName = currentPath + "remove.log";
		outfile.open(fileName.c_str());

		S le;
		S l;

		l.push_back("lustucru");
		l.push_back("pouet");
		l.push_back("saperlipopette");
		l.push_back("pouet");
		l.push_back("cafe");
		l.push_back("cafe");
		l.push_back("pouet");
		l.push_back("pouet");

		l.remove("introuvable");
		le.remove("empty");
		displayV2(l, "removing not present element", outfile, 3);

		l.remove("saperlipopette");
		displayV2(l, "removing present element", outfile, 3);

		l.remove("cafe");
		displayV2(l, "removing present elements", outfile, 3);

		l.remove("lustucru");
		displayV2(l, "removing present element", outfile, 3);

		l.remove("pouet");
		displayV2(l, "removing last elements", outfile, 3);

		outfile.close();
	}

	//REMOVE_IF

	{
		fileName = currentPath + "remove_if.log";
		outfile.open(fileName.c_str());

		is_odd predOdd;
		is_peer predPeer;

		I l(setI.begin(), setI.end());
		l.remove_if(predOdd);
		displayv2(l, "after remove_if for is_odd", outfile, 5);

		l.insert(l.end(), setI.begin(), setI.end());
		l.remove_if(predPeer);
		displayv2(l, "after remove_if for is_peer", outfile, 5);

		outfile.close();
	}

	//SORT

	{
		fileName = currentPath + "sort.log";
		outfile.open(fileName.c_str());

		S mylist;
		S::iterator it;
		mylist.push_back ("one");
		mylist.push_back ("two");
		mylist.push_back ("Three");
		mylist.push_back ("four");
		mylist.push_back ("cinq");
		mylist.push_back ("Six");
		mylist.push_back ("Four");
		mylist.push_back ("one");

		mylist.sort();
		displayV2(mylist, "normal sort", outfile, 3);

		mylist.sort(compare_nocase);
		displayV2(mylist, "non case sensitive sort", outfile, 3);

		IL l;
		I::size_type seed = 0;
		while (l.size() != 500)
			l.push_back(IntLog(getRandom< I >(seed, 500)));

		I l2(l);

		switchLog(l, outfile);
		switchLog(l2, outfile);

		l.sort();
		displayV2(l, "normal sort", outfile, 50);

		l2.sort(greater<IntLog>());
		displayV2(l2, "greater sort", outfile, 50);

		switchLog(l, outfile);
		switchLog(l2, outfile);

		outfile.close();
	}

	//MERGE

	{
		fileName = currentPath + "merge.log";
		outfile.open(fileName.c_str());

		IL l1;
		IL l2;
		I::size_type seed;
		for (int i = 0; i < 30; ++i)
		{
			l1.push_back(IntLog(getRandom< I >(seed, 40)));
			l2.push_back(IntLog(getRandom< I >(seed, 40)));
		}
		IL l3(l1);
		IL l4(l2);

		switchLog(l1, outfile);
		switchLog(l2, outfile);
		switchLog(l3, outfile);
		switchLog(l4, outfile);

		l1.sort();
		l2.sort();
		l1.merge(l2);
		displayV2(l1, "after normal merge", outfile, 5);
		displayV2(l2, "the merged one", outfile, 5);

		l3.sort(greater<IntLog>());
		l4.sort(greater<IntLog>());

		l3.merge(l4, greater<IntLog>());
		displayV2(l3, "after greater merge", outfile, 5);
		displayV2(l4, "the merged one", outfile, 5);

		switchLog(l1, outfile);
		switchLog(l2, outfile);
		switchLog(l3, outfile);
		switchLog(l4, outfile);

		outfile.close();
	}

	//UNIQUE

	{
		fileName = currentPath + "unique.log";
		outfile.open(fileName.c_str());

		double mydoubles[]={ 12.15,  2.72, 73.0,  12.77,  3.14,
						12.77, 73.35, 72.25, 15.3,  72.25 };

		D mylist(mydoubles, mydoubles + 10);

		mylist.sort();
		mylist.unique();
		displayV2(mylist, "sorted + unique", outfile, 5);

		mylist.unique(same_integral_part);
		displayV2(mylist, "sorted + unique(same integral part)", outfile, 5);

		mylist.unique(is_near());
		displayV2(mylist, "sorted + unique(is near())", outfile, 5);

		I l;
		for (int i = -10; i < 10; ++i)
		{
			l.push_back(i * i);
			l.push_back(i * i);
		}

		I l2(l);

		l.unique();
		displayV2(l, "not sorted + unique", outfile, 8);

		l2.sort();
		l2.unique();
		displayV2(l2, "sorted + unique", outfile, 8);

		outfile.close();
	}

	//REVERSE

	{
		fileName = currentPath + "reverse.log";
		outfile.open(fileName.c_str());

		IL l;
		for (int i = 0; i < 30; ++i)
			l.push_back(IntLog(i));

		switchLog(l, outfile);

		l.reverse();
		displayV2(l, "reverse", outfile, 5);

		switchLog(l, outfile);

		outfile.close();
	}

	//GET_ALLOCATOR

	{
		fileName = currentPath + "get_allocator.log";
		outfile.open(fileName.c_str());

		char adr = 'o';
		char otherchar = 'u';
		dummyAllocator<char> dumb(&adr);
		list<char, dummyAllocator<char> > l(dumb);
		dummyAllocator<char> cpy = l.get_allocator();
		outfile << *cpy.adresse(otherchar) << " must be equal to " << *dumb.adresse(otherchar);

		outfile.close();
	}

	//RELATIONAL

	{
		fileName = currentPath + "relational.log";
		outfile.open(fileName.c_str());

		I l1;
		I l2;

		outfile << l1 == l2 << " | " << l1 != l2 << " | " << l1 < l2 << " | " << l1 <= l2 << " | " << l1 > l2 << " | " << l1 >= l2 << '\n';

		l1.push_back(0);
		l2.push_back(0);

		outfile << l1 == l2 << " | " << l1 != l2 << " | " << l1 < l2 << " | " << l1 <= l2 << " | " << l1 > l2 << " | " << l1 >= l2 << '\n';

		l2.pop_back();
		l2.push_back(1);

		outfile << l1 == l2 << " | " << l1 != l2 << " | " << l1 < l2 << " | " << l1 <= l2 << " | " << l1 > l2 << " | " << l1 >= l2 << '\n';

		l1.push_back(3);

		outfile << l1 == l2 << " | " << l1 != l2 << " | " << l1 < l2 << " | " << l1 <= l2 << " | " << l1 > l2 << " | " << l1 >= l2 << '\n';

		l1.sort(greater<int>());

		outfile << l1 == l2 << " | " << l1 != l2 << " | " << l1 < l2 << " | " << l1 <= l2 << " | " << l1 > l2 << " | " << l1 >= l2 << '\n';

		l1.sort();

		outfile << l1 == l2 << " | " << l1 != l2 << " | " << l1 < l2 << " | " << l1 <= l2 << " | " << l1 > l2 << " | " << l1 >= l2 << '\n';

		l2.pop_back();

		outfile << l1 == l2 << " | " << l1 != l2 << " | " << l1 < l2 << " | " << l1 <= l2 << " | " << l1 > l2 << " | " << l1 >= l2 << '\n';

		l1.assign(150, 4);
		l2.assign(151, 4);

		outfile << l1 == l2 << " | " << l1 != l2 << " | " << l1 < l2 << " | " << l1 <= l2 << " | " << l1 > l2 << " | " << l1 >= l2 << '\n';

		l1.assign(1, 5);

		outfile << l1 == l2 << " | " << l1 != l2 << " | " << l1 < l2 << " | " << l1 <= l2 << " | " << l1 > l2 << " | " << l1 >= l2 << '\n';

		outfile.close();
	}
}