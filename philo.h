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
    int flag;
    long long max_t;
    struct s_arg *arg;
    pthread_mutex_t check_m;
    pthread_mutex_t eat_m;
}   t_philo;

typedef struct s_arg
{
    int philo_n;
    long long death_time;
    long long eat_time;
    long long sleep_time;
    long long start_time;
    int eat_num;
    int isdead;
    pthread_mutex_t write_m;
    pthread_mutex_t dead_m;
    pthread_mutex_t *fork_m;
    t_philo *philo;
} t_arg;

int	ft_atoi(const char *str);
int arg_init(char **argv, int argc, t_arg *p);
int philo_work(t_arg *arg);
long long get_time();
void interval_usleep(long long sleep_t, t_arg *arg);
void philo_print(t_philo *philo, int type);
int print_error(t_arg *arg, int n);
void take_fork(t_philo *philo);
void clean_fork(t_philo *philo);
void philo_eat(t_philo *philo);

#endif