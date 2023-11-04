/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:38:01 by man               #+#    #+#             */
/*   Updated: 2023/11/04 17:19:23 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long get_time()
{
    struct timeval t;

    gettimeofday(&t, NULL);
    return((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

void take_fork(t_philo *philo)
{
	pthread_mutex_lock(&philo->arg->fork_m[philo->left_fork]);
	philo_print(philo, 0);
	pthread_mutex_lock(&philo->arg->fork_m[philo->right_fork]);
	philo_print(philo, 0);
}

void clean_fork(t_philo *philo)
{
	philo_print(philo, 2);
	pthread_mutex_unlock(&philo->arg->fork_m[philo->left_fork]);
	pthread_mutex_unlock(&philo->arg->fork_m[philo->right_fork]);
	interval_usleep(philo->arg->sleep_time * 1000, philo->arg);
}

void philo_eat(t_philo *philo)
{
    t_arg *arg;

    arg = philo->arg;
    pthread_mutex_lock(&philo->check_m);
    philo->flag = 1;
    philo->last_eat = get_time();
    philo->max_t = philo->last_eat + arg->death_time;
    philo_print(philo, 1);
    interval_usleep(arg->eat_time * 1000, philo->arg);
    philo->eat_count++;
    philo->flag = 0;
    pthread_mutex_unlock(&philo->check_m);
    pthread_mutex_unlock(&philo->eat_m);
}