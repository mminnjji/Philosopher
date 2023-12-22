/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/14 14:59:40 by man               #+#    #+#             */
/*   Updated: 2023/12/21 15:42:21 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	sleep_think(t_arg *arg, t_philo *philo)
{
	philo_print(arg, philo->id, "is sleeping");
	interval_usleep((long long)arg->sleep_time, arg);
	philo_print(arg, philo->id, "is thinking");
}

void	*philo_one(t_arg *arg, t_philo *philo)
{
	philo_print(arg, philo->id, "has taken a fork");
	philo_print(arg, philo->id, "died");
	pthread_mutex_lock(&(arg->dead_check));
	arg->isdead = 1;
	pthread_mutex_unlock(&(arg->dead_check));
	return (NULL);
}

void	util(t_philo *philo)
{
	pthread_mutex_unlock(&(philo->arg->dead_check));
	philo_eat(philo);
	pthread_mutex_lock(&(philo->arg->num_check));
}

void	*thread_work(void *a)
{
	t_philo	*philo;

	philo = (t_philo *)a;
	if (philo->arg->philo_n == 1)
		return (philo_one(philo->arg, philo));
	// if (philo->id % 2 == 0 && philo->id == philo->arg->philo_n)
	//  	usleep(100);
	while (1)
	{
		pthread_mutex_lock(&(philo->arg->dead_check));
		if (mutex_check(philo->arg->isdead, &(philo->arg->dead_check)))
			break ;
		util(philo);
		if (mutex_check(philo->arg->eatend, &(philo->arg->num_check)))
			break ;
		pthread_mutex_unlock(&(philo->arg->num_check));
		sleep_think(philo->arg, philo);
		usleep(100);
	}
	return (NULL);
}
