#include <sys/types.h>
#include <sys/stat.h>
#include "../includes/test_elements.hpp"

#if FT == 1
	#include "../includes/unordered_map.hpp"
	#include "../includes/map.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <unordered_map>
	#include <map>
	#define FT 0
	using namespace std;
#endif

void unordered_map_tests(const std::string& currentPath, std::ostream& except)
{
	std::string fileName;
	std::ofstream outfile;

	std::boolalpha(outfile);

	typedef unordered_map<std::string, std::string> S;
	typedef unordered_map<int, int> I;
	typedef unordered_map<float, int> F;
	typedef unordered_map<double, int> D;
	typedef unordered_map<int*, int> P;

	map<std::string, std::string> mapS;
	mapS["hello"] = "java";
	mapS["John"] = "discord";
	mapS["John Bob"] = "suisse";
	mapS["hlelo"] = "patate";
	mapS["coucou"] = "bateau";
	mapS["couscous"] = "calembredaine";
	mapS["max"] = "master";
	mapS["pastis"] = "ricard";
	mapS["pikachu"] = "pikachu";

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
		outfile << it->first.empty() << " | " << cit->first.empty() << '\n';

		it->second = "pouet";
		outfile << it->second << '\n';

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

	//ACCESS

	{
		fileName = currentPath + "access.log";
		outfile.open(fileName.c_str());

		unordered_map<int, int> m;

		m[2] = 3;
		outfile << m[2] << '\n';
		m[2] = 5;
		outfile << m[2] << " | " << m.at(2) << '\n';
		m.at(2) = 8;
		outfile << m[2] << " | " << m.at(2) << '\n';

		const unordered_map<int, int> mc(m);
		outfile << mc.at(2) << '\n';

		try
		{
			mc.at(3);
		}
		catch(const out_of_range& e)
		{
			except << "UNORDERED_MAP : const : at : out_of_range\n" << e.what() << '\n';
		}

		try
		{
			m.at(3);
		}
		catch(const out_of_range& e)
		{
			except << "UNORDERED_MAP : not const : at : out_of_range\n" << e.what() << '\n';
		}
		
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

		inserted = f.insert(make_pair(2.4, 7));
		outfile << "inserted ?" << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert 1 on empty hmap", outfile);
		inserted = f.insert(make_pair(2.4, 7));
		outfile << "inserted ?" << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same element", outfile);
		inserted = f.insert(make_pair(2.4, 8));
		outfile << "inserted ?" << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same key", outfile);
		inserted = f.insert(make_pair(6.477, 7));
		outfile << "inserted ?" << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert 1 other", outfile);
		inserted = f.insert(make_pair(6.477, 7));
		outfile << "inserted ?" << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same element", outfile);
		inserted = f.insert(make_pair(6.477, 8));
		outfile << "inserted ?" << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same key", outfile);
		inserted = f.insert(make_pair(6.747, 7));
		outfile << "inserted ?" << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert 1 other", outfile);
		inserted = f.insert(make_pair(6.747, 7));
		outfile << "inserted ?" << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same element", outfile);
		inserted = f.insert(make_pair(6.747, 8));
		outfile << "inserted ?" << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same key", outfile);
		inserted = f.insert(make_pair(0, 7));
		outfile << "inserted ?" << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert 1 other", outfile);
		inserted = f.insert(make_pair(0, 7));
		outfile << "inserted ?" << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same element", outfile);
		inserted = f.insert(make_pair(0, 8));
		outfile << "inserted ?" << inserted.second << " : " << *(inserted.first) << '\n';
		displayHashMapForTests(f, "insert same key", outfile);

		it = f2.insert(f2.end(), make_pair(7.1, 2));
		outfile << *it << '\n';
		displayHashMapForTests(f2, "hint insert 1 element", outfile);
		it = f2.insert(f2.end(), make_pair(7.1, 2));
		outfile << *it << '\n';
		displayHashMapForTests(f2, "hint insert same element", outfile);
		it = f2.insert(f2.begin(), make_pair(5.1, 2));
		outfile << *it << '\n';
		displayHashMapForTests(f2, "hint insert 1 element", outfile);
		it = f2.insert(f2.begin(), make_pair(-7.1, 2));
		outfile << *it << '\n';
		displayHashMapForTests(f2, "hint insert 1 element", outfile);
		it = f2.insert(++f2.begin(), make_pair(7.0, 2));
		outfile << *it << '\n';
		displayHashMapForTests(f2, "hint insert 1 element", outfile);
		it = f2.insert(++f2.begin(), make_pair(7, 2));
		outfile << *it << '\n';
		displayHashMapForTests(f2, "hint insert 1 element", outfile);

		const map<float, int> m(f.begin(), f.end());

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
			mi.insert(make_pair(i, -i));
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
		outfile << (it == mi.erase(mi.begin(), ++(++(mi.begin())))) << '\n';

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
		p.insert(make_pair(&a, a));
		p.insert(make_pair(&b, b));
		p.insert(make_pair(&c, c));

		p.clear();
		displayHashMapForTests(p, "after clear", outfile);

		p.insert(make_pair((int*)0, a));
		p.insert(make_pair((int*)1, b));
		p.insert(make_pair((int*)2, c));
		displayHashMapForTests(p, "reuse", outfile);

		outfile.close();
	}

	//SWAP

	{
		fileName = currentPath + "swap.log";
		outfile.open(fileName.c_str());

		F f1;
		F f2;

		f1[2.6] = 4;
		f1[1.6] = 7;
		f1[5.6] = 411;
		f1[2.7] = 415;
		f1[2.1] = 74;

		f2[1] = 1;
		f2[3] = 12;
		f2[15] = 31;
		f2[166666.1] = 14;

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
			m[(i * i) - i + 4] = i;

		for (int i = -100; i < 100; ++i)
		{
			cit = m.find(i);
			it = m.find(i);
			count += m.count(i);
			if (cit != m.end())
			{
				outfile << *cit << " found\n";
				it->second = it->first / 10;
				outfile << *it << '\n';
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
		{
			mi[2 * i] = -i;
			mi[2 * i] = i;
			mi[2 * i] = 15 * i;
			mi[2 * i] = i;
		}

		its = mi.equal_range(3);
		cits = mi.equal_range(3);

		if (its.first == mi.end() && its.second == mi.end() && cits.first == mi.end() && cits.second == mi.end())
			outfile << "no range for 3\n";

		its = mi.equal_range(8);
		cits = mi.equal_range(8);
		while (its.first != its.second)
		{
			its.first->second = its.first->first * 5;
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
			mi[3 * (i * i) - (4 * i) + 7] = i;

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
				lite->second = 42;
			}
		}

		displayHashMap(mi, "bucket view after use of bucket iterator (local_iterator), begin(i) and end(i)", outfile);

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

		mi.insert(make_pair(6, 123456));
		displayHashMap(mi, "diminish max_load_factor and insert existing key", outfile);

		mi.insert(make_pair(789456, 123456));
		displayHashMap(mi, "diminish max_load_factor and insert something", outfile);

		mi.max_load_factor(1.5);
		displayHashMap(mi, "augment max_load_factor but insert nothing", outfile);

		mi.insert(make_pair(-789456, 123456));
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
		pair<const double, int> pouet(4.310, 1);

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

		d1.insert(make_pair(1.1, 2));
		d1.insert(make_pair(1.3, 2));
		d1.insert(make_pair(1.5, 2));
		d1.insert(make_pair(1.2, 2));
		d1.insert(make_pair(1.11, 2));
		d1.insert(make_pair(1.122, 2));
		d1.insert(make_pair(1.133, 2));
		d1.insert(make_pair(1.144, 2));

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

		d1.insert(make_pair(0, 0));
		outfile << "musn't be equal : " << (d2 == d1) << '\n';
		outfile << "musn't be equal : " << (d1 == cd1) << '\n';
		outfile << "must be different : " << (d1 != d2) << '\n';
		outfile << "must be different : " << (d1 != cd1) << '\n';

		d1.erase(1.2);
		outfile << "musn't be equal : " << (d2 == d1) << '\n';
		outfile << "musn't be equal : " << (d1 == cd1) << '\n';
		outfile << "must be different : " << (d1 != d2) << '\n';
		outfile << "must be different : " << (d1 != cd1) << '\n';

		d1.insert(make_pair(5.5, 3));
		d2.insert(make_pair(5.5, 4));
		outfile << "musn't be equal : " << (d2 == d1) << '\n';
		outfile << "must be different : " << (d1 != d2) << '\n';

		outfile.close();
	}

}
