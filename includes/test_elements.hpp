#ifndef TESTS_ELEMENTS_HPP_ABSTRACTDATA_BY_LE_N
# define TESTS_ELEMENTS_HPP_ABSTRACTDATA_BY_LE_N

#include <fstream>
#include <iostream>
#include <cstddef>
#include <memory>
#include <utility>
#include <../includes/utility.hpp>
#if FT==1
	#include "map.hpp"
	#include "set.hpp"
	using namespace ft;
#else
	#include <set>
	#include <map>
	using namespace std;
#endif

// COMMON TOOLS

std::string	myItoa(long long nb);
long long myPow(long long a, long long b);
double myFabs(double d);
size_t getRandom(size_t& seed, size_t modulo);

// END OF COMMON TOOLS

// COMMON TESTS LIBRARY

template <class T, class U>
std::ostream&	operator<<(std::ostream& o, const ft::pair<T, U>& p)
	{
		o << '(' << p.first << ", " << p.second << ')';
		return o;
	}

template <class T, class U>
	std::ostream&	operator<<(std::ostream& o, const std::pair<T, U>& p)
	{
		o << '(' << p.first << ", " << p.second << ')';
		return o;
	}

template < class T >
class dummyAllocator
{
	public :

	typedef T			value_type;
	typedef T*			pointer;
	typedef const T*	const_pointer;
	typedef T&			reference;
	typedef const T&	const_reference;
	typedef size_t		size_type;
	typedef ptrdiff_t	difference_type;
	template <class U> struct rebind { typedef dummyAllocator<U> other; };

	dummyAllocator() : dummyValue(0) {}
	dummyAllocator(pointer t) : dummyValue(t) {}
	dummyAllocator(const dummyAllocator& o) : dummyValue(o.dummyValue) {}
	~dummyAllocator() {}

	dummyAllocator& operator=(const dummyAllocator& o)
	{
		dummyValue = o.dummyValue;
		return *this;
	}

	pointer adresse( reference x )
	{
		(void) x;
		return dummyValue;
	}

	pointer allocate(size_type n, pointer hint = 0)
	{
		return dummyValue + n + hint;
	}

	void	deallocate(pointer p, size_type n)
	{
		(void) p;
		(void) n;
	}

	size_type max_size() const throw()
	{
		return 0;
	}

	void construct( pointer p, const_reference val)
	{
		(void) p;
		(void) val;
	}

	void destroy( pointer p )
	{
		(void) p;
	}
	private :

	pointer dummyValue;
};

template < typename T >
class dummyAllocator2 : public std::allocator< T >
{
	public :

	int i;

	dummyAllocator2() : std::allocator<T>()
	{
		i = 0;
	}

	dummyAllocator2(int nb) : std::allocator<T>()
	{
		i = nb;
	}

	dummyAllocator2(const dummyAllocator2& o) : std::allocator<T>(o)
	{
		i = o.i;
	}
};

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
void	displayHashMap(T& container, const char* name, std::ostream& outfile)
{
	typename T::const_local_iterator clit;

	outfile << "----------> " << name << " <---------\n";
	outfile << "empty : " << container.empty() << '\n';
	outfile << "size : " << container.size() << '\n';
	outfile << "bucket count : " << container.bucket_count() << '\n';
	outfile << "max_bucket count : " << container.max_bucket_count() << '\n';
	outfile << "load_factor : " << container.load_factor() << '\n';
	outfile << "max_load_factor : " << container.max_load_factor() << '\n';
	if (!container.empty())
	{
		outfile << name << " content : \n";
		for (size_t i = 0; i < container.bucket_count(); ++i)
		{
			if (container.bucket_size(i))
			{
				outfile << "bucket[" << i << "] : {";
				clit = container.begin(i);
				while (clit != container.end(i))
				{
					outfile << *clit;
					if (++clit != container.end(i))
						outfile << ", ";
					else
						outfile << "}\n";
				}
			}
		}
	}
	outfile << '\n';
}

template < typename T >
void	displayHashMapForTests(T& container, const char* name, std::ostream& outfile, typename T::size_type eltsPerLine = 1)
{
	typename T::const_local_iterator clit;
	multiset<typename T::value_type> sort;

	outfile << "----------> " << name << " <---------\n";
	outfile << "empty : " << container.empty() << '\n';
	outfile << "size : " << container.size() << '\n';
	outfile << "max_size : " << container.max_size() << '\n';
	if (eltsPerLine && !container.empty())
	{
		sort.insert(container.begin(), container.end());
		displayData(sort, outfile, eltsPerLine);
	}
	else
		outfile << '\n';
}

