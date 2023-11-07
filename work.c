/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   work.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:37:59 by man               #+#    #+#             */
/*   Updated: 2023/11/07 16:46:24 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void philo_eat(t_philo *philo)
{
    t_arg *arg;

    arg = philo->arg;
    pthread_mutex_lock(&(arg->fork[philo->left_fork]));
	philo_print(arg, philo->id, "has taken a fork");
	pthread_mutex_lock(&(arg->fork[philo->right_fork]));
	philo_print(arg, philo->id, "has taken a fork");
	pthread_mutex_lock(&(arg->eat_check));
	philo_print(arg, philo->id, "is eating");
	philo->last_eat = get_time();
	pthread_mutex_unlock(&(arg->eat_check));
	interval_usleep(arg->eat_time, arg);
	(philo->eat_count)++;
	pthread_mutex_unlock(&(arg->fork[philo->left_fork]));
	pthread_mutex_unlock(&(arg->fork[philo->right_fork]));
}

void end_check(t_arg *arg)
{
    int i;
    t_philo **philo;

    philo = arg->philo;
    i = 0;
    while (i < arg->philo_n)
    {
        pthread_join(philo[i]->th_id, NULL);
        i++;
    }
    i = 0;
    while (i < arg->philo_n)
    {
        pthread_mutex_destroy(&(arg->fork[i]));
        i++;
    }
    pthread_mutex_destroy(&(arg->work));
}

void dead_check(t_arg *arg, t_philo **philo)
{
    int i;

    while (!(arg->eatend))
    {
        i = 0;
        while (i < arg->philo_n && !(arg->isdead))
        {
            pthread_mutex_lock(&(arg->eat_check));
            if (get_time() - philo[i]->last_eat > arg->death_time)
            {
                philo_print(arg, i + 1, "died");
                arg->isdead = 1;
            }
            pthread_mutex_unlock(&(arg->eat_check));
            usleep(100);
            i++;
        }
        if (arg->isdead)
            break;
        i = 0;
        while (arg->eat_num != 0 && i < arg->philo_n && philo[i]->eat_count >= arg->eat_num)
            i++;
        if (i == arg->philo_n)
            arg->eatend = 1;
    }
}

void *thread_work(void *a)
{
    int i;
    t_arg *arg;
    t_philo *philo;

    i = 0;
    philo = (t_philo *)a;
    arg = philo->arg;
    if (philo->id % 2 == 0)
        usleep(100);
    while (!(arg->isdead))
    {
        philo_eat(philo);
        if (arg->eatend)
            break;
        philo_print(arg, philo->id, "is sleeping");
        interval_usleep(arg->sleep_time, arg);
        philo_print(arg, philo->id, "is thinking");
        i++;
    }
    return (NULL);
}

int philo_work(t_arg *arg)
{
    int i;
    t_philo **philo;

    i = 0;
    philo = arg->philo;
    arg->start_time = get_time();
    while (i < arg->philo_n)
    {
        if (pthread_create(&(philo[i]->th_id), NULL, thread_work, philo[i]))
            return (1);
        philo[i]->last_eat = get_time(); // 맛있~ 게 먹이고 시간재기
        i++; 
    }
    dead_check(arg, philo);
    end_check(arg);
    return (0);
}