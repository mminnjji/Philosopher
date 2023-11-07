/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:37:56 by man               #+#    #+#             */
/*   Updated: 2023/11/07 17:02:14 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int philo_init(t_arg *arg)
{
    int i;
    t_philo **philo;

    i = 0;
    philo = malloc(sizeof(t_philo *));
    if (!philo)
        return (0);
    while (i < arg->philo_n)
    {
        philo[i] = malloc(sizeof(t_philo));
        if (!philo[i])
            return (0);
        philo[i]->id = i + 1; // 1번부터 번호매김
        philo[i]->eat_count = 0;
		philo[i]->left_fork = i + 1;
		philo[i]->right_fork = (i + 2) % arg->philo_n; // n -> 1로 이어지도록 
		philo[i]->last_eat = 0; // 마지막으로 먹은 시간
		philo[i]->arg = arg; // arg 요소 미리 갖고 있기 - 이중참조
        i++;
    }
    arg->philo = philo;
    return (1);
}

int arg_init(char **argv, int argc, t_arg *p)
{
    int i;

    p->philo_n = ft_atoi(argv[1]);
    p->death_time = ft_atoi(argv[2]);
    p->eat_time = ft_atoi(argv[3]);
    p->sleep_time = ft_atoi(argv[4]);
    p->eat_num = -1;
    p->isdead = 0;
    p->eatend = 0;
    if (argc == 6)
        p->eat_num = ft_atoi(argv[5]) - 1;
    if (p->philo_n < 1 || p->death_time < 0 || p->eat_time < 0 || p->sleep_time < 0 || (argc == 6 && p->eat_num < 0))
        return (1);
    p->fork = malloc(sizeof(pthread_mutex_t) * p->philo_n);
    if (!p->fork)
        return (2);
    i = 0;
    while (i < p->philo_n)
    {
        if (pthread_mutex_init(&(p->fork[i]), NULL) == -1) // 포크를 뮤텍스로 만들어 지정하기
            return (3);
        i++;
    }
    if (pthread_mutex_init(&(p->eat_check), NULL) == -1 ||  pthread_mutex_init(&(p->work), NULL) == -1)
        return (3);
    return (0);
}

int main(int argc, char **argv)
{
    t_arg arg;
    int err;

    if (argc < 5 || argc > 6)
        return (print_error(0));
    err = arg_init(argv, argc, &arg);
    if (err == 1)
        return (print_error(1));
    if (err == 2)
        return (print_error(2));
    if (err == 3)
        return (print_error(3));
    if (!philo_init(&arg))
        return (print_error(1));
    if (philo_work(&arg))
        return (print_error(3));
    return(0);
}