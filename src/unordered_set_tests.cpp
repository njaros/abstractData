#include <sys/types.h>
#include <sys/stat.h>
#include "../includes/test_elements.hpp"

#if FT == 1
	#include "../includes/unordered_set.hpp"
	#include "../includes/set.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <unordered_set>
	#include <set>
	#define FT 0
	using namespace std;
#endif

void unordered_set_tests(const std::string& currentPath)
{
	std::string fileName;
	std::ofstream outfile;

	std::boolalpha(outfile);

	typedef unordered_set<std::string> S;
	typedef unordered_set<int> I;
	typedef unordered_set<float> F;
	typedef unordered_set<double> D;
	typedef unordered_set<int*> P;

	set<std::string> mapS;
	mapS.insert("hello");
	mapS.insert("John");
	mapS.insert("John Bob");
	mapS.insert("hlelo");
	mapS.insert("coucou");
	mapS.insert("couscous");
	mapS.insert("max");
	mapS.insert("pastis");
	mapS.insert("pikachu");

	//CONSTRUCTORS

	{
		fileName = currentPath + "constructor.log";
		outfile.open(fileName.c_str());

		S ue;
		displayHashMapForTests(ue, "empty hmap", outfile);

		S ur1(mapS.begin(), mapS.end());
		displayHashMapForTests(ur1, "range constructor hmap", outfile);

		S ur2(mapS.begin(), mapS.end(), 60);
		displayHashMapForTests(ur2, "range constructor hmap 60 buckets", outfile);

		const S uc1(ur1);
		displayHashMapForTests(uc1, "copy constructor", outfile);

		S uc2(uc1);
		displayHashMapForTests(uc2, "copy constructor", outfile);

		uc2 = ue;
		displayHashMapForTests(uc2, "operator=() filled => empty", outfile);

		uc2 = ue;
		displayHashMapForTests(uc2, "operator=() empty => empty", outfile);

		uc2 = uc1;
		displayHashMapForTests(uc2, "operator=() empty => filled", outfile);

		uc2 = uc1;
		displayHashMapForTests(uc2, "operator=() filled => filled", outfile);

		outfile.close();
	}

	//ITERATORS

	{
		fileName = currentPath + "iterator.log";
		outfile.open(fileName.c_str());

		S s(mapS.begin(), mapS.end());
		S sc(s);
		int count = 0;

		forwardIteratorTests(s, outfile);

		S::iterator it = s.begin();
		S::const_iterator cit = sc.begin();
		outfile << it->empty() << " | " << cit->empty() << '\n';

		while (cit != sc.end())
		{
			++count;
			++cit;
		}
		outfile << count << " | ";

		while (it != s.end())
		{
			++count;
			++it;
		}
		outfile << count << '\n';

		outfile.close();
	}

	//INSERT

	{
		fileName = currentPath + "insert.log";
		outfile.open(fileName.c_str());

		pair<F::iterator, bool> inserted;
		F::iterator it;
		F f;
		F f2;
		F f3;

		inserted = f.insert(2.4);
		outfile << "iserted ? " << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert 1 on empty hmap", outfile);
		inserted = f.insert(2.4);
		outfile << "iserted ? " << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same element", outfile);
		inserted = f.insert(2.4);
		outfile << "iserted ? " << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same key", outfile);
		inserted = f.insert(6.477);
		outfile << "iserted ? " << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert 1 other", outfile);
		inserted = f.insert(6.477);
		outfile << "iserted ? " << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same element", outfile);
		inserted = f.insert(6.477);
		outfile << "iserted ? " << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same key", outfile);
		inserted = f.insert(6.747);
		outfile << "iserted ? " << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert 1 other", outfile);
		inserted = f.insert(6.747);
		outfile << "iserted ? " << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same element", outfile);
		inserted = f.insert(6.747);
		outfile << "iserted ? " << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same key", outfile);
		inserted = f.insert(0);
		outfile << "iserted ? " << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert 1 other", outfile);
		inserted = f.insert(0);
		outfile << "iserted ? " << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same element", outfile);
		inserted = f.insert(0);
		outfile << "iserted ? " << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same key", outfile);

		it = f2.insert(f2.end(), 7.1);
		outfile << *it << '\n';
		displayHashMapForTests(f2, "hint insert 1 element", outfile);
		it = f2.insert(f2.end(), 7.1);
		outfile << *it << '\n';
		displayHashMapForTests(f2, "hint insert same element", outfile);
		it = f2.insert(f2.begin(), 5.1);
		outfile << *it << '\n';
		displayHashMapForTests(f2, "hint insert 1 element", outfile);
		it = f2.insert(f2.begin(), -7.1);
		outfile << *it << '\n';
		displayHashMapForTests(f2, "hint insert 1 element", outfile);
		it = f2.insert(++f2.begin(), 7.0);
		outfile << *it << '\n';
		displayHashMapForTests(f2, "hint insert 1 element", outfile);
		it = f2.insert(++f2.begin(), 7);
		outfile << *it << '\n';
		displayHashMapForTests(f2, "hint insert 1 element", outfile);

		const set<float> m(f.begin(), f.end());

		f3.insert(f2.begin(), f2.begin());
		displayHashMapForTests(f3, "range insert nothing", outfile);
		f3.insert(f2.begin(), f2.end());
		displayHashMapForTests(f3, "range insert", outfile);
		f3.insert(f2.begin(), f2.begin());
		displayHashMapForTests(f3, "range insert nothing", outfile);
		f3.insert(++m.begin(), --m.end());
		displayHashMapForTests(f3, "range insert", outfile);

		outfile.close();
	}

	//erase

	{
		fileName = currentPath + "erase.log";
		outfile.open(fileName.c_str());

		I mi;

		for (int i = -30; i < 30; ++i)
		{
			mi.insert(i);
		}

		outfile << mi.erase(12) << '\n';
		outfile << mi.erase(12) << '\n';
		outfile << mi.erase(54) << '\n';
		outfile << mi.erase(1) << '\n';
		outfile << mi.erase(2) << '\n';
		outfile << mi.erase(5) << '\n';
		outfile << mi.erase(5) << '\n';
		outfile << mi.erase(-15) << '\n';

		displayHashMapForTests(mi, "erase by key", outfile, 10);
	
		I mi2(mi);

		I::iterator it = mi2.begin();
		while (it != mi2.end())
			it = mi2.erase(it);

		displayHashMapForTests(mi2, "full erase by iterator", outfile);

		mi.erase(mi.find(0));
		mi.erase(mi.find(-10));
		mi.erase(mi.find(10));
		displayHashMapForTests(mi, "erase by iterator", outfile, 10);

		it = mi.begin();
		++(++it);
		outfile << (it == mi.erase(mi.begin(), it)) << '\n';

		it = mi.erase(mi.begin(), mi.end());
		outfile << (it == mi.end()) << '\n';
		displayHashMapForTests(mi, "erase all by range", outfile, 10);
		it = mi.erase(mi.begin(), mi.end());
		outfile << (it == mi.end()) << '\n';
		displayHashMapForTests(mi, "erase all by range, crash test", outfile, 10);
		outfile.close();
	}

	//CLEAR

	{
		fileName = currentPath + "clear.log";
		outfile.open(fileName.c_str());

		int a = 3;
		int b = 45;
		int c = 4;

		P p;
		p.insert(&a);
		p.insert(&b);
		p.insert(&c);

		p.clear();
		displayHashMapForTests(p, "after clear", outfile);

		p.insert((int*)0);
		p.insert((int*)1);
		p.insert((int*)2);
		displayHashMapForTests(p, "reuse", outfile);

		outfile.close();
	}

	//SWAP

	{
		fileName = currentPath + "swap.log";
		outfile.open(fileName.c_str());

		F f1;
		F f2;

		f1.insert(2.6);
		f1.insert(1.6);
		f1.insert(5.6);
		f1.insert(2.7);
		f1.insert(2.1);

		f2.insert(1);
		f2.insert(3);
		f2.insert(15);
		f2.insert(166666.1);

		f1.swap(f2);
		displayHashMapForTests(f1, "after swap", outfile);
		displayHashMapForTests(f2, "after swap", outfile);

		swap(f2, f1);
		displayHashMapForTests(f1, "again", outfile);
		displayHashMapForTests(f2, "again", outfile);

		outfile.close();
	}

	//FIND + COUNT

	{
		fileName = currentPath + "find.log";
		outfile.open(fileName.c_str());

		int count = 0;

		I m;
		I::const_iterator cit;
		I::iterator it;
		for (int i = -20; i < 20; ++i)
			m.insert((i * i) - i + 4);

		for (int i = -100; i < 100; ++i)
		{
			cit = m.find(i);
			it = m.find(i);
			count += m.count(i);
			if (cit != m.end())
			{
				outfile << *cit << " found " << *it << '\n';
			}
		}

		outfile << count << '\n';

		I m2;
		if (m2.find(9) == m2.end())
			outfile << "crash test\n";
		
		outfile.close();
	}

	//EQUAL_RANGE

	{
		fileName = currentPath + "equal_range.log";
		outfile.open(fileName.c_str());

		I mi;
		pair<I::iterator, I::iterator> its;
		pair<I::const_iterator, I::const_iterator> cits;

		for (int i = 0; i < 300; ++i)
			mi.insert(2 * i);

		its = mi.equal_range(3);
		cits = mi.equal_range(3);

		if (its.first == mi.end() && its.second == mi.end() && cits.first == mi.end() && cits.second == mi.end())
			outfile << "no range for 3\n";

		its = mi.equal_range(8);
		cits = mi.equal_range(8);
		while (its.first != its.second)
		{
			outfile << *(its.first++) << '\n';
		}
		while (cits.first != cits.second)
		{
			outfile << *(cits.first++) << '\n';
		}

		outfile.close();
	}

	//all nexts tests will defer because of different implementation of hash and dynamical reallocation of buckets
	//those log files will not be compared

	//BUCKET

	{
		fileName = currentPath + "NCbucket.log";
		outfile.open(fileName.c_str());

		I mi;
		for (int i = 0; i < 50; ++i)
			mi.insert(3 * (i * i) - (4 * i) + 7);

		displayHashMap(mi, "bucket view", outfile);

		for (I::size_type i = 0; i < mi.bucket_count(); ++i)
		{
			outfile << "bucket " << i << " has a size of " << mi.bucket_size(i) << '\n';
			outfile << i * i << " can be set at bucket " << mi.bucket(i * i) << '\n';
		}

		for (I::size_type i = 0; i < 7; ++i)
		{
			for (I::local_iterator lite = mi.begin(i); lite != mi.end(i); ++lite)
			{
				outfile << *lite << ' ';
			}
		}
		outfile << '\n';

		displayHashMap(mi, "bucket view", outfile);

		mi.rehash(100);
		displayHashMap(mi, "rehash for more bucket", outfile);

		mi.rehash(30);
		displayHashMap(mi, "rehash for less bucket", outfile);

		mi.reserve(130);
		displayHashMap(mi, "reserve more", outfile);

		mi.reserve(40);
		displayHashMap(mi, "reserve less", outfile);

		mi.max_load_factor(0.5);
		displayHashMap(mi, "diminish max_load_factor but insert nothing", outfile);

		mi.insert(6);
		displayHashMap(mi, "diminish max_load_factor and insert existing key", outfile);

		mi.insert(789456);
		displayHashMap(mi, "diminish max_load_factor and insert something", outfile);

		mi.max_load_factor(1.5);
		displayHashMap(mi, "augment max_load_factor but insert nothing", outfile);

		mi.insert(-789456);
		displayHashMap(mi, "augment max_load_factor and insert something", outfile);

		outfile.close();
	}

	//OBSERVERS

	{
		fileName = currentPath + "NCobservers.log";
		outfile.open(fileName.c_str());

		D d;
		D::hasher hash = d.hash_function();
		D::key_equal equal = d.key_eq();
		D::allocator_type alloc = d.get_allocator();
		double pouet = 4.3101;

		outfile << hash(3.409) << '\n';
		outfile << equal(3.09, 3.09) << '|' << equal(4.0849, 4.2918) << '|' << equal(5.009, 1.993) << '\n';
		outfile << alloc.address(pouet) << '\n';

		outfile.close();
	}

	//RELATIONNAL

	{
		fileName = currentPath + "relationnal.log";
		outfile.open(fileName.c_str());

		D d1;
		D d2;

		d1.insert(1.1);
		d1.insert(1.3);
		d1.insert(1.5);
		d1.insert(1.2);
		d1.insert(1.11);
		d1.insert(1.122);
		d1.insert(1.133);
		d1.insert(1.144);

		d2 = d1;

		const D cd1(d1);

		outfile << "must be equal : " << (d2 == d1) << '\n';
		outfile << "must be equal : " << (d2 == cd1) << '\n';
		outfile << "must be equal : " << (d1 == d2) << '\n';
		outfile << "must be equal : " << (cd1 == d2) << '\n';
		outfile << "musn't be different : " << (d2 != d1) << '\n';
		outfile << "musn't be different : " << (d2 != cd1) << '\n';
		outfile << "musn't be different : " << (d1 != d2) << '\n';
		outfile << "musn't be different : " << (cd1 != d2) << '\n';

		d2.rehash(100);

		outfile << "must be equal : " << (d2 == d1) << '\n';

		d1.insert(0);
		outfile << "musn't be equal : " << (d2 == d1) << '\n';
		outfile << "musn't be equal : " << (d1 == cd1) << '\n';
		outfile << "must be different : " << (d1 != d2) << '\n';
		outfile << "must be different : " << (d1 != cd1) << '\n';

		d1.erase(1.2);
		outfile << "musn't be equal : " << (d2 == d1) << '\n';
		outfile << "musn't be equal : " << (d1 == cd1) << '\n';
		outfile << "must be different : " << (d1 != d2) << '\n';
		outfile << "must be different : " << (d1 != cd1) << '\n';

		outfile.close();
	}
}
