/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   isIntegral.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: njaros <njaros@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/07/22 09:38:18 by njaros            #+#    #+#             */
/*   Updated: 2022/07/22 16:03:00 by njaros           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef IS_INTEGRAL_HPP
# define IS_INTEGRAL_HPP

#include "vector.hpp"

namespace ft
{

template < typename T, T v >
struct integral_constant
{

	static const T					value = v;
	typedef T						value_type;
	typedef integral_constant<T, v>	type;
	operator T()					{return (v);}

};

typedef integral_constant<bool, true>	true_type;
typedef integral_constant<bool, false>	false_type;

template < typename T >
struct isIntegral : public false_type							{};
template <>
struct isIntegral<bool> : public true_type						{};
template <>
struct isIntegral<char> : public true_type						{};
template <>
struct isIntegral<wchar_t> : public true_type					{};
template <>
struct isIntegral<signed char> : public true_type				{};
template <>
struct isIntegral<short int> : public true_type					{};
template <>
struct isIntegral<int> : public true_type						{};
template <>
struct isIntegral<long int> : public true_type					{};
template <>
struct isIntegral<long long int> : public true_type				{};
template <>
struct isIntegral<unsigned char> : public true_type				{};
template <>
struct isIntegral<unsigned short int> : public true_type		{};
template <>
struct isIntegral<unsigned int> : public true_type				{};
template <>
struct isIntegral<unsigned long int> : public true_type			{};
template <>
struct isIntegral<unsigned long long int> : public true_type	{};

template < typename T >
struct is_integral : public isIntegral<T> {};
}

#endif