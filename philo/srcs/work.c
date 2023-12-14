/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:37:59 by man               #+#    #+#             */
/*   Updated: 2023/12/14 15:36:56 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	philo_eat(t_philo *philo)
{
	t_arg	*arg;

	arg = philo->arg;
	if (arg->eat_num == -1 || philo->eat_count <= arg->eat_num)
	{
		pthread_mutex_lock(&(arg->fork[philo->left_fork]));
		philo_print(arg, philo->id, "has taken a fork");
		pthread_mutex_lock(&(arg->fork[philo->right_fork]));
		philo_print(arg, philo->id, "has taken a fork");
		pthread_mutex_lock(&(arg->eat_check));
		philo_print(arg, philo->id, "is eating");
		philo->last_eat = get_time();
		pthread_mutex_unlock(&(arg->eat_check));
		interval_usleep((long long)arg->eat_time, arg);
		pthread_mutex_lock(&(arg->count_check));
		(philo->eat_count)++;
		pthread_mutex_unlock(&(arg->count_check));
		pthread_mutex_unlock(&(arg->fork[philo->left_fork]));
		pthread_mutex_unlock(&(arg->fork[philo->right_fork]));
	}
}

int	philo_work(t_arg *arg)
{
	int		i;
	t_philo	*philo;

	i = 0;
	philo = arg->philo;
	arg->start_time = get_time();
	while (i < arg->philo_n)
	{
		if (pthread_create(&(philo[i].th_id), NULL, thread_work, &philo[i]))
			return (1);
		pthread_mutex_lock(&(arg->eat_check));
		arg->philo[i].last_eat = get_time();
		pthread_mutex_unlock(&(arg->eat_check));
		i++;
	}
	dead_check(arg, philo);
	end_check(arg);
	return (0);
}
