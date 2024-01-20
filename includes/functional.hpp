#ifndef FUNCTIONAL_HPP
# define FUNCTIONAL_HPP

#include <string>
#include "type_traits.hpp"

namespace ft
{

	template <class Arg, class Result>
	struct unary_function
	{
		typedef Arg argument_type;
		typedef Result result_type;
	};

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
	* is_floating_point
	* is_string
	*/

	size_t	_do_hash_pls(const unsigned char* first, size_t len);

	//because of the -Werror flag, i need to split the FNV_offset_basis number in 2.
	const size_t FNV_offset_basis_first_part = 7000000000000000000;
	const size_t FNV_offset_basis_second_part= 7695981039346656037;
	const size_t FNV_offset_basis = FNV_offset_basis_first_part + FNV_offset_basis_second_part;
	const size_t FNV_prime = 1099511628211;

	//NECESSARY TEMPLATE SPECIALIZATIONS FOR HASH STRUCTURE (which are not in type_traits)

	template <class T>
	struct isString : public false_type {};

	template <>
	struct isString<std::string> : public true_type {};
	template <>
	struct isString<std::wstring> : public true_type {};

	template <class T>
	struct is_string : public isString<T> {};

	//_hash templated functions, one function for each type I have to be able to hash


	template < class T >
	typename ft::enable_if< ft::is_integral< T >::value, size_t >::type
		_hash(T elt)
	{
		const size_t	nbBytes = sizeof(elt);
		unsigned char	bytes[nbBytes];

		for (size_t i = 0; i < nbBytes; ++i)
		{
			bytes[i] = (unsigned char)elt;
			elt >>= 8;
		}
		return _do_hash_pls(bytes, nbBytes);
	}

	template < class T >
	typename ft::enable_if<ft::is_pointer< T >::value, size_t >::type
		_hash(const T& elt)
	{
		__uint64_t		hashable = reinterpret_cast<__uint64_t>(elt);
		const size_t	nbBytes = sizeof(elt);
		unsigned char	bytes[nbBytes];
		
		for (size_t i = 0; i < nbBytes; ++i)
		{
			bytes[i] = (unsigned char)hashable;
			hashable >>= 8;
		}
		return _do_hash_pls(bytes, nbBytes);
	}

	template <class T>
	typename enable_if<sizeof(T) == 4, size_t>::type
		_hash_floating_point(T elt)
	{
		__uint32_t	toUInt32 = *reinterpret_cast<__uint32_t*>(&elt);
		unsigned char		bytes[4];

		for (size_t i = 0; i < 4; ++i)
		{
			bytes[i] = (unsigned char)toUInt32;
			toUInt32 >>= 8;
		}
		return _do_hash_pls(bytes, 4);
	}

	template <class T>
	typename enable_if<sizeof(T) == 8, size_t>::type
		_hash_floating_point(T elt)
	{
		__uint64_t		toUInt64 = *reinterpret_cast<__uint64_t*>(&elt);
		unsigned char	bytes[8];

		for (size_t i = 0; i < 8; ++i)
		{
			bytes[i] = (unsigned char)toUInt64;
			toUInt64 >>= 8;
		}
		return _do_hash_pls(bytes, 8);
	}

	template < class T >
	typename ft::enable_if<ft::is_floating_point< T >::value, size_t >::type
		_hash(T elt)
	{
		return _hash_floating_point(elt);
	}

	template < class T >
	typename ft::enable_if<ft::is_string< T >::value, size_t >::type
		_hash(const T& elt)
	{
		const size_t			nbBytes = elt.length() * sizeof(typename T::value_type);
		const unsigned char*	bytes = reinterpret_cast<const unsigned char*>(elt.c_str());

		return _do_hash_pls(bytes, nbBytes);
	}

	template < class T >
	struct hash : public ft::unary_function< T, size_t >
	{
		size_t operator()(const T& elt) const { return _hash(elt); }
	};
}


#endif // !FUNCTIONAL_HPP
