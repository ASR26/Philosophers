/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   starter.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 08:04:38 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/24 11:29:49 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_eating(t_philo *p)
{
	t_rules	*r;

	r = p->rules;
	if (r->nb_philo == 1)
	{
		pthread_mutex_lock(&(r->forks[p->left_fork_id]));
		printer(r, p->id, "has taken a fork");
		smart_sleep(r->tt_die + 1, r);
		r->dead = 1;
		pthread_mutex_unlock(&(r->forks[p->left_fork_id]));
		printer(r, p->id, "has died");
		return ;
	}
	pthread_mutex_lock(&(r->forks[p->left_fork_id]));
	printer(r, p->id, "has taken a fork");
	pthread_mutex_lock(&(r->forks[p->right_fork_id]));
	printer(r, p->id, "has taken a fork");
	pthread_mutex_lock(&(r->meal_check));
	printer(r, p->id, "is eating");
	p->t_last_meal = get_time();
	pthread_mutex_unlock(&(r->meal_check));
	smart_sleep(r->tt_eat, r);
	(p->nb_meals)++;
	pthread_mutex_unlock(&(r->forks[p->left_fork_id]));
	pthread_mutex_unlock(&(r->forks[p->right_fork_id]));
}

void	*p_thread(void *v_philo)
{
	t_philo	*p;
	t_rules	*r;

	p = (t_philo *)v_philo;
	r = p->rules;
	if (p->id % 2)
		usleep(10000);
	while (!(r->dead))
	{
		philo_eating(p);
		if (r->nb_philo > 1)
		{
			if (r->all_eaten)
				break ;
			printer (r, p->id, "is sleeping");
			smart_sleep(r->tt_sleep, r);
			printer (r, p->id, "is thinking");
		}
	}
	return (NULL);
}

void	exit_starter(t_rules *r, t_philo *p)
{
	int	i;

	i = 0;
	while (i < r->nb_philo)
	{
		pthread_join(p[i].thread_id, NULL);
		i++;
	}
	i = 0;
	while (i < r->nb_philo)
	{
		pthread_mutex_destroy(&(r->forks[i]));
		i++;
	}
	pthread_mutex_destroy(&(r->writing));
}

void	death_checker(t_rules *r, t_philo *p)
{
	int	i;

	while (!(r->all_eaten))
	{
		i = 0;
		while (i < r->nb_philo && !(r->dead))
		{
			pthread_mutex_lock(&(r->meal_check));
			if (time_passed(p[i].t_last_meal, get_time()) > r->tt_die)
			{
				printer(r, i, "has died");
				r->dead = 1;
			}
			pthread_mutex_unlock(&(r->meal_check));
			i++;
			usleep(100);
		}
		if (r->dead)
			break ;
		i = 0;
		while (r->nb_eat != -1 && i < r->nb_philo && p[i].nb_meals >= r->nb_eat)
			i++;
		if (i == r->nb_philo)
			r->all_eaten = 1;
	}
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
		if (pthread_create(&(p[i].thread_id), NULL, p_thread, &(p[i])))
			return (1);
		p[i].t_last_meal = get_time();
		i++;
	}
	if (rules->nb_philo > 1)
		death_checker(rules, rules->philo);
	exit_starter(rules, p);
	return (0);
}
