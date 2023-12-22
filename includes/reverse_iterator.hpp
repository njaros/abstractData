#ifndef REVERSE
#define REVERSE

#include "iterator.hpp"

namespace ft {

    template<class Iterator>
    class reverse_iterator {

    public:

        typedef Iterator                                                iterator_type;
        typedef typename iterator_traits<Iterator>::iterator_category   iterator_category;
        typedef typename iterator_traits<Iterator>::value_type          value_type;
        typedef typename iterator_traits<Iterator>::difference_type     difference_type;
        typedef typename iterator_traits<Iterator>::pointer             pointer;
        typedef typename iterator_traits<Iterator>::reference           reference;

        reverse_iterator() {}

        reverse_iterator(iterator_type it) : _revIt(it) {}

        template <class Iter>
        reverse_iterator(const reverse_iterator<Iter> &other) : _revIt(other.base()) {}

		iterator_type		base() const {return (_revIt);}
		reference			operator*()	const {return *(--base());}
		reverse_iterator	operator+(difference_type n) const {
			return reverse_iterator(_revIt.operator-(n));
		}
        reverse_iterator	&operator++() {
            --_revIt;
            return (*this);
        }
        reverse_iterator	operator++(int) {
			reverse_iterator	temp = *this;
			++(*this);
			return temp;
 		}
        reverse_iterator	&operator+=(difference_type n) {
			_revIt -= n;
			return (*this);
        }
		reverse_iterator	operator-(difference_type n) const {
			return reverse_iterator(_revIt.operator+(n));
        }
		reverse_iterator	&operator--() {
			++_revIt;
			return (*this);
        }
        reverse_iterator	operator--(int) {
            reverse_iterator    temp = *this;
            --(*this);
            return temp;
        }
        reverse_iterator	&operator-=(difference_type n) {
            _revIt += n;
            return (*this);
        }
        pointer		operator->() const {return (--base()).operator->();}
        reference	operator[](difference_type  n) const {
            return (base()[-n - 1]);
        }

    private :

        iterator_type _revIt;
    };

// NON MEMBERS FUNCTIONS

    //COMPARISONS
    template <class Iterator1, class Iterator2>
    bool    operator==(const reverse_iterator<Iterator1>&lhs, const reverse_iterator<Iterator2>&rhs) {return (lhs.base() == rhs.base());}
	template <class Iterator1, class Iterator2>
    bool    operator!=(const reverse_iterator<Iterator1>&lhs, const reverse_iterator<Iterator2>&rhs) {return (lhs.base() != rhs.base());}
	template <class Iterator1, class Iterator2>
    bool    operator<(const reverse_iterator<Iterator1>&lhs, const reverse_iterator<Iterator2>&rhs) {return (lhs.base() > rhs.base());}
	template <class Iterator1, class Iterator2>
    bool    operator<=(const reverse_iterator<Iterator1>&lhs, const reverse_iterator<Iterator2>&rhs) {return (lhs.base() >= rhs.base());}
	template <class Iterator1, class Iterator2>
    bool	operator>(const reverse_iterator<Iterator1>&lhs, const reverse_iterator<Iterator2>&rhs) {return (lhs.base() < rhs.base());}
	template <class Iterator1, class Iterator2>
    bool	operator>=(const reverse_iterator<Iterator1>&lhs, const reverse_iterator<Iterator2>&rhs) {return (lhs.base() <= rhs.base());}

    //OPERATIONS
	template <class Iterator>
	reverse_iterator<Iterator>	operator+(const reverse_iterator<Iterator>& rev_it, typename reverse_iterator<Iterator>::difference_type n) {
		return (rev_it + n);
	}
    template <class Iterator>
    typename reverse_iterator<Iterator>::difference_type operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
        return (rhs.base() - lhs.base());
    }
}

#endif