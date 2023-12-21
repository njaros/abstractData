#ifndef STACK_HPP
# define STACK_HPP

#include "vector.hpp"

namespace ft {

	template <class T, class Container = ft::vector<T> >
	class stack {

		template <class TN, class ContainerN>
		friend bool operator==(const stack<TN, ContainerN> &lhs, const stack<TN, ContainerN> &rhs);
		template <class TN, class ContainerN>
		friend bool operator!=(const stack<TN, ContainerN> &lhs, const stack<TN, ContainerN> &rhs);
		template <class TN, class ContainerN>
		friend bool operator>(const stack<TN, ContainerN> &lhs, const stack<TN, ContainerN> &rhs);
		template <class TN, class ContainerN>
		friend bool operator>=(const stack<TN, ContainerN> &lhs, const stack<TN, ContainerN> &rhs);
		template <class TN, class ContainerN>
		friend bool operator<(const stack<TN, ContainerN> &lhs, const stack<TN, ContainerN> &rhs);
		template <class TN, class ContainerN>
		friend bool operator<=(const stack<TN, ContainerN> &lsh, const stack<TN, ContainerN> &rhs);

    public:

        typedef T           value_type;
        typedef Container   container_type;
        typedef size_t      size_type;

    protected:

// THE FAMOUS "c"

        container_type c;

    public:

// CONSTRUCTOR

        explicit stack(const container_type &container = container_type()) : c(container) {}

// MEMBER FUNCTIONS

        bool empty()        const {return c.empty();}
        size_type size()    const {return c.size();}
        value_type  &top() {return c.back();}
        const value_type &top() const {return c.back();}
        void    push(const value_type &val) {c.push_back(val);}
        void    pop()   {c.pop_back();}

    };

// NON MEMBER RELATIONAL OPERATORS

    template <class T, class Container>
    bool operator==(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return (lhs.c == rhs.c);
    }

    template <class T, class Container>
    bool operator!=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return (lhs.c != rhs.c);
    }

    template <class T, class Container>
    bool operator<(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return (lhs.c < rhs.c);
    }

    template <class T, class Container>
    bool operator<=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return (lhs.c <= rhs.c);
    }

    template <class T, class Container>
    bool operator>(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return (lhs.c > rhs.c);
    }

    template <class T, class Container>
    bool operator>=(const stack<T, Container>& lhs, const stack<T, Container>& rhs) {
        return (lhs.c >= rhs.c);
    }

}

#endif