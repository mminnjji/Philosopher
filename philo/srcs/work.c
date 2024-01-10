/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:37:59 by man               #+#    #+#             */
/*   Updated: 2024/01/10 17:04:52 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	philo_fork_pick(t_philo *philo, t_arg **arg)
{
	if (philo->id % 2 != 0 && philo->id != (*arg)->philo_n - 1)
	{
		pthread_mutex_lock(&((*arg)->fork[philo->left_fork]));
		philo_print(*arg, philo->id, "has taken a fork");
		pthread_mutex_lock(&((*arg)->fork[philo->right_fork]));
	}
	else
	{
		pthread_mutex_lock(&((*arg)->fork[philo->right_fork]));
		philo_print(*arg, philo->id, "has taken a fork");
		pthread_mutex_lock(&((*arg)->fork[philo->left_fork]));
	}
	philo_print(*arg, philo->id, "has taken a fork");
}

void	philo_fork_no(t_philo *philo, t_arg **arg)
{
	if (philo->id % 2 != 0 && philo->id != (*arg)->philo_n - 1)
	{
		pthread_mutex_unlock(&((*arg)->fork[philo->left_fork]));
		pthread_mutex_unlock(&((*arg)->fork[philo->right_fork]));
	}
	else
	{
		pthread_mutex_unlock(&((*arg)->fork[philo->right_fork]));
		pthread_mutex_unlock(&((*arg)->fork[philo->left_fork]));
	}
}

void	philo_eat_util(t_arg **arg, t_philo **philo)
{
	pthread_mutex_unlock(&((*arg)->num_check));
	pthread_mutex_unlock(&((*arg)->count_check));
	pthread_mutex_lock(&((*arg)->last_check));
	(*philo)->last_check = 1;
	pthread_mutex_unlock(&((*arg)->last_check));
}

void	philo_eat(t_philo *philo)
{
	t_arg	*arg;

	arg = philo->arg;
	pthread_mutex_lock(&(arg->count_check));
	pthread_mutex_lock(&(arg->num_check));
	if (arg->eat_num == -1 || philo->eat_count < arg->eat_num || !arg->eatend)
	{
		pthread_mutex_unlock(&(arg->num_check));
		pthread_mutex_unlock(&(arg->count_check));
		philo_fork_pick(philo, &arg);
		pthread_mutex_lock(&(arg->eat_check));
		philo->last_eat = get_time();
		philo_print(arg, philo->id, "is eating");
		pthread_mutex_unlock(&(arg->eat_check));
		pthread_mutex_lock(&(arg->count_check));
		(philo->eat_count)++;
		pthread_mutex_unlock(&(arg->count_check));
		interval_usleep((long long)arg->eat_time, arg);
		philo_fork_no(philo, &arg);
	}
	else
		philo_eat_util(&arg, &philo);
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
