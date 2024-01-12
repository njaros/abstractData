#ifndef TESTS_ELEMENTS_HPP_ABSTRACTDATA_BY_LE_N
# define TESTS_ELEMENTS_HPP_ABSTRACTDATA_BY_LE_N

#include <fstream>
#include <iostream>
#include <cstddef>

// COMMON TOOLS

std::string	itoa(int nb);

template < class T >
typename T::size_type getRandom(typename T::size_type& seed, typename T::size_type modulo)
		{
			typename T::size_type toReturn;

			if (!modulo)
				return modulo;

			if (!seed)
				seed = 482346873;
			toReturn = seed % modulo;
			seed /= modulo;
			return toReturn;
		}

// END OF COMMON TOOLS

// COMMON TESTS LIBRARY

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
		v.push_back(itoa(i));
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

//Only works with std::string as value_type
template <class Cont> void ReverseRandomIteratorTests(std::ofstream& outfile)
{
	typedef typename Cont::reverse_iterator It;
	typedef typename Cont::const_reverse_iterator Cit;

	It it;
	Cont v;
		
	for (int i = 0; i < 20; ++i)
	{
		v.push_back(itoa(i));
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

// END OF COMMON TESTS LIBRARY

void flat_basket_tests();
void vector_tests(const std::string& currentPath, std::ostream& except);
void deque_tests(const std::string& currentPath, std::ostream& except);
void stack_tests(const std::string& currentPath);
void queue_tests(const std::string& currentPath);
void priority_queue_tests(const std::string& currentPath);
void map_tests(const std::string& currentPath, std::ostream& except);

#endif