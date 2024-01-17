#ifndef MONKEY_TESTER_BIDIRECTIONAL_ITERATOR_TPP
# define MONKEY_TESTER_BIDIRECTIONAL_ITERATOR_TPP
# include <iterator>
#include "forward_iterator.tpp"

template <typename X, typename T>
void bidirectional_iterator(T t)
{
	forward_iterator<X, T>(t);
	static_assert(std::is_convertible<typename std::iterator_traits<X>::iterator_category, std::bidirectional_iterator_tag>::value, "wrong type for iterator_category");

	X a = X();
	--a;
	static_assert(std::is_same<decltype(--a), X&>::value, "wrong return type for operator++()");
	a--;
	static_assert(std::is_convertible<decltype(a--), const X&>::value, "wrong return type for operator++(int)");
	(void)*a--;
	static_assert(std::is_convertible<decltype(*a--), T>::value, "wrong return type for *it++");
}

template <typename X, typename T>
void bidirectional_iteratorSet(T t)
{
	forward_iteratorSet<X, T>(t);
	static_assert(std::is_convertible<typename std::iterator_traits<X>::iterator_category, std::bidirectional_iterator_tag>::value, "wrong type for iterator_category");

	X a = X();
	--a;
	static_assert(std::is_same<decltype(--a), X&>::value, "wrong return type for operator++()");
	a--;
	static_assert(std::is_convertible<decltype(a--), const X&>::value, "wrong return type for operator++(int)");
	(void)*a--;
	static_assert(std::is_convertible<decltype(*a--), T>::value, "wrong return type for *it++");
}

#endif