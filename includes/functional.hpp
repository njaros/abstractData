#ifndef FUNCTIONAL_HPP
# define FUNCTIONAL_HPP

#include <string>

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

	const size_t FNV_offset_basis = 14695981039346656037;
	const size_t FNV_prime = 1099511628211;

	//NECESSARY TEMPLATE SPECIALIZATIONS FOR HASH STRUCTURE (which are not in type_traits)

	template <class T>
	struct isString : public false_type {};

	template <>
	struct isString<std::string> : public true_type {};
	template <>
	struct isString<std::wstring> : public true_type {};
	template <>
	struct isString<std::u16string> : public true_type {};
	template <>
	struct isString<std::u32string> : public true_type {};

	template <class T>
	struct is_string : public isString<T> {};

	//_hash templated functions, one function for each type I have to be able to hash

	size_t	_do_hash_pls(const unsigned char* first, size_t len)
	{
		size_t h;

		h = FNV_offset_basis;
		for (size_t i = 0; i < len; ++i)
		{
			h ^= first[i];
			h *= FNV_prime;
		}
		return h;
	}

	template < class T >
	size_t	_hash(T elt, typename ft::enable_if<ft::is_integral< T >::value >::type* = 0)
	{
		const size_t	nbBytes = sizeof(elt);
		unsigned char	bytes[nbBytes];

		for (int i = 0; i < nbBytes; ++i)
		{
			bytes[i] = (unsigned char)elt;
			elt >>= 8;
		}
		return _do_hash_pls(bytes, nbBytes);
	}

	template < class T >
	size_t	_hash(const T& elt, typename ft::enable_if<ft::is_pointer< T >::value >::type* = 0)
	{
		const size_t	nbBytes = sizeof(elt);
		unsigned char	bytes[nbBytes];
		
		for (int i = 0; i < nbBytes; ++i)
		{
			bytes[i] = (unsigned char)elt;
			elt >>= 8;
		}
		return _do_hash_pls(bytes, nbBytes);
	}

	template <class T>
	size_t	_hash_floating_point(T elt, typename enable_if<sizeof(T) == 4>::type* = 0)
	{
		unsigned __int32	toUInt32 = *reinterpret_cast<unsigned __int32*>(&elt);
		unsigned char		bytes[4];

		for (int i = 0; i < 4; ++i)
		{
			bytes[i] = (unsigned char)toUInt32;
			toUInt32 >>= 8;
		}
		return _do_hash_pls(bytes, 4);
	}

	template <class T>
	size_t	_hash_floating_point(T elt, typename enable_if<sizeof(T) == 8>::type* = 0)
	{
		unsigned __int64	toUInt64 = *reinterpret_cast<unsigned __int64*>(&elt);
		unsigned char		bytes[8];

		for (int i = 0; i < 8; ++i)
		{
			bytes[i] = (unsigned char)toUInt64;
			toUInt64 >>= 8;
		}
		return _do_hash_pls(bytes, 8);
	}

	template < class T >
	size_t	_hash(T elt, typename ft::enable_if<ft::is_floating_point< T >::value >::type* = 0)
	{
		return _hash_floating_point(elt);
	}

	template < class T >
	size_t	_hash(const T& elt, typename ft::enable_if<ft::is_string< T >::value >::type* = 0)
	{
		const size_t			nbBytes = elt.length() * sizeof(typename T::value_type);
		const unsigned char*	bytes = reinterpret_cast<const unsigned char*>(elt.c_str());

		return _do_hash_pls(bytes, nbBytes);
	}

	template < class T >
	struct hash : public ft::unary_function< T, size_t >
	{
		size_t operator()(const T& elt) { return _hash(elt); }
	};
}


#endif // !FUNCTIONAL_HPP
