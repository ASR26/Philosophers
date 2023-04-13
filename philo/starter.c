/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 08:04:38 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/24 09:46:09 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * This function will handle the eating process of a philosopher
 */

void	philo_eating(t_philo *p)
{
	t_rules	*r; //we create a new rule so we do not modify our philosopher rules

	r = p->rules; //we assign value to our new rule
	if (r->nb_philo == 1)
	{
		pthread_mutex_lock(&(r->forks[p->left_fork_id]));
		printer(r, p->id, "has taken a fork"); //print a message for the fork taken
		smart_sleep(r->tt_die + 1, r); //since the philosopher cannot eat we will wait for the time necessary for him to die
		r->dead = 1; //set dead to 1 since 1 philosopher has died
		pthread_mutex_unlock(&(r->forks[p->left_fork_id])); //unlock the fork mutex to avoid leaks
		printer(r, p->id, "has died"); //print dead message
		return ; //finish this function since the philosopher is dead so he cannot do anything else
	}
	pthread_mutex_lock(&(r->forks[p->left_fork_id])); //we lock the mutex of the left fork
	printer(r, p->id, "has taken a fork"); //print a message for the fork taken
	pthread_mutex_lock(&(r->forks[p->right_fork_id])); //we lock the mutex of the right fork
	printer(r, p->id, "has taken a fork"); //print a message for the fork taken
	pthread_mutex_lock(&(r->meal_check)); //we lock the meal check so only one will use this mutex
	printer(r, p->id, "is eating"); //we print eating message
	p->t_last_meal = get_time(); //we get the time for our new last meal
	pthread_mutex_unlock(&(r->meal_check)); //we unlock the meal check mutex since others can use it
	smart_sleep(r->tt_eat, r); //we call the smart sleep function to use the time to eat for waiting
	(p->nb_meals)++; //increase the number of meals
	pthread_mutex_unlock(&(r->forks[p->left_fork_id])); //unlock left fork
	pthread_mutex_unlock(&(r->forks[p->right_fork_id])); //unlock right fork
}

/*
 * This function will be the first thing that the new thread will do, it will manage the philosopher behaviour
 */

void	*p_thread(void *v_philo)
{
	t_philo	*p; //this wil be the pointer for our philosopher
	t_rules	*r; //this will be the pointer for our rules

	p = (t_philo *)v_philo; // point to our argument casting it as a "t_philo" because it was "void"
	r = p->rules; //point to the rules
	if (p->id % 2) //if the id is odd
		usleep(10000); //we wait for 10000 microseconds (10 miliseconds). We do this to avoid getting the same fork by 2 philosophers
	while (!(r->dead)) //while there is not a dead philosopher
	{
		philo_eating(p); //we call the eating function
		if (r->nb_philo > 1)
		{
			if (r->all_eaten) //if everybody has eaten
				break ; //we are finished so we break and return
			printer (r, p->id, "is sleeping"); //we print sleep message
			smart_sleep(r->tt_sleep, r); //we wait the time for sleep using our "smart_sleep" function
			printer (r, p->id, "is thinking"); //we print think message
		}
	}
	return (NULL); //we return NULL since the (void *) cannot return nothing
}

/*
 * This function will free and manage all remains after starter end
 */

void	exit_starter(t_rules *r, t_philo *p)
{
	int	i; //this will be our iterator


	i = 0; //we set our iterator to 0
	while (i < r->nb_philo) //for each philosopher
	{
		pthread_join(p[i].thread_id, NULL); //we wait for every philosopher thread to finish
		i++;
	}
	i = 0; //set the iterator to 0 again
	while (i < r->nb_philo) //for each philosopher (forks in this case)
	{
		pthread_mutex_destroy(&(r->forks[i])); //we destroy every fork mutex (because we have the same amount of philosophers and forks)
		i++;
	}
	pthread_mutex_destroy(&(r->writing)); //we destroy the writing mutex
}

/*
 * This function will check if there is a dead philosopher and if they have eaten
 */

void	death_checker(t_rules *r, t_philo *p)
{
	int	i; //this will be our iterator

	while (!(r->all_eaten)) //while they have not finished eating
	{
		i = 0; //we set the iterator to 0
		while (i < r->nb_philo && !(r->dead)) //for each philosopher and while they are all alive
		{
			pthread_mutex_lock(&(r->meal_check)); //we lock meal_check mutex
			if (time_passed(p[i].t_last_meal, get_time()) > r->tt_die) //if the time since the last meal is higher than time to die we print dead message and set death to 1
			{
				printer(r, i, "has died");
				r->dead = 1;
			}
			pthread_mutex_unlock(&(r->meal_check)); //we unlock meal_check
			i++;
			usleep(100); //sleep for 100 microseconds
		}
		if (r->dead) //if there is a dead philosopher we finish our loop
			break ;
		i = 0; //we set the iterator to 0
		while (r->nb_eat != -1 && i < r->nb_philo && p[i].nb_meals >= r->nb_eat) //if nb_eat is not -1 (was not defined at the execution), for each philosopher, if he have eaten at least nb_eat times
			i++; //go to the next philosopher
		if (i == r->nb_philo) //if we have passed every philosopher means all of them have eaten enough
			r->all_eaten = 1; //set all_eaten to 1
	}
}

/*
 * This function will do all the job (calling other functions of course) once everyting is initialized
 */

int	starter(t_rules *rules)
{
	int		i; //i will be a iterator
	t_philo	*p; //p will be a pointer to a philosopher since we have an array of them

	i = 0; //set the iterator to 0
	p = rules->philo; //point to the first philo so we can use the iterator to access to them
	rules->first_time = get_time(); //we save the first timestamp in first_time using our function "get_time"
	while (i < rules->nb_philo) //while we have a philosopher to check
	{
		if (pthread_create(&(p[i].thread_id), NULL, p_thread, &(p[i]))) //we create a thread for a philo, giving it: the direction of the philosopher thread_id,
		// NULL (since we want a default thread), the function p_thread, the direction of the philosopher
			return (1); //if the pthread_create does not return 0, it means error so we return 1 
		p[i].t_last_meal = get_time(); //we save the time of the last meal for this philosopher
		i++; //go to the next philosopher
	}
	if (rules->nb_philo > 1)
		death_checker(rules, rules->philo); //we check if we have a dead philosopher
	exit_starter(rules, p); //we exit the starter function, freeing what is necessary
	return (0); //finish the function
}