//std reallocation implementation defers according to OS used, so compare capacity is non sense.
template < typename T >
void	displayV2(const T& container, const char* name, std::ostream& stream, typename T::size_type eltsPerLine = 1)
{
	stream << "----------> " << name << " <---------\n";
	stream << "is empty : " << container.empty() << '\n';
	stream << "size " << " : " << container.size() << '\n';
	stream << "max_size " << " : " << container.max_size() << "\n";
	if (eltsPerLine)
		displayData(container, stream, eltsPerLine);
	else
		stream << '\n';
}

//allows to display content of containers with casting the type as int. usefull to display whitespaces
template < typename T >
void	displayDataCastInt(const T& container, const char* name, std::ostream& stream, typename T::size_type eltsPerLine = 1)
{
	typename T::const_iterator	it;
	typename T::const_iterator	end;
	typename T::size_type		eltCount;

	if (eltsPerLine > container.size())
		eltsPerLine = container.size();
	it = container.begin();
	end = container.end();
	stream << name << " content : " << '\n';
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
		stream << (int)*it;
		it++;
		++eltCount;
	}
	stream << "\n\n";
}

//std reallocation implementation defers according to OS used, so compare capacity is non sense.
template < typename T >
void	displayForAdaptator(const T& container, const char* name, std::ostream& stream)
{
	stream << "----------> " << name << " <---------\n";
	stream << "is empty : " << container.empty() << '\n';
	stream << "size " << " : " << container.size() << '\n';
	stream << '\n';
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

//Only works with std::string as value_type
template <class Cont> void randomIteratorTests(std::ofstream& outfile)
{
	typedef typename Cont::iterator It;
	typedef typename Cont::const_iterator Cit;

	It it;
	Cont v;
		
	for (int i = 0; i < 20; ++i)
	{
		v.push_back(myItoa(i));
	}

	const Cont const_v(v);
	Cit cit = const_v.begin();
	Cit citRange = v.end() - 3;
	it = v.begin();

	outfile << "++, --, +, -, +=, -=, *(), *++, *--, ->() const_iterator tests\n\n";
	outfile << *cit << " | size = " << cit->size() << '\n';
	outfile << *++cit << " must be equal to " << *cit++ << '\n';
	outfile << *(cit + 2) << " | " << *(2 + cit) << '\n';
	outfile << *--cit << " must be equal to " << *cit-- << '\n';
	outfile << *(cit += 8) << '\n';
	outfile << *(cit - 2) << '\n';
	outfile << *(cit -= 3) << '\n';
	outfile << "range of const_v from const_iterator begin to cit = " << cit - const_v.begin() << '\n';

	outfile << "\n++, --, +, -, +=, -=, *(), *++, *--, ->() iterator tests\n\n";
	outfile << *it << " | size = " << it->size() << '\n';
	outfile << *++it << " must be equal to " << *it++ << '\n';
	outfile << *(it + 2) << " | " << *(2 + it) << '\n';
	outfile << *--it << " must be equal to " << *it-- << '\n';
	outfile << *(it += 8) << '\n';
	outfile << *(it - 2) << '\n';
	outfile << *(it -= 3) << '\n';
	outfile << "range of v from iterator begin to it = " << it - v.begin() << '\n';
	outfile << "range of v from it to const_iterator end() - 3 = " << citRange - it << '\n';

	it = v.end();
	cit = const_v.end();
	Cit cit2(it);
	Cit cit3(cit2);
	outfile << "\niterator and const_iterator comparisons : \n\n";
	outfile << "is " << *--cit << " == " << *--cit2 << " ? " << (cit == cit2) << '\n';
	outfile << "is " << *--cit3 << " == " << *cit2 << " ? " << (cit3 == cit2) << '\n';
	outfile << "is " << *--cit3 << " == " << *cit2 << " ? " << (cit3 == cit2) << '\n';
	outfile << "is " << *--it << " == " << *cit2 << " ? " << (it == cit2) << '\n';
	outfile << "is " << *it << " == " << *--cit2 << " ? " << (it == cit2) << '\n';
	outfile << "is " << *--cit << " != " << *--cit2 << " ? " << (cit != cit2) << '\n';
	outfile << "is " << *--cit << " != " << *cit2 << " ? " << (cit != cit2) << '\n';
	it = v.begin();
	cit = const_v.begin();
	cit2 = it;
	cit3 = it;
	outfile << "is " << *cit << " != " << *cit2 << " ? " << (cit != cit2) << '\n';
	outfile << "is " << *it << " != " << *cit << " ? " << (it != cit) << '\n';
	outfile << "is " << *it << " != " << *cit2 << " ? " << (it != cit2) << '\n';
	outfile << "is " << *cit2 << " != " << *cit3 << " ? " << (cit2 != cit3) << '\n';
	outfile << "is " << *it << " < " << *cit2 << " ? " << (it < cit2) << '\n';
	outfile << "is " << *cit2 << " < " << *cit3 << " ? " << (cit2 < cit3) << '\n';
	outfile << "is " << *it << " <= " << *cit2 << " ? " << (it <= cit2) << '\n';
	outfile << "is " << *cit2 << " <= " << *cit3 << " ? " << (cit2 <= cit3) << '\n';
	outfile << "is " << *it << " > " << *cit2 << " ? " << (it > cit2) << '\n';
	outfile << "is " << *cit2 << " > " << *cit3 << " ? " << (cit2 > cit3) << '\n';
	outfile << "is " << *it << " >= " << *cit2 << " ? " << (it >= cit2) << '\n';
	outfile << "is " << *cit2 << " >= " << *cit3 << " ? " << (cit2 >= cit3) << '\n';

	++cit2;
	outfile << "is " << *it << " < " << *cit2 << " ? " << (it < cit2) << '\n';
	outfile << "is " << *cit2 << " < " << *cit3 << " ? " << (cit2 < cit3) << '\n';
	outfile << "is " << *it << " <= " << *cit2 << " ? " << (it <= cit2) << '\n';
	outfile << "is " << *cit2 << " <= " << *cit3 << " ? " << (cit2 <= cit3) << '\n';
	outfile << "is " << *it << " > " << *cit2 << " ? " << (it > cit2) << '\n';
	outfile << "is " << *cit2 << " > " << *cit3 << " ? " << (cit2 > cit3) << '\n';
	outfile << "is " << *it << " >= " << *cit2 << " ? " << (it >= cit2) << '\n';
	outfile << "is " << *cit2 << " >= " << *cit3 << " ? " << (cit2 >= cit3) << '\n';
	it += 3;
	cit3 += 2;
	outfile << "is " << *it << " < " << *cit2 << " ? " << (it < cit2) << '\n';
	outfile << "is " << *cit2 << " < " << *cit3 << " ? " << (cit2 < cit3) << '\n';
	outfile << "is " << *it << " <= " << *cit2 << " ? " << (it <= cit2) << '\n';
	outfile << "is " << *cit2 << " <= " << *cit3 << " ? " << (cit2 <= cit3) << '\n';
	outfile << "is " << *it << " > " << *cit2 << " ? " << (it > cit2) << '\n';
	outfile << "is " << *cit2 << " > " << *cit3 << " ? " << (cit2 > cit3) << '\n';
	outfile << "is " << *it << " >= " << *cit2 << " ? " << (it >= cit2) << '\n';
	outfile << "is " << *cit2 << " >= " << *cit3 << " ? " << (cit2 >= cit3) << '\n';

}

template <class Cont> void randomIteratorTestsForDeque(std::ofstream& outfile)
{
	typedef typename Cont::iterator It;
	typedef typename Cont::const_iterator Cit;

	It it;
	Cont v;
		
	for (char i = '0'; i < '0' + 20; ++i)
	{
		v.push_back(i);
	}

	const Cont const_v(v);
	Cit cit = const_v.begin();
	Cit citRange = v.end() - 3;
	it = v.begin();

	outfile << "++, --, +, -, +=, -=, *(), *++, *--, ->() const_iterator tests\n\n";
	outfile << *++cit << " must be equal to " << *cit++ << '\n';
	outfile << *(cit + 2) << " | " << *(2 + cit) << '\n';
	outfile << *--cit << " must be equal to " << *cit-- << '\n';
	outfile << *(cit += 8) << '\n';
	outfile << *(cit - 2) << '\n';
	outfile << *(cit -= 3) << '\n';
	outfile << "range of const_v from const_iterator begin to cit = " << cit - const_v.begin() << '\n';

	outfile << "\n++, --, +, -, +=, -=, *(), *++, *--, ->() iterator tests\n\n";
	outfile << *++it << " must be equal to " << *it++ << '\n';
	outfile << *(it + 2) << " | " << *(2 + it) << '\n';
	outfile << *--it << " must be equal to " << *it-- << '\n';
	outfile << *(it += 8) << '\n';
	outfile << *(it - 2) << '\n';
	outfile << *(it -= 3) << '\n';
	outfile << "range of v from iterator begin to it = " << it - v.begin() << '\n';
	outfile << "range of v from it to const_iterator end() - 3 = " << citRange - it << '\n';

	it = v.end();
	cit = const_v.end();
	Cit cit2(it);
	Cit cit3(cit2);
	outfile << "\niterator and const_iterator comparisons : \n\n";
	outfile << "is " << *--cit << " == " << *--cit2 << " ? " << (cit == cit2) << '\n';
	outfile << "is " << *--cit3 << " == " << *cit2 << " ? " << (cit3 == cit2) << '\n';
	outfile << "is " << *--cit3 << " == " << *cit2 << " ? " << (cit3 == cit2) << '\n';
	outfile << "is " << *--it << " == " << *cit2 << " ? " << (it == cit2) << '\n';
	outfile << "is " << *it << " == " << *--cit2 << " ? " << (it == cit2) << '\n';
	outfile << "is " << *--cit << " != " << *--cit2 << " ? " << (cit != cit2) << '\n';
	outfile << "is " << *--cit << " != " << *cit2 << " ? " << (cit != cit2) << '\n';
	it = v.begin();
	cit = const_v.begin();
	cit2 = it;
	cit3 = it;
	outfile << "is " << *cit << " != " << *cit2 << " ? " << (cit != cit2) << '\n';
	outfile << "is " << *it << " != " << *cit << " ? " << (it != cit) << '\n';
	outfile << "is " << *it << " != " << *cit2 << " ? " << (it != cit2) << '\n';
	outfile << "is " << *cit2 << " != " << *cit3 << " ? " << (cit2 != cit3) << '\n';
	outfile << "is " << *it << " < " << *cit2 << " ? " << (it < cit2) << '\n';
	outfile << "is " << *cit2 << " < " << *cit3 << " ? " << (cit2 < cit3) << '\n';
	outfile << "is " << *it << " <= " << *cit2 << " ? " << (it <= cit2) << '\n';
	outfile << "is " << *cit2 << " <= " << *cit3 << " ? " << (cit2 <= cit3) << '\n';
	outfile << "is " << *it << " > " << *cit2 << " ? " << (it > cit2) << '\n';
	outfile << "is " << *cit2 << " > " << *cit3 << " ? " << (cit2 > cit3) << '\n';
	outfile << "is " << *it << " >= " << *cit2 << " ? " << (it >= cit2) << '\n';
	outfile << "is " << *cit2 << " >= " << *cit3 << " ? " << (cit2 >= cit3) << '\n';

	++cit2;
	outfile << "is " << *it << " < " << *cit2 << " ? " << (it < cit2) << '\n';
	outfile << "is " << *cit2 << " < " << *cit3 << " ? " << (cit2 < cit3) << '\n';
	outfile << "is " << *it << " <= " << *cit2 << " ? " << (it <= cit2) << '\n';
	outfile << "is " << *cit2 << " <= " << *cit3 << " ? " << (cit2 <= cit3) << '\n';
	outfile << "is " << *it << " > " << *cit2 << " ? " << (it > cit2) << '\n';
	outfile << "is " << *cit2 << " > " << *cit3 << " ? " << (cit2 > cit3) << '\n';
	outfile << "is " << *it << " >= " << *cit2 << " ? " << (it >= cit2) << '\n';
	outfile << "is " << *cit2 << " >= " << *cit3 << " ? " << (cit2 >= cit3) << '\n';
	it += 3;
	cit3 += 2;
	outfile << "is " << *it << " < " << *cit2 << " ? " << (it < cit2) << '\n';
	outfile << "is " << *cit2 << " < " << *cit3 << " ? " << (cit2 < cit3) << '\n';
	outfile << "is " << *it << " <= " << *cit2 << " ? " << (it <= cit2) << '\n';
	outfile << "is " << *cit2 << " <= " << *cit3 << " ? " << (cit2 <= cit3) << '\n';
	outfile << "is " << *it << " > " << *cit2 << " ? " << (it > cit2) << '\n';
	outfile << "is " << *cit2 << " > " << *cit3 << " ? " << (cit2 > cit3) << '\n';
	outfile << "is " << *it << " >= " << *cit2 << " ? " << (it >= cit2) << '\n';
	outfile << "is " << *cit2 << " >= " << *cit3 << " ? " << (cit2 >= cit3) << '\n';

}

template <class Cont>
void bidirectionalIteratorTests(Cont& c, std::ofstream& outfile)
{
	typedef typename Cont::iterator It;
	typedef typename Cont::const_iterator Cit;

	It it;
	const Cont c2(c);
		
	Cit cit = c2.begin();
	it = c.begin();

	outfile << "++, --, *(), *++, *-- const_iterator tests\n\n";
	outfile << *++cit << " must be equal to " << *cit++ << '\n';
	outfile << *--cit << " must be equal to " << *cit-- << '\n';

	outfile << "\n++, --, *(), *++, *-- iterator tests\n\n";
	outfile << *++it << " must be equal to " << *it++ << '\n';
	outfile << *--it << " must be equal to " << *it-- << '\n';

	it = c.end();
	cit = c2.end();
	Cit cit2(it);
	Cit cit3(cit2);
	outfile << "\niterator and const_iterator comparisons : \n\n";
	outfile << "is " << *--cit << " == " << *--cit2 << " ? " << (cit == cit2) << '\n';
	outfile << "is " << *--cit3 << " == " << *cit2 << " ? " << (cit3 == cit2) << '\n';
	outfile << "is " << *--cit3 << " == " << *cit2 << " ? " << (cit3 == cit2) << '\n';
	outfile << "is " << *--it << " == " << *cit2 << " ? " << (it == cit2) << '\n';
	outfile << "is " << *it << " == " << *--cit2 << " ? " << (it == cit2) << '\n';
	outfile << "is " << *--cit << " != " << *--cit2 << " ? " << (cit != cit2) << '\n';
	outfile << "is " << *--cit << " != " << *cit2 << " ? " << (cit != cit2) << '\n';
	it = c.begin();
	cit = c2.begin();
	cit2 = it;
	cit3 = it;
	outfile << "is " << *cit << " != " << *cit2 << " ? " << (cit != cit2) << '\n';
	outfile << "is " << *it << " != " << *cit << " ? " << (it != cit) << '\n';
	outfile << "is " << *it << " != " << *cit2 << " ? " << (it != cit2) << '\n';
	outfile << "is " << *cit2 << " != " << *cit3 << " ? " << (cit2 != cit3) << '\n';
}

//Only works with std::string as value_type
template <class Cont> void ReverseRandomIteratorTests(std::ofstream& outfile)
{
	typedef typename Cont::reverse_iterator It;
	typedef typename Cont::const_reverse_iterator Cit;

	It it;
	Cont v;
		
	for (int i = 0; i < 20; ++i)
	{
		v.push_back(myItoa(i));
	}

	const Cont const_v(v);
	Cit cit = const_v.rbegin();
	it = v.rbegin();

	outfile << "++, --, +, -, +=, -=, *(), *++, *--, ->() const_reverse_iterator tests\n\n";
	outfile << *cit << " | size = " << cit->size() << '\n';
	outfile << *++cit << " must be equal to " << *cit++ << '\n';
	outfile << *(cit + 2) << " | " << *(2 + cit) << '\n';
	outfile << *--cit << " must be equal to " << *cit-- << '\n';
	outfile << *(cit += 8) << '\n';
	outfile << *(cit - 2) << '\n';
	outfile << *(cit -= 3) << '\n';

	outfile << "\n++, --, +, -, +=, -=, *(), *++, *--, ->() reverse_iterator tests\n\n";
	outfile << *it << " | size = " << it->size() << '\n';
	outfile << *++it << " must be equal to " << *it++ << '\n';
	outfile << *(it + 2) << " | " << *(2 + it) << '\n';
	outfile << *--it << " must be equal to " << *it-- << '\n';
	outfile << *(it += 8) << '\n';
	outfile << *(it - 2) << '\n';
	outfile << *(it -= 3) << '\n';

	it = v.rend();
	cit = const_v.rend();
	Cit cit2(it);
	Cit cit3(cit2);
	outfile << "\nreverse_iterator and const_reverse_iterator comparisons : \n\n";
	outfile << "is " << *--cit << " == " << *--cit2 << " ? " << (cit == cit2) << '\n';
	outfile << "is " << *--cit3 << " == " << *cit2 << " ? " << (cit3 == cit2) << '\n';
	outfile << "is " << *--cit3 << " == " << *cit2 << " ? " << (cit3 == cit2) << '\n';
	outfile << "is " << *--it << " == " << *cit2 << " ? " << (it == cit2) << '\n';
	outfile << "is " << *it << " == " << *--cit2 << " ? " << (it == cit2) << '\n';
	outfile << "is " << *--cit << " != " << *--cit2 << " ? " << (cit != cit2) << '\n';
	outfile << "is " << *--cit << " != " << *cit2 << " ? " << (cit != cit2) << '\n';
	it = v.rbegin();
	cit = const_v.rbegin();
	cit2 = it;
	cit3 = it;
	outfile << "is " << *cit << " != " << *cit2 << " ? " << (cit != cit2) << '\n';
	outfile << "is " << *it << " != " << *cit << " ? " << (it != cit) << '\n';
	outfile << "is " << *it << " != " << *cit2 << " ? " << (it != cit2) << '\n';
	outfile << "is " << *cit2 << " != " << *cit3 << " ? " << (cit2 != cit3) << '\n';
	outfile << "is " << *it << " < " << *cit2 << " ? " << (it < cit2) << '\n';
	outfile << "is " << *cit2 << " < " << *cit3 << " ? " << (cit2 < cit3) << '\n';
	outfile << "is " << *it << " <= " << *cit2 << " ? " << (it <= cit2) << '\n';
	outfile << "is " << *cit2 << " <= " << *cit3 << " ? " << (cit2 <= cit3) << '\n';
	outfile << "is " << *it << " > " << *cit2 << " ? " << (it > cit2) << '\n';
	outfile << "is " << *cit2 << " > " << *cit3 << " ? " << (cit2 > cit3) << '\n';
	outfile << "is " << *it << " >= " << *cit2 << " ? " << (it >= cit2) << '\n';
	outfile << "is " << *cit2 << " >= " << *cit3 << " ? " << (cit2 >= cit3) << '\n';

	++cit2;
	outfile << "is " << *it << " < " << *cit2 << " ? " << (it < cit2) << '\n';
	outfile << "is " << *cit2 << " < " << *cit3 << " ? " << (cit2 < cit3) << '\n';
	outfile << "is " << *it << " <= " << *cit2 << " ? " << (it <= cit2) << '\n';
	outfile << "is " << *cit2 << " <= " << *cit3 << " ? " << (cit2 <= cit3) << '\n';
	outfile << "is " << *it << " > " << *cit2 << " ? " << (it > cit2) << '\n';
	outfile << "is " << *cit2 << " > " << *cit3 << " ? " << (cit2 > cit3) << '\n';
	outfile << "is " << *it << " >= " << *cit2 << " ? " << (it >= cit2) << '\n';
	outfile << "is " << *cit2 << " >= " << *cit3 << " ? " << (cit2 >= cit3) << '\n';
	it += 3;
	cit3 += 2;
	outfile << "is " << *it << " < " << *cit2 << " ? " << (it < cit2) << '\n';
	outfile << "is " << *cit2 << " < " << *cit3 << " ? " << (cit2 < cit3) << '\n';
	outfile << "is " << *it << " <= " << *cit2 << " ? " << (it <= cit2) << '\n';
	outfile << "is " << *cit2 << " <= " << *cit3 << " ? " << (cit2 <= cit3) << '\n';
	outfile << "is " << *it << " > " << *cit2 << " ? " << (it > cit2) << '\n';
	outfile << "is " << *cit2 << " > " << *cit3 << " ? " << (cit2 > cit3) << '\n';
	outfile << "is " << *it << " >= " << *cit2 << " ? " << (it >= cit2) << '\n';
	outfile << "is " << *cit2 << " >= " << *cit3 << " ? " << (cit2 >= cit3) << '\n';

}

template <class Cont> void ReverseRandomIteratorTestsForDeque(std::ofstream& outfile)
{
	typedef typename Cont::reverse_iterator It;
	typedef typename Cont::const_reverse_iterator Cit;

	It it;
	Cont v;
		
	for (char i = '0'; i < '0' + 20; ++i)
	{
		v.push_back(i);
	}

	const Cont const_v(v);
	Cit cit = const_v.rbegin();
	it = v.rbegin();

	outfile << "++, --, +, -, +=, -=, *(), *++, *--, ->() const_reverse_iterator tests\n\n";
	outfile << *++cit << " must be equal to " << *cit++ << '\n';
	outfile << *(cit + 2) << " | " << *(2 + cit) << '\n';
	outfile << *--cit << " must be equal to " << *cit-- << '\n';
	outfile << *(cit += 8) << '\n';
	outfile << *(cit - 2) << '\n';
	outfile << *(cit -= 3) << '\n';

	outfile << "\n++, --, +, -, +=, -=, *(), *++, *--, ->() reverse_iterator tests\n\n";
	outfile << *++it << " must be equal to " << *it++ << '\n';
	outfile << *(it + 2) << " | " << *(2 + it) << '\n';
	outfile << *--it << " must be equal to " << *it-- << '\n';
	outfile << *(it += 8) << '\n';
	outfile << *(it - 2) << '\n';
	outfile << *(it -= 3) << '\n';

	it = v.rend();
	cit = const_v.rend();
	Cit cit2(it);
	Cit cit3(cit2);
	outfile << "\nreverse_iterator and const_reverse_iterator comparisons : \n\n";
	outfile << "is " << *--cit << " == " << *--cit2 << " ? " << (cit == cit2) << '\n';
	outfile << "is " << *--cit3 << " == " << *cit2 << " ? " << (cit3 == cit2) << '\n';
	outfile << "is " << *--cit3 << " == " << *cit2 << " ? " << (cit3 == cit2) << '\n';
	outfile << "is " << *--it << " == " << *cit2 << " ? " << (it == cit2) << '\n';
	outfile << "is " << *it << " == " << *--cit2 << " ? " << (it == cit2) << '\n';
	outfile << "is " << *--cit << " != " << *--cit2 << " ? " << (cit != cit2) << '\n';
	outfile << "is " << *--cit << " != " << *cit2 << " ? " << (cit != cit2) << '\n';
	it = v.rbegin();
	cit = const_v.rbegin();
	cit2 = it;
	cit3 = it;
	outfile << "is " << *cit << " != " << *cit2 << " ? " << (cit != cit2) << '\n';
	outfile << "is " << *it << " != " << *cit << " ? " << (it != cit) << '\n';
	outfile << "is " << *it << " != " << *cit2 << " ? " << (it != cit2) << '\n';
	outfile << "is " << *cit2 << " != " << *cit3 << " ? " << (cit2 != cit3) << '\n';
	outfile << "is " << *it << " < " << *cit2 << " ? " << (it < cit2) << '\n';
	outfile << "is " << *cit2 << " < " << *cit3 << " ? " << (cit2 < cit3) << '\n';
	outfile << "is " << *it << " <= " << *cit2 << " ? " << (it <= cit2) << '\n';
	outfile << "is " << *cit2 << " <= " << *cit3 << " ? " << (cit2 <= cit3) << '\n';
	outfile << "is " << *it << " > " << *cit2 << " ? " << (it > cit2) << '\n';
	outfile << "is " << *cit2 << " > " << *cit3 << " ? " << (cit2 > cit3) << '\n';
	outfile << "is " << *it << " >= " << *cit2 << " ? " << (it >= cit2) << '\n';
	outfile << "is " << *cit2 << " >= " << *cit3 << " ? " << (cit2 >= cit3) << '\n';

	++cit2;
	outfile << "is " << *it << " < " << *cit2 << " ? " << (it < cit2) << '\n';
	outfile << "is " << *cit2 << " < " << *cit3 << " ? " << (cit2 < cit3) << '\n';
	outfile << "is " << *it << " <= " << *cit2 << " ? " << (it <= cit2) << '\n';
	outfile << "is " << *cit2 << " <= " << *cit3 << " ? " << (cit2 <= cit3) << '\n';
	outfile << "is " << *it << " > " << *cit2 << " ? " << (it > cit2) << '\n';
	outfile << "is " << *cit2 << " > " << *cit3 << " ? " << (cit2 > cit3) << '\n';
	outfile << "is " << *it << " >= " << *cit2 << " ? " << (it >= cit2) << '\n';
	outfile << "is " << *cit2 << " >= " << *cit3 << " ? " << (cit2 >= cit3) << '\n';
	it += 3;
	cit3 += 2;
	outfile << "is " << *it << " < " << *cit2 << " ? " << (it < cit2) << '\n';
	outfile << "is " << *cit2 << " < " << *cit3 << " ? " << (cit2 < cit3) << '\n';
	outfile << "is " << *it << " <= " << *cit2 << " ? " << (it <= cit2) << '\n';
	outfile << "is " << *cit2 << " <= " << *cit3 << " ? " << (cit2 <= cit3) << '\n';
	outfile << "is " << *it << " > " << *cit2 << " ? " << (it > cit2) << '\n';
	outfile << "is " << *cit2 << " > " << *cit3 << " ? " << (cit2 > cit3) << '\n';
	outfile << "is " << *it << " >= " << *cit2 << " ? " << (it >= cit2) << '\n';
	outfile << "is " << *cit2 << " >= " << *cit3 << " ? " << (cit2 >= cit3) << '\n';
}

template <class Cont>
void bidirectionalReverseIteratorTests(Cont& c, std::ofstream& outfile)
{
	typedef typename Cont::reverse_iterator It;
	typedef typename Cont::const_reverse_iterator Cit;

	It it;
	const Cont c2(c);
		
	Cit cit = c2.rbegin();
	it = c.rbegin();

	outfile << "++, --, *(), *++, *-- reverse_const_iterator tests\n\n";
	outfile << *++cit << " must be equal to " << *cit++ << '\n';
	outfile << *--cit << " must be equal to " << *cit-- << '\n';

	outfile << "\n++, --, *(), *++, *-- reverse_iterator tests\n\n";
	outfile << *++it << " must be equal to " << *it++ << '\n';
	outfile << *--it << " must be equal to " << *it-- << '\n';

	it = c.rend();
	cit = c2.rend();
	Cit cit2(it);
	Cit cit3(cit2);
	outfile << "\niterator and const_reverse_iterator comparisons : \n\n";
	outfile << "is " << *--cit << " == " << *--cit2 << " ? " << (cit == cit2) << '\n';
	outfile << "is " << *--cit3 << " == " << *cit2 << " ? " << (cit3 == cit2) << '\n';
	outfile << "is " << *--cit3 << " == " << *cit2 << " ? " << (cit3 == cit2) << '\n';
	outfile << "is " << *--it << " == " << *cit2 << " ? " << (it == cit2) << '\n';
	outfile << "is " << *it << " == " << *--cit2 << " ? " << (it == cit2) << '\n';
	outfile << "is " << *--cit << " != " << *--cit2 << " ? " << (cit != cit2) << '\n';
	outfile << "is " << *--cit << " != " << *cit2 << " ? " << (cit != cit2) << '\n';
	it = c.rbegin();
	cit = c2.rbegin();
	cit2 = it;
	cit3 = it;
	outfile << "is " << *cit << " != " << *cit2 << " ? " << (cit != cit2) << '\n';
	outfile << "is " << *it << " != " << *cit << " ? " << (it != cit) << '\n';
	outfile << "is " << *it << " != " << *cit2 << " ? " << (it != cit2) << '\n';
	outfile << "is " << *cit2 << " != " << *cit3 << " ? " << (cit2 != cit3) << '\n';
}

template <class Cont>
void forwardIteratorTests(Cont& c, std::ofstream& outfile)
{
	typedef typename Cont::iterator It;
	typedef typename Cont::const_iterator Cit;

	It it;
	const Cont c2(c);
		
	Cit cit = c2.begin();
	it = c.begin();

	outfile << "++ const_iterator tests\n\n";
	outfile << (++cit == cit++) << '\n';
	outfile << (cit++ == cit++) << '\n';
	outfile << (cit++ == ++cit) << '\n';
	outfile << (++cit == ++cit) << '\n';

	outfile << "\n++ iterator tests\n\n";
	outfile << (++it == it++) << '\n';
	outfile << (it++ == it++) << '\n';
	outfile << (it++ == ++it) << '\n';
	outfile << (++it == ++it) << '\n';

	it = c.begin();
	cit = c2.begin();
	Cit cit2(it);
	Cit cit3(cit2);
	outfile << "\niterator and const_iterator comparisons : \n\n";
	outfile << (*++cit == *++cit2) << "  " << (cit == cit2) << '\n';
	outfile << (++cit3 == cit2) << "  " << (cit3 == cit2) << '\n';
	outfile << (++cit3 == cit2) << "  " << (cit3 == cit2) << '\n';
	outfile << (++it == cit2) << "  " << (it == cit2) << '\n';
	outfile << (it == ++cit2) << "  " << (it == cit2) << '\n';
	outfile << (++cit != ++cit2) << "  " << (cit != cit2) << '\n';
	outfile << (++cit != cit2) << "  " << (cit != cit2) << '\n';
	it = c.begin();
	cit = c2.begin();
	cit2 = it;
	cit3 = it;
	outfile << (*cit != *cit2) << "  " << (cit != cit2) << '\n';
	outfile << (*it != *cit) << "  " << (it != cit) << '\n';
	outfile << (*it != *cit2) << "  " << (it != cit2) << '\n';
	outfile << (*cit2 != *cit3) << "  " << (cit2 != cit3) << '\n';
}

// END OF COMMON TESTS LIBRARY

//MANDATORY TESTS

void flat_basket_tests();
void vector_tests(const std::string& currentPath, std::ostream& except);
void deque_tests(const std::string& currentPath, std::ostream& except);
void stack_tests(const std::string& currentPath);
void queue_tests(const std::string& currentPath);
void priority_queue_tests(const std::string& currentPath);
void map_tests(const std::string& currentPath, std::ostream& except);
void multimap_tests(const std::string& currentPath);
void set_tests(const std::string& currentPath);
void multiset_tests(const std::string& currentPath);
void list_tests(const std::string& currentPath);

//BONUS TESTS

void unordered_map_tests(const std::string& currentPath, std::ostream& except);
void unordered_set_tests(const std::string& currentPath);
void unordered_multimap_tests(const std::string& currentPath);
void unordered_multiset_tests(const std::string& currentPath);

#endif