/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asolano- <asolano-@student.42malaga.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 11:20:06 by asolano-          #+#    #+#             */
/*   Updated: 2023/03/24 09:33:10 by asolano-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
 * This function will print a message, for this it will need, the rules, the 
 * and the message to print
 */

void	printer(t_rules *rules, int id, char *str)
{
	pthread_mutex_lock(&(rules->writing)); //we lock the writing mutex
	if (!(rules->dead) || rules->nb_philo == 1) //if there is not any dead philosopher
	{
		printf("%lli ", get_time() - rules->first_time); //we print the times passed
		printf("%i ", id + 1); //we print the id + 1 since we want to start from 1
		printf("%s\n", str); //we print the message needed
	}
	pthread_mutex_unlock(&(rules->writing)); //we unlock the writing mutex
	return ;
}

int	ft_isdigit(int n)
{
	if (n >= '0' && n <= '9')
		return (1);
	else
		return (0);
}
