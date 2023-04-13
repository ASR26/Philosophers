/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 08:04:38 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/31 08:50:11 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eating(t_philo *p)
{
	t_rules	*r;

	r = p->rules;
	if (r->nb_philo == 1)
	{
		sem_wait(r->forks);
		printer(r, p->id, "has taken a fork");
		smart_sleep(r->tt_die + 1, r);
		r->dead = 1;
		sem_post(r->forks);
		printer(r, p->id, "has died");
		exit (1);
	}
	sem_wait(r->forks);
	printer(r, p->id, "has taken a fork");
	sem_wait(r->forks);
	printer(r, p->id, "has taken a fork");
	sem_wait(r->meal_check);
	printer(r, p->id, "is eating");
	p->t_last_meal = get_time();
	sem_post(r->meal_check);
	smart_sleep(r->tt_eat, r);
	(p->nb_meals)++;
	sem_post(r->forks);
	sem_post(r->forks);
}

void	*death_checker(void *v_philo)
{
	t_rules	*r;
	t_philo	*p;

	p = (t_philo *)v_philo;
	r = p->rules;
	while (1)
	{
		sem_wait(r->meal_check);
		if (time_passed(p->t_last_meal, get_time()) > r->tt_die)
		{
			printer(r, p->id, "has died");
			r->dead = 1;
			sem_wait(r->writing);
			exit(1);
		}
		sem_post(r->meal_check);
		if (r->dead)
			break ;
		usleep(1000);
		if (p->nb_meals >= r->nb_eat && r->nb_eat != -1)
			break ;
	}
	return (NULL);
}

void	p_process(void *v_philo)
{
	t_philo	*p;
	t_rules	*r;

	p = (t_philo *)v_philo;
	r = p->rules;
	p->t_last_meal = get_time();
	pthread_create(&(p->death_checker), NULL, death_checker, v_philo);
	if (p->id % 2)
		usleep(10000);
	while (!(r->dead))
	{
		philo_eating(p);
		if (r->nb_philo > 1)
		{
			if (p->nb_meals >= r->nb_eat && r->nb_eat != -1)
				break ;
			printer (r, p->id, "is sleeping");
			smart_sleep(r->tt_sleep, r);
			printer (r, p->id, "is thinking");
		}
	}
	pthread_join(p->death_checker, NULL);
	if (r->dead)
		exit(1);
	exit(0);
}

void	exit_starter(t_rules *r)
{
	int	i;
	int	ret;

	i = 0;
	while (i < r->nb_philo)
	{
		waitpid(-1, &ret, 0);
		if (ret != 0)
		{
			i = 0;
			while (i < r->nb_philo)
				kill(r->philo[i++].proc_id, SIGTERM);
			break ;
		}
		i++;
	}
	sem_close(r->forks);
	sem_close(r->meal_check);
	sem_close(r->writing);
	sem_unlink("forks");
	sem_unlink("meal_check");
	sem_unlink("writing");
}

int	starter(t_rules *rules)
{
	int		i;
	t_philo	*p;

	i = 0;
	p = rules->philo;
	rules->first_time = get_time();
	while (i < rules->nb_philo)
	{
		p[i].proc_id = fork();
		if (p[i].proc_id < 0)
			return (1);
		if (p[i].proc_id == 0)
			p_process(&(p[i]));
		i++;
		usleep(100);
	}
	exit_starter(rules);
	return (0);
}
