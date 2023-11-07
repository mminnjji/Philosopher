#include "philo.h"

int	ft_isdigit(int c)
{
	if (c <= '9' && c >= '0')
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	int			i;
	long long	plus;
	long long	sum;

	i = 0;
	plus = 1;
	sum = 0;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) \
				|| str[i] == 32))
		i++;
	if (str[i] == '+' || str[i] == '-')
		if (str[i++] == '-')
			plus *= -1;
	while (ft_isdigit(str[i]))
	{
		if ((sum == LLONG_MAX / 10 && str[i] > \
					LLONG_MAX % 10 + '0' + (plus != 1)) \
					|| sum > LLONG_MAX / 10)
			return (-1 * ((plus == 1)));
		sum *= 10;
		sum += (str[i++] - '0');
	}
	return ((int)(sum * plus));
}