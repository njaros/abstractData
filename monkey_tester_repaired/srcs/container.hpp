#ifndef MONKEY_TESTER_CONTAINER_HPP
#define MONKEY_TESTER_CONTAINER_HPP
# include "common.hpp"

namespace container {
	template<class C>
	void empty(C &a, C &b) {
		FNAME
		if (toPrint)
		{
			std::cout << "a.empty() = " << a.empty() << std::endl;
			std::cout << "b.empty() = " << b.empty() << std::endl;
		}
	}

	template<class C>
	void swap(C &a, C &b) {
		FNAME
		if (toPrint)
			std::cout << "a.swap(b)" << std::endl;
		a.swap(b);
	}

	template<class C>
	void default_constructor(C &a, C &)
	{
		FNAME
		if (toPrint)
			std::cout << "a = C()" << std::endl;
		a = C();
	}

	template<class C>
	void copy_constructor(C &a, C &) {
		FNAME
		if (toPrint)
			std::cout << "a = C(a)" << std::endl;
		a = C(a);
	}

	template<class C>
	void operator_assign(C &a, C &b) {
		FNAME
		C c = a;
		if (toPrint)
		{
			std::cout << "C c = a" << std::endl;
			std::cout << "print c:" << std::endl;
			monkey::print(c);
		}

		a = b;
		if (toPrint)
		{
			std::cout << "a = b" << std::endl;
			std::cout << "print a:" << std::endl;
			monkey::print(a);
		}
	}

	template<class C>
	void comparison(C &a, C &b) {
		FNAME

		bool res1 = (a < b);
		bool res2 = (a > b);
		bool res3 = (a <= b);
		bool res4 = (a >= b);
		bool res5 = (a == b);
		bool res6 = (a != b);
		if (toPrint)
		{
			std::cout << "a < b: " << res1 << std::endl;
			std::cout << "a > b: " << res2 << std::endl;
			std::cout << "a <= b: " << res3 << std::endl;
			std::cout << "a >= b: " << res4 << std::endl;
			std::cout << "a == b: " << res5 << std::endl;
			std::cout << "a != b: " << res6 << std::endl;
		}
	}

	template <class C>
	void reverse_iterators(C &a, C &b)
	{
		FNAME
		if (toPrint)
			std::cout << "reverse_printing a" << std::endl;
		for (typename C::reverse_iterator it = a.rbegin(); it != a.rend(); it++)
		{
			if (toPrint)
				std::cout << "-> [" << *it << "]" << std::endl;
			else
				*it;
		}
	}

	template <class C>
	void	clear(C &a, C &)
	{
		FNAME
		if (toPrint)
			std::cout << "a.clear()" << std::endl;
		a.clear();
	}

	BUILD_TABLE(
			default_constructor<C>,
			empty<C>,
			swap<C>,
			copy_constructor<C>,
			operator_assign<C>,
			reverse_iterators<C>,
			clear<C>,
			comparison<C>
			)
}

#endif