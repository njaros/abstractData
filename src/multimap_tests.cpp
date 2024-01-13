#include <sys/types.h>
#include <sys/stat.h>
#include "../includes/test_elements.hpp"

#if FT == 1
	#include "../includes/map.hpp"
	#define FT 1
	using namespace ft;
#else
	#include <map>
	#define FT 0
	using namespace std;
#endif

void	multimap_tests(const std::string& currentPath, std::ostream& except)
{
	std::string fileName;
	std::ofstream outfile;

	std::boolalpha(outfile);

	//CONSTRUCTORS

	{
		fileName = currentPath + "constructor.log";
		outfile.open(fileName.c_str());

		multimap<int, std::string> me;
		displayV2(me, "map default constructor", outfile);

		me[3] = "pouet";
		me[2] = "lol";
		me[8] = "gigi";
		me[0] = "0";

		const multimap<int, std::string> mc(me);
		displayV2(mc, "const multimap copy constructor", outfile, 2);

		multimap<int, std::string> mc2(mc);
		displayV2(mc2, "multimap copy constructor with a const multimap as parameter", outfile, 2);

		mc2[2] = "another value";
		outfile << mc2[2] << " was a deep copy of " << mc.at(2) << '\n';

		multimap<int, std::string> mr(++mc.begin(), mc.end());
		displayV2(mr, "multimap range constructor with a const multimap iterators as parameters", outfile, 3);
		
		const multimap<int, std::string> mce;

		multimap<int, std::string> me3;

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

		me[0] = "another value";
		outfile << me[0] << " was a deep copy of " << me3[0] << '\n';

		outfile.close();
	}

	//ITERATORS

	{
		fileName = currentPath + "iterator.log";
		outfile.open(fileName.c_str());

		multimap<int, int> m;
		for (int i = 0; i < 30; ++i)
			m[i] = i * i - i;

		bidirectionalIteratorTests(m, outfile);
		bidirectionalReverseIteratorTests(m, outfile);

		const multimap<int, int> m2(m);
		multimap<int, int>::iterator it;
		multimap<int, int>::const_iterator cit;
		
		cit = m2.begin();
		outfile << cit->first << " | " << cit->second << '\n';
		++cit;
		outfile << cit++->first << " | " << cit++->second<< '\n';
		outfile << cit->first << " | " << cit->second << '\n';

		cit = m2.end();
		--cit;
		outfile << cit->first << " | " << cit->second << '\n';
		--cit;
		outfile << cit--->first << " | " << cit--->second << '\n'; //don't worry, I never write that thing in usual codes. It must work anyway
		outfile << cit->first << " | " << cit->second << '\n';

		it = m.begin();

		outfile << it->first << " | " << it->second << '\n';
		++it;
		outfile << it++->first << " | " << it++->second << '\n';
		outfile << it->first << " | " << it->second << '\n';

		it = m.end();
		--it;
		outfile << it->first << " | " << it->second << '\n';
		--it;
		outfile << it--->first << " | " << it--->second << '\n';
		outfile << it->first << " | " << it->second << '\n';

		it->second = 456;
		outfile << *it << '\n';

		outfile.close();
	}

	//ACCESS

	{
		fileName = currentPath + "access.log";
		outfile.open(fileName.c_str());
		multimap<int, int> m;

		m[2] = 3;
		outfile << m[2] << '\n';
		m[2] = 5;
		outfile << m[2] << " | " << m.at(2) << '\n';
		m.at(2) = 8;
		outfile << m[2] << " | " << m.at(2) << '\n';

		const multimap<int, int> mc(m);
		outfile << mc.at(2) << '\n';

		try
		{
			mc.at(3);
		}
		catch(const out_of_range& e)
		{
			except << "MULTIMAP : const : at : out_of_range\n" << e.what() << '\n';
		}

		try
		{
			m.at(3);
		}
		catch(const out_of_range& e)
		{
			except << "MULTIMAP : not const : at : out_of_range\n" << e.what() << '\n';
		}

		outfile.close();
	}

	//INSERT

	{
		fileName = currentPath + "insert.log";
		outfile.open(fileName.c_str());

		multimap<int, int>::iterator insertResult;
		multimap<int, int>::iterator hint;
		multimap<int, int> me;
		multimap<int, int> range;
		pair<int, int> p;

		outfile << "single element inserts :\n\n";
		for (int i = -20; i <= 20; ++i)
		{
			p.first = i * i;
			p.second = i;
			insertResult = me.insert(p);
			outfile << "inserting " << p << " => succeed ? true" << ". "
					<< insertResult->first << " => " << insertResult->second << '\n';			
		}
		displayV2(me, "multimap view after inserts", outfile, 5);

		hint = me.begin();
		p.first = 5;
		hint = me.insert(hint, p);
		outfile << *hint << '\n';
		displayV2(me, "non sense hint insert", outfile, 5);

		p.first += 1;
		hint = me.insert(hint, p);
		outfile << *hint << '\n';
		displayV2(me, "more sense hint insert", outfile, 5);

		p.second = 0;
		hint = me.insert(hint, p);
		outfile << *hint << '\n';
		displayV2(me, "hint insert with same key", outfile, 5);

		for (int i = 0; i < 30; ++i)
			range[i] = -i;
		me.insert(range.find(7), range.find(27));
		displayV2(me, "range insert", outfile, 7);

		outfile.close();
	}

	//ERASE

	{
		multimap<int, int> m;
		multimap<int, int>::size_type seed;
		multimap<int, int>::size_type count;
		multimap<int, int>::size_type eraseCount;
		bool started = false;

		seed = 0;
		count = 0;
		eraseCount = 0;
		for (int i = 0; i < 50; ++i)
			m[i] = i;

		for (int i = 0; i < 50; ++i)
			m[i] = i;

		while (m.size() > 30 && (seed || !started))
		{
			started = true;
			++count;
			eraseCount += m.erase(getRandom< multimap<int, int> >(seed, 50));
		}
		outfile << "number of erase attempts : " << count << " | truely erased : " << eraseCount << '\n';
		displayV2(m, "erase by key", outfile, 6);
		
		for (int i = 0; i < 50; ++i)
			m[i] = i;
		
		started = false;
		seed = 0;
		while (m.size() > 30 && (seed || !started))
		{
			started = true;
			m.erase(m.find(getRandom< multimap<int, int> >(seed, 50)));
		}

		displayV2(m, "erase by iterator", outfile, 6);

		for (int i = 0; i < 500; ++i)
			m[i] = i * i;

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

		multimap<std::string, std::string> m;

		m["alcool"] = "biere";
		m["alcool"] = "biere";
		m["chat"] = "animal";
		m["viande"] = "chat";
		m["cocorico"] = "France";
		m["pouet"] = "lol";

		displayV2(m, "before clear()", outfile);
		m.clear();
		displayV2(m, "after clear()", outfile);

		m["ecole"] = "42";
		m["42"] = "Saint Etienne";
		m["42"] = "Saint Etienne";
		m["multimap"] = "still working fine after clear";

		displayV2(m, "reuse of cleared multimap", outfile);

		outfile.close();
	}

	//SWAP

	{
		fileName = currentPath + "swap.log";
		outfile.open(fileName.c_str());

		multimap<std::string, std::string> m1;
		multimap<std::string, std::string> m2;

		std::string* ptr1;
		std::string* ptr2;

		m1["legume"] = "chou";
		m1["carotte"] = "aimable";
		m1["gentil"] = "lapin";
		m1["lapin"] = "carotte";
		m1["legume"] = "on boucle la non ?";

		ptr1 = &(m1["lapin"]);

		m2["mois"] = "juillet";
		m2["juillet"] = "vacances";
		m2["vacances"] = "camping";
		m2["vacances"] = "camping";
		m2["camping"] = "Franck";
		m2["Franck"] = "y Vincent";

		m1.swap(m2);

		displayV2(m1, "m1 after swap", outfile);
		displayV2(m2, "m2 after swap", outfile);
		
		ptr2 = &(m2["lapin"]);

		outfile << "ptr1 == ptr2 ? " << ptr1 == ptr2 << '\n';

		swap(m1, m2);

		displayV2(m1, "m1 after swap", outfile);
		displayV2(m2, "m2 after swap", outfile);

		outfile.close();
	}

	//OBSERVERS

	{
		fileName = currentPath + "observer.log";
		outfile.open(fileName.c_str());

		multimap<int, int, greater<int> > m;

		m[1] = 4;
		m[3] = 3;
		m[9] = 0;
		m[12] = 4;

		outfile << m.value_comp()(*(m.find(12)), *(m.find(9))) << ' ' <<
			m.value_comp()(*(m.find(1)), *(m.find(9))) << ' ' <<
			m.value_comp()(*(m.find(3)), *(m.find(3))) << ' ' <<
			m.value_comp()(*(m.find(3)), *(m.find(12))) << '\n';

		outfile << m.key_comp()(2, 9) << ' ' <<
			m.key_comp()(9, 9) << ' ' <<
			m.key_comp()(12, 9) << ' ' <<
			m.key_comp()(1, 2) << '\n';

		multimap<int, int> m2;

		m2[1] = 4;
		m2[3] = 3;
		m2[9] = 0;
		m2[12] = 4;

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

		multimap<int, int>::size_type count = 0;

		multimap<int, int>::iterator it;
		multimap<int, int>::iterator it2;
		multimap<int, int>::const_iterator cit;
		multimap<int, int>::const_iterator cit2;

		multimap<int, int> m;
		for (int i = 0; i < 501; i += 5)
		{
			m[i + 2] = i;
			m[i + 3] = i;
			m[i + 2] = i;
			m[i + 3] = i;
		}
		const multimap<int, int> m2(m);

		outfile << "====find====\n\n"

		cit = m.find(1);
		outfile << cit == m.end() << '\n';

		it = m.find(1);
		outfile << it == cit << '\n';

		cit = m2.find(1);
		outfile << cit == m2.end() << '\n';

		it = m.find(3);
		outfile << *it << '\n';

		cit = m.find(7);
		outfile << *cit << '\n';

		cit = m2.find(8);
		outfile << *cit << '\n';

		it->second = -11;

		outfile << *it << " muse be same as " << *(m.find(3)) << '\n';


		outfile << "\n====count====\n\n"
		for (int i = 0; i < 501; ++i)
		{
			count += m.count(i);
			count += m2.count(i);
		}
		outfile << count << '\n';

		outfile << "\n====lower_bound====\n\n";

		m.lower_bound(0) = -58;

		outfile << *(m.lower_bound(2)) << '\n';
		outfile << *(m.lower_bound(0)) << '\n';
		outfile << *(m.lower_bound(5)) << '\n';
		outfile << *(m.lower_bound(7)) << '\n';

		outfile << *(m2.lower_bound(2)) << '\n';
		outfile << *(m2.lower_bound(0)) << '\n';
		outfile << *(m2.lower_bound(5)) << '\n';
		outfile << *(m2.lower_bound(7)) << '\n';

		outfile << "\n====upper_bound====\n\n";

		m.upper_bound(2) = 789;

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

		outfile << it == it2 << '\n';
		outfile << cit == cit2 << '\n';

		it = m.equal_range(10).first;
		it2 = m.equal_range(10).second;

		cit = m2.equal_range(100).first;
		cit2 = m2.equal_range(100).second;

		outfile << it == it2 << '\n';
		outfile << cit == cit2 << '\n';

		it = m.equal_range(401).first;
		it2 = m.equal_range(401).second;

		cit = m2.equal_range(101).first;
		cit2 = m2.equal_range(101).second;

		outfile << it == it2 << '\n';
		outfile << cit == cit2 << '\n';

		it = m.equal_range(4000).first;
		it2 = m.equal_range(4000).second;

		cit = m2.equal_range(10000).first;
		cit2 = m2.equal_range(10000).second;

		outfile << it == it2 << '\n';
		outfile << cit == cit2 << '\n';

		outfile << it == m.end() << ' ' << cit == m2.end() << '\n';

		outfile.close();
	}

	//GET_ALLOCATOR

	{
		fileName = currentPath + "get_allocator.log";
		outfile.open(fileName.c_str());

		char adr = 'o';
		char otherchar = 'u';
		dummyAllocator<char> dumb(&adr);
		multimap<char, char, less<char>, dummyAllocator<char> > m(less<char>(), dumb);
		dummyAllocator<char> cpy = m.get_allocator();
		outfile << *cpy.adresse(otherchar) << " must be equal to " << *dumb.adresse(otherchar);

		outfile.close();
	}

	//RELATIONAL

	{
		fileName = currentPath + "relational.log";
		outfile.open(fileName.c_str());

		multimap<int, int> m1;
		multimap<int, int> m2;
		multimap<int, int> m3;
		multimap<int, int> m4;

		for (int i = 0; i < 10; ++i)
		{
			m1[i] = i;
			m2[i] = i + 1;
			m3[i] = i;
			m4[i] = i;
			m1[i] = i;
			m2[i] = i + 1;
			m3[i] = i;
			m4[i] = i;
			m1[i] = i;
			m2[i] = i + 1;
			m4[i] = i;
		}
		m4[9] = 8;
		const multimap<int, int> mc1(m1);

		outfile << m1 == m2 << '|' << m1 == m3 << '|' << m1 == mc1 << '\n';
		outfile << m1 != m2 << '|' << m1 != m3 << '|' << m1 != mc1 << '\n';
		outfile << m1 < m2 << '|' << m1 < m3 << '|' << m1 < m4 << '|' << m1 < mc1 << '\n';
		outfile << m1 > m2 << '|' << m1 > m3 << '|' << m1 > m4 << '|' << m1 > mc1 << '\n';
		outfile << m1 <= m2 << '|' << m1 <= m3 << '|' << m1 <= m4 << '|' << m1 <= mc1 << '\n';
		outfile << m1 >= m2 << '|' << m1 >= m3 << '|' << m1 >= m4 << '|' << m1 >= mc1 << '\n';

		m1[10] = 10;

		outfile << m1 == m2 << '|' << m1 == m3 << '|' << m1 == mc1 << '\n';
		outfile << m1 != m2 << '|' << m1 != m3 << '|' << m1 != mc1 << '\n';
		outfile << m1 < m2 << '|' << m1 < m3 << '|' << m1 < m4 << '|' << m1 < mc1 << '\n';
		outfile << m1 > m2 << '|' << m1 > m3 << '|' << m1 > m4 << '|' << m1 > mc1 << '\n';
		outfile << m1 <= m2 << '|' << m1 <= m3 << '|' << m1 <= m4 << '|' << m1 <= mc1 << '\n';
		outfile << m1 >= m2 << '|' << m1 >= m3 << '|' << m1 >= m4 << '|' << m1 >= mc1 << '\n';

		outfile.close();
	}

}