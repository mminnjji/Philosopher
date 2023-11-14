/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:37:59 by man               #+#    #+#             */
/*   Updated: 2023/11/09 17:23:38 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	philo_eat(t_philo *philo)
{
	t_arg	*arg;

	arg = philo->arg;
	pthread_mutex_lock(&(arg->fork[philo->left_fork]));
	philo_print(arg, philo->id, "has taken a fork");
	pthread_mutex_lock(&(arg->fork[philo->right_fork]));
	philo_print(arg, philo->id, "has taken a fork");
	pthread_mutex_lock(&(arg->eat_check));
	philo_print(arg, philo->id, "is eating");
	philo->last_eat = get_time();
	pthread_mutex_unlock(&(arg->eat_check));
	interval_usleep((long long)arg->eat_time, arg);
	(philo->eat_count)++;
	pthread_mutex_unlock(&(arg->fork[philo->left_fork]));
	pthread_mutex_unlock(&(arg->fork[philo->right_fork]));
}

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
			i++;
		}
		pthread_mutex_lock(&(arg->dead_check));
		if (arg->isdead)
		{
			pthread_mutex_unlock(&(arg->dead_check));
			break ;
		}
		pthread_mutex_unlock(&(arg->dead_check));
		i = 0;
		while (arg->eat_num != -1 && i < arg->philo_n && \
				philo[i].eat_count >= arg->eat_num)
			i++;
		if (i == arg->philo_n)
			arg->eatend = 1;
	}
}

void	*thread_work(void *a)
{
	t_arg	*arg;
	t_philo	*philo;

	philo = (t_philo *)a;
	arg = philo->arg;
	if (arg->philo_n == 1)
	{
		philo_print(arg, philo->id, "has taken a fork");
		philo_print(arg, philo->id, "died");
		pthread_mutex_lock(&(arg->dead_check));
		arg->isdead = 1;
		pthread_mutex_unlock(&(arg->dead_check));
		return (NULL);
	}
	if (philo->id % 2 == 0)
		usleep(100);
	while (!(arg->isdead))
	{
		pthread_mutex_lock(&(arg->dead_check));
		if (arg->isdead)
		{
			pthread_mutex_unlock(&(arg->dead_check));
			break;
		}
		pthread_mutex_unlock(&(arg->dead_check));
		philo_eat(philo);
		if (arg->eatend)
			break ;
		philo_print(arg, philo->id, "is sleeping");
		interval_usleep((long long)arg->sleep_time, arg);
		philo_print(arg, philo->id, "is thinking");
	}
	return (NULL);
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