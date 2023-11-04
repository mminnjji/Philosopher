/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:37:56 by man               #+#    #+#             */
/*   Updated: 2023/11/04 17:14:45 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void interval_usleep(long long sleep_t, t_arg *arg)
{
    long long n;

    n = get_time();
    while (!(arg->isdead))
	{
		if (n - get_time() >= sleep_t)
			break ;
		usleep(50);
	}
}

void philo_print(t_philo *philo, int type)
{
    pthread_mutex_lock(&(philo->arg->write_m));
    if (type == 4)
    {
        printf("%lli ", get_time() - philo->arg->start_time);
		printf("%i ", philo->id + 1);
        if (type == 0)
		    printf(" died\n");
    }
	if (!(philo->arg->isdead))
	{
		printf("%lli ", get_time() - philo->arg->start_time);
		printf("%i ", philo->id + 1);
        if (type == 0)
		    printf(" has taken a fork\n");
        if (type == 1)
            printf(" is eating\n");
        if (type == 2)
            printf(" is sleeping\n");
        if (type == 3)
            printf("min_eat reached\n");
	}
	pthread_mutex_unlock(&(philo->arg->write_m));
}

void clear_mutex(t_arg *arg)
{
    int i;

    if (arg->fork_m)
    {
        i = 0;
        while (i < arg->philo_n)
            pthread_mutex_destroy(&arg->fork_m[i++]);
        free(arg->fork_m);
    }
    if (arg->philo)
    {
        i = 0;
        while (i < arg->philo_n)
        {
            pthread_mutex_destroy(&arg->philo[i].check_m);
            pthread_mutex_destroy(&arg->philo[i++].eat_m);
        }
        free(arg->philo);
    }
    pthread_mutex_destroy(&arg->write_m);
    pthread_mutex_destroy(&arg->dead_m);
}

int print_error(t_arg *arg, int n)
{
    if (n == 0)
        write(2, "Bad argument\n", 13);
    else if (n == 1)
        write(2, "Malloc error - no memory\n", 25);
    else if (n == 2)
        write(2, "Mutex initializing error\n", 25);
    clear_mutex(arg);
    return (0);
}

int main(int argc, char **argv)
{
    t_arg arg;
    int err;

    if (argc < 5 || argc > 6)
        return (print_error(&arg, 0));
    err = arg_init(argv, argc, &arg);
    if (err == 1)
        return (print_error(&arg, 1));
    if (err == 2)
        return (print_error(&arg, 2));
    if (err == 3)
        return (print_error(&arg, 3));
    if (philo_work(&arg))
        return (print_error(&arg, 3));
    clear_mutex(&arg);
    return(0);
}