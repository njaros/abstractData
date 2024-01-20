#ifndef MONKEY_TESTER_PRIORITY_QUEUE_HPP
#define MONKEY_TESTER_PRIORITY_QUEUE_HPP
# include "common.hpp"
# include "container.hpp"

namespace priority_queue {
	template<class C>
	void underlying_constructor(C &a, C &) {
		FNAME
		std::vector<typename C::value_type> v = monkey::get_random_vector<typename C::value_type>();
		typename C::container_type underlying(v.begin(), v.end());
		if (toPrint)
		{
			std::cout << "a = C(compare(), underlying) with underlying: " << std::endl;
			monkey::print_range(v.begin(), v.end());
		}
		a = C(typename C::value_compare(), underlying);
	}

	template <class C>
	void	push(C &a, C &)
	{
		FNAME
		typename C::value_type value((monkey::get_value()));
		if (toPrint)
			std::cout << "a.push(value) with value = [" << value << "]" << std::endl;
		a.push(value);
	}

	template <class C>
	void	pop(C &a, C &)
	{
		FNAME
		if (a.empty())
			return;
		if (toPrint)
			std::cout << "a.pop()" << std::endl;
		a.pop();
	}


	BUILD_TABLE(
			container::default_constructor,
			container::default_constructor<C>,
			container::empty<C>,
			container::copy_constructor<C>,
			container::operator_assign<C>,
			underlying_constructor,
			push,
			push,
			push,
			pop
			)
}

#endif