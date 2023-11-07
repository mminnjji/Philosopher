#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_philo
{
    int id;
    int left_fork;
    int right_fork;
    int eat_count;
    long long last_eat;
    struct s_arg *arg;
    pthread_t th_id;
}   t_philo;

typedef struct s_arg
{
    int philo_n;
    int death_time;
    int eat_time;
    int sleep_time;
    int eat_num;
    int eatend;
    int isdead;
    long long start_time;
    pthread_mutex_t eat_check;
    pthread_mutex_t work;
    pthread_mutex_t *fork;
    struct s_philo **philo;
} t_arg;

int	ft_atoi(const char *str);
void philo_eat(t_philo *philo);
void *thread_work(void *a);
void end_check(t_arg *arg);
void dead_check(t_arg *arg, t_philo **philo);
int philo_work(t_arg *arg);
long long get_time();
void interval_usleep(long long sleep_t, t_arg *arg);
void philo_print(t_arg *arg, int id, char *str);
int print_error(int n);

#endif