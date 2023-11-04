#include "philo.h"

static int mutex_init(t_arg *arg)
{
    int i;

    pthread_mutex_init(&arg->write_m, NULL);
    pthread_mutex_init(&arg->dead_m, NULL);
    pthread_mutex_lock(&arg->dead_m);
    arg->fork_m = (pthread_mutex_t*)malloc(sizeof(*(arg->fork_m)) * arg->philo_n);
    if (!arg->fork_m)
        return (3);
    i = 0;
    while (i < arg->philo_n)
        pthread_mutex_init(&arg->fork_m[i++], NULL);
    return (0);
}

static void philo_init(t_arg *arg)
{
    int i;

    i = 0;
    while (i < arg->philo_n)
    {
        arg->philo[i].id = i; // 1번부터 번호매김
        arg->philo[i].eat_count = 0;
		arg->philo[i].left_fork = i;
		arg->philo[i].right_fork = (i + 1) % arg->philo_n; // n -> 1로 이어지도록 
		arg->philo[i].last_eat = 0; // 마지막으로 먹은 시간
		arg->philo[i].arg = arg; // arg 요소 미리 갖고 있기 - 이중참조
        arg->philo[i].flag = 0;
        pthread_mutex_init(&arg->philo[i].check_m, NULL);
        pthread_mutex_init(&arg->philo[i].check_m, NULL);
        pthread_mutex_lock(&arg->philo[i].eat_m);
        i++;
    }
}

int arg_init(char **argv, int argc, t_arg *p)
{
    p->philo_n = ft_atoi(argv[1]);
    p->death_time = (long long)ft_atoi(argv[2]);
    p->eat_time = (long long)ft_atoi(argv[3]);
    p->sleep_time = (long long)ft_atoi(argv[4]);
    p->start_time = get_time();
    p->eat_num = 0;
    p->isdead = 0;
    if (argc == 6)
        p->eat_num = ft_atoi(argv[5]);
    if (p->philo_n < 1 || p->death_time < 0 || p->eat_time < 0 || p->sleep_time < 0 || (argc == 6 && p->eat_num <= 0))
        return (1);
    p->fork_m = NULL;
    p->philo = NULL;
    p->philo = (t_philo *)malloc(sizeof(*(p->philo)) * p->philo_n);
    if (!p->philo)
        return (2);
    philo_init(p);
    return (mutex_init(p));
}