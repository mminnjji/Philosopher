/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:37:59 by man               #+#    #+#             */
/*   Updated: 2023/11/04 17:18:40 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void *thread_work_count(void *a)
{
	t_arg *arg;
	int		i;
	int		j;

	arg = (t_arg*)a;
	j = 0;
	while (j < arg->eat_num)
	{
		i = 0;
		while (i < arg->philo_n)
			pthread_mutex_lock(&arg->philo[i++].eat_m);
		j++;
	}
	philo_print(&arg->philo[0], 3);
	pthread_mutex_unlock(&arg->dead_m);
	return ((void*)0);
}


static void *monitor(void *a)
{
	t_philo		*philo;

	philo = (t_philo*)a;
	while (1)
	{
		pthread_mutex_lock(&philo->check_m);
		if (!philo->flag && get_time() > philo->max_t)
		{
            philo->arg->isdead = 1;
			philo_print(philo, 4);
			pthread_mutex_unlock(&philo->check_m);
			pthread_mutex_unlock(&philo->arg->dead_m);
			return ((void*)0);
		}
		pthread_mutex_unlock(&philo->check_m);
		usleep(100);
	}
}

static void *thread_work(void *a)
{
    pthread_t th;
    t_philo *philo;

    philo = (t_philo *)a;
    philo->last_eat = get_time();
    philo->max_t = philo->last_eat + philo->arg->death_time;
    if (pthread_create(&th, NULL, &monitor, a) != 0)
        return ((void *)1);
    pthread_detach(th);
    while (1)
    {
        take_fork(philo);
        philo_eat(philo);
        clean_fork(philo);
        philo_print(philo, 2);
    }
    return (NULL);
}

int philo_work(t_arg *arg)
{
    int i;
    void *philo;
    pthread_t   th;

    i = 0;
    arg->start_time = get_time();
    if (arg->eat_num > 0)
    {
        if (pthread_create(&th, NULL, &thread_work_count, (void*)arg) != 0)
            return (0);
        pthread_detach(th);
    }
    while (i < arg->philo_n)
    {
        philo = (void *)(&arg->philo[i]);
        if (pthread_create(&th, NULL, &thread_work, philo) != 0)
            return (1);
        pthread_detach(th);
        usleep(100);
        i++;
    }
    return (0);
}