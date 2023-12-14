/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: man <man@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 09:37:56 by man               #+#    #+#             */
/*   Updated: 2023/12/14 15:38:19 by man              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	philo_init(t_arg **arg)
{
	int		i;

	i = 0;
	if (!(*arg)->philo)
		return (0);
	while (i < (*arg)->philo_n)
	{
		(*arg)->philo[i].id = i + 1;
		(*arg)->philo[i].eat_count = 0;
		(*arg)->philo[i].left_fork = i;
		(*arg)->philo[i].right_fork = (i + 1) % (*arg)->philo_n;
		(*arg)->philo[i].last_eat = 0;
		(*arg)->philo[i].arg = (*arg);
		i++;
	}
	return (1);
}

int	mutex_init(t_arg *p)
{
	int	i;

	i = 0;
	while (i < p->philo_n)
	{
		if (pthread_mutex_init(&(p->fork[i]), NULL) == -1)
			return (1);
		i++;
	}
	if (pthread_mutex_init(&(p->eat_check), NULL) == -1 || \
			pthread_mutex_init(&(p->work), NULL) == -1 || \
			pthread_mutex_init(&(p->dead_check), NULL) == -1 || \
			pthread_mutex_init(&(p->num_check), NULL) == -1 || \
			pthread_mutex_init(&(p->count_check), NULL) == -1)
		return (1);
	return (0);
}

int	arg_init(char **argv, int argc, t_arg *p)
{
	p->philo_n = ft_atoi(argv[1]);
	p->death_time = ft_atoi(argv[2]);
	p->eat_time = ft_atoi(argv[3]);
	p->sleep_time = ft_atoi(argv[4]);
	p->eat_num = -1;
	p->isdead = 0;
	p->eatend = 0;
	if (argc == 6)
		p->eat_num = ft_atoi(argv[5]) - 1;
	if (p->philo_n < 1 || p->death_time < 0 || p->eat_time < 0 || \
			p->sleep_time < 0 || (argc == 6 && p->eat_num < 0))
		return (1);
	p->fork = malloc(sizeof(pthread_mutex_t) * p->philo_n);
	if (!(p->fork))
		return (2);
	if (mutex_init(p))
		return (3);
	return (0);
}

int	main(int argc, char **argv)
{
	t_arg	*arg;
	int		err;

	if (argc < 5 || argc > 6)
		return (print_error(0));
	arg = malloc(sizeof(t_arg));
	if (!arg)
		return (print_error(2));
	err = arg_init(argv, argc, arg);
	if (err < 4 && err > 0)
		return (print_error(err));
	arg->philo = malloc(sizeof(t_philo) * (arg->philo_n));
	if (!philo_init(&arg))
		return (print_error(1));
	if (philo_work(arg))
		return (print_error(4));
	pthread_mutex_destroy(&(arg->dead_check));
	pthread_mutex_destroy(&(arg->eat_check));
	pthread_mutex_destroy(&(arg->count_check));
	pthread_mutex_destroy(&(arg->num_check));
	free(arg->philo);
	free(arg);
	return (0);
}
