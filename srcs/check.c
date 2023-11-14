/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:59:49 by man               #+#    #+#             */
/*   Updated: 2023/11/14 15:45:54 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	end_check(t_arg *arg)
{
	int		i;
	t_philo	*philo;

	philo = arg->philo;
	i = 0;
	while (i < arg->philo_n)
	{
		pthread_join(philo[i].th_id, NULL);
		i++;
	}
	i = 0;
	while (i < arg->philo_n)
	{
		pthread_mutex_destroy(&(arg->fork[i]));
		i++;
	}
	free(arg->fork);
	pthread_mutex_destroy(&(arg->work));
}

void	if_dead(t_arg *arg, t_philo *philo, int i)
{
    pthread_mutex_unlock(&(arg->dead_check));
	pthread_mutex_lock(&(arg->eat_check));
	if (get_time() - philo[i].last_eat > arg->death_time)
	{
		philo_print(arg, i + 1, "died");
		pthread_mutex_lock(&(arg->dead_check));
		arg->isdead = 1;
		pthread_mutex_unlock(&(arg->dead_check));
	}
	pthread_mutex_unlock(&(arg->eat_check));
	usleep(100);
}

void	if_count(t_arg *arg, t_philo *philo)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&(arg->count_check));
	while (arg->eat_num != -1 && i < arg->philo_n && \
			philo[i].eat_count >= arg->eat_num)
		i++;
	pthread_mutex_unlock(&(arg->count_check));
	pthread_mutex_lock(&(arg->num_check));
	if (i == arg->philo_n)
		arg->eatend = 1;
	pthread_mutex_unlock(&(arg->num_check));
}

void	dead_check(t_arg *arg, t_philo *philo)
{
	int	i;

	while (!(arg->eatend))
	{
		i = 0;
		while (i < arg->philo_n)
		{
			pthread_mutex_lock(&(arg->dead_check));
			if (arg->isdead)
			{
				pthread_mutex_unlock(&(arg->dead_check));
				break ;
			}
			if_dead(arg, philo, i);
			i++;
		}
		pthread_mutex_lock(&(arg->dead_check));
		if (arg->isdead)
		{
			pthread_mutex_unlock(&(arg->dead_check));
			break ;
		}
		pthread_mutex_unlock(&(arg->dead_check));
		if_count(arg, philo);
	}
}
