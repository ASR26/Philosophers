/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/12 10:20:23 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/24 11:28:10 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	usage(void)
{
	printf("-------------------------------------------");
	printf("\nHow to use:\n");
	printf("./philo number_of_philosophers time_to_die time_to_eat ");
	printf("time_to_sleep [number_of_meals]\n");
	printf("number of philosophers: 1-200\n");
	printf("time_to_die: time that takes a philosopher to starve in ");
	printf("miliseconds must be > 0\n");
	printf("time_to_eat: time that takes a philosopher to eat in ");
	printf("miliseconds, must be > 0\n");
	printf("time_to_sleep: time that takes a philosopher to sleep after ");
	printf("eating in miliseconds, must be > 0\n");
	printf("number_of_meals (optional): number of times that every ");
	printf("philosopher needs to eat in order to end the program, ");
	printf("must be > 0\n");
}

long long	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return ((t.tv_sec * 1000) + (t.tv_usec / 1000));
}

long long	time_passed(long long past, long long current)
{
	return (current - past);
}

void	smart_sleep(long long time, t_rules *r)
{
	long long	i;

	i = get_time();
	while (!(r->dead))
	{
		if (time_passed(i, get_time()) >= time)
			break ;
		usleep(50);
	}
}

int	ft_atoi(const char *str)
{
	size_t	num;
	size_t	isneg;
	size_t	i;

	i = 0;
	num = 0;
	isneg = 0;
	while (str[i] != '\0' && (str[i] == ' ' || str [i] == '\t' || str[i] == '\n'
			|| str[i] == '\r' || str[i] == '\v' || str[i] == '\f'))
		i++;
	if (str[i] != '\0' && str[i] == '-')
	{
		isneg = 1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (str[i] != '\0' && ft_isdigit(str[i]))
			num = (num * 10) + (str[i++] - '0');
	if (isneg == 1)
		return (-num);
	return (num);
}
