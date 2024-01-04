#ifndef FUNCTIONAL_HPP
# define FUNCTIONAL_HPP

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

	//HASH

	/*
	* doc : https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
	* template specializations to do : 
	* is_integral
	* is_pointer
	* is_iterable (or thing like that)
	* if is_iterable, I have to redirect to templated function which has only template specialization : is_integral or is_pointer
	*/

}


#endif // !FUNCTIONAL_HPP
