/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thi-huon <thi-huon@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 16:22:27 by thi-huon          #+#    #+#             */
/*   Updated: 2024/12/07 16:22:43 by thi-huon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

int	ft_putnbr(int nbr)
{
	char		num;
	int			len;
	long int	n;
	int			recursive_len;

	n = nbr;
	len = 0;
	if (n < 0)
	{
		if (ft_putchar('-') == -1)
			return (-1);
		n = -n;
		len++;
	}
	if (n >= 10)
	{
		recursive_len = ft_putnbr(n / 10);
		if (recursive_len == -1)
			return (-1);
		len += recursive_len;
	}
	num = (n % 10) + '0';
	if (ft_putchar(num) == -1)
		return (-1);
	return (len + 1);
}
