#ifndef FUNCTIONAL_HPP
# define FUNCTIONAL_HPP

#include "pair.hpp"

namespace ft
{
	template<class A, class B, class Res>
	struct binary_function
	{
		typedef A	first_argument_type;
		typedef B	second_argument_type;
		typedef Res	result_type;
	};

	template<class T>
	struct less : binary_function<T, T, bool>
	{
		bool operator()(const T& a, const T& b) const
		{
			return a < b;
		}
	};

	template<class T>
	struct greater : binary_function<T, T, bool>
	{
		bool operator()(const T& a, const T& b) const
		{
			return b < a;
		}
	};

	template <class T>
	struct equal_to : binary_function <T, T, bool>
	{
		bool operator() (const T& x, const T& y) const
		{
			return x == y;
		}
	};

}


#endif // !FUNCTIONAL_HPP
