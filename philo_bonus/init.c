/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 08:30:10 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/30 08:52:23 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * This function will initialize every semaphore so we can use them later
 */

int	init_semaphores(t_rules *r)
{
	//we unlink our semaphores, because in case the program ends unexpectedly they will be open
	sem_unlink("meal_check");
	sem_unlink("writing");
	sem_unlink("forks");
	//we open our semaphores giving them the permissions and the value
	r->meal_check = sem_open("meal_check", O_CREAT, S_IRWXU, 1);
	r->writing = sem_open("writing", O_CREAT, S_IRWXU, 1);
	r->forks = sem_open("forks", O_CREAT, S_IRWXU, r->nb_philo);
	if (r->meal_check <= 0 || r->writing <= 0 | r->forks <= 0) //if any of the values is nor valid we return an error
		return (1);
	return (0); //we return a success
}

/*
 * This function will initialize every philosopher
 */
int	init_philo(t_rules *rules)
{
	int	i;

	i = 0; //we set our iterator to 0
	while (i < rules->nb_philo) //while we need a new iterator we repeat it
	{
		rules->philo[i].id = i; //we assign an id to our philosopher
		rules->philo[i].nb_meals = 0; //we assign 0 meals to our philosopher since he has not eaten yet
		rules->philo[i].t_last_meal = 0; //we assign the time of his last meal to 0 since he has not eaten yet
		rules->philo[i].rules = rules; //we assign him the rules so he can access to them anytime
		i++; //we go to the next philosopher
	}
	return (0);
}

/*
 * This function will initialize every necessary value for our program
 */
int	init_all(t_rules *rules, char **argv)
{
	rules->nb_philo = ft_atoi(argv[1]); //we set the number of philosophers
	rules->tt_die = ft_atoi(argv[2]); //we set the time necessary to die
	rules->tt_eat = ft_atoi(argv[3]); //we set the time necessary to eat
	rules->tt_sleep = ft_atoi(argv[4]); //we set the time necessary to sleep
	rules->dead = 0; //we set dead variable to 0 since they are all alive
	if (rules->nb_philo < 1 || rules->tt_die < 0 || rules->tt_eat < 0 || \
		rules->tt_sleep < 0 || rules->nb_philo > 200) //if any of the values is not valid we return 1 (error)
		return (1);
	if (argv[5]) //if number of meals exist
	{
		rules->nb_eat = ft_atoi(argv[5]); // we set the number of meals
		if (rules->nb_eat < 1) //if number of meals is not valid we return an error
			return (1);
	}
	else
		rules->nb_eat = -1; //if number of meals does not exist we set it to -1
	if (init_semaphores(rules)) //we initialize the semaphores 
		return (ERROR_SEMAPHORE); //if the initialization does not return 0 we return an error code for that (2)
	init_philo(rules); //we initialize philosophers
	return (0);
}
