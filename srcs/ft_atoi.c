/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:24:14 by man               #+#    #+#             */
/*   Updated: 2023/11/08 13:56:08 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_isdigit(int c)
{
	if (c <= '9' && c >= '0')
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	int			i;
	long long	plus;
	long long	sum;

	i = 0;
	plus = 1;
	sum = 0;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) \
				|| str[i] == 32))
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			plus *= -1;
	while (ft_isdigit(str[i]))
	{
		if ((sum == LLONG_MAX / 10 && str[i] > \
					LLONG_MAX % 10 + '0' + (plus != 1)) \
					|| sum > LLONG_MAX / 10)
			return (-1 * ((plus == 1)));
		sum *= 10;
		sum += (str[i++] - '0');
	}
	return ((int)(sum * plus));
}
