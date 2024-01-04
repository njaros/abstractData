/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaros <njaros@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/21 17:16:37 by njaros            #+#    #+#             */
/*   Updated: 2022/07/22 16:07:45 by njaros           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILITY_HPP_ABSTRACTDATA_BY_LE_N
# define UTILITY_HPP_ABSTRACTDATA_BY_LE_N

namespace ft
{

template < class T1, class T2 >

class pair
{

	public :

		typedef T1	first_type;
		typedef T2	second_type;

		first_type	first;
		second_type	second;

		pair() : first(T1()), second(T2()) {}
		template<class U, class V> 
		explicit pair( const pair< U, V >& other ) : first(other.first), second(other.second) {}
		pair( const first_type &a, const second_type &b ) : first(a), second(b) {}

		pair	&operator=( const pair &other )
		{
			first = other.first;
			second = other.second;
            return (*this);
		}

	private :
	
};

template <class T1, class T2>
bool operator== (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{ return lhs.first==rhs.first && lhs.second==rhs.second; }

template <class T1, class T2>
bool operator!= (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{ return !(lhs==rhs); }

template <class T1, class T2>
bool operator<  (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{ return lhs.first<rhs.first || (!(rhs.first<lhs.first) && lhs.second<rhs.second); }

template <class T1, class T2>
bool operator<= (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{ return !(rhs<lhs); }

template <class T1, class T2>
bool operator>  (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{ return rhs<lhs; }

template <class T1, class T2>
bool operator>= (const pair<T1,T2>& lhs, const pair<T1,T2>& rhs)
{ return !(lhs<rhs); }

template < class T1, class T2 >
ft::pair< T1, T2 >	make_pair(T1 x, T2 y)	{return (ft::pair<T1, T2>(x, y));}

}
#endif
