/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:38:01 by man               #+#    #+#             */
/*   Updated: 2023/11/09 17:17:29 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long long	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void	interval_usleep(long long sleep_t, t_arg *arg)
{
	long long	n;

	n = get_time();
	while (1)
	{
		pthread_mutex_lock(&(arg->dead_check));
		if (arg->isdead)
		{
			pthread_mutex_unlock(&(arg->dead_check));
			break;
		}
		pthread_mutex_unlock(&(arg->dead_check));
		if (get_time() - n >= sleep_t)
			break ;
		usleep(50);
	}
}

void	philo_print(t_arg *arg, int id, char *str)
{
	pthread_mutex_lock(&(arg->work));
	pthread_mutex_lock(&(arg->dead_check));
	if (!(arg->isdead))
	{
		printf("%lli ", get_time() - arg->start_time);
		printf("%i ", id);
		printf("%s\n", str);
	}
	pthread_mutex_unlock(&(arg->dead_check));
	pthread_mutex_unlock(&(arg->work));
}

int	print_error(int n)
{
	if (n == 1)
		write(2, "Bad argument\n", 13);
	else if (n == 2)
		write(2, "Malloc error - no memory\n", 25);
	else if (n == 3)
		write(2, "Mutex initializing error\n", 25);
	else if (n == 3)
		write(2, "Thread create error\n", 20);
	return (0);
}
