/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 08:04:38 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/30 08:47:43 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * This is the eating function where the philosophers will take forks and eat
 */
void	philo_eating(t_philo *p)
{
	t_rules	*r; //we will hold our rules here for easir access

	r = p->rules; //we asign the rules
	if (r->nb_philo == 1) //if there is only one philosopher we want an specific behaviour
	{
		sem_wait(r->forks); //we lock the fork
		printer(r, p->id, "has taken a fork"); //print the message
		smart_sleep(r->tt_die + 1, r); //wait for time to die + 1
		r->dead = 1; //set dead to 1
		sem_post(r->forks); //unlock the fork
		printer(r, p->id, "has died"); //print dead message
		exit (1); //exit the program
	}
	//we lock 2 forks and print 2 messages
	sem_wait(r->forks);
	printer(r, p->id, "has taken a fork");
	sem_wait(r->forks);
	printer(r, p->id, "has taken a fork");
	//we lock the meal_check semaphore and print the eating message
	sem_wait(r->meal_check);
	printer(r, p->id, "is eating");
	p->t_last_meal = get_time(); //we update the last meal time
	sem_post(r->meal_check); // we unlock the semaphore
	smart_sleep(r->tt_eat, r); //we wait for the time needed to eat
	(p->nb_meals)++; //we increase the number of meals for the philosopher
	//we unlock both forks
	sem_post(r->forks);
	sem_post(r->forks);
}

/*
 * this function will be running in a thread during the execution of the program to check if our
 * philosophers are dead or if they have eaten enough
 */
void	*death_checker(void *v_philo)
{
	t_rules	*r; //this will be our rules variable
	t_philo	*p; //this will be our philosophers array

	p = (t_philo *)v_philo; //we cast our philosopher which was void because the function must receive them like that
	r = p->rules; //we asign our rules
	while (1) //in an infinite loop
	{
		sem_wait(r->meal_check); //we lock the semaphore for checking meals, since we do not want more than 1 philosopher checking at the same time
		if (time_passed(p->t_last_meal, get_time()) > r->tt_die) //if the time passed is longer than the time to die
		{
			printer(r, p->id, "has died"); //print a dead message
			r->dead = 1; //set dead variable to 1
			sem_wait(r->writing); //lock the writing semaphore since we do not want that any other philosopher writes anymore
			exit(1); //exit the program
		}
		sem_post(r->meal_check); //unlock the semaphore for checking meals
		if (r->dead) //if any of them have died we stop the loop
			break ;
		usleep(1000); //we wait for 1 ms to avoid overlaping
		if (p->nb_meals >= r->nb_eat && r->nb_eat != -1) //if the have eaten enough we break the loop
			break ;
	}
	return (NULL); //we must return NULL since it is a void * function
}

/*
 * This function will manage the philosopher behaviour telling him what to do
 */
void	p_process(void *v_philo)
{
	t_philo	*p;
	t_rules	*r;

	p = (t_philo *)v_philo; //we cast our philosopher (which must be void as parameter) to t_philo *, so we can use it properly
	r = p->rules; //we asign our rules to a variable to easier access
	p->t_last_meal = get_time(); //we get our current time so we know when a philosopher has started a process
	pthread_create(&(p->death_checker), NULL, death_checker, v_philo); //we create a thread on 'death checker', it will execute death_checker function and we will give v_philo as the argument to the function
	if (p->id % 2) //if the pid is odd we tell it to sleep for 10 ms to avoid 2 philosophers trying to eat at the same exact time
		usleep(10000);
	while (!(r->dead)) //while they are not dead
	{
		philo_eating(p); //we call the eating function
		if (r->nb_philo > 1) //if there is more than 1 philosopher
		{
			if (p->nb_meals >= r->nb_eat && r->nb_eat != -1) //if they have eaten enough we break the loop
				break ;
			printer (r, p->id, "is sleeping"); //we print they are sleeping
			smart_sleep(r->tt_sleep, r); //we make them sleep
			printer (r, p->id, "is thinking"); //we print they are thinking
		}
	}
	pthread_join(p->death_checker, NULL); //we stop our thread because we do not want to check anymore if they are dead or if they have eaten enough
	if (r->dead) //if any of them is dead we exit with an error (1), if they are alive we exit with a success (0)
		exit(1);
	exit(0);
}

/*
 * This function will waut for the child process to end kill the parent and close every semaphore
 */
void	exit_starter(t_rules *r)
{
	int	i; //this will be our iterator
	int	ret; //this will hold the return value

	i = 0;
	while (i < r->nb_philo) //for each philosopher
	{
		waitpid(-1, &ret, 0); //we wait for the child process (-1 means any child process) to change its state (ret)
		if (ret != 0) //if ret is not 0
		{
			i = 0; //we set our iterator to 0 again, since we want to go through every philosopher
			while (i < r->nb_philo) //for each philosopher
				kill(r->philo[i++].proc_id, 15); //we send a terminate signal to every process id (SIGTERM = 15)
			break ; //we stop the loop to finish the function
		}
		i++;
	}
	//we close our three semaphores
	sem_close(r->forks);
	sem_close(r->meal_check);
	sem_close(r->writing);
	//we unlink them for future executions
	sem_unlink("forks");
	sem_unlink("meal_check");
	sem_unlink("writing");
}

/*
 * This function will do all the process after everything is properly initialized
 */
int	starter(t_rules *rules)
{
	int		i; //this will be out iterator
	t_philo	*p; //this will be our philosopher

	i = 0; //we set our iterator to 0
	p = rules->philo; //we assign the philosophers array to our variable
	rules->first_time = get_time(); //we assign our first time so we can check how long they have been alive
	while (i < rules->nb_philo) //for each philosopher
	{
		p[i].proc_id = fork(); //we assign to our philosopher process id a fork
		if (p[i].proc_id < 0) //if it is less than 0 we return an error
			return (1);
		if (p[i].proc_id == 0) //if it is 0 it means we are in the child process
			p_process(&(p[i])); //the child will execute the p_process function (this will manage the philosopher behaviour) giving it our philosopher memory location
		i++; //we go to the next philosopher
		usleep(100); //we wait for 0.1 miliseconds to avoid overlaps
	}
	exit_starter(rules); //we go to the exit starter function, which will free everything to avoid leaks
	return (0);
}
