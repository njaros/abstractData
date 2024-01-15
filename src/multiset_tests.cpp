#include <sys/types.h>
#include <sys/stat.h>
#include "../includes/test_elements.hpp"

#if FT == 1
	#include "../includes/set.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <set>
	#define FT 0
	using namespace std;
#endif

void	multiset_tests(const std::string& currentPath)
{
	std::string fileName;
	std::ofstream outfile;

	std::boolalpha(outfile);

	//CONSTRUCTORS

	{
		fileName = currentPath + "constructor.log";
		outfile.open(fileName.c_str());

		multiset<std::string> me;
		displayV2(me, "multiset default constructor", outfile);

		me.insert("pouet");
		me.insert("lol");
		me.insert("gigi");
		me.insert("0");

		const multiset<std::string> mc(me);
		displayV2(mc, "const multiset copy constructor", outfile, 2);

		outfile << "must be false : " << (&(*(mc.begin())) == &(*(me.begin()))) << '\n';

		multiset<std::string> mc2(mc);
		displayV2(mc2, "multiset copy constructor with a const multiset as parameter", outfile, 2);

		outfile << "must be false : " << (&(*(mc.begin())) == &(*(mc2.begin()))) << '\n';

		multiset<std::string> mr(++mc.begin(), mc.end());
		displayV2(mr, "multiset range constructor with a const multiset iterators as parameters", outfile, 3);
		
		const multiset<std::string> mce;

		multiset<std::string> me3;

		me = mce;
		displayV2(me, "operator= notEmpty = constEmpty", outfile, 2);

		mc2 = me;
		displayV2(mc2, "operator= notEmpty = empty", outfile, 2);
		
		me3 = mce;
		displayV2(me3, "operator= empty = constEmpty", outfile, 2);

		me3 = mr;
		displayV2(me3, "operator= empty = notEmpty", outfile, 2);

		me3 = mc;
		displayV2(me3, "operator= notEmpty = const notEmpty", outfile, 2);

		me = me3;
		me = me3;
		displayV2(me, "operator= notEmpty = notEmpty", outfile, 2);

		outfile << "must be false " << (&(*(me.begin())) == &(*(me3.begin()))) << '\n';

		outfile.close();
	}

	//ITERATORS

	{
		fileName = currentPath + "iterator.log";
		outfile.open(fileName.c_str());

		multiset<std::string> m;
		for (long long i = 0; i < 9; ++i)
			m.insert(myItoa(myPow(10, i)));

		bidirectionalIteratorTests(m, outfile);
		bidirectionalReverseIteratorTests(m, outfile);

		const multiset<std::string> m2(m);
		multiset<std::string>::iterator it;
		multiset<std::string>::const_iterator cit;
		
		cit = m2.begin();
		outfile << *cit << '\n';
		++cit;
		outfile << cit++->size() << '\n';
		outfile << cit->size() << '\n';

		cit = m2.end();
		--cit;
		outfile << *cit << '\n';
		--cit;
		outfile << cit--->size() << '\n'; //don't worry, I never write that thing in usual codes. It must work anyway
		outfile << cit->size() << '\n';

		it = m.begin();

		outfile << *it << '\n';
		++it;
		outfile << it++->size() << '\n';
		outfile << it->size() << '\n';

		it = m.end();
		--it;
		outfile << it->size() << '\n';
		--it;
		outfile << it--->size() << '\n';
		outfile << it->size() << '\n';

		outfile << *it << '\n';

		outfile.close();
	}

	//INSERT

	{
		fileName = currentPath + "insert.log";
		outfile.open(fileName.c_str());

		multiset<int>::iterator insertResult;
		multiset<int>::iterator hint;
		multiset<int> me;
		multiset<int> range;

		outfile << "single element inserts :\n\n";
		for (int i = -20; i <= 20; ++i)
		{
			insertResult = me.insert(i * i);
			outfile << "inserting " << i * i << " => succeed ? true : " << *insertResult << '\n';			
		}
		displayV2(me, "multiset view after inserts", outfile, 5);

		hint = me.begin();
		hint = me.insert(hint, 5);
		outfile << *hint << '\n';
		displayV2(me, "non sense hint insert", outfile, 5);

		hint = me.insert(hint, 6);
		outfile << *hint << '\n';
		displayV2(me, "more sense hint insert", outfile, 5);

		hint = me.insert(hint, 6);
		outfile << *hint << '\n';
		displayV2(me, "hint insert with same key", outfile, 5);

		for (int i = 0; i < 30; ++i)
		{
			range.insert(i);
			range.insert(i);
		}
		me.insert(range.find(7), range.find(27));
		displayV2(me, "range insert", outfile, 20);

		outfile.close();
	}

	//ERASE

	{
		fileName = currentPath + "erase.log";
		outfile.open(fileName.c_str());

		multiset<int> m;
		multiset<int>::iterator it;
		multiset<int>::size_type seed;
		multiset<int>::size_type count;
		multiset<int>::size_type eraseCount;
		bool started = false;

		seed = 0;
		count = 0;
		eraseCount = 0;
		for (int i = 0; i < 50; ++i)
			m.insert(i);

		for (int i = 0; i < 50; ++i)
			m.insert(i);

		while (m.size() > 30 && (seed || !started))
		{
			started = true;
			++count;
			eraseCount += m.erase(getRandom< multiset<int> >(seed, 50));
		}
		outfile << "number of erase attempts : " << count << " | truely erased : " << eraseCount << '\n';
		displayV2(m, "erase by key", outfile, 6);
		
		for (int i = 0; i < 50; ++i)
			m.insert(i);
		
		started = false;
		seed = 0;
		while (m.size() > 30 && (seed || !started))
		{
			started = true;
			it = m.find(getRandom< multiset<int> >(seed, 50));
			if (it != m.end())
			m.erase(it);
		}

		displayV2(m, "erase by iterator", outfile, 6);

		for (int i = 0; i < 500; ++i)
			m.insert(i);

		m.erase(m.find(4), m.find(490));

		displayV2(m, "erase by range", outfile, 4);

		m.erase(m.find(3), m.end());

		displayV2(m, "erase by range, end() included", outfile, 4);

		m.erase(m.begin(), m.find(1));

		displayV2(m, "erase by range, begin() included, 1 element", outfile, 4);

		m.erase(2);
		m.erase(1);
		displayV2(m, "nothing left", outfile);

		outfile.close();
	}

	//CLEAR

	{
		fileName = currentPath + "clear.log";
		outfile.open(fileName.c_str());

		multiset<std::string> m;

		m.insert("biere");
		m.insert("biere");
		m.insert("animal");
		m.insert("viande");
		m.insert("cocorico");
		m.insert("lol");

		displayV2(m, "before clear()", outfile);
		m.clear();
		displayV2(m, "after clear()", outfile);

		m.insert("42");
		m.insert("Saint Etienne");
		m.insert("Saint Etienne");
		m.insert("multiset");

		displayV2(m, "reuse of cleared multiset", outfile);
		outfile.close();
	}

	//SWAP

	{
		fileName = currentPath + "swap.log";
		outfile.open(fileName.c_str());

		multiset<std::string> m1;
		multiset<std::string> m2;

		const std::string* ptr1;
		const std::string* ptr2;

		m1.insert("chou");
		m1.insert("carotte");
		m1.insert("gentil");
		m1.insert("lapin");
		m1.insert("carotte");
		m1.insert("legume");

		ptr1 = &(*(m1.find("lapin")));

		m2.insert("mois");
		m2.insert("camping");
		m2.insert("juillet");
		m2.insert("vacances");
		m2.insert("camping");
		m2.insert("Franck");

		m1.swap(m2);

		displayV2(m1, "m1 after swap", outfile);
		displayV2(m2, "m2 after swap", outfile);
		
		ptr2 = &(*(m2.find("lapin")));

		outfile << "ptr1 == ptr2 ? " << (ptr1 == ptr2) << '\n';

		swap(m1, m2);

		displayV2(m1, "m1 after swap", outfile);
		displayV2(m2, "m2 after swap", outfile);

		outfile.close();
	}

	//OBSERVERS

	{
		fileName = currentPath + "observer.log";
		outfile.open(fileName.c_str());

		multiset<int, greater<int> > m;

		m.insert(1);
		m.insert(3);
		m.insert(9);
		m.insert(12);
		m.insert(1);
		m.insert(3);
		m.insert(9);
		m.insert(12);

		outfile << m.value_comp()(*(m.find(12)), *(m.find(9))) << ' ' <<
			m.value_comp()(*(m.find(1)), *(m.find(9))) << ' ' <<
			m.value_comp()(*(m.find(3)), *(m.find(3))) << ' ' <<
			m.value_comp()(*(m.find(3)), *(m.find(12))) << '\n';

		outfile << m.key_comp()(2, 9) << ' ' <<
			m.key_comp()(9, 9) << ' ' <<
			m.key_comp()(12, 9) << ' ' <<
			m.key_comp()(1, 2) << '\n';

		multiset<int> m2;

		m2.insert(1);
		m2.insert(3);
		m2.insert(9);
		m2.insert(12);
		m2.insert(1);
		m2.insert(3);
		m2.insert(9);
		m2.insert(12);

		outfile << m2.value_comp()(*(m2.find(12)), *(m2.find(9))) << ' ' <<
			m2.value_comp()(*(m2.find(1)), *(m2.find(9))) << ' ' <<
			m2.value_comp()(*(m2.find(3)), *(m2.find(3))) << ' ' <<
			m2.value_comp()(*(m2.find(3)), *(m2.find(12))) << '\n';

		outfile << m2.key_comp()(2, 9) << ' ' <<
			m2.key_comp()(9, 9) << ' ' <<
			m2.key_comp()(12, 9) << ' ' <<
			m2.key_comp()(1, 2) << '\n';


		outfile.close();
	}

	//OPERATION

	{
		fileName = currentPath + "operation.log";
		outfile.open(fileName.c_str());

		multiset<int>::size_type count = 0;

		multiset<int>::iterator it;
		multiset<int>::iterator it2;
		multiset<int>::const_iterator cit;
		multiset<int>::const_iterator cit2;

		multiset<int> m;
		for (int i = 0; i < 501; i += 5)
		{
			m.insert(i + 2);
			m.insert(i + 3);
			m.insert(i + 2);
			m.insert(i + 3);
		}
		const multiset<int> m2(m);

		outfile << "====find====\n\n";

		cit = m.find(1);
		outfile << (cit == m.end()) << '\n';

		it = m.find(1);
		outfile << (it == cit) << '\n';

		cit = m2.find(1);
		outfile << (cit == m2.end()) << '\n';

		it = m.find(3);
		outfile << *it << '\n';

		cit = m.find(7);
		outfile << *cit << '\n';

		cit = m2.find(8);
		outfile << *cit << '\n';

		outfile << *it << " muse be same as " << *(m.find(3)) << '\n';


		outfile << "\n====count====\n\n";
		for (int i = 0; i < 501; ++i)
		{
			count += m.count(i);
			count += m2.count(i);
		}
		outfile << count << '\n';

		outfile << "\n====lower_bound====\n\n";

		outfile << *(m.lower_bound(2)) << '\n';
		outfile << *(m.lower_bound(0)) << '\n';
		outfile << *(m.lower_bound(5)) << '\n';
		outfile << *(m.lower_bound(7)) << '\n';

		outfile << *(m2.lower_bound(2)) << '\n';
		outfile << *(m2.lower_bound(0)) << '\n';
		outfile << *(m2.lower_bound(5)) << '\n';
		outfile << *(m2.lower_bound(7)) << '\n';

		outfile << "\n====upper_bound====\n\n";

		outfile << *(m.upper_bound(2)) << '\n';
		outfile << *(m.upper_bound(0)) << '\n';
		outfile << *(m.upper_bound(5)) << '\n';
		outfile << *(m.upper_bound(8)) << '\n';

		outfile << *(m2.upper_bound(2)) << '\n';
		outfile << *(m2.upper_bound(0)) << '\n';
		outfile << *(m2.upper_bound(5)) << '\n';
		outfile << *(m2.upper_bound(7)) << '\n';

		outfile << "\n====equal_range====\n\n";

		count = 0;

		it = m.equal_range(53).first;
		it2 = m.equal_range(53).second;

		cit = m2.equal_range(103).first;
		cit2 = m2.equal_range(103).second;

		outfile << (it == it2) << '\n';
		outfile << (cit == cit2) << '\n';

		it = m.equal_range(10).first;
		it2 = m.equal_range(10).second;

		cit = m2.equal_range(100).first;
		cit2 = m2.equal_range(100).second;

		outfile << (it == it2) << '\n';
		outfile << (cit == cit2) << '\n';

		it = m.equal_range(401).first;
		it2 = m.equal_range(401).second;

		cit = m2.equal_range(101).first;
		cit2 = m2.equal_range(101).second;

		outfile << (it == it2) << '\n';
		outfile << (cit == cit2) << '\n';

		it = m.equal_range(4000).first;
		it2 = m.equal_range(4000).second;

		cit = m2.equal_range(10000).first;
		cit2 = m2.equal_range(10000).second;

		outfile << (it == it2) << '\n';
		outfile << (cit == cit2) << '\n';

		outfile << (it == m.end()) << ' ' << (cit == m2.end()) << '\n';

		outfile.close();
	}

	//GET_ALLOCATOR

	{
		fileName = currentPath + "get_allocator.log";
		outfile.open(fileName.c_str());

		multiset<char> m;
		std::allocator<char> cpy = m.get_allocator();
		// must not crash

		outfile.close();
	}

	//RELATIONAL

	{
		fileName = currentPath + "relational.log";
		outfile.open(fileName.c_str());

		multiset<int> m1;
		multiset<int> m2;
		multiset<int> m3;
		multiset<int> m4;

		for (int i = 0; i < 10; ++i)
		{
			m1.insert(i);
			m2.insert(i + 1);
			m3.insert(i);
			m4.insert(i);
			m1.insert(i);
			m2.insert(i + 1);
			m3.insert(i);
			m4.insert(i);
			m1.insert(i);
			m2.insert(i + 1);
			m4.insert(i);
		}
		m3.erase(9);
		m3.insert(10);
		m3.insert(10);
		m4.erase(9);
		m4.insert(-1);
		m4.insert(-1);
		const multiset<int> mc1(m1);

		outfile << (m1 == m2) << '|' << (m1 == m3) << '|' << (m1 == mc1) << '\n';
		outfile << (m1 != m2) << '|' << (m1 != m3) << '|' << (m1 != mc1) << '\n';
		outfile << (m1 < m2) << '|' << (m1 < m3) << '|' << (m1 < m4) << '|' << (m1 < mc1) << '\n';
		outfile << (m1 > m2) << '|' << (m1 > m3) << '|' << (m1 > m4) << '|' << (m1 > mc1) << '\n';
		outfile << (m1 <= m2) << '|' << (m1 <= m3) << '|' << (m1 <= m4) << '|' << (m1 <= mc1) << '\n';
		outfile << (m1 >= m2) << '|' << (m1 >= m3) << '|' << (m1 >= m4) << '|' << (m1 >= mc1) << '\n';

		m1.insert(10);

		outfile << (m1 == m2) << '|' << (m1 == m3) << '|' << (m1 == mc1) << '\n';
		outfile << (m1 != m2) << '|' << (m1 != m3) << '|' << (m1 != mc1) << '\n';
		outfile << (m1 < m2) << '|' << (m1 < m3) << '|' << (m1 < m4) << '|' << (m1 < mc1) << '\n';
		outfile << (m1 > m2) << '|' << (m1 > m3) << '|' << (m1 > m4) << '|' << (m1 > mc1) << '\n';
		outfile << (m1 <= m2) << '|' << (m1 <= m3) << '|' << (m1 <= m4) << '|' << (m1 <= mc1) << '\n';
		outfile << (m1 >= m2) << '|' << (m1 >= m3) << '|' << (m1 >= m4) << '|' << (m1 >= mc1) << '\n';

		outfile.close();
	}

}