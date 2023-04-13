/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 08:30:10 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/30 10:54:08 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_semaphores(t_rules *r)
{
	sem_unlink("meal_check");
	sem_unlink("writing");
	sem_unlink("forks");
	r->meal_check = sem_open("meal_check", O_CREAT, S_IRWXU, 1);
	r->writing = sem_open("writing", O_CREAT, S_IRWXU, 1);
	r->forks = sem_open("forks", O_CREAT, S_IRWXU, r->nb_philo);
	if (r->meal_check <= 0 || r->writing <= 0 | r->forks <= 0)
		return (1);
	return (0);
}

int	init_philo(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philo)
	{
		rules->philo[i].id = i;
		rules->philo[i].nb_meals = 0;
		rules->philo[i].t_last_meal = 0;
		rules->philo[i].rules = rules;
		i++;
	}
	return (0);
}

int	init_all(t_rules *rules, char **argv)
{
	rules->nb_philo = ft_atoi(argv[1]);
	rules->tt_die = ft_atoi(argv[2]);
	rules->tt_eat = ft_atoi(argv[3]);
	rules->tt_sleep = ft_atoi(argv[4]);
	rules->dead = 0;
	if (rules->nb_philo < 1 || rules->tt_die < 0 || rules->tt_eat < 0 || \
		rules->tt_sleep < 0 || rules->nb_philo > 200)
		return (1);
	if (argv[5])
	{
		rules->nb_eat = ft_atoi(argv[5]);
		if (rules->nb_eat < 1)
			return (1);
	}
	else
		rules->nb_eat = -1;
	if (init_semaphores(rules))
		return (ERROR_SEMAPHORE);
	init_philo(rules);
	return (0);
}
