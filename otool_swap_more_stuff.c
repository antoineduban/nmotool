/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   otool_swap_more_stuff.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aduban <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/23 15:51:44 by aduban            #+#    #+#             */
/*   Updated: 2017/01/23 15:53:59 by aduban           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "otool.h"

uint32_t	swap(uint32_t val)
{
	val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF);
	return (val << 16) | (val >> 16);
}

int64_t		swap_endian_64(int64_t i)
{
	return (((i >> 0x38) & 0x00000000000000ffLL)
		| ((i >> 0x28) & 0x000000000000ff00LL)
		| ((i >> 0x18) & 0x0000000000ff0000LL)
		| ((i >> 0x08) & 0x00000000ff000000LL)
		| ((i << 0x08) & 0x000000ff00000000LL)
		| ((i << 0x18) & 0x0000ff0000000000LL)
		| ((i << 0x28) & 0x00ff000000000000LL)
		| ((i << 0x38) & 0xff00000000000000LL));
}

int			set_swap(int i)
{
	static int s = 0;

	if (i == 0)
		s = 0;
	else if (i == 1)
		s = 1;
	return (s);
}