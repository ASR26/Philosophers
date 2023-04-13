/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/16 08:30:10 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/23 08:58:56 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * This function will initialize some mutex, necessary for the proper execution
 * of the program
 */

int	init_mutex(t_rules *rules) //it will receive the rules which are set already
{
	int	i;

	i = 0;
	while (i < rules->nb_philo) //for each philosopher, given by nb_philo (number of philosophers)
	{
		if (pthread_mutex_init(&(rules->forks[i]), NULL)) //we create a mutex for every fork, which are as many as philosophers, this function returns 0 for success
			return (1); //in case "pthread_mutex_init" return 1 it means there has been an error so we stop this function and return 1
		i++; //go to the next fork
	}
	if (pthread_mutex_init(&(rules->writing), NULL)) //same as before but for writing this time, "writing" mutex is for the printing messages time, so we avoid overlapping values during message prints
		return (1);
	if (pthread_mutex_init(&(rules->meal_check), NULL)) //same as before but for meal_check this time, "meal_check" mutex is for the eating time, so we avoid overlapping during eating time
		return (1);
	return (0); // finish the function
}

/*
 * This function will initialize a philosopher with its necessary values
 */

int	init_philo(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philo) //while we have any philosopher to create we create a new one
	{
		rules->philo[i].id = i; //id will be its number so it will go from 0 to nb_philo
		rules->philo[i].nb_meals = 1; //nb_meals will be the times this philosopher has eaten, starting from 0 since he has not eaten yet
		rules->philo[i].left_fork_id = i; //his left fork will be the same as his id
		rules->philo[i].right_fork_id = (i + 1) % rules->nb_philo; //his right fork will be his id + 1, in case this is the last philosopher he will take the first fork as his right one
		rules->philo[i].t_last_meal = 0; //t_last_meal will be the time of his last meal, starting from 0 since he has not eaten yet
		rules->philo[i].rules = rules; //rules will be the rules themselves, so each philosopher can access to them anytime
		i++; //go to the next philosopher
	}
	return (0);
}

/*
 * This is the main initializing function, it will set everything so the program can work properly
 * It will receive 2 arguments:
 * -rules: it is an structure which will hold every necesary value for the program (we will explain them later)
 * -argv: it is the argv recived in "main"
 */

int	init_all(t_rules *rules, char **argv)
{
	rules->nb_philo = ft_atoi(argv[1]); //nb_philo is the number of philosophers, cannot be less than 1 nor more than 200
	rules->tt_die = ft_atoi(argv[2]); //tt_die is the time to die for a philosopher, cannot be less than 0
	rules->tt_eat = ft_atoi(argv[3]); //tt_eat is the time a philosopher takes to eat once, cannot be less than 0
	rules->tt_sleep = ft_atoi(argv[4]); //tt_sleep is the time a philosopher spends sleeping, cannot be less than 0
	rules->dead = 0; //dead will work as a boolean, it starts as 0 and is set to 1 if a philosopher dies
	rules->all_eaten = 0; //all_eaten will work as a boolean, it starts to 0 and is set to 1 if every philosopher has eaten the given amount of times
	if (rules->nb_philo < 1 || rules->tt_die < 0 || rules->tt_eat < 0 || \
		rules->tt_sleep < 0 || rules->nb_philo > 200) // if any of the values given is not valid this function will end, returning 1
		return (1);
	if (argv[5]) //if there is an argv[5] it will be nb_eat
	{
		rules->nb_eat = ft_atoi(argv[5]); //nb_eat is the amount of times that a philosopher needs to eat, cannot be less than 1
		if (rules->nb_eat < 1) //if it is less than 1, this function returns 1
			return (1);
	}
	else
		rules->nb_eat = -1; //if  nb_eats is not given we set it to -1, so we can check later that it is not necessary
	if (init_mutex(rules)) //we call init_mutex function which will initialize some necessary mutex, in case there is an error there it will return 1 so we will return ERROR_MUTEX (2) for the "select_error" function
		return (ERROR_MUTEX);
	init_philo(rules); //we call init_philo so we create every necessary philosopher and asign them their own values
	return (0); //finish the function
}
